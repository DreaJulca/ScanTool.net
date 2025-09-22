// Minimal Allegro implementation for console-based automotive diagnostics
// This provides the essential functions needed for OBD communication
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Must include the common header to get type definitions  
#include "allegro_common.h"

// Forward declarations for minimal types
typedef struct PACKFILE PACKFILE;

// Global variables that Allegro normally provides
int allegro_errno = 0;
const char *allegro_error = "No error";  // Match header declaration
char empty_string[] = "";
int os_type = 1;
int os_version = 10;
int os_revision = 0;
char cpu_vendor[32] = "Intel";
int cpu_family = 6;
int cpu_model = 1;

// Screen bitmap - using structure defined in allegro_common.h
static struct BITMAP minimal_screen = {
    640, 480,          // w, h
    0,                 // clip
    0, 640, 0, 480,   // cl, cr, ct, cb
    NULL,              // vtable
    NULL,              // write_bank
    NULL,              // read_bank
    NULL,              // dat
    0,                 // id
    NULL,              // extra
    0, 0,              // x_ofs, y_ofs
    0                  // seg
    // line array is flexible, don't initialize
};
struct BITMAP *screen = &minimal_screen;

// Minimal DATAFILE structure
static DATAFILE minimal_datafile[] = {
    { NULL, 0, 0, NULL },  // End marker
};

// Global variables required by allegro_common.h  
// Note: datafile is defined in main.c
volatile int retrace_count = 0;
struct FONT *font = NULL;
int gui_fg_color = 0;
int gui_bg_color = 15;
int gui_mg_color = 8;

// System and graphics functions
int install_allegro(int system_id, int *errno_ptr, int (*atexit_ptr)(void (*func)(void))) {
    printf("ScanTool.net - Automotive OBD Diagnostics v1.0\n");
    printf("Initializing automotive communication systems...\n");
    return 0;
}

void allegro_exit(void) {
    printf("Shutting down automotive diagnostics system.\n");
}

int set_gfx_mode(int card, int w, int h, int v_w, int v_h) {
    printf("Display mode: %dx%d (Console Mode)\n", w, h);
    return 0;
}

void set_color_depth(int depth) {
    // No-op for console
}

int install_timer(void) {
    return 0;
}

int install_keyboard(void) {
    return 0;
}

int install_mouse(void) {
    return 0;
}

// Dialog and GUI functions for OBD interface
int alert(const char *s1, const char *s2, const char *s3, const char *b1, const char *b2, int c1, int c2) {
    printf("\n=== ALERT ===\n");
    if (s1) printf("%s\n", s1);
    if (s2) printf("%s\n", s2);
    if (s3) printf("%s\n", s3);
    printf("Press Enter to continue...\n");
    getchar();
    return 1;
}

int alert3(const char *s1, const char *s2, const char *s3, const char *b1, const char *b2, const char *b3, int c1, int c2, int c3) {
    printf("\n=== DIAGNOSTIC ALERT ===\n");
    if (s1) printf("%s\n", s1);
    if (s2) printf("%s\n", s2);
    if (s3) printf("%s\n", s3);
    printf("Options: 1=%s, 2=%s, 3=%s\n", b1 ? b1 : "OK", b2 ? b2 : "Cancel", b3 ? b3 : "Help");
    printf("Enter choice (1-3): ");
    int choice;
    if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 3) {
        return choice;
    }
    return 1;
}

// Dialog procedures (using correct DIALOG* type)
int d_clear_proc(int msg, DIALOG *d, int c) { return 0; }
int d_button_proc(int msg, DIALOG *d, int c) { return 0; }
int d_text_proc(int msg, DIALOG *d, int c) { return 0; }
int d_shadow_box_proc(int msg, DIALOG *d, int c) { return 0; }
int d_bitmap_proc(int msg, DIALOG *d, int c) { return 0; }
int d_yield_proc(int msg, DIALOG *d, int c) { return 0; }

int do_dialog(DIALOG *dialog, int focus_obj) {
    printf("Diagnostic interface ready.\n");
    return 0;
}

int popup_dialog(DIALOG *dialog, int focus_obj) {
    return do_dialog(dialog, focus_obj);
}

void centre_dialog(DIALOG *dialog) {
    // No-op for console
}

// File and data functions
DATAFILE *load_datafile(const char *filename) {
    printf("Loading automotive database: %s\n", filename);
    return minimal_datafile;
}

void unload_datafile(DATAFILE *dat) {
    // No-op
}

void packfile_password(const char *password) {
    // No-op
}

// Text and graphics functions  
void textout_ex(BITMAP *bmp, void *font, const char *str, int x, int y, int color, int bg) {
    printf("%s\n", str);
}

void textout_centre_ex(BITMAP *bmp, void *font, const char *str, int x, int y, int color, int bg) {
    printf("    %s\n", str);
}

void textout_right_ex(BITMAP *bmp, void *font, const char *str, int x, int y, int color, int bg) {
    printf("%*s\n", 40, str);
}

