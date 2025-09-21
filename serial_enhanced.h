#ifndef SERIAL_ENHANCED_H
#define SERIAL_ENHANCED_H

#include "serial.h"

// Enhanced protocol support
#define PROTOCOL_UDS              0x20  // Unified Diagnostic Services
#define PROTOCOL_KWP2000          0x21  // Keyword Protocol 2000
#define PROTOCOL_CCP              0x22  // CAN Calibration Protocol
#define PROTOCOL_XCP              0x23  // Universal Measurement and Calibration Protocol
#define PROTOCOL_J1939            0x24  // Heavy duty vehicle protocol

// Enhanced message types
#define MSG_TYPE_SINGLE_FRAME     0x00
#define MSG_TYPE_FIRST_FRAME      0x10
#define MSG_TYPE_CONSECUTIVE_FRAME 0x20
#define MSG_TYPE_FLOW_CONTROL     0x30

// Flow control status
#define FC_CONTINUE_TO_SEND       0x00
#define FC_WAIT                   0x01
#define FC_OVERFLOW               0x02

// Enhanced timeouts for different operations
#define TIMEOUT_PROGRAMMING       30000  // 30 seconds for programming operations
#define TIMEOUT_CALIBRATION       15000  // 15 seconds for calibration
#define TIMEOUT_SECURITY_ACCESS   5000   // 5 seconds for security access

// UDS Service Identifiers (ISO 14229)
#define UDS_DIAGNOSTIC_SESSION_CONTROL  0x10
#define UDS_ECU_RESET                   0x11
#define UDS_SECURITY_ACCESS             0x27
#define UDS_COMMUNICATION_CONTROL       0x28
#define UDS_TESTER_PRESENT              0x3E
#define UDS_READ_DATA_BY_IDENTIFIER     0x22
#define UDS_WRITE_DATA_BY_IDENTIFIER    0x2E
#define UDS_INPUT_OUTPUT_CONTROL        0x2F
#define UDS_ROUTINE_CONTROL             0x31
#define UDS_REQUEST_DOWNLOAD            0x34
#define UDS_REQUEST_UPLOAD              0x35
#define UDS_TRANSFER_DATA               0x36
#define UDS_REQUEST_TRANSFER_EXIT       0x37

// UDS Session Types
#define SESSION_DEFAULT                 0x01
#define SESSION_PROGRAMMING             0x02
#define SESSION_EXTENDED_DIAGNOSTIC     0x03
#define SESSION_SAFETY_SYSTEM           0x04

// CAN message structure for advanced protocols
typedef struct {
    unsigned int id;           // CAN ID
    unsigned char data[8];     // Data bytes
    unsigned char length;      // Data length
    unsigned char is_extended; // Extended frame flag
    unsigned long timestamp;   // Message timestamp
} CAN_MESSAGE;

// ISO-TP (Transport Protocol) message structure
typedef struct {
    unsigned char *data;       // Message data
    unsigned int length;       // Total message length
    unsigned int transmitted;  // Bytes transmitted so far
    unsigned char sequence;    // Sequence counter
    int flow_control_status;   // Flow control state
} ISOTP_MESSAGE;

// Enhanced communication session
typedef struct {
    int protocol_type;
    unsigned int tx_id;        // Transmit CAN ID
    unsigned int rx_id;        // Receive CAN ID
    int session_active;
    int security_access_granted;
    unsigned char current_session_type;
    ISOTP_MESSAGE current_tx_msg;
    ISOTP_MESSAGE current_rx_msg;
    volatile int transfer_in_progress;
} ENHANCED_COMM_SESSION;

// Function prototypes for enhanced communication
int init_enhanced_communication(void);
void shutdown_enhanced_communication(void);

// Protocol-specific functions
int uds_send_diagnostic_message(unsigned char service, unsigned char *data, int length);
int uds_receive_response(unsigned char *response, int *length, int timeout_ms);
int kwp2000_send_request(unsigned char service, unsigned char *data, int length);
int kwp2000_receive_response(unsigned char *response, int *length, int timeout_ms);

// ISO-TP functions for multi-frame messages
int isotp_send_message(unsigned char *data, int length);
int isotp_receive_message(unsigned char *data, int *length, int timeout_ms);
int isotp_send_single_frame(unsigned char *data, int length);
int isotp_send_first_frame(unsigned char *data, int length);
int isotp_send_consecutive_frame(unsigned char *data, int length, int sequence);
int isotp_send_flow_control(int status, int block_size, int separation_time);

// CAN message handling
int send_can_message(CAN_MESSAGE *msg);
int receive_can_message(CAN_MESSAGE *msg, int timeout_ms);
int set_can_filter(unsigned int id, unsigned int mask);

// Enhanced error handling
int handle_negative_response(unsigned char *response, char *error_description);
const char* get_uds_error_description(unsigned char error_code);
const char* get_kwp2000_error_description(unsigned char error_code);

// Session management
int start_diagnostic_session(int session_type);
int stop_diagnostic_session(void);
int request_security_access(int access_level);
int send_security_key(unsigned char *key, int key_length);

// Timing and synchronization
void start_periodic_tester_present(void);
void stop_periodic_tester_present(void);
int send_tester_present(void);

// Data transfer for programming/calibration
int request_download(unsigned int address, unsigned int size);
int transfer_data_block(unsigned char *data, int length, int block_number);
int request_transfer_exit(void);
int request_upload(unsigned int address, unsigned int size);

// Vehicle identification
int read_vehicle_identification_number(char *vin);
int read_ecu_identification(char *ecu_id, char *software_version, char *hardware_version);
int read_calibration_identification(char *cal_id);

// Enhanced logging for advanced protocols
void log_enhanced_communication(const char *direction, unsigned char *data, int length);
void log_isotp_message(const char *type, ISOTP_MESSAGE *msg);

// Performance monitoring
typedef struct {
    unsigned long messages_sent;
    unsigned long messages_received;
    unsigned long errors_count;
    unsigned long average_response_time_ms;
    unsigned long max_response_time_ms;
    unsigned long min_response_time_ms;
} COMM_PERFORMANCE_STATS;

void reset_comm_performance_stats(void);
COMM_PERFORMANCE_STATS* get_comm_performance_stats(void);

extern ENHANCED_COMM_SESSION enhanced_session;

#endif