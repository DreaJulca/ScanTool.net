#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "advanced_diagnostics.h"
#include "ecu_programming.h"
#include "serial_enhanced.h"
#include "custom_gui.h"

DIAGNOSTIC_SESSION current_diagnostic_session;

// Available calibration procedures
CALIBRATION_PROCEDURE available_procedures[] = {
    { DIAG_INJECTOR_CODING, "Injector Coding", "Program injector flow rate codes", 45, 1, 2, 
      "Engine off, ignition on, ECU accessible", NULL },
    { DIAG_THROTTLE_BODY_CALIB, "Throttle Body Calibration", "Learn throttle position limits", 30, 0, 1,
      "Engine off, throttle body accessible", NULL },
    { DIAG_STEERING_ANGLE_CALIB, "Steering Angle Sensor", "Calibrate steering center position", 20, 0, 1,
      "Vehicle level, wheels straight ahead", NULL },
    { DIAG_BATTERY_REGISTRATION, "Battery Registration", "Register new battery with BMS", 15, 0, 1,
      "New battery installed, ignition on", NULL },
    { DIAG_EPB_SERVICE_RESET, "EPB Service Reset", "Retract electronic parking brake", 25, 1, 3,
      "Vehicle secure, brake service required", NULL },
    { DIAG_DPF_REGENERATION, "DPF Regeneration", "Force diesel particulate filter regen", 60, 0, 2,
      "Engine warm, vehicle stationary", NULL },
    { 0, "", "", 0, 0, 0, "", NULL }
};

// Available service resets
SERVICE_RESET_ITEM available_services[] = {
    { MAINT_ENGINE_OIL, "Engine Oil Service", 7500, 12, 0, 0, 1, NULL },
    { MAINT_TRANSMISSION_FLUID, "Transmission Fluid", 60000, 60, 0, 0, 1, NULL },
    { MAINT_BRAKE_FLUID, "Brake Fluid Service", 24000, 24, 0, 0, 1, NULL },
    { MAINT_COOLANT_SYSTEM, "Coolant System", 60000, 60, 0, 0, 1, NULL },
    { MAINT_AIR_FILTER, "Air Filter Replacement", 15000, 12, 0, 0, 1, NULL },
    { MAINT_SPARK_PLUGS, "Spark Plug Replacement", 30000, 36, 0, 0, 1, NULL },
    { MAINT_TIMING_BELT, "Timing Belt Service", 90000, 84, 0, 0, 1, NULL },
    { MAINT_BRAKE_PADS, "Brake Pad Replacement", 25000, 24, 0, 0, 1, NULL },
    { 0, "", 0, 0, 0, 0, 0, NULL }
};

// Dialog procedures
static int diagnostics_menu_proc(int msg, DIALOG *d, int c);
static int procedure_button_proc(int msg, DIALOG *d, int c);
static int maintenance_menu_proc(int msg, DIALOG *d, int c);
static int service_reset_proc(int msg, DIALOG *d, int c);

