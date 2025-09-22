#ifndef ALLEGRO_COMMON_H
#define ALLEGRO_COMMON_H

/*
 * allegro_common.h - Single-point Allegro inclusion
 * 
 * This header provides forward declarations for all files except allegro_impl.c
 * Only allegro_impl.c should include the full allegro.h
 */

#ifdef ALLEGRO_IMPL_C
    // ONLY allegro_impl.c defines this and gets full Allegro
    #define ALLEGRO_USE_CONSOLE
    #include <allegro.h>
#else
    // All other files get forward declarations only
    struct BITMAP;
    struct FONT;
    struct DIALOG;
    struct SAMPLE;
    struct MIDI;
    
    // DATAFILE structure definition (needed for array access)
    typedef struct DATAFILE {
        void *dat;                /* the actual data */
        int type;                 /* what type of data? */
        long size;                /* how big is it? */
        void *prop;               /* object properties */
    } DATAFILE;
    
    // DIALOG structure definition (needed for dialog arrays)
    typedef struct DIALOG {
        int (*proc)(int, struct DIALOG *, int);   /* dialog procedure */
        int x, y, w, h;           /* position and size of the object */
        int fg, bg;               /* foreground and background colors */
        int key;                  /* keyboard shortcut */
        int flags;                /* flags about the object state */
        int d1, d2;               /* any data the object might require */
        void *dp, *dp2, *dp3;     /* pointers to more object data */
    } DIALOG;
    
    // Pack file structure
    typedef struct PACKFILE PACKFILE;
    
    // BITMAP structure definition (basic structure for accessing width/height)
    typedef struct BITMAP {
        int w, h;                 /* width and height in pixels */
        int clip;                 /* flag if clipping is turned on */
        int cl, cr, ct, cb;       /* clip left, right, top and bottom values */
        void *vtable;             /* drawing method vtable */
        void *write_bank;         /* C func on some machines, asm on i386 */
        void *read_bank;          /* C func on some machines, asm on i386 */
        void *dat;                /* the memory we allocated for the bitmap */
        unsigned long id;         /* for identifying sub-bitmaps */
        void *extra;              /* points to a structure with more info */
        int x_ofs;                /* horizontal offset (for sub-bitmaps) */
        int y_ofs;                /* vertical offset (for sub-bitmaps) */
        int seg;                  /* bitmap segment */
        unsigned char *line[0];   /* pointers to the start of each line */
    } BITMAP;
    
    // Note: BITMAP structure is already defined above
    
    typedef struct FONT FONT;
    typedef struct DIALOG DIALOG;
    typedef struct SAMPLE SAMPLE;
    typedef struct MIDI MIDI;
    
    // Extern declarations for global variables (defined in allegro_impl.c)
    extern BITMAP *screen;
    extern DATAFILE *datafile;
    extern const char *allegro_error;
    extern volatile int retrace_count;
    extern FONT *font;
    extern int gui_fg_color, gui_bg_color, gui_mg_color;
    
    // Serial communication functions
    int serial_port_opened(void);
    
    // Platform detection
    #ifdef _WIN32
    #define ALLEGRO_WINDOWS
    #endif
    
    // Common constants
    #define GFX_AUTODETECT_WINDOWED    1
    #define GFX_AUTODETECT_FULLSCREEN  2
    #define D_EXIT                     0x0001
    #define D_SELECTED                 0x0002
    #define D_GOTFOCUS                 0x0004
    #define D_GOTMOUSE                 0x0008
    #define D_HIDDEN                   0x0010
    #define D_DISABLED                 0x0020
    #define D_DIRTY                    0x0040
    #define D_INTERNAL                 0x0080
    #define D_CLOSE                    0x0100
    #define D_REDRAW                   0x0200
    #define D_REDRAWME                 0x0400
    
    // Dialog return codes
    #define D_O_K                      0        /* normal exit status */
    #define D_USED_CHAR                16       /* object has used the keypress */
    
    // Key constants
    #define KEY_PGUP                   0x49
    #define KEY_PGDN                   0x51
    
    // Transparent color constant
    #define C_TRANSP                   -1       /* transparent color */
    
    // Utility macros
    #define MAX(a, b)                  ((a) > (b) ? (a) : (b))
    #define MIN(a, b)                  ((a) < (b) ? (a) : (b))
    
    // Pack file constants
    #define F_READ                     "r"
    #define F_WRITE                    "w"
    #define F_READ_PACKED              "rp"
    #define F_WRITE_PACKED             "wp"
    
    // OS type constants
    #define OSTYPE_UNKNOWN             0
    #define OSTYPE_WIN3                1
    #define OSTYPE_WIN95               2
    #define OSTYPE_WIN98               3
    #define OSTYPE_WINME               4
    #define OSTYPE_WINNT               5
    #define OSTYPE_WIN2000             6
    #define OSTYPE_WINXP               7
    #define OSTYPE_UNIX                8
    #define OSTYPE_BEOS                9
    #define OSTYPE_QNX                 10
    #define OSTYPE_MACOS               11
    #define OSTYPE_OS2                 12
    #define OSTYPE_WARP                13
    #define OSTYPE_DOSEMU              14
    #define OSTYPE_OPENDOS             15
    #define OSTYPE_LINUX               16
    #define OSTYPE_FREEBSD             17
    
    // Screen constants (use extern variables to be set by Allegro)
    extern int allegro_screen_w;
    extern int allegro_screen_h;
    #define SCREEN_W                   allegro_screen_w    /* screen width */
    #define SCREEN_H                   allegro_screen_h    /* screen height */
    
    // Timer constants and macros
    #define MSEC_TO_TIMER(x)           ((x) * 1.193182)
    #define BPS_TO_TIMER(x)            (1193182L / (x))
    #define SECS_TO_TIMER(x)           ((x) * 1193182L)
    
    // Message constants for custom dialogs
    #define MSG_SAVE_OPTIONS           1000         /* save options message */
    
    // Allegro static function macros
    #define STATIC_FUNCTION(type, name, args)  static type name args
    #define END_OF_STATIC_FUNCTION(name)       /* end of static function */
    #define LOCK_VARIABLE(var)                 /* lock variable */
    #define LOCK_FUNCTION(func)                /* lock function */
    #define END_OF_FUNCTION(name)              /* end of function */
    
    // Boolean constants
    #define TRUE                       1
    #define FALSE                      0
    
    // Unicode format constants
    #define U_ASCII                    0
    #define U_ASCII_CP                 1
    #define U_UNICODE                  2
    #define U_UTF8                     3
    
    // Standard constants
    #define EXIT_SUCCESS               0
    #define EXIT_FAILURE               1
    
    // Color constants (only if not already defined by globals.h)
    #ifndef C_BLACK
    #define C_BLACK                    0
    #endif
    // Colors in the main palette (matching original ScanTool values)
    #ifndef C_TRANSP
    #define C_TRANSP                   -1
    #endif
    #ifndef C_BLACK
    #define C_BLACK                    0
    #endif
    #ifndef C_WHITE
    #define C_WHITE                    1
    #endif
    #ifndef C_RED
    #define C_RED                      255
    #endif
    #ifndef C_BLUE
    #define C_BLUE                     254
    #endif
    #ifndef C_GREEN
    #define C_GREEN                    99
    #endif
    #ifndef C_DARK_YELLOW
    #define C_DARK_YELLOW              54
    #endif
    #ifndef C_PURPLE
    #define C_PURPLE                   9
    #endif
    #ifndef C_DARK_GRAY
    #define C_DARK_GRAY                126
    #endif
    #ifndef C_GRAY
    #define C_GRAY                     50
    #endif
    #ifndef C_LIGHT_GRAY
    #define C_LIGHT_GRAY               55
    #endif
    
    // Key constants
    #define KEY_ESC                    0x01
    #define KEY_ENTER                  0x1C
    #define KEY_SPACE                  0x39
    
    // Message constants for dialog procedures
    #define MSG_START                  0
    #define MSG_END                    1
    #define MSG_DRAW                   2
    #define MSG_CLICK                  3
    #define MSG_DCLICK                 4
    #define MSG_KEY                    5
    #define MSG_CHAR                   6
    #define MSG_UCHAR                  7
    #define MSG_XCHAR                  8
    #define MSG_WANTFOCUS              9
    #define MSG_GOTFOCUS               10
    #define MSG_LOSTFOCUS              11
    #define MSG_GOTMOUSE               12
    #define MSG_LOSTMOUSE              13
    #define MSG_IDLE                   14
    #define MSG_RADIO                  15
    #define MSG_WHEEL                  16
    #define MSG_LPRESS                 17
    #define MSG_LRELEASE               18
    #define MSG_MPRESS                 19
    #define MSG_MRELEASE               20
    #define MSG_RPRESS                 21
    #define MSG_RRELEASE               22
    #define MSG_WANTMOUSE              23
    #define MSG_USER                   24
