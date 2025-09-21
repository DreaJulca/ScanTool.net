#ifndef ADVANCED_DIAGNOSTICS_H
#define ADVANCED_DIAGNOSTICS_H

#include "globals.h"

// Advanced diagnostic test types
#define DIAG_INJECTOR_CODING           0x01
#define DIAG_THROTTLE_BODY_CALIB       0x02
#define DIAG_STEERING_ANGLE_CALIB      0x03
#define DIAG_BATTERY_REGISTRATION      0x04
#define DIAG_EPB_SERVICE_RESET         0x05
#define DIAG_DPF_REGENERATION          0x06
#define DIAG_TRANSMISSION_ADAPT        0x07
#define DIAG_ABS_BLEEDING              0x08
#define DIAG_AIRBAG_CALIBRATION        0x09

// Service intervals and maintenance
#define MAINT_ENGINE_OIL               0x10
#define MAINT_TRANSMISSION_FLUID       0x11
#define MAINT_BRAKE_FLUID              0x12
#define MAINT_COOLANT_SYSTEM           0x13
#define MAINT_AIR_FILTER               0x14
#define MAINT_SPARK_PLUGS              0x15
#define MAINT_TIMING_BELT              0x16
#define MAINT_BRAKE_PADS               0x17

// Calibration procedures
typedef struct {
    int procedure_id;
    char name[64];
    char description[256];
    int estimated_time_minutes;
    int requires_special_tools;
    int safety_level;  // 1=low, 2=medium, 3=high risk
    char prerequisites[256];
    int (*execute_function)(void);
} CALIBRATION_PROCEDURE;

// Service reset structure
typedef struct {
    int service_type;
    char service_name[64];
    int interval_miles;
    int interval_months;
    int current_miles;
    int months_since_service;
    int reset_available;
    int (*reset_function)(void);
} SERVICE_RESET_ITEM;

// Advanced diagnostic session
typedef struct {
    int active_procedure;
    int current_step;
    int total_steps;
    char current_instruction[512];
    char status_message[256];
    int completion_percentage;
    int requires_user_action;
    int error_code;
    char error_description[256];
} DIAGNOSTIC_SESSION;

// Function prototypes for calibration procedures
int display_advanced_diagnostics_menu(void);
int perform_injector_coding_wizard(void);
int perform_throttle_body_calibration(void);
int perform_steering_angle_sensor_calibration(void);
int perform_battery_registration_procedure(void);
int perform_epb_service_reset(void);

// Injector coding functions
int injector_coding_step1_read_current_codes(void);
int injector_coding_step2_input_new_codes(void);
int injector_coding_step3_write_codes_to_ecu(void);
int injector_coding_step4_verify_operation(void);

// Throttle body calibration functions  
int throttle_calib_step1_preparation(void);
int throttle_calib_step2_min_position_learn(void);
int throttle_calib_step3_max_position_learn(void);
int throttle_calib_step4_verification(void);

// Steering angle sensor calibration
int sas_calib_step1_center_steering(void);
int sas_calib_step2_full_lock_left(void);
int sas_calib_step3_full_lock_right(void);
int sas_calib_step4_set_zero_point(void);

// Battery registration functions
int battery_reg_step1_read_battery_info(void);
int battery_reg_step2_input_new_specs(void);
int battery_reg_step3_update_bms_settings(void);
int battery_reg_step4_reset_counters(void);

// Electronic parking brake service
int epb_reset_step1_preparation(void);
int epb_reset_step2_retract_calipers(void);
int epb_reset_step3_service_mode(void);
int epb_reset_step4_reinstall_pads(void);
int epb_reset_step5_calibration(void);

// Maintenance reset functions
int display_maintenance_reset_menu(void);
int reset_engine_oil_service_interval(void);
int reset_transmission_service_interval(void);
int reset_brake_service_interval(void);
int reset_general_inspection_interval(void);

// Utility functions
int verify_vehicle_compatibility(int procedure_id);
int check_diagnostic_prerequisites(int procedure_id);
void display_safety_warning(int safety_level);
void log_diagnostic_procedure(int procedure_id, int result);
int save_calibration_data(int procedure_id, unsigned char *data, int length);
int load_calibration_data(int procedure_id, unsigned char *data, int *length);

// Progress and status management
void init_diagnostic_session(void);
void update_diagnostic_progress(int step, int total_steps, const char *instruction);
void set_diagnostic_error(int error_code, const char *description);
void complete_diagnostic_session(int success);

// Communication helpers for advanced protocols
int send_uds_request(unsigned char service, unsigned char *data, int data_len);
int send_kwp2000_request(unsigned char service, unsigned char *data, int data_len);
int send_manufacturer_specific_command(unsigned char *command, int cmd_len);

extern DIAGNOSTIC_SESSION current_diagnostic_session;
extern CALIBRATION_PROCEDURE available_procedures[];
extern SERVICE_RESET_ITEM available_services[];

#endif