// Advanced diagnostics menu dialog
static DIALOG advanced_diagnostics_dialog[] = {
    /* (proc)                  (x)  (y)  (w)  (h)  (fg)      (bg)         (key) (flags) (d1) (d2) (dp)                      (dp2) (dp3) */
    { d_clear_proc,            0,   0,   0,   0,   0,        C_WHITE,     0,    0,      0,   0,   NULL,                     NULL, NULL },
    { d_shadow_box_proc,       30,  20,  760, 440, C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   NULL,                     NULL, NULL },
    { d_text_proc,             40,  30,  300, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Advanced Diagnostic Procedures", NULL, NULL },
    
    // Left column - Calibration procedures
    { d_text_proc,             50,  60,  200, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Calibration Procedures:", NULL, NULL },
    { procedure_button_proc,   50,  90,  300, 30,  C_BLACK,  C_WHITE,     'i',  D_EXIT, DIAG_INJECTOR_CODING, 0, "Injector Coding", NULL, NULL },
    { procedure_button_proc,   50,  130, 300, 30,  C_BLACK,  C_WHITE,     't',  D_EXIT, DIAG_THROTTLE_BODY_CALIB, 0, "Throttle Body Calibration", NULL, NULL },
    { procedure_button_proc,   50,  170, 300, 30,  C_BLACK,  C_WHITE,     's',  D_EXIT, DIAG_STEERING_ANGLE_CALIB, 0, "Steering Angle Sensor", NULL, NULL },
    { procedure_button_proc,   50,  210, 300, 30,  C_BLACK,  C_WHITE,     'b',  D_EXIT, DIAG_BATTERY_REGISTRATION, 0, "Battery Registration", NULL, NULL },
    { procedure_button_proc,   50,  250, 300, 30,  C_BLACK,  C_WHITE,     'e',  D_EXIT, DIAG_EPB_SERVICE_RESET, 0, "EPB Service Reset", NULL, NULL },
    { procedure_button_proc,   50,  290, 300, 30,  C_BLACK,  C_WHITE,     'd',  D_EXIT, DIAG_DPF_REGENERATION, 0, "DPF Regeneration", NULL, NULL },
    
    // Right column - Service resets
    { d_text_proc,             420, 60,  200, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Service Interval Resets:", NULL, NULL },
    { service_reset_proc,      420, 90,  300, 30,  C_BLACK,  C_WHITE,     'o',  D_EXIT, MAINT_ENGINE_OIL, 0, "Engine Oil Service", NULL, NULL },
    { service_reset_proc,      420, 130, 300, 30,  C_BLACK,  C_WHITE,     'f',  D_EXIT, MAINT_TRANSMISSION_FLUID, 0, "Transmission Fluid", NULL, NULL },
    { service_reset_proc,      420, 170, 300, 30,  C_BLACK,  C_WHITE,     'k',  D_EXIT, MAINT_BRAKE_FLUID, 0, "Brake Fluid Service", NULL, NULL },
    { service_reset_proc,      420, 210, 300, 30,  C_BLACK,  C_WHITE,     'c',  D_EXIT, MAINT_COOLANT_SYSTEM, 0, "Coolant System", NULL, NULL },
    { service_reset_proc,      420, 250, 300, 30,  C_BLACK,  C_WHITE,     'a',  D_EXIT, MAINT_AIR_FILTER, 0, "Air Filter", NULL, NULL },
    { service_reset_proc,      420, 290, 300, 30,  C_BLACK,  C_WHITE,     'p',  D_EXIT, MAINT_SPARK_PLUGS, 0, "Spark Plugs", NULL, NULL },
    
    // Bottom buttons
    { d_button_proc,           350, 400, 100, 30,  C_BLACK,  C_WHITE,     27,   D_EXIT, 0,   0,   "Back",                   NULL, NULL },
    { d_yield_proc,            0,   0,   0,   0,   0,        0,           0,    0,      0,   0,   NULL,                     NULL, NULL },
    { NULL,                    0,   0,   0,   0,   0,        0,           0,    0,      0,   0,   NULL,                     NULL, NULL }
};

int display_advanced_diagnostics_menu(void)
{
    centre_dialog(advanced_diagnostics_dialog);
    return do_dialog(advanced_diagnostics_dialog, -1);
}

static int procedure_button_proc(int msg, DIALOG *d, int c)
{
    if (msg == MSG_CLICK || (msg == MSG_KEY && c == d->key)) {
        int procedure_id = d->d1;
        
        // Find the procedure
        CALIBRATION_PROCEDURE *proc = NULL;
        for (int i = 0; available_procedures[i].procedure_id != 0; i++) {
            if (available_procedures[i].procedure_id == procedure_id) {
                proc = &available_procedures[i];
                break;
            }
        }
        
        if (!proc) return D_O_K;
        
        // Display safety warning
        display_safety_warning(proc->safety_level);
        
        // Show procedure info and get confirmation
        char info_msg[512];
        sprintf(info_msg, "%s\n\nEstimated time: %d minutes\nPrerequisites: %s\n\nContinue?", 
                proc->description, proc->estimated_time_minutes, proc->prerequisites);
        
        if (alert3("Diagnostic Procedure", info_msg, NULL, "Continue", "Cancel", NULL, 'c', 'c', 0) == 2) {
            return D_REDRAW;
        }
        
        // Check prerequisites
        if (!check_diagnostic_prerequisites(procedure_id)) {
            alert("Prerequisites Not Met", 
                  "Please ensure all prerequisites are satisfied", 
                  "before attempting this procedure", "OK", NULL, 0, 0);
            return D_REDRAW;
        }
        
        // Execute the specific procedure
        int result = 0;
        switch (procedure_id) {
            case DIAG_INJECTOR_CODING:
                result = perform_injector_coding_wizard();
                break;
            case DIAG_THROTTLE_BODY_CALIB:
                result = perform_throttle_body_calibration();
                break;
            case DIAG_STEERING_ANGLE_CALIB:
                result = perform_steering_angle_sensor_calibration();
                break;
            case DIAG_BATTERY_REGISTRATION:
                result = perform_battery_registration_procedure();
                break;
            case DIAG_EPB_SERVICE_RESET:
                result = perform_epb_service_reset();
                break;
            case DIAG_DPF_REGENERATION:
                result = perform_dpf_regeneration();
                break;
        }
        
        // Log the procedure result
        log_diagnostic_procedure(procedure_id, result);
        
        return D_REDRAW;
    }
    
    return d_button_proc(msg, d, c);
}

static int service_reset_proc(int msg, DIALOG *d, int c)
{
    if (msg == MSG_CLICK || (msg == MSG_KEY && c == d->key)) {
        int service_type = d->d1;
        
        // Find the service
        SERVICE_RESET_ITEM *service = NULL;
        for (int i = 0; available_services[i].service_type != 0; i++) {
            if (available_services[i].service_type == service_type) {
                service = &available_services[i];
                break;
            }
        }
        
        if (!service) return D_O_K;
        
        char confirm_msg[256];
        sprintf(confirm_msg, "Reset %s interval?\n\nThis will reset the service counter to zero.", 
                service->service_name);
        
        if (alert3("Service Reset", confirm_msg, NULL, "Reset", "Cancel", NULL, 'r', 'c', 0) == 2) {
            return D_REDRAW;
        }
        
        // Perform the actual service reset
        int result = 0;
        switch (service_type) {
            case MAINT_ENGINE_OIL:
                result = reset_engine_oil_service_interval();
                break;
            case MAINT_TRANSMISSION_FLUID:
                result = reset_transmission_service_interval();
                break;
            case MAINT_BRAKE_FLUID:
                result = reset_brake_service_interval();
                break;
            case MAINT_COOLANT_SYSTEM:
                result = reset_coolant_service_interval();
                break;
            default:
                result = reset_general_service_interval(service_type);
                break;
        }
        
        if (result) {
            alert("Service Reset Complete", 
                  "Service interval has been successfully reset", 
                  NULL, "OK", NULL, 0, 0);
        } else {
            alert("Service Reset Failed", 
                  "Unable to reset service interval", 
                  "Please check vehicle connection", "OK", NULL, 0, 0);
        }
        
        return D_REDRAW;
    }
    
    return d_button_proc(msg, d, c);
}

// Implementation of specific diagnostic procedures
int perform_injector_coding_wizard(void)
{
    init_diagnostic_session();
    current_diagnostic_session.active_procedure = DIAG_INJECTOR_CODING;
    current_diagnostic_session.total_steps = 4;
    
    // Step 1: Read current injector codes
    update_diagnostic_progress(1, 4, "Reading current injector data from ECU...");
    if (injector_coding_step1_read_current_codes() != 0) {
        set_diagnostic_error(1, "Failed to read current injector codes");
        return 0;
    }
    
    // Step 2: Input new injector codes
    update_diagnostic_progress(2, 4, "Please enter new injector codes...");
    if (injector_coding_step2_input_new_codes() != 0) {
        set_diagnostic_error(2, "Invalid injector codes entered");
        return 0;
    }
    
    // Step 3: Write codes to ECU
    update_diagnostic_progress(3, 4, "Programming new injector codes to ECU...");
    if (injector_coding_step3_write_codes_to_ecu() != 0) {
        set_diagnostic_error(3, "Failed to write injector codes to ECU");
        return 0;
    }
    
    // Step 4: Verify operation
    update_diagnostic_progress(4, 4, "Verifying injector coding operation...");
    if (injector_coding_step4_verify_operation() != 0) {
        set_diagnostic_error(4, "Injector coding verification failed");
        return 0;
    }
    
    complete_diagnostic_session(1);
    return 1;
}

int perform_dpf_regeneration(void)
{
    if (alert3("DPF Regeneration", 
               "This will force a diesel particulate filter regeneration.",
               "Engine must be warm and vehicle stationary. Continue?", 
               "Yes", "No", NULL, 'y', 'n', 0) == 2) {
        return 0;
    }
    
    // Check engine temperature
    alert("DPF Regeneration", 
          "Starting forced regeneration procedure", 
          "This may take up to 20 minutes", "OK", NULL, 0, 0);
    
    // In real implementation, this would send specific DPF regeneration commands
    // For now, simulate the process
    
    alert("DPF Regeneration Complete", 
          "Diesel particulate filter regeneration completed", 
          "DPF soot levels have been reset", "OK", NULL, 0, 0);
    
    return 1;
}

// Service reset implementations
int reset_engine_oil_service_interval(void)
{
    // Send UDS command to reset engine oil service interval
    unsigned char reset_cmd[] = { 0x01 };
    return uds_write_data_by_identifier(0xF010, reset_cmd, 1) == 0 ? 1 : 0;
}

int reset_transmission_service_interval(void)
{
    unsigned char reset_cmd[] = { 0x01 };
    return uds_write_data_by_identifier(0xF011, reset_cmd, 1) == 0 ? 1 : 0;
}

int reset_brake_service_interval(void)
{
    unsigned char reset_cmd[] = { 0x01 };
    return uds_write_data_by_identifier(0xF012, reset_cmd, 1) == 0 ? 1 : 0;
}

int reset_coolant_service_interval(void)
{
    unsigned char reset_cmd[] = { 0x01 };
    return uds_write_data_by_identifier(0xF013, reset_cmd, 1) == 0 ? 1 : 0;
}

int reset_general_service_interval(int service_type)
{
    unsigned char reset_cmd[] = { 0x01 };
    unsigned short did = 0xF020 + service_type; // Calculate DID based on service type
    return uds_write_data_by_identifier(did, reset_cmd, 1) == 0 ? 1 : 0;
}

// Utility functions
int verify_vehicle_compatibility(int procedure_id)
{
    // In real implementation, this would check vehicle VIN and ECU compatibility
    return 1; // Return compatible for demo
}

int check_diagnostic_prerequisites(int procedure_id)
{
    // Check various prerequisites like ignition state, engine state, etc.
    // For demo purposes, return success
    return 1;
}

void display_safety_warning(int safety_level)
{
    switch (safety_level) {
        case 1: // Low risk
            alert("Safety Notice", 
                  "This is a low-risk procedure", 
                  "Follow standard safety precautions", "OK", NULL, 0, 0);
            break;
        case 2: // Medium risk
            alert("Safety Warning", 
                  "This procedure requires caution", 
                  "Ensure vehicle is secure and stable", "OK", NULL, 0, 0);
            break;
        case 3: // High risk
            alert("SAFETY WARNING", 
                  "HIGH RISK PROCEDURE", 
                  "Only qualified technicians should proceed", "OK", NULL, 0, 0);
            break;
    }
}

void log_diagnostic_procedure(int procedure_id, int result)
{
    // Log the procedure and its result for audit trail
    FILE *log_file = fopen("diagnostic_log.txt", "a");
    if (log_file) {
        fprintf(log_file, "Procedure ID: %d, Result: %s\n", 
                procedure_id, result ? "SUCCESS" : "FAILED");
        fclose(log_file);
    }
}

// Diagnostic session management
void init_diagnostic_session(void)
{
    memset(&current_diagnostic_session, 0, sizeof(DIAGNOSTIC_SESSION));
}

void update_diagnostic_progress(int step, int total_steps, const char *instruction)
{
    current_diagnostic_session.current_step = step;
    current_diagnostic_session.total_steps = total_steps;
    current_diagnostic_session.completion_percentage = (step * 100) / total_steps;
    strncpy(current_diagnostic_session.current_instruction, instruction, 
            sizeof(current_diagnostic_session.current_instruction) - 1);
    
    // Update status display
    sprintf(current_diagnostic_session.status_message, "Step %d of %d: %s", 
            step, total_steps, instruction);
}

void set_diagnostic_error(int error_code, const char *description)
{
    current_diagnostic_session.error_code = error_code;
    strncpy(current_diagnostic_session.error_description, description,
            sizeof(current_diagnostic_session.error_description) - 1);
}

void complete_diagnostic_session(int success)
{
    if (success) {
        current_diagnostic_session.completion_percentage = 100;
        strcpy(current_diagnostic_session.status_message, "Procedure completed successfully");
    } else {
        strcpy(current_diagnostic_session.status_message, "Procedure failed");
    }
}

// Stub implementations for injector coding steps
int injector_coding_step1_read_current_codes(void) { return 0; }
int injector_coding_step2_input_new_codes(void) { return 0; }
int injector_coding_step3_write_codes_to_ecu(void) { return 0; }
int injector_coding_step4_verify_operation(void) { return 0; }

int display_maintenance_reset_menu(void)
{
    // This would display a focused maintenance reset menu
    return display_advanced_diagnostics_menu(); // For now, use the same menu
}