#endif

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif

// Core system functions (always available)
int init_allegro_system(void);
void cleanup_allegro_system(void);
void _add_exit_func(void (*func)(void), const char *desc);
void _remove_exit_func(void (*func)(void));
int serial_port_opened(void);

// Individual component installation
int allegro_install_timer(void);
int allegro_install_keyboard(void);
int allegro_install_mouse(void);

// Wrapper functions for Allegro functionality (always available)
int scantool_set_gfx_mode(int card, int w, int h, int v_w, int v_h);
void scantool_set_color_depth(int depth);
int scantool_readkey(void);
void scantool_yield_timeslice(void);
int scantool_alert(const char *s1, const char *s2, const char *s3, 
                   const char *b1, const char *b2, int c1, int c2);
void scantool_message(const char *msg);
int scantool_do_dialog(DIALOG *dialog, int focus_obj);
int allegro_popup_dialog(DIALOG *dialog, int focus_obj);

// Dialog procedures
int d_clear_proc(int msg, DIALOG *d, int c);
int d_bitmap_proc(int msg, DIALOG *d, int c);
int d_button_proc(int msg, DIALOG *d, int c);
int d_edit_proc(int msg, DIALOG *d, int c);
int d_text_proc(int msg, DIALOG *d, int c);
int d_yield_proc(int msg, DIALOG *d, int c);
int d_box_proc(int msg, DIALOG *d, int c);
int d_shadow_box_proc(int msg, DIALOG *d, int c);
int d_check_proc(int msg, DIALOG *d, int c);

