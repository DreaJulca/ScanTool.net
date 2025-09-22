#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "serial.h"
#include "serial_enhanced.h"

ENHANCED_COMM_SESSION enhanced_session;
static COMM_PERFORMANCE_STATS perf_stats;

int init_enhanced_communication(void)
{
    memset(&enhanced_session, 0, sizeof(ENHANCED_COMM_SESSION));
    memset(&perf_stats, 0, sizeof(COMM_PERFORMANCE_STATS));
    
    enhanced_session.protocol_type = PROTOCOL_UDS;
    enhanced_session.tx_id = 0x7E0; // Default ECU request ID
    enhanced_session.rx_id = 0x7E8; // Default ECU response ID
    
    return 0;
}

void shutdown_enhanced_communication(void)
{
    if (enhanced_session.session_active) {
        stop_diagnostic_session();
    }
    
    memset(&enhanced_session, 0, sizeof(ENHANCED_COMM_SESSION));
}

int uds_send_diagnostic_message(unsigned char service, unsigned char *data, int length)
{
    if (!enhanced_session.session_active && service != UDS_DIAGNOSTIC_SESSION_CONTROL) {
        // Auto-start diagnostic session if not active
        if (start_diagnostic_session(SESSION_DEFAULT) != 0) {
            return -1;
        }
    }
    
    // Prepare UDS message
    unsigned char message[256];
    message[0] = service;
    if (data && length > 0) {
        memcpy(&message[1], data, length);
        length++;
    } else {
        length = 1;
    }
    
    // Send via ISO-TP
    int result = isotp_send_message(message, length);
    
    // Update performance statistics
    perf_stats.messages_sent++;
    if (result != 0) {
        perf_stats.errors_count++;
    }
    
    return result;
}

int uds_receive_response(unsigned char *response, int *length, int timeout_ms)
{
    int result = isotp_receive_message(response, length, timeout_ms);
    
    if (result == 0) {
        perf_stats.messages_received++;
        
        // Check for negative response
        if (response[0] == 0x7F) {
            char error_desc[256];
            handle_negative_response(response, error_desc);
            return -1;
        }
    } else {
        perf_stats.errors_count++;
    }
    
    return result;
}

int kwp2000_send_request(unsigned char service, unsigned char *data, int length)
{
    // KWP2000 message format
    unsigned char message[256];
    message[0] = 0x80 | (length + 1); // Format byte
    message[1] = service;
    
    if (data && length > 0) {
        memcpy(&message[2], data, length);
    }
    
    // Calculate checksum
    unsigned char checksum = 0;
    for (int i = 0; i < length + 2; i++) {
        checksum += message[i];
    }
    message[length + 2] = checksum;
    
    // Send message
    send_command((char*)message); // Convert to existing serial interface
    
    perf_stats.messages_sent++;
    return 0;
}

int kwp2000_receive_response(unsigned char *response, int *length, int timeout_ms)
{
    char buffer[256];
    
    start_serial_timer(timeout_ms);
    
    while (!serial_time_out) {
        if (read_comport(buffer) == DATA) {
            stop_serial_timer();
            
            // Parse KWP2000 response
            *length = (unsigned char)buffer[0] & 0x7F;
            memcpy(response, &buffer[1], *length);
            
            perf_stats.messages_received++;
            return 0;
        }
    }
    
    stop_serial_timer();
    perf_stats.errors_count++;
    return -1;
}

int isotp_send_message(unsigned char *data, int length)
{
    if (length <= 7) {
        // Single frame
        return isotp_send_single_frame(data, length);
    } else {
        // Multi-frame transmission
        int result = isotp_send_first_frame(data, length);
        if (result != 0) return result;
        
        // Wait for flow control
        unsigned char fc_frame[8];
        if (receive_can_message((CAN_MESSAGE*)fc_frame, TIMEOUT_SECURITY_ACCESS) != 0) {
            return -1;
        }
        
        // Check flow control status
        if ((fc_frame[0] & 0xF0) != MSG_TYPE_FLOW_CONTROL || 
            (fc_frame[0] & 0x0F) != FC_CONTINUE_TO_SEND) {
            return -1;
        }
        
        // Send consecutive frames
        int remaining = length - 6; // First frame carries 6 bytes
        int sequence = 1;
        
        while (remaining > 0) {
            int chunk_size = (remaining > 7) ? 7 : remaining;
            result = isotp_send_consecutive_frame(&data[6 + (sequence - 1) * 7], 
                                                chunk_size, sequence);
            if (result != 0) return result;
            
            sequence = (sequence + 1) & 0x0F;
            remaining -= chunk_size;
        }
    }
    
    return 0;
}

