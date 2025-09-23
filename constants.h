#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * String Constants for ScanTool.net
 * This file centralizes all hardcoded strings found throughout the codebase
 * to improve maintainability and localization support.
 * Generated from CodeScope analysis on 2025-09-22
 */

/* =============================================================================
 * UI BUTTON LABELS AND CAPTIONS
 * ============================================================================= */
#define STR_ABOUT_COMPUTER           "About this &computer"
#define STR_OBD_INFORMATION          "&OBD Information"
#define STR_CREDITS                  "C&redits"
#define STR_MAIN_MENU                "Main &Menu"
#define STR_CLOSE                    "&Close"
#define STR_REFRESH                  "Refresh"

/* =============================================================================
 * ACKNOWLEDGMENTS AND CREDITS
 * ============================================================================= */
#define STR_THANKS_HEADER            "We would like to thank the following people and organizations:"
#define STR_THANKS_DELORIE           "- DJ Delorie (www.delorie.com) for the DJGPP compiler "
#define STR_THANKS_BLOODSHED         "- Bloodshed Software (www.bloodshed.net) for the Dev-C++ IDE"
#define STR_THANKS_HARGREAVES        "- Shawn Hargreaves for creating the Allegro library"
#define STR_THANKS_DZCOMM            "- Dim Zegebart and Neil Townsend for the DZComm serial library"
#define STR_THANKS_CUGNIERE          "- Julien Cugniere for his Allegro dialog editor"
#define STR_THANKS_BOTCAZOU          "- Eric Botcazou and Allegro mailing list folks for their tips and suggestions"
#define STR_THANKS_LOPEZ             "- Joaquín Mª López Muñoz (joaquin@tid.es) for listports library"
#define STR_THANKS_USERS             "- All users who provided feedback and bug reports"

/* =============================================================================
 * ABOUT DIALOG TEXT
 * ============================================================================= */
#define STR_WHAT_IS_IT               "What is it?"
#define STR_WHAT_CAN_IT_DO           "What can it do?"
#define STR_WHERE_TO_GET             "Where can I get it?"

#define STR_WHAT_IS_IT_DESC          "ScanTool.net is multi-platform, open-source software designed to work with the ElmScan and OBDLink families of OBD-II interfaces, our inexpensive alternatives to professional diagnostic scan tools."
#define STR_WHERE_TO_GET_DESC        "You can download the latest version of this software, and buy your scan tool from www.ScanTool.net. There, you can also find contact information for your local distributor, and links to third-party software."

/* =============================================================================
 * SENSOR LABELS AND DESCRIPTIONS
 * ============================================================================= */