// GUI utility functions
int alert(const char *s1, const char *s2, const char *s3, const char *b1, const char *b2, int c1, int c2);
void centre_dialog(DIALOG *dialog);
int do_dialog(DIALOG *dialog, int focus_obj);
int popup_dialog(DIALOG *dialog, int focus_obj);

// Timer functions
#ifndef ALLEGRO_IMPL_C
int install_int_ex(void (*proc)(void), int speed);
int install_int(void (*proc)(void), int speed);
void remove_int(void (*proc)(void));
int allegro_msec_to_timer(int milliseconds);
#endif

// Graphics functions
#ifndef ALLEGRO_IMPL_C
BITMAP *create_bitmap(int width, int height);
void destroy_bitmap(BITMAP *bitmap);
void blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, 
          int dest_x, int dest_y, int width, int height);
int set_gfx_mode(int card, int w, int h, int v_w, int v_h);
#endif

// Mouse functions
void scare_mouse(void);
void unscare_mouse(void);
void show_mouse(BITMAP *bmp);

// Dialog utility functions
#ifndef ALLEGRO_IMPL_C
void broadcast_dialog_message(int msg, int c);
#endif
int d_list_proc(int msg, DIALOG *d, int c);
int d_radio_proc(int msg, DIALOG *d, int c);
int d_rtext_proc(int msg, DIALOG *d, int c);
int d_textbox_proc(int msg, DIALOG *d, int c);

// Drawing functions
#ifndef ALLEGRO_IMPL_C
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void circle(BITMAP *bmp, int x, int y, int radius, int color);
void circlefill(BITMAP *bmp, int x, int y, int radius, int color);
#endif

