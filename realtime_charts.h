#ifndef REALTIME_CHARTS_H
#define REALTIME_CHARTS_H

#include "globals.h"

// Chart types
#define CHART_TYPE_LINE            1
#define CHART_TYPE_BAR             2
#define CHART_TYPE_GAUGE           3
#define CHART_TYPE_DIGITAL         4

// Chart update rates (Hz)
#define CHART_RATE_1HZ             1
#define CHART_RATE_5HZ             5
#define CHART_RATE_10HZ            10
#define CHART_RATE_20HZ            20

// Maximum data points to store for scrolling charts
#define MAX_CHART_POINTS           1000

// Chart colors
#define CHART_COLOR_RPM            C_RED
#define CHART_COLOR_SPEED          C_BLUE  
#define CHART_COLOR_COOLANT        C_GREEN
#define CHART_COLOR_THROTTLE       C_PURPLE
#define CHART_COLOR_LOAD           C_DARK_YELLOW

typedef struct {
    float values[MAX_CHART_POINTS];
    int current_index;
    int data_count;
    float min_value;
    float max_value;
    float current_value;
    char label[32];
    char units[16];
    int color;
    int visible;
} CHART_DATA_SERIES;

typedef struct {
    int chart_type;
    int x, y, width, height;
    char title[64];
    CHART_DATA_SERIES *series[8];  // Support up to 8 data series per chart
    int series_count;
    int update_rate_hz;
    int auto_scale;
    float y_min, y_max;
    int grid_enabled;
    int legend_enabled;
    int time_window_seconds;
} REALTIME_CHART;

typedef struct {
    REALTIME_CHART *charts[16];  // Support up to 16 charts
    int chart_count;
    int active_chart;
    int recording_enabled;
    int playback_mode;
    char recording_filename[256];
    FILE *recording_file;
    int total_layout_types;
    int current_layout;
} CHART_MANAGER;

// Layout presets
#define LAYOUT_SINGLE_CHART        0
#define LAYOUT_QUAD_CHARTS         1  
#define LAYOUT_ENGINE_OVERVIEW     2
#define LAYOUT_TRANSMISSION_FOCUS  3
#define LAYOUT_EMISSIONS_MONITOR   4
#define LAYOUT_CUSTOM              5

// Function prototypes
int display_realtime_charts(void);
REALTIME_CHART* create_chart(int type, int x, int y, int w, int h, const char *title);
CHART_DATA_SERIES* add_data_series(REALTIME_CHART *chart, const char *label, const char *units, int color);
void update_chart_data(CHART_DATA_SERIES *series, float value);
void render_chart(REALTIME_CHART *chart, BITMAP *buffer);
void render_line_chart(REALTIME_CHART *chart, BITMAP *buffer);
void render_gauge_chart(REALTIME_CHART *chart, BITMAP *buffer);
void render_bar_chart(REALTIME_CHART *chart, BITMAP *buffer);
void render_digital_display(REALTIME_CHART *chart, BITMAP *buffer);

// Chart management
void init_chart_manager(void);
void shutdown_chart_manager(void);
void add_chart_to_manager(REALTIME_CHART *chart);
void remove_chart_from_manager(int chart_index);
void set_chart_layout(int layout_type);
void update_all_charts(void);
void render_all_charts(BITMAP *buffer);

// Data recording and playback
int start_data_recording(const char *filename);
void stop_data_recording(void);
int load_recorded_data(const char *filename);
void start_playback(void);
void stop_playback(void);
void save_chart_config(const char *filename);
int load_chart_config(const char *filename);

// Chart configuration
void configure_chart_series(REALTIME_CHART *chart);
void set_chart_auto_scale(REALTIME_CHART *chart, int enabled);
void set_chart_y_range(REALTIME_CHART *chart, float min_val, float max_val);
void toggle_chart_grid(REALTIME_CHART *chart);
void toggle_chart_legend(REALTIME_CHART *chart);

// Performance monitoring
void update_chart_performance_stats(void);
float get_chart_update_fps(void);
int get_chart_memory_usage(void);

// Alert system for charts
typedef struct {
    char parameter_name[32];
    float warning_threshold;
    float critical_threshold;
    int alert_enabled;
    int current_state;  // 0=normal, 1=warning, 2=critical
} CHART_ALERT;

void add_chart_alert(const char *param_name, float warning, float critical);
void check_chart_alerts(void);
void display_alert_notifications(BITMAP *buffer);

extern CHART_MANAGER chart_manager;

#endif