#define STR_SENSOR_THROTTLE_POS      "Absolute Throttle Position:"
#define STR_SENSOR_ENGINE_RPM        "Engine RPM:"
#define STR_SENSOR_VEHICLE_SPEED     "Vehicle Speed:"
#define STR_SENSOR_ENGINE_LOAD       "Calculated Load Value:"
#define STR_SENSOR_TIMING_ADVANCE    "Timing Advance (Cyl. #1):"
#define STR_SENSOR_INTAKE_PRESSURE   "Intake Manifold Pressure:"
#define STR_SENSOR_AIR_FLOW          "Air Flow Rate (MAF sensor):"
#define STR_SENSOR_FUEL_SYS1         "Fuel System 1 Status:"
#define STR_SENSOR_FUEL_SYS2         "Fuel System 2 Status:"
#define STR_SENSOR_STFT_BANK1        "Short Term Fuel Trim (Bank 1):"
#define STR_SENSOR_LTFT_BANK1        "Long Term Fuel Trim (Bank 1):"
#define STR_SENSOR_STFT_BANK2        "Short Term Fuel Trim (Bank 2):"
#define STR_SENSOR_LTFT_BANK2        "Long Term Fuel Trim (Bank 2):"
#define STR_SENSOR_INTAKE_TEMP       "Intake Air Temperature:"
#define STR_SENSOR_COOLANT_TEMP      "Coolant Temperature:"
#define STR_SENSOR_FUEL_PRESSURE     "Fuel Pressure (gauge):"
#define STR_SENSOR_SEC_AIR_STATUS    "Secondary air status:"
#define STR_SENSOR_PTO_STATUS        "Power Take-Off Status:"
#define STR_SENSOR_O2_1_BANK1        "O2 Sensor 1, Bank 1:"
#define STR_SENSOR_O2_2_BANK1        "O2 Sensor 2, Bank 1:"
#define STR_SENSOR_O2_3_BANK1        "O2 Sensor 3, Bank 1:"
#define STR_SENSOR_O2_4_BANK1        "O2 Sensor 4, Bank 1:"
#define STR_SENSOR_O2_1_BANK2        "O2 Sensor 1, Bank 2:"
#define STR_SENSOR_O2_2_BANK2        "O2 Sensor 2, Bank 2:"
#define STR_SENSOR_O2_3_BANK2        "O2 Sensor 3, Bank 2:"
#define STR_SENSOR_O2_4_BANK2        "O2 Sensor 4, Bank 2:"
#define STR_SENSOR_OBD_CONFORMS      "OBD conforms to:"
#define STR_SENSOR_O2_1_BANK1_WR     "O2 Sensor 1, Bank 1 (WR):"
#define STR_SENSOR_O2_2_BANK1_WR     "O2 Sensor 2, Bank 1 (WR):"
#define STR_SENSOR_O2_3_BANK1_WR     "O2 Sensor 3, Bank 1 (WR):"
#define STR_SENSOR_O2_4_BANK1_WR     "O2 Sensor 4, Bank 1 (WR):"
#define STR_SENSOR_O2_1_BANK2_WR     "O2 Sensor 1, Bank 2 (WR):"
#define STR_SENSOR_O2_2_BANK2_WR     "O2 Sensor 2, Bank 2 (WR):"
#define STR_SENSOR_O2_3_BANK2_WR     "O2 Sensor 3, Bank 2 (WR):"
#define STR_SENSOR_O2_4_BANK2_WR     "O2 Sensor 4, Bank 2 (WR):"
#define STR_SENSOR_ENGINE_RUNTIME    "Time Since Engine Start:"
#define STR_SENSOR_FRP_RELATIVE      "FRP rel. to manifold vacuum:"
#define STR_SENSOR_FRP_WIDERANGE     "Fuel Pressure (gauge):"
#define STR_SENSOR_CMD_EGR           "Commanded EGR:"
#define STR_SENSOR_EGR_ERROR         "EGR Error:"
#define STR_SENSOR_EVAP_PURGE        "Commanded Evaporative Purge:"
#define STR_SENSOR_FUEL_LEVEL        "Fuel Level Input:"
#define STR_SENSOR_WARMUPS           "Warm-ups since ECU reset:"
#define STR_SENSOR_DISTANCE_RESET    "Distance since ECU reset:"

/* =============================================================================
 * MENU DESCRIPTIONS AND TOOLTIPS
 * ============================================================================= */
#define STR_MENU_MOUSE_HINT          "Roll mouse cursor over menu buttons to see their descriptions."
#define STR_MENU_CODES_DESC          "Read codes and their definitions, turn off MIL and erase diagnostic test data."
#define STR_MENU_SENSORS_DESC        "Display current sensor data (RPM, Engine Load, Coolant Temperature, Speed, etc.)"
#define STR_MENU_FREEZE_FRAME_DESC   "Display freeze frame data (not implemented in this version)."
#define STR_MENU_TESTS_DESC          "Display mode 5, 6, & 7 test results (not implemented in this version)."
#define STR_MENU_OPTIONS_DESC        "Configure program options."
#define STR_MENU_ABOUT_DESC          "Learn more about this program, and find out where you can buy the OBD-II interface."

/* =============================================================================
 * ERROR MESSAGES
 * ============================================================================= */
#define STR_ERROR_PORT_NOT_OPENED    "COM Port could not be opened."
#define STR_ERROR_PORT_NOT_SELECTED  "COM Port has not been selected."
#define STR_ERROR_FEATURE_NOT_IMPL   "This feature is not implemented"
#define STR_ERROR_INTERFACE_NOT_FOUND "Interface was not found"
#define STR_ERROR_CONNECTION_LOST    "Connection with interface lost"
#define STR_ERROR_PROTOCOL_DETECT    "Protocol could not be detected."
#define STR_ERROR_DISPLAY_MODE       "Error switching display modes."
#define STR_ERROR_OPTIONS_SAVE       "Options could not be saved, because file"
#define STR_ERROR_MEM_COMPORT_STRINGS "Could not allocate memory for comport_list_strings."
#define STR_ERROR_MEM_COMPORT_NUMBERS "Could not allocate memory for comport_list_numbers."