// Timer functions
#ifndef ALLEGRO_IMPL_C
int install_int(void (*proc)(void), int speed);
#endif

// Input functions
#ifndef ALLEGRO_IMPL_C
void simulate_keypress(int key);
#endif

// GUI text functions
#ifndef ALLEGRO_IMPL_C
void gui_textout_ex(BITMAP *bmp, const char *s, int x, int y, int fg, int bg, int center);
int gui_strlen(const char *s);
#endif

// Dialog message functions
int object_message(DIALOG *dialog, int msg, int c);

// Alert functions
int alert3(const char *s1, const char *s2, const char *s3, 
           const char *b1, const char *b2, const char *b3, int c1, int c2, int c3);

    // These globals are provided by Allegro directly (declare as extern for use)
    extern char empty_string[];
    extern int os_type;
    extern int os_version;
    extern int os_revision;
    extern const char *os_name;
    extern char cpu_vendor[];
    extern int cpu_family;
    extern int cpu_model;

    // Text and drawing functions
    #ifndef ALLEGRO_IMPL_C
    int text_height(void *font);
    void _draw_textbox(char *thetext, int *listsize, int draw, int offset,
                       int wword, int tabsize, int x, int y, int w, int h,
                       int disabled, int fore, int deselect, int disable);
    void _draw_scrollable_frame(DIALOG *d, int listsize, int offset, int height, int fg_color, int bg);
    void pivot_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, int angle);
    int itofix(int x);
    #endif

    // Debug function
    void assert_function(int condition);
    #ifdef ASSERT
    #undef ASSERT
    #endif
    #define ASSERT(condition) assert_function(condition)

// Config functions  
int get_config_int(const char *section, const char *name, int def);
void set_config_int(const char *section, const char *name, int val);
void flush_config_file(void);

// Pack file functions
#ifndef ALLEGRO_IMPL_C
void packfile_password(const char *password);
PACKFILE *pack_fopen(const char *filename, const char *mode);
int pack_feof(PACKFILE *f);
int pack_fclose(PACKFILE *f);
int pack_fread(void *p, int n, PACKFILE *f);
int pack_fwrite(const void *p, int n, PACKFILE *f);
int pack_getc(PACKFILE *f);
#endif

// Allegro system access functions
const char* allegro_get_error(void);
void allegro_set_window_title(const char *title);

// Config file functions
void allegro_set_config_file(const char *filename);
const char* allegro_get_config_string(const char *section, const char *name, const char *def);
void allegro_set_config_string(const char *section, const char *name, const char *val);
void allegro_flush_config_file(void);

// Datafile functions  
void allegro_packfile_password(const char *password);
DATAFILE* allegro_load_datafile(const char *filename);
void allegro_unload_datafile(DATAFILE *dat);

// Dialog functions with various signatures
int allegro_alert3(const char *s1, const char *s2, const char *s3, 
                   const char *b1, const char *b2, const char *b3, int c1, int c2, int c3);

// Button procedure wrappers
int allegro_d_button_proc(int msg, DIALOG *d, int c);
int allegro_d_text_proc(int msg, DIALOG *d, int c);

// Bitmap functions
BITMAP* allegro_create_bitmap(int width, int height);
void allegro_destroy_bitmap(BITMAP *bitmap);
void allegro_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, 
                  int dest_x, int dest_y, int width, int height);

// GUI functions  
void allegro_gui_textout_ex(BITMAP *bmp, const FONT *f, const char *s, 
                           int x, int y, int color, int bg, int centre);

// Function declarations for Allegro functions (implemented in allegro_impl.c)
void set_uformat(int type);
void allegro_set_palette(void *palette);
void set_mouse_sprite(BITMAP *sprite);
void allegro_exit(void);
int install_timer(void);
int install_keyboard(void);
int install_mouse(void);
void unload_datafile(DATAFILE *dat);

// End of main macro
#define END_OF_MAIN()

#ifdef __cplusplus
}
#endif

#endif // ALLEGRO_COMMON_H