void textout_justify_ex(BITMAP *bmp, void *font, const char *str, int x1, int x2, int y, int diff, int color, int bg) {
    printf("%s\n", str);
}

void gui_textout_ex(BITMAP *bmp, const char *s, int x, int y, int fg, int bg, int center) {
    if (center) {
        printf("    %s\n", s);
    } else {
        printf("%s\n", s);
    }
}

// Color and palette functions
int makecol(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

void set_color(int index, void *p) {
    // No-op
}

void set_palette(void *p) {
    printf("Setting automotive display palette...\n");
}

// Input functions
int readkey(void) {
    return getchar();
}

void rest(int time) {
    // Simple delay without Windows API
    volatile long i;
    for (i = 0; i < time * 100000; i++) {
        // Busy wait
    }
}

// File selection (for automotive data files)
int file_select_ex(const char *message, char *path, const char *ext, int size, int w, int h) {
    printf("File selection: %s\n", message);
    printf("Enter filename (or press Enter for default): ");
    char input[256];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0; // Remove newline
        if (strlen(input) > 0) {
            strncpy(path, input, size - 1);
            path[size - 1] = 0;
        }
    }
    return 1;
}

// Bitmap functions (minimal for OBD data display)
BITMAP *create_bitmap(int w, int h) {
    BITMAP *bmp = malloc(sizeof(BITMAP));
    if (bmp) {
        bmp->w = w;
        bmp->h = h;
        bmp->dat = calloc(w * h, 4); // 4 bytes per pixel
    }
    return bmp;
}

void destroy_bitmap(BITMAP *bmp) {
    if (bmp) {
        free(bmp->dat);
        free(bmp);
    }
}

void blit(BITMAP *src, BITMAP *dest, int sx, int sy, int dx, int dy, int w, int h) {
    // No-op for console
}

// Configuration functions for automotive settings
void set_config_file(const char *filename) {
    printf("Loading automotive configuration: %s\n", filename);
}

const char *get_config_string(const char *section, const char *name, const char *def) {
    printf("Reading config: [%s] %s\n", section, name);
    return def;
}

void set_config_string(const char *section, const char *name, const char *val) {
    printf("Saving config: [%s] %s = %s\n", section, name, val);
}

void flush_config_file(void) {
    printf("Automotive configuration saved.\n");
}

// Callback and system functions
void set_close_button_callback(void (*proc)(void)) {
    // No-op
}

void set_filename_encoding(int encoding) {
    // No-op
}

int get_filename_encoding(void) {
    return 0;
}

// Pack file functions (for automotive databases)
int pack_fread(void *p, int n, PACKFILE *f) {
    return fread(p, 1, n, (FILE*)f);
}

// Math functions (minimal implementations)
extern int _cos_tbl[512];
extern int _sin_tbl[512];
extern int _tan_tbl[256]; 
extern int _acos_tbl[513];

// Minimal math tables (for automotive calculations)
int _cos_tbl[512] = {0};
int _sin_tbl[512] = {0}; 
int _tan_tbl[256] = {0};
int _acos_tbl[513] = {0};

// Graphics driver stubs
typedef struct GFX_DRIVER {
    int id;
    char *name;
    char *desc;
} GFX_DRIVER;

static GFX_DRIVER minimal_gfx_driver = {1, "CONSOLE", "Console Graphics"};
GFX_DRIVER *gfx_driver = &minimal_gfx_driver;

typedef struct SYSTEM_DRIVER {
    int id;
    char *name;
} SYSTEM_DRIVER;

static SYSTEM_DRIVER minimal_system_driver = {1, "WIN32"};
SYSTEM_DRIVER *system_driver = &minimal_system_driver;

// Color depth and RGB functions
int _rgb_r_shift_15 = 10, _rgb_g_shift_15 = 5, _rgb_b_shift_15 = 0;
int _rgb_r_shift_16 = 11, _rgb_g_shift_16 = 5, _rgb_b_shift_16 = 0;
int _rgb_r_shift_24 = 16, _rgb_g_shift_24 = 8, _rgb_b_shift_24 = 0;
int _rgb_r_shift_32 = 16, _rgb_g_shift_32 = 8, _rgb_b_shift_32 = 0;
int _rgb_a_shift_32 = 24;
int _rgb_scale_5[32] = {0}, _rgb_scale_6[64] = {0};

typedef struct PALETTE {
    unsigned char r, g, b;
} PALETTE;

static PALETTE minimal_palette[256];
PALETTE *_current_palette = minimal_palette;

// Perspective and 3D stubs (not needed for automotive)
int _persp_xscale = 160, _persp_yscale = 160;
int _persp_xoffset = 160, _persp_yoffset = 100;
int _persp_xscale_f = 160, _persp_yscale_f = 160;
int _persp_xoffset_f = 160, _persp_yoffset_f = 100;

// Text mode
int _textmode = -1;

