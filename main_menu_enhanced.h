#ifndef MAIN_MENU_ENHANCED_H
#define MAIN_MENU_ENHANCED_H

#include "main_menu.h"

// Enhanced menu item definitions
#define ECU_PROGRAMMING           12
#define REALTIME_CHARTS          13  
#define ADVANCED_DIAGNOSTICS     14
#define MAINTENANCE_SERVICES     15
#define FIRMWARE_UPDATE          16

// Function prototypes for new menu items
static int ecu_programming_proc(int msg, DIALOG *d, int c);
static int realtime_charts_proc(int msg, DIALOG *d, int c);
static int advanced_diagnostics_proc(int msg, DIALOG *d, int c);
static int maintenance_services_proc(int msg, DIALOG *d, int c);
static int firmware_update_proc(int msg, DIALOG *d, int c);

// Enhanced main menu display
int display_enhanced_main_menu();

#endif