/* =============================================================================
 * STATUS MESSAGES
 * ============================================================================= */
#define STR_STATUS_SHUTTING_DOWN     "\n\nShutting Down All Modules...\n----------------------------"
#define STR_STATUS_RESET_HARDWARE    "Resetting hardware interface..."
#define STR_STATUS_CHECKING_GENUINE  "Checking if scan tool is genuine..."
#define STR_STATUS_ECU_TIMEOUT       "Waiting for ECU timeout..."
#define STR_STATUS_DETECTING_PROTOCOL "Detecting OBD protocol..."

/* =============================================================================
 * CONFIGURATION OPTIONS
 * ============================================================================= */
#define STR_CONFIG_MEASUREMENTS      "System Of Measurements:"
#define STR_CONFIG_RESET_WARNING     "This operation may cause scan tool to stop responding."
#define STR_CONFIG_MEASUREMENTS_KEY  "system_of_measurements"

/* =============================================================================
 * CONFIRMATION DIALOGS
 * ============================================================================= */
#define STR_CONFIRM_EXIT             "Do you really want to exit?"

/* =============================================================================
 * CPU TYPE STRINGS (for about.c)
 * ============================================================================= */
#define STR_CPU_UNKNOWN              " Unknown"
#define STR_CPU_8086                 " 8086"
#define STR_CPU_8088                 " 8088"
#define STR_CPU_80186                " 80186"
#define STR_CPU_80188                " 80188"
#define STR_CPU_80286                " 80286"
#define STR_CPU_80386_DX             " 80386 DX"
#define STR_CPU_80386_SX             " 80386 SX"
#define STR_CPU_80486_DX             " 80486 DX"
#define STR_CPU_80486_SX             " 80486 SX"
#define STR_CPU_PENTIUM              " Pentium"
#define STR_CPU_PENTIUM_PRO          " Pentium Pro"
#define STR_CPU_PENTIUM_II           " Pentium II"
#define STR_CPU_CELERON              " Celeron"
#define STR_CPU_PENTIUM_III_KATMAI   " Pentium III (Katmai)"
#define STR_CPU_PENTIUM_III_COPPER   " Pentium III (Coppermine)"
#define STR_CPU_PENTIUM_IV           " Pentium IV or better"
#define STR_CPU_ATHLON_THUNDERBIRD   " Athlon (Thunderbird)"
#define STR_CPU_ATHLON_25UM          " Athlon (25 um)"
#define STR_CPU_ATHLON_18UM          " Athlon (18 um)"
#define STR_CPU_ATHLON_PALAMINO      " Athlon (Palamino)"
#define STR_CPU_DURON                " Duron"
#define STR_CPU_DURON_MORGAN         " Duron (Morgan)"
#define STR_CPU_MEDIAGX_MMX          " MediaGX MMX Enhanced"

/* =============================================================================
 * FORMATTING STRINGS
 * ============================================================================= */
#define STR_FORMAT_SPACING_LONG      ",                     "
#define STR_FORMAT_SPACING_MED       ",                      "

/* =============================================================================
 * INCLUDE FILENAMES (typically in header files)
 * ============================================================================= */
#define STR_INCLUDE_TROUBLE_READER   "trouble_code_reader.h"

/* =============================================================================
 * SERIAL COMMUNICATION CONSTANTS
 * ============================================================================= */
#define STR_SERIAL_MODULE_SHUTDOWN   "serial_module_shutdown"
#define STR_COM_PORT_FORMAT          "\\\\.\\COM%i"
#define STR_COMMAND_FORMAT           "%s\r"
#define STR_LOG_TX_ERROR             "TX ERROR"
#define STR_LOG_TX                   "TX"
#define STR_LOG_RX                   "RX"
#define STR_SERIAL_QUERY             "?\r"

#endif /* CONSTANTS_H */