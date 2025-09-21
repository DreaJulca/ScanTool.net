#ifndef ECU_PROGRAMMING_H
#define ECU_PROGRAMMING_H

// ECU Programming Module - Advanced automotive diagnostics and programming
// Supports injector coding, throttle calibration, steering angle sensor calibration, etc.

#include "globals.h"
#include "allegro_common.h"

// Programming operation types
#define PROG_INJECTOR_CODING           1
#define PROG_THROTTLE_CALIBRATION      2  
#define PROG_STEERING_ANGLE_CALIB      3
#define PROG_BATTERY_REGISTRATION      4
#define PROG_EPB_SERVICE_RESET         5
#define PROG_MAINTENANCE_RESET         6
#define PROG_ECU_FLASH_UPDATE          7

// Programming states
#define PROG_STATE_IDLE                0
#define PROG_STATE_AUTHENTICATING      1
#define PROG_STATE_DOWNLOADING         2
#define PROG_STATE_FLASHING            3
#define PROG_STATE_VERIFYING           4
#define PROG_STATE_COMPLETE            5
#define PROG_STATE_ERROR               6

// Security access levels
#define SECURITY_LEVEL_BASIC           0x01
#define SECURITY_LEVEL_PROGRAMMING     0x23
#define SECURITY_LEVEL_CALIBRATION     0x67

// UDS (Unified Diagnostic Services) commands
#define UDS_DIAGNOSTIC_SESSION_CONTROL 0x10
#define UDS_ECU_RESET                  0x11
#define UDS_SECURITY_ACCESS            0x27
#define UDS_COMMUNICATION_CONTROL      0x28
#define UDS_WRITE_DATA_BY_IDENTIFIER   0x2E
#define UDS_READ_DATA_BY_IDENTIFIER    0x22
#define UDS_REQUEST_DOWNLOAD           0x34
#define UDS_TRANSFER_DATA              0x36
#define UDS_REQUEST_TRANSFER_EXIT      0x37
#define UDS_TESTER_PRESENT             0x3E

// Diagnostic session types
#define SESSION_DEFAULT                0x01
#define SESSION_PROGRAMMING            0x02
#define SESSION_EXTENDED_DIAGNOSTIC    0x03

// Data identifiers for various calibrations
#define DID_INJECTOR_FLOW_RATE         0xF190
#define DID_THROTTLE_MIN_POSITION      0xF191
#define DID_THROTTLE_MAX_POSITION      0xF192
#define DID_STEERING_ANGLE_OFFSET      0xF193
#define DID_BATTERY_CAPACITY           0xF194
#define DID_EPB_CALIBRATION            0xF195

typedef struct {
    int operation_type;
    int state;
    int progress_percent;
    char status_message[256];
    char error_message[256];
    int security_level;
    int session_type;
    unsigned char seed[4];
    unsigned char key[4];
} ECU_PROG_SESSION;

typedef struct {
    unsigned short data_id;
    char description[64];
    void (*calibration_func)(void);
    int requires_security;
    int session_required;
} CALIBRATION_ITEM;

// Function prototypes
int display_ecu_programming_menu(void);
int perform_injector_coding(void);
int perform_throttle_calibration(void);
int perform_steering_angle_calibration(void);
int perform_battery_registration(void);
int perform_epb_service_reset(void);
int perform_maintenance_reset(void);
int perform_ecu_programming(const char *firmware_file);

// UDS protocol functions
int uds_start_diagnostic_session(int session_type);
int uds_request_security_access(int security_level);
int uds_send_security_key(unsigned char *key);
int uds_read_data_by_identifier(unsigned short did, unsigned char *data, int *length);
int uds_write_data_by_identifier(unsigned short did, unsigned char *data, int length);
int uds_request_download(unsigned int address, unsigned int size);
int uds_transfer_data(unsigned char *data, int length, int block_sequence);
int uds_request_transfer_exit(void);
int uds_ecu_reset(int reset_type);

// Security functions
unsigned int calculate_security_key(unsigned char *seed, int algorithm);
int verify_programming_preconditions(void);

// Firmware management
int load_firmware_file(const char *filename, unsigned char **data, unsigned int *size);
int verify_firmware_checksum(unsigned char *data, unsigned int size);

// Progress callbacks
void update_programming_progress(int percent, const char *message);

extern ECU_PROG_SESSION current_prog_session;

#endif