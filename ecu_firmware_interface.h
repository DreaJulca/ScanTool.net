#ifndef ECU_FIRMWARE_INTERFACE_H
#define ECU_FIRMWARE_INTERFACE_H

// ECU Firmware Integration Interface
// This module provides integration with your custom ECU firmware project

#include "globals.h"

// Firmware file formats
#define FIRMWARE_FORMAT_HEX           1
#define FIRMWARE_FORMAT_BIN           2
#define FIRMWARE_FORMAT_S19           3
#define FIRMWARE_FORMAT_ELF           4

// Programming methods
#define PROG_METHOD_BOOTLOADER        1
#define PROG_METHOD_JTAG              2
#define PROG_METHOD_SWD               3
#define PROG_METHOD_CAN_BOOTLOADER    4

// ECU memory regions
#define MEMORY_REGION_FLASH           0x08000000
#define MEMORY_REGION_RAM             0x20000000
#define MEMORY_REGION_EEPROM          0x08080000
#define MEMORY_REGION_CALIBRATION     0x08060000

// Firmware project structure
typedef struct {
    char project_name[64];
    char version_string[32];
    char build_date[32];
    char git_commit[64];
    unsigned int flash_start_address;
    unsigned int flash_size;
    unsigned int checksum;
    char description[256];
} FIRMWARE_INFO;

// Build configuration
typedef struct {
    char build_tool_path[256];      // Path to build system (make, cmake, etc.)
    char project_directory[256];    // ECU project root directory
    char output_directory[256];     // Build output directory
    char target_config[64];         // Build target (Debug, Release, etc.)
    char compiler_flags[512];       // Additional compiler flags
    int auto_build_enabled;         // Auto-build before programming
} BUILD_CONFIG;

// Programming session
typedef struct {
    int method;                     // Programming method
    int progress_percent;          // Current progress
    char status_message[256];      // Current status
    char interface_device[64];     // Programming interface device
    unsigned int baud_rate;        // Communication baud rate
    int erase_before_program;      // Erase flash before programming
    int verify_after_program;      // Verify programming success
} PROGRAMMING_SESSION;

// Function prototypes

// Project management
int load_ecu_project(const char *project_path);
int build_ecu_firmware(void);
int get_firmware_info(FIRMWARE_INFO *info);
int validate_firmware_file(const char *firmware_path);

// Build system integration
int configure_build_system(BUILD_CONFIG *config);
int execute_build_command(const char *command);
int monitor_build_progress(void);
int get_build_output(char *output_buffer, int buffer_size);

// Firmware programming
int display_firmware_programming_dialog(void);
int program_ecu_firmware(const char *firmware_file, int method);
int erase_ecu_flash(void);
int verify_ecu_programming(void);
int read_ecu_firmware(const char *output_file);

// Bootloader communication
int enter_bootloader_mode(void);
int exit_bootloader_mode(void);
int bootloader_erase_sector(unsigned int address);
int bootloader_write_page(unsigned int address, unsigned char *data, int length);
int bootloader_read_page(unsigned int address, unsigned char *data, int length);
int bootloader_verify_checksum(void);

// Development tools
int display_memory_editor(void);
int display_calibration_editor(void);
int live_parameter_monitoring(void);
int real_time_debugging(void);

// File format handlers
int parse_hex_file(const char *filename, unsigned char **data, unsigned int *size, unsigned int *start_addr);
int parse_bin_file(const char *filename, unsigned char **data, unsigned int *size);
int parse_s19_file(const char *filename, unsigned char **data, unsigned int *size, unsigned int *start_addr);
int write_hex_file(const char *filename, unsigned char *data, unsigned int size, unsigned int start_addr);

// Repository integration
int git_get_current_commit(char *commit_hash);
int git_get_branch_name(char *branch_name);
int git_check_uncommitted_changes(void);
int git_tag_firmware_version(const char *version_tag);

// Version management
int compare_firmware_versions(const char *version1, const char *version2);
int create_firmware_backup(const char *version_label);
int restore_firmware_backup(const char *version_label);
int list_firmware_versions(char **version_list, int *count);

// Calibration data management
int read_calibration_data(unsigned int address, unsigned char *data, int length);
int write_calibration_data(unsigned int address, unsigned char *data, int length);
int backup_calibration_data(const char *backup_file);
int restore_calibration_data(const char *backup_file);
int compare_calibration_data(const char *file1, const char *file2);

// Hardware abstraction
int detect_programming_interface(void);
int configure_programming_interface(const char *device_name);
int test_programming_connection(void);
int get_programming_interface_info(char *info_buffer);

// Security and protection
int unlock_ecu_flash(void);
int lock_ecu_flash(void);
int read_ecu_security_status(void);
int generate_unlock_sequence(unsigned char *unlock_data);

// Progress and status callbacks
typedef void (*progress_callback_t)(int percent, const char *message);
typedef void (*error_callback_t)(int error_code, const char *error_message);

void set_programming_progress_callback(progress_callback_t callback);
void set_programming_error_callback(error_callback_t callback);

// Utility functions
unsigned int calculate_checksum(unsigned char *data, unsigned int length);
int verify_checksum(unsigned char *data, unsigned int length, unsigned int expected);
void format_hex_address(unsigned int address, char *formatted);
void format_file_size(unsigned int size, char *formatted);

// Configuration file management
int save_ecu_project_config(const char *config_file);
int load_ecu_project_config(const char *config_file);
int get_default_config_path(char *config_path);

// Error codes
#define ECU_FW_SUCCESS                0
#define ECU_FW_ERROR_FILE_NOT_FOUND   1
#define ECU_FW_ERROR_INVALID_FORMAT   2
#define ECU_FW_ERROR_CHECKSUM_FAILED  3
#define ECU_FW_ERROR_COMMUNICATION    4
#define ECU_FW_ERROR_PROGRAMMING      5
#define ECU_FW_ERROR_VERIFICATION     6
#define ECU_FW_ERROR_BUILD_FAILED     7
#define ECU_FW_ERROR_SECURITY_LOCKED  8

extern BUILD_CONFIG current_build_config;
extern PROGRAMMING_SESSION current_programming_session;
extern FIRMWARE_INFO current_firmware_info;

#endif