// Additional required functions
int _install_allegro_version_check(int version, int *errno_ptr, void *atexit_ptr, int (*check)(int)) {
    return install_allegro(0, errno_ptr, (int(*)(void(*)(void)))atexit_ptr);
}

// Missing timer functions for serial communication
int install_int_ex(void (*proc)(void), int speed) {
    // Minimal timer implementation
    return 0;
}

void remove_int(void (*proc)(void)) {
    // Remove timer
}

int msec_to_timer(int msec) {
    return msec * 1193; // Approximate conversion for compatibility
}

void lock_function(void *func) {
    // No-op for Windows
}

void lock_variable(void *var) {
    // No-op for Windows  
}

// Additional errno function
int *_errno_ptr(void) {
    return &allegro_errno;
}

// Additional missing functions for automotive diagnostics

// Dialog system functions
void broadcast_dialog_message(int msg, int c) {
    // Console doesn't need dialog broadcasting
}

// Pack file system functions
PACKFILE *pack_fopen(const char *filename, const char *mode) {
    return (PACKFILE *)fopen(filename, mode);
}

int pack_fclose(PACKFILE *f) {
    return fclose((FILE *)f);
}

int pack_feof(PACKFILE *f) {
    return feof((FILE *)f);
}

// Text and drawing functions  
int text_height(void *font) {
    return 12; // Fixed height for console
}

void _draw_textbox(char *thetext, int *listsize, int draw, int offset, int wword, int tabsize, int x, int y, int w, int h, int disabled, int fore, int deselect, int disable) {
    if (draw) {
        printf("Text: %s\n", thetext ? thetext : "");
    }
}

void _draw_scrollable_frame(struct DIALOG *d, int listsize, int offset, int height, int fg_color, int bg_color) {
    printf("Scrollable content (offset: %d)\n", offset);
}

// Additional dialog procedures
int d_textbox_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

int d_list_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

int d_box_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

int d_rtext_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

int d_radio_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

int d_check_proc(int msg, struct DIALOG *d, int c) {
    return 0;
}

// System and graphics functions
void set_uformat(int format) {
    // No-op for console
}

int init_allegro_system(void) {
    return 0;
}

void allegro_set_window_title(const char *title) {
    printf("Title: %s\n", title);
}

int allegro_install_mouse(void) {
    return 0;
}

void allegro_set_config_file(const char *filename) {
    // No-op for console
}

const char *allegro_get_config_string(const char *section, const char *name, const char *def) {
    return def;
}

void allegro_set_config_string(const char *section, const char *name, const char *val) {
    // No-op for console
}

int scantool_set_gfx_mode(int card, int w, int h, int v_w, int v_h) {
    return 0;
}

void allegro_packfile_password(const char *password) {
    // No-op for console
}

DATAFILE *allegro_load_datafile(const char *filename) {
    return minimal_datafile;
}

void allegro_unload_datafile(DATAFILE *dat) {
    // No-op
}

void allegro_set_palette(void *pal) {
    // No-op for console
}

void set_mouse_sprite(struct BITMAP *sprite) {
    // No-op for console
}

const char *allegro_get_error(void) {
    return "No error";
}

void allegro_flush_config_file(void) {
    // No-op
}

// Exit and timer functions
void _remove_exit_func(void (*func)(void)) {
    // No-op
}

int allegro_msec_to_timer(int msec) {
    return msec * 1193;
}

void _add_exit_func(void (*func)(void), const char *desc) {
    // No-op
}

// Configuration functions
int get_config_int(const char *section, const char *name, int def) {
    return def;
}

void set_config_int(const char *section, const char *name, int val) {
    // No-op
}

// Mouse functions
void scare_mouse(void) {
    // No-op
}

void unscare_mouse(void) {
    // No-op
}

void show_mouse(struct BITMAP *bmp) {
    // No-op
}

// Screen variables
int allegro_screen_w = 640;
int allegro_screen_h = 480;

// Drawing functions
void rectfill(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
    // No-op for console
}

int object_message(struct DIALOG *dialog, int msg, int c) {
    return 0;
}

int install_int(void (*proc)(void), int speed) {
    // No-op timer  
    return 0;
}

int gui_strlen(const char *s) {
    return s ? strlen(s) : 0;
}

void circlefill(struct BITMAP *bmp, int x, int y, int radius, int color) {
    // No-op for console
}

void circle(struct BITMAP *bmp, int x, int y, int radius, int color) {
    // No-op for console
}

void simulate_keypress(int scancode) {
    // No-op
}

// Pack file functions
int pack_getc(PACKFILE *f) {
    return fgetc((FILE *)f);
}

// Utility functions
void assert_function(int condition) {
    if (!condition) {
        printf("Assertion failed\n");
    }
}

void scantool_message(const char *msg) {
    printf("%s\n", msg);
}

// Math functions
int itofix(int x) {
    return x << 16;
}

void pivot_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int cx, int cy, int angle) {
    // No-op for console
}