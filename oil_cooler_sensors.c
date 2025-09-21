#include "allegro_common.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
#include "oil_cooler_sensors.h"
#include "sensors.h"
#include "serial_enhanced.h"
#include "custom_gui.h"

OIL_COOLER_CONFIG oil_cooler_config;

// Dialog for oil cooler sensor setup
static DIALOG oil_cooler_setup_dialog[] = {
    /* (proc)              (x)  (y)  (w)  (h)  (fg)      (bg)         (key) (flags) (d1) (d2) (dp)                    (dp2) (dp3) */
    { d_clear_proc,        0,   0,   0,   0,   0,        C_WHITE,     0,    0,      0,   0,   NULL,                   NULL, NULL },
    { d_shadow_box_proc,   50,  50,  540, 350, C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   NULL,                   NULL, NULL },
    { d_text_proc,         60,  60,  300, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Oil Cooler Sensor Setup", NULL, NULL },
    
    // Oil temperature section
    { d_text_proc,         70,  90,  200, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Oil Temperature Sensor:", NULL, NULL },
    { d_check_proc,        70,  115, 200, 20,  C_BLACK,  C_WHITE,     't',  0,      1,   0,   "Enable Oil Temp Monitoring", NULL, NULL },
    { d_button_proc,       70,  140, 150, 25,  C_BLACK,  C_WHITE,     'c',  D_EXIT, 0,   0,   "Calibrate Temp",       NULL, NULL },
    
    // Oil pressure section  
    { d_text_proc,         70,  180, 200, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Oil Pressure Sensor:", NULL, NULL },
    { d_check_proc,        70,  205, 200, 20,  C_BLACK,  C_WHITE,     'p',  0,      1,   0,   "Enable Oil Pressure Monitoring", NULL, NULL },
    { d_button_proc,       70,  230, 150, 25,  C_BLACK,  C_WHITE,     'r',  D_EXIT, 0,   0,   "Calibrate Pressure",   NULL, NULL },
    
    // Alert thresholds
    { d_text_proc,         320, 90,  200, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Alert Thresholds:", NULL, NULL },
    { d_text_proc,         330, 115, 120, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Temp Warning:", NULL, NULL },
    { d_edit_proc,         450, 115, 60,  20,  C_BLACK,  C_WHITE,     0,    0,      3,   0,   "220",                  NULL, NULL },
    { d_text_proc,         330, 140, 120, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Temp Critical:", NULL, NULL },
    { d_edit_proc,         450, 140, 60,  20,  C_BLACK,  C_WHITE,     0,    0,      3,   0,   "240",                  NULL, NULL },
    { d_text_proc,         330, 165, 120, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Pressure Warning:", NULL, NULL },
    { d_edit_proc,         450, 165, 60,  20,  C_BLACK,  C_WHITE,     0,    0,      3,   0,   "15",                   NULL, NULL },
    { d_text_proc,         330, 190, 120, 20,  C_BLACK,  C_LIGHT_GRAY, 0,   0,      0,   0,   "Pressure Critical:", NULL, NULL },
    { d_edit_proc,         450, 190, 60,  20,  C_BLACK,  C_WHITE,     0,    0,      3,   0,   "10",                   NULL, NULL },
    
    // Buttons
    { d_button_proc,       200, 350, 80,  30,  C_BLACK,  C_WHITE,     's',  D_EXIT, 0,   0,   "Save",                 NULL, NULL },
    { d_button_proc,       300, 350, 80,  30,  C_BLACK,  C_WHITE,     27,   D_EXIT, 0,   0,   "Cancel",               NULL, NULL },
    { d_yield_proc,        0,   0,   0,   0,   0,        0,           0,    0,      0,   0,   NULL,                   NULL, NULL },
    { NULL,                0,   0,   0,   0,   0,        0,           0,    0,      0,   0,   NULL,                   NULL, NULL }
};

int setup_oil_cooler_sensors(void)
{
    // Initialize default configuration
    memset(&oil_cooler_config, 0, sizeof(OIL_COOLER_CONFIG));
    
    // Set default values
    oil_cooler_config.temp_warning_threshold = 220;    // °F
    oil_cooler_config.temp_critical_threshold = 240;   // °F
    oil_cooler_config.pressure_warning_threshold = 15; // PSI
    oil_cooler_config.pressure_critical_threshold = 10; // PSI
    
    // Default temperature sensor calibration (typical oil temp sensor)
    oil_cooler_config.temp_cal.voltage_min = 0.5f;    // 0.5V
    oil_cooler_config.temp_cal.voltage_max = 4.5f;    // 4.5V  
    oil_cooler_config.temp_cal.temp_min = 100.0f;     // 100°F at 0.5V
    oil_cooler_config.temp_cal.temp_max = 300.0f;     // 300°F at 4.5V
    
    // Default pressure sensor calibration (0-100 PSI sensor)
    oil_cooler_config.pressure_cal.voltage_min = 0.5f;    // 0.5V
    oil_cooler_config.pressure_cal.voltage_max = 4.5f;    // 4.5V
    oil_cooler_config.pressure_cal.pressure_min = 0.0f;   // 0 PSI at 0.5V
    oil_cooler_config.pressure_cal.pressure_max = 100.0f; // 100 PSI at 4.5V
    
    // Try to load existing calibration
    load_oil_sensor_calibration();
    
    return 0;
}

int display_oil_cooler_setup_dialog(void)
{
    centre_dialog(oil_cooler_setup_dialog);
    
    // Set current values in dialog
    oil_cooler_setup_dialog[4].flags = oil_cooler_config.oil_temp_enabled ? D_SELECTED : 0;
    oil_cooler_setup_dialog[7].flags = oil_cooler_config.oil_pressure_enabled ? D_SELECTED : 0;
    
    int result = do_dialog(oil_cooler_setup_dialog, -1);
    
    if (result == 16) { // Save button
        // Update configuration from dialog
        oil_cooler_config.oil_temp_enabled = oil_cooler_setup_dialog[4].flags & D_SELECTED;
        oil_cooler_config.oil_pressure_enabled = oil_cooler_setup_dialog[7].flags & D_SELECTED;
        
        // Get threshold values
        oil_cooler_config.temp_warning_threshold = atoi((char*)oil_cooler_setup_dialog[11].dp);
        oil_cooler_config.temp_critical_threshold = atoi((char*)oil_cooler_setup_dialog[13].dp);
        oil_cooler_config.pressure_warning_threshold = atoi((char*)oil_cooler_setup_dialog[15].dp);
        oil_cooler_config.pressure_critical_threshold = atoi((char*)oil_cooler_setup_dialog[17].dp);
        
        save_oil_sensor_calibration();
        
        alert("Oil Cooler Setup", "Configuration saved successfully", 
              "Oil sensors are now configured", "OK", NULL, 0, 0);
    }
    
    return result;
}

int calibrate_oil_temperature_sensor(void)
{
    if (alert3("Oil Temperature Sensor Calibration",
               "This will calibrate your oil temperature sensor.",
               "You'll need to measure actual oil temperature.", 
               "Continue", "Cancel", NULL, 'c', 'c', 0) == 2) {
        return 0;
    }
    
    // Step 1: Read low temperature point
    alert("Calibration Step 1", 
          "Start engine and let it reach normal operating temperature.",
          "Record the actual oil temperature and press OK", "OK", NULL, 0, 0);
    
    float low_voltage = 2.1f; // Simulate reading voltage from ECU
    char temp_input[16] = "180";
    
    if (alert3("Low Temperature Point", 
               "Enter the actual oil temperature (°F):", temp_input,
               "OK", "Cancel", NULL, 'o', 'c', 0) == 2) {
        return 0;
    }
    
    float low_temp = atof(temp_input);
    
    // Step 2: Read high temperature point  
    alert("Calibration Step 2",
          "Drive the vehicle to warm up the oil further.",
          "Record a higher oil temperature and press OK", "OK", NULL, 0, 0);
    
    float high_voltage = 3.2f; // Simulate reading higher voltage
    strcpy(temp_input, "220");
    
    if (alert3("High Temperature Point",
               "Enter the actual oil temperature (°F):", temp_input,
               "OK", "Cancel", NULL, 'o', 'c', 0) == 2) {
        return 0;
    }
    
    float high_temp = atof(temp_input);
    
    // Calculate calibration
    oil_cooler_config.temp_cal.voltage_min = low_voltage;
    oil_cooler_config.temp_cal.voltage_max = high_voltage;
    oil_cooler_config.temp_cal.temp_min = low_temp;
    oil_cooler_config.temp_cal.temp_max = high_temp;
    oil_cooler_config.temp_cal.calibrated = 1;
    
    alert("Calibration Complete",
          "Oil temperature sensor has been calibrated",
          NULL, "OK", NULL, 0, 0);
    
    return 1;
}

int calibrate_oil_pressure_sensor(void)
{
    if (alert3("Oil Pressure Sensor Calibration",
               "This will calibrate your oil pressure sensor.",
               "You'll need a known good pressure gauge for reference.",
               "Continue", "Cancel", NULL, 'c', 'c', 0) == 2) {
        return 0;
    }
    
    // Step 1: Engine off (low pressure)
    alert("Calibration Step 1",
          "Turn off engine and wait for oil pressure to drop to zero.",
          "Press OK when pressure gauge reads 0 PSI", "OK", NULL, 0, 0);
    
    float low_voltage = 0.5f; // Simulate reading from ECU
    
    // Step 2: Engine running (normal pressure)
    alert("Calibration Step 2", 
          "Start engine and let it idle.",
          "Record the oil pressure and press OK", "OK", NULL, 0, 0);
    
    float high_voltage = 3.8f; // Simulate reading higher voltage
    char pressure_input[16] = "45";
    
    if (alert3("Normal Pressure Reading",
               "Enter the actual oil pressure (PSI):", pressure_input,
               "OK", "Cancel", NULL, 'o', 'c', 0) == 2) {
        return 0;
    }
    
    float normal_pressure = atof(pressure_input);
    
    // Calculate calibration  
    oil_cooler_config.pressure_cal.voltage_min = low_voltage;
    oil_cooler_config.pressure_cal.voltage_max = high_voltage;
    oil_cooler_config.pressure_cal.pressure_min = 0.0f;
    oil_cooler_config.pressure_cal.pressure_max = normal_pressure;
    oil_cooler_config.pressure_cal.calibrated = 1;
    
    alert("Calibration Complete",
          "Oil pressure sensor has been calibrated",
          NULL, "OK", NULL, 0, 0);
    
    return 1;
}

int read_oil_temperature(float *temperature)
{
    if (!oil_cooler_config.oil_temp_enabled || !oil_cooler_config.temp_cal.calibrated) {
        return -1;
    }
    
    // Read raw voltage from ECU (in real implementation, this would be an actual ECU read)
    unsigned char response[8];
    int length;
    
    if (uds_read_data_by_identifier(OIL_TEMP_SENSOR_PID, response, &length) != 0) {
        return -1; // Communication error
    }
    
    // Convert raw ADC value to voltage (assuming 12-bit ADC, 5V reference)
    float voltage = ((response[0] << 8) | response[1]) * 5.0f / 4095.0f;
    
    // Apply calibration to get temperature
    float voltage_range = oil_cooler_config.temp_cal.voltage_max - oil_cooler_config.temp_cal.voltage_min;
    float temp_range = oil_cooler_config.temp_cal.temp_max - oil_cooler_config.temp_cal.temp_min;
    
    *temperature = oil_cooler_config.temp_cal.temp_min + 
                   ((voltage - oil_cooler_config.temp_cal.voltage_min) / voltage_range) * temp_range;
    
    return 0;
}

int read_oil_pressure(float *pressure)
{
    if (!oil_cooler_config.oil_pressure_enabled || !oil_cooler_config.pressure_cal.calibrated) {
        return -1;
    }
    
    // Read raw voltage from ECU
    unsigned char response[8];
    int length;
    
    if (uds_read_data_by_identifier(OIL_PRESSURE_SENSOR_PID, response, &length) != 0) {
        return -1;
    }
    
    // Convert raw ADC value to voltage
    float voltage = ((response[0] << 8) | response[1]) * 5.0f / 4095.0f;
    
    // Apply calibration to get pressure
    float voltage_range = oil_cooler_config.pressure_cal.voltage_max - oil_cooler_config.pressure_cal.voltage_min;
    float pressure_range = oil_cooler_config.pressure_cal.pressure_max - oil_cooler_config.pressure_cal.pressure_min;
    
    *pressure = oil_cooler_config.pressure_cal.pressure_min +
                ((voltage - oil_cooler_config.pressure_cal.voltage_min) / voltage_range) * pressure_range;
    
    return 0;
}

void oil_temp_formula(int raw_data, char *buf)
{
    float temperature;
    
    if (read_oil_temperature(&temperature) == 0) {
        if (system_of_measurements == METRIC) {
            float temp_c = (temperature - 32.0f) * 5.0f / 9.0f;
            sprintf(buf, "%.1f°C", temp_c);
        } else {
            sprintf(buf, "%.1f°F", temperature);
        }
    } else {
        sprintf(buf, "N/A");
    }
}

void oil_pressure_formula(int raw_data, char *buf)
{
    float pressure;
    
    if (read_oil_pressure(&pressure) == 0) {
        if (system_of_measurements == METRIC) {
            float pressure_kpa = pressure * 6.895f; // Convert PSI to kPa
            sprintf(buf, "%.1f kPa", pressure_kpa);
        } else {
            sprintf(buf, "%.1f PSI", pressure);
        }
    } else {
        sprintf(buf, "N/A");
    }
}

void check_oil_sensor_alerts(void)
{
    float temperature, pressure;
    
    // Check oil temperature alerts
    if (oil_cooler_config.oil_temp_enabled && read_oil_temperature(&temperature) == 0) {
        if (temperature >= oil_cooler_config.temp_critical_threshold) {
            alert("CRITICAL OIL TEMPERATURE", 
                  "Oil temperature is dangerously high!",
                  "Stop driving immediately!", "OK", NULL, 0, 0);
        } else if (temperature >= oil_cooler_config.temp_warning_threshold) {
            alert("Oil Temperature Warning",
                  "Oil temperature is higher than normal",
                  "Monitor closely and consider stopping", "OK", NULL, 0, 0);
        }
    }
    
    // Check oil pressure alerts  
    if (oil_cooler_config.oil_pressure_enabled && read_oil_pressure(&pressure) == 0) {
        if (pressure <= oil_cooler_config.pressure_critical_threshold) {
            alert("CRITICAL OIL PRESSURE",
                  "Oil pressure is dangerously low!",
                  "Stop engine immediately!", "OK", NULL, 0, 0);
        } else if (pressure <= oil_cooler_config.pressure_warning_threshold) {
            alert("Oil Pressure Warning", 
                  "Oil pressure is lower than normal",
                  "Check oil level and monitor closely", "OK", NULL, 0, 0);
        }
    }
}

void add_oil_sensors_to_sensor_list(void)
{
    // This function would integrate oil sensors into the main sensor display
    // In a full implementation, you would add these to the sensors[] array in sensors.c
}

int save_oil_sensor_calibration(void)
{
    FILE *cal_file = fopen("oil_sensors.cfg", "wb");
    if (!cal_file) return 0;
    
    fwrite(&oil_cooler_config, sizeof(OIL_COOLER_CONFIG), 1, cal_file);
    fclose(cal_file);
    return 1;
}

int load_oil_sensor_calibration(void)
{
    FILE *cal_file = fopen("oil_sensors.cfg", "rb");
    if (!cal_file) return 0;
    
    fread(&oil_cooler_config, sizeof(OIL_COOLER_CONFIG), 1, cal_file);
    fclose(cal_file);
    return 1;
}