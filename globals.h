#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include "allegro_common.h"
#include "resource.h"

//#define LOG_COMMS   // Uncomment this line to enable serial comm logging

// system_of_measurements
#define METRIC     0
#define IMPERIAL   1
#define BRITISH    2

// Display mode flags
#define FULL_SCREEN_MODE            0
#define WINDOWED_MODE               1
#define FULLSCREEN_MODE_SUPPORTED   2
#define WINDOWED_MODE_SUPPORTED     4
#define WINDOWED_MODE_SET           8

// Password for Datafiles (Tr. Code Definitions and Resources)
#define PASSWORD NULL

// Colors are now defined in allegro_common.h to avoid conflicts

//
extern int is_not_genuine_scan_tool;

// Options
extern int system_of_measurements;
extern int display_mode;

// Enhanced features flags
extern int enhanced_mode_enabled;
extern int realtime_charts_enabled;
extern int advanced_diagnostics_enabled;

// File names
extern char options_file_name[20];
extern char data_file_name[20];
extern char code_defs_file_name[20];
extern char log_file_name[20];
#ifdef LOG_COMMS
extern char comm_log_file_name[20];
#endif

void write_log(const char *log_string);

#ifdef LOG_COMMS
void write_comm_log(const char *marker, const char *data);
#endif

extern DATAFILE *datafile;

#endif
