#ifndef OIL_COOLER_SENSORS_H
#define OIL_COOLER_SENSORS_H

#include "globals.h"

// Oil cooler specific sensor definitions
#define OIL_TEMP_SENSOR_PID           0xF1A0  // Custom PID for oil temperature
#define OIL_PRESSURE_SENSOR_PID       0xF1A1  // Custom PID for oil pressure

// Sensor calibration data
typedef struct {
    float voltage_min;      // Minimum sensor voltage (0V)
    float voltage_max;      // Maximum sensor voltage (5V)
    float temp_min;         // Temperature at min voltage (°F or °C)
    float temp_max;         // Temperature at max voltage
    float pressure_min;     // Pressure at min voltage (PSI or kPa)
    float pressure_max;     // Pressure at max voltage
    int calibrated;         // Is this sensor calibrated?
} OIL_SENSOR_CALIBRATION;

// Oil cooler monitoring configuration
typedef struct {
    int oil_temp_enabled;
    int oil_pressure_enabled;
    int temp_warning_threshold;     // °F or °C
    int temp_critical_threshold;
    int pressure_warning_threshold; // PSI or kPa
    int pressure_critical_threshold;
    int alert_enabled;
    OIL_SENSOR_CALIBRATION temp_cal;
    OIL_SENSOR_CALIBRATION pressure_cal;
} OIL_COOLER_CONFIG;

// Function prototypes
int setup_oil_cooler_sensors(void);
int calibrate_oil_temperature_sensor(void);
int calibrate_oil_pressure_sensor(void);
int read_oil_temperature(float *temperature);
int read_oil_pressure(float *pressure);
void oil_temp_formula(int raw_data, char *buf);
void oil_pressure_formula(int raw_data, char *buf);
int display_oil_cooler_setup_dialog(void);
int save_oil_sensor_calibration(void);
int load_oil_sensor_calibration(void);

// Real-time monitoring
int start_oil_cooler_monitoring(void);
void stop_oil_cooler_monitoring(void);
void check_oil_sensor_alerts(void);

// Integration with existing sensor system
void add_oil_sensors_to_sensor_list(void);

extern OIL_COOLER_CONFIG oil_cooler_config;

#endif