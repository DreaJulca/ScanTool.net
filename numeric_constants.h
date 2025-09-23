#ifndef NUMERIC_CONSTANTS_H
#define NUMERIC_CONSTANTS_H

/*
 * Numeric Constants for ScanTool.net
 * This file centralizes all magic numbers found throughout the codebase
 * to improve code readability and maintainability.
 * Generated from CodeScope analysis on 2025-09-22
 */

/* =============================================================================
 * CPU TYPE CONSTANTS
 * ============================================================================= */
#define CPU_TYPE_486                     486
#define CPU_FAMILY_PENTIUM_14            14
#define CPU_FAMILY_PENTIUM_15            15
#define CPU_FAMILY_PENTIUM_13            13
#define CPU_FAMILY_PENTIUM_25            25
#define CPU_FAMILY_PENTIUM_18            18
#define CPU_VERSION_WIN95                95
#define CPU_VERSION_WIN98                98

/* =============================================================================
 * MENU AND UI CONSTANTS
 * ============================================================================= */
#define MENU_ITEM_FREEZE_FRAME           10
#define MENU_ITEM_TESTS                  11

/* =============================================================================
 * SENSOR PAGE CONSTANTS
 * ============================================================================= */
#define SENSOR_PAGE_SIZE                 9

/* =============================================================================
 * OBD PROTOCOL CONSTANTS
 * ============================================================================= */
#define OBD_PID_SUPPORTED_01_20          0100
#define OBD_REQUEST_MODE_01              0100

/* =============================================================================
 * RESOURCE AND BITMAP INDICES (from scantool.h)
 * ============================================================================= */
#define RESOURCE_MAIN_PALETTE_IDX        14
#define RESOURCE_FREEZE_FRAME2_IDX       10
#define RESOURCE_FREEZE_FRAME3_IDX       11
#define RESOURCE_OPTIONS2_IDX            18
#define RESOURCE_OPTIONS3_IDX            19
#define RESOURCE_TESTS2_IDX              28
#define RESOURCE_TESTS3_IDX              29

/* =============================================================================
 * HTML REPORT CONSTANTS (from reports)
 * ============================================================================= */
#define HTML_REPORT_WIDTH_333            333
#define HTML_REPORT_WIDTH_555            555
#define HTML_REPORT_CODE_495057          495057

#endif /* NUMERIC_CONSTANTS_H */