int isotp_receive_message(unsigned char *data, int *length, int timeout_ms)
{
    CAN_MESSAGE can_msg;
    
    if (receive_can_message(&can_msg, timeout_ms) != 0) {
        return -1;
    }
    
    unsigned char pci = can_msg.data[0] & 0xF0;
    
    switch (pci) {
        case MSG_TYPE_SINGLE_FRAME:
            *length = can_msg.data[0] & 0x0F;
            memcpy(data, &can_msg.data[1], *length);
            return 0;
            
        case MSG_TYPE_FIRST_FRAME:
            *length = ((can_msg.data[0] & 0x0F) << 8) | can_msg.data[1];
            memcpy(data, &can_msg.data[2], 6);
            
            // Send flow control
            isotp_send_flow_control(FC_CONTINUE_TO_SEND, 0, 0);
            
            // Receive consecutive frames
            int received = 6;
            int expected_sequence = 1;
            
            while (received < *length) {
                if (receive_can_message(&can_msg, timeout_ms) != 0) {
                    return -1;
                }
                
                if ((can_msg.data[0] & 0xF0) != MSG_TYPE_CONSECUTIVE_FRAME) {
                    return -1;
                }
                
                int sequence = can_msg.data[0] & 0x0F;
                if (sequence != expected_sequence) {
                    return -1;
                }
                
                int chunk_size = (*length - received > 7) ? 7 : (*length - received);
                memcpy(&data[received], &can_msg.data[1], chunk_size);
                
                received += chunk_size;
                expected_sequence = (expected_sequence + 1) & 0x0F;
            }
            
            return 0;
            
        default:
            return -1;
    }
}

int isotp_send_single_frame(unsigned char *data, int length)
{
    CAN_MESSAGE can_msg;
    can_msg.id = enhanced_session.tx_id;
    can_msg.is_extended = 0;
    can_msg.length = length + 1;
    can_msg.data[0] = MSG_TYPE_SINGLE_FRAME | length;
    memcpy(&can_msg.data[1], data, length);
    
    return send_can_message(&can_msg);
}

int isotp_send_first_frame(unsigned char *data, int length)
{
    CAN_MESSAGE can_msg;
    can_msg.id = enhanced_session.tx_id;
    can_msg.is_extended = 0;
    can_msg.length = 8;
    can_msg.data[0] = MSG_TYPE_FIRST_FRAME | ((length >> 8) & 0x0F);
    can_msg.data[1] = length & 0xFF;
    memcpy(&can_msg.data[2], data, 6);
    
    return send_can_message(&can_msg);
}

int isotp_send_consecutive_frame(unsigned char *data, int length, int sequence)
{
    CAN_MESSAGE can_msg;
    can_msg.id = enhanced_session.tx_id;
    can_msg.is_extended = 0;
    can_msg.length = length + 1;
    can_msg.data[0] = MSG_TYPE_CONSECUTIVE_FRAME | (sequence & 0x0F);
    memcpy(&can_msg.data[1], data, length);
    
    return send_can_message(&can_msg);
}

int isotp_send_flow_control(int status, int block_size, int separation_time)
{
    CAN_MESSAGE can_msg;
    can_msg.id = enhanced_session.tx_id;
    can_msg.is_extended = 0;
    can_msg.length = 3;
    can_msg.data[0] = MSG_TYPE_FLOW_CONTROL | (status & 0x0F);
    can_msg.data[1] = block_size;
    can_msg.data[2] = separation_time;
    
    return send_can_message(&can_msg);
}

int send_can_message(CAN_MESSAGE *msg)
{
    // Convert CAN message to ELM327 format and send
    char elm_cmd[32];
    sprintf(elm_cmd, "%03X", msg->id);
    
    for (int i = 0; i < msg->length; i++) {
        sprintf(&elm_cmd[3 + i * 2], "%02X", msg->data[i]);
    }
    
    send_command(elm_cmd);
    return 0;
}

int receive_can_message(CAN_MESSAGE *msg, int timeout_ms)
{
    char response[256];
    
    start_serial_timer(timeout_ms);
    
    while (!serial_time_out) {
        if (read_comport(response) == DATA) {
            stop_serial_timer();
            
            // Parse ELM327 response format: "7E8 02 41 00"
            if (strlen(response) >= 7) {
                sscanf(response, "%X", &msg->id);
                
                // Extract data bytes
                char *data_start = strchr(response, ' ');
                if (data_start) {
                    data_start++;
                    msg->length = 0;
                    
                    while (*data_start && msg->length < 8) {
                        unsigned int temp;
                        if (sscanf(data_start, "%02X", &temp) == 1) {
                            msg->data[msg->length] = (unsigned char)temp;
                            msg->length++;
                            data_start += 3; // Move past "XX "
                        } else {
                            break;
                        }
                    }
                    
                    return 0;
                }
            }
        }
    }
    
    stop_serial_timer();
    return -1;
}

int start_diagnostic_session(int session_type)
{
    unsigned char request[] = { session_type };
    unsigned char response[256];
    int response_length;
    
    if (uds_send_diagnostic_message(UDS_DIAGNOSTIC_SESSION_CONTROL, request, 1) != 0) {
        return -1;
    }
    
    if (uds_receive_response(response, &response_length, TIMEOUT_SECURITY_ACCESS) != 0) {
        return -1;
    }
    
    enhanced_session.session_active = 1;
    enhanced_session.current_session_type = session_type;
    
    return 0;
}

