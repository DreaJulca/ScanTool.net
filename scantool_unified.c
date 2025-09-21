/*
 * scantool_unified.c - Single compilation unit for ScanTool.net
 * 
 * This file includes all source files to create a single compilation unit
 * and avoid multiple definition errors with Allegro inline functions.
 */

// Include Allegro implementation first
#include "allegro_impl.c"

// Include all other source files
#include "about.c"
#include "advanced_diagnostics.c"
#include "custom_gui.c"
#include "ecu_programming.c"
#include "enhanced_communication.c"
#include "error_handlers.c"
#include "get_port_names.c"
#include "listports.c"
#include "main.c"
#include "main_menu.c"
#include "main_menu_enhanced.c"
#include "oil_cooler_sensors.c"
#include "options.c"
#include "realtime_charts.c"
#include "reset.c"
#include "sensors.c"
#include "serial.c"
#include "serial_enhanced.c"
#include "trouble_code_reader.c"