int stop_diagnostic_session(void)
{
    if (enhanced_session.session_active) {
        start_diagnostic_session(SESSION_DEFAULT);
        enhanced_session.session_active = 0;
    }
    
    return 0;
}

int request_security_access(int access_level)
{
    unsigned char request[] = { access_level };
    unsigned char response[256];
    int response_length;
    
    // Request seed
    if (uds_send_diagnostic_message(UDS_SECURITY_ACCESS, request, 1) != 0) {
        return -1;
    }
    
    if (uds_receive_response(response, &response_length, TIMEOUT_SECURITY_ACCESS) != 0) {
        return -1;
    }
    
    // Calculate key from seed (simplified)
    unsigned char key_request[5];
    key_request[0] = access_level + 1;
    
    // In real implementation, this would use proper security algorithm
    for (int i = 0; i < 4; i++) {
        key_request[i + 1] = response[i + 1] ^ 0xAA; // Simple XOR for demo
    }
    
    // Send key
    if (uds_send_diagnostic_message(UDS_SECURITY_ACCESS, key_request, 5) != 0) {
        return -1;
    }
    
    if (uds_receive_response(response, &response_length, TIMEOUT_SECURITY_ACCESS) != 0) {
        return -1;
    }
    
    enhanced_session.security_access_granted = 1;
    return 0;
}

int send_security_key(unsigned char *key, int key_length)
{
    unsigned char response[256];
    int response_length;
    
    if (uds_send_diagnostic_message(UDS_SECURITY_ACCESS, key, key_length) != 0) {
        return -1;
    }
    
    return uds_receive_response(response, &response_length, TIMEOUT_SECURITY_ACCESS);
}

int handle_negative_response(unsigned char *response, char *error_description)
{
    if (response[0] != 0x7F) {
        strcpy(error_description, "Not a negative response");
        return 0;
    }
    
    unsigned char service = response[1];
    unsigned char nrc = response[2]; // Negative Response Code
    
    sprintf(error_description, "Service 0x%02X failed: %s", 
            service, get_uds_error_description(nrc));
    
    return nrc;
}

const char* get_uds_error_description(unsigned char error_code)
{
    switch (error_code) {
        case 0x10: return "General Reject";
        case 0x11: return "Service Not Supported";
        case 0x12: return "Sub-Function Not Supported";
        case 0x13: return "Incorrect Message Length";
        case 0x22: return "Conditions Not Correct";
        case 0x24: return "Request Sequence Error";
        case 0x31: return "Request Out Of Range";
        case 0x33: return "Security Access Denied";
        case 0x35: return "Invalid Key";
        case 0x36: return "Exceed Number Of Attempts";
        case 0x37: return "Required Time Delay Not Expired";
        case 0x70: return "Upload Download Not Accepted";
        case 0x71: return "Transfer Data Suspended";
        case 0x72: return "General Programming Failure";
        case 0x73: return "Wrong Block Sequence Counter";
        case 0x78: return "Request Correctly Received - Response Pending";
        default: return "Unknown Error";
    }
}

const char* get_kwp2000_error_description(unsigned char error_code)
{
    switch (error_code) {
        case 0x10: return "General Reject";
        case 0x11: return "Service Not Supported";
        case 0x12: return "Sub-Function Not Supported - Invalid Format";
        case 0x21: return "Busy Repeat Request";
        case 0x22: return "Conditions Not Correct Or Request Sequence Error";
        case 0x23: return "Routine Not Complete";
        default: return "Unknown KWP2000 Error";
    }
}

void start_periodic_tester_present(void)
{
    // In real implementation, this would start a timer to periodically send
    // tester present messages to keep the diagnostic session alive
}

void stop_periodic_tester_present(void)
{
    // Stop the periodic tester present timer
}

int send_tester_present(void)
{
    unsigned char response[256];
    int response_length;
    
    if (uds_send_diagnostic_message(UDS_TESTER_PRESENT, NULL, 0) != 0) {
        return -1;
    }
    
    return uds_receive_response(response, &response_length, AT_TIMEOUT);
}

void reset_comm_performance_stats(void)
{
    memset(&perf_stats, 0, sizeof(COMM_PERFORMANCE_STATS));
}

COMM_PERFORMANCE_STATS* get_comm_performance_stats(void)
{
    return &perf_stats;
}

void log_enhanced_communication(const char *direction, unsigned char *data, int length)
{
    FILE *log_file = fopen("enhanced_comm_log.txt", "a");
    if (log_file) {
        fprintf(log_file, "[%s] ", direction);
        for (int i = 0; i < length; i++) {
            fprintf(log_file, "%02X ", data[i]);
        }
        fprintf(log_file, "\n");
        fclose(log_file);
    }
}

void log_isotp_message(const char *type, ISOTP_MESSAGE *msg)
{
    FILE *log_file = fopen("isotp_log.txt", "a");
    if (log_file) {
        fprintf(log_file, "[%s] Length: %d, Transmitted: %d, Sequence: %d\n", 
                type, msg->length, msg->transmitted, msg->sequence);
        fclose(log_file);
    }
}