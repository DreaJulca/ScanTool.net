#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
#include "realtime_charts.h"
#include "sensors.h"
#include "custom_gui.h"

CHART_MANAGER chart_manager;
static BITMAP *chart_buffer = NULL;
static volatile int chart_timer_counter = 0;

// Timer callback for chart updates
void chart_timer_callback(void)
{
    chart_timer_counter++;
}
END_OF_FUNCTION(chart_timer_callback)

int display_realtime_charts(void)
{
    init_chart_manager();
    
    // Create default engine monitoring layout
    REALTIME_CHART *rpm_chart = create_chart(CHART_TYPE_GAUGE, 50, 50, 200, 150, "Engine RPM");
    CHART_DATA_SERIES *rpm_series = add_data_series(rpm_chart, "RPM", "RPM", CHART_COLOR_RPM);
    
    REALTIME_CHART *speed_chart = create_chart(CHART_TYPE_LINE, 270, 50, 300, 150, "Vehicle Speed");
    CHART_DATA_SERIES *speed_series = add_data_series(speed_chart, "Speed", "MPH", CHART_COLOR_SPEED);
    
    REALTIME_CHART *temp_chart = create_chart(CHART_TYPE_GAUGE, 50, 220, 200, 150, "Coolant Temp");
    CHART_DATA_SERIES *temp_series = add_data_series(temp_chart, "Coolant", "°F", CHART_COLOR_COOLANT);
    
    REALTIME_CHART *throttle_chart = create_chart(CHART_TYPE_BAR, 270, 220, 300, 150, "Throttle Position");
    CHART_DATA_SERIES *throttle_series = add_data_series(throttle_chart, "Throttle", "%", CHART_COLOR_THROTTLE);
    
    add_chart_to_manager(rpm_chart);
    add_chart_to_manager(speed_chart);
    add_chart_to_manager(temp_chart);
    add_chart_to_manager(throttle_chart);
    
    // Create buffer for double buffering
    chart_buffer = create_bitmap(SCREEN_W, SCREEN_H);
    if (!chart_buffer) {
        alert("Error", "Could not create chart buffer", NULL, "OK", NULL, 0, 0);
        return 0;
    }
    
    // Install timer for chart updates
    install_int_ex(chart_timer_callback, BPM_TO_TIMER(600)); // 10Hz update rate
    
    int exit_charts = 0;
    int last_timer = chart_timer_counter;
    
    while (!exit_charts) {
        // Check for exit condition
        if (keypressed()) {
            int key = readkey();
            if ((key & 0xFF) == 27) { // ESC key
                exit_charts = 1;
                continue;
            }
            if ((key & 0xFF) == 'r' || (key & 0xFF) == 'R') {
                // Toggle recording
                if (chart_manager.recording_enabled) {
                    stop_data_recording();
                } else {
                    start_data_recording("chart_data.csv");
                }
            }
        }
        
        // Update charts at timer intervals
        if (chart_timer_counter != last_timer) {
            last_timer = chart_timer_counter;
            
            // Simulate sensor data updates (in real implementation, read from vehicle)
            update_chart_data(rpm_series, 1500.0f + sin(chart_timer_counter * 0.1f) * 500.0f);
            update_chart_data(speed_series, 35.0f + cos(chart_timer_counter * 0.05f) * 10.0f);
            update_chart_data(temp_series, 180.0f + sin(chart_timer_counter * 0.03f) * 15.0f);
            update_chart_data(throttle_series, 25.0f + sin(chart_timer_counter * 0.08f) * 20.0f);
            
            // Clear buffer and render all charts
            clear_bitmap(chart_buffer);
            render_all_charts(chart_buffer);
            
            // Display status information
            textprintf_ex(chart_buffer, font, 10, 10, C_BLACK, C_WHITE, 
                         "Real-time Charts - ESC to exit, R to toggle recording");
            
            if (chart_manager.recording_enabled) {
                textprintf_ex(chart_buffer, font, 10, 25, C_RED, C_WHITE, "RECORDING");
            }
            
            // Blit to screen
            blit(chart_buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
        
        rest(1); // Small delay to prevent 100% CPU usage
    }
    
    // Cleanup
    remove_int(chart_timer_callback);
    destroy_bitmap(chart_buffer);
    shutdown_chart_manager();
    
    return 1;
}

void init_chart_manager(void)
{
    memset(&chart_manager, 0, sizeof(CHART_MANAGER));
    chart_manager.current_layout = LAYOUT_ENGINE_OVERVIEW;
}

void shutdown_chart_manager(void)
{
    // Free all charts and data series
    for (int i = 0; i < chart_manager.chart_count; i++) {
        if (chart_manager.charts[i]) {
            // Free data series
            for (int j = 0; j < chart_manager.charts[i]->series_count; j++) {
                if (chart_manager.charts[i]->series[j]) {
                    free(chart_manager.charts[i]->series[j]);
                }
            }
            free(chart_manager.charts[i]);
        }
    }
    
    if (chart_manager.recording_enabled) {
        stop_data_recording();
    }
    
    memset(&chart_manager, 0, sizeof(CHART_MANAGER));
}

REALTIME_CHART* create_chart(int type, int x, int y, int w, int h, const char *title)
{
    REALTIME_CHART *chart = malloc(sizeof(REALTIME_CHART));
    if (!chart) return NULL;
    
    memset(chart, 0, sizeof(REALTIME_CHART));
    chart->chart_type = type;
    chart->x = x;
    chart->y = y;
    chart->width = w;
    chart->height = h;
    strncpy(chart->title, title, sizeof(chart->title) - 1);
    
    chart->update_rate_hz = CHART_RATE_10HZ;
    chart->auto_scale = 1;
    chart->grid_enabled = 1;
    chart->legend_enabled = 1;
    chart->time_window_seconds = 60;
    
    return chart;
}

CHART_DATA_SERIES* add_data_series(REALTIME_CHART *chart, const char *label, const char *units, int color)
{
    if (!chart || chart->series_count >= 8) return NULL;
    
    CHART_DATA_SERIES *series = malloc(sizeof(CHART_DATA_SERIES));
    if (!series) return NULL;
    
    memset(series, 0, sizeof(CHART_DATA_SERIES));
    strncpy(series->label, label, sizeof(series->label) - 1);
    strncpy(series->units, units, sizeof(series->units) - 1);
    series->color = color;
    series->visible = 1;
    series->min_value = 0.0f;
    series->max_value = 100.0f;
    
    chart->series[chart->series_count] = series;
    chart->series_count++;
    
    return series;
}

void update_chart_data(CHART_DATA_SERIES *series, float value)
{
    if (!series) return;
    
    series->current_value = value;
    series->values[series->current_index] = value;
    series->current_index = (series->current_index + 1) % MAX_CHART_POINTS;
    
    if (series->data_count < MAX_CHART_POINTS) {
        series->data_count++;
    }
    
    // Update min/max for auto-scaling
    if (value < series->min_value) series->min_value = value;
    if (value > series->max_value) series->max_value = value;
}

void add_chart_to_manager(REALTIME_CHART *chart)
{
    if (chart_manager.chart_count < 16) {
        chart_manager.charts[chart_manager.chart_count] = chart;
        chart_manager.chart_count++;
    }
}

void render_all_charts(BITMAP *buffer)
{
    for (int i = 0; i < chart_manager.chart_count; i++) {
        if (chart_manager.charts[i]) {
            render_chart(chart_manager.charts[i], buffer);
        }
    }
}

void render_chart(REALTIME_CHART *chart, BITMAP *buffer)
{
    if (!chart || !buffer) return;
    
    // Draw chart background
    rectfill(buffer, chart->x, chart->y, 
             chart->x + chart->width, chart->y + chart->height, C_WHITE);
    rect(buffer, chart->x, chart->y, 
         chart->x + chart->width, chart->y + chart->height, C_BLACK);
    
    // Draw title
    textprintf_centre_ex(buffer, font, chart->x + chart->width / 2, chart->y + 5, 
                        C_BLACK, C_WHITE, "%s", chart->title);
    
    // Render based on chart type
    switch (chart->chart_type) {
        case CHART_TYPE_LINE:
            render_line_chart(chart, buffer);
            break;
        case CHART_TYPE_GAUGE:
            render_gauge_chart(chart, buffer);
            break;
        case CHART_TYPE_BAR:
            render_bar_chart(chart, buffer);
            break;
        case CHART_TYPE_DIGITAL:
            render_digital_display(chart, buffer);
            break;
    }
}

void render_line_chart(REALTIME_CHART *chart, BITMAP *buffer)
{
    if (!chart->series_count) return;
    
    int plot_x = chart->x + 10;
    int plot_y = chart->y + 25;
    int plot_w = chart->width - 20;
    int plot_h = chart->height - 40;
    
    // Draw grid if enabled
    if (chart->grid_enabled) {
        for (int i = 1; i < 5; i++) {
            int grid_y = plot_y + (plot_h * i) / 5;
            line(buffer, plot_x, grid_y, plot_x + plot_w, grid_y, C_GRAY);
        }
    }
    
    // Render each data series
    for (int s = 0; s < chart->series_count; s++) {
        CHART_DATA_SERIES *series = chart->series[s];
        if (!series || !series->visible || series->data_count < 2) continue;
        
        float y_range = series->max_value - series->min_value;
        if (y_range <= 0) y_range = 1.0f;
        
        int prev_x = -1, prev_y = -1;
        
        for (int i = 0; i < series->data_count - 1; i++) {
            int idx1 = (series->current_index - series->data_count + i + MAX_CHART_POINTS) % MAX_CHART_POINTS;
            int idx2 = (idx1 + 1) % MAX_CHART_POINTS;
            
            float val1 = series->values[idx1];
            float val2 = series->values[idx2];
            
            int x1 = plot_x + (plot_w * i) / (series->data_count - 1);
            int y1 = plot_y + plot_h - (int)((val1 - series->min_value) / y_range * plot_h);
            int x2 = plot_x + (plot_w * (i + 1)) / (series->data_count - 1);
            int y2 = plot_y + plot_h - (int)((val2 - series->min_value) / y_range * plot_h);
            
            line(buffer, x1, y1, x2, y2, series->color);
        }
    }
    
    // Draw current value
    if (chart->series_count > 0 && chart->series[0]) {
        textprintf_ex(buffer, font, chart->x + 5, chart->y + chart->height - 15, 
                     chart->series[0]->color, C_WHITE, "%.1f %s", 
                     chart->series[0]->current_value, chart->series[0]->units);
    }
}

void render_gauge_chart(REALTIME_CHART *chart, BITMAP *buffer)
{
    if (!chart->series_count || !chart->series[0]) return;
    
    CHART_DATA_SERIES *series = chart->series[0];
    
    int center_x = chart->x + chart->width / 2;
    int center_y = chart->y + chart->height / 2 + 10;
    int radius = min(chart->width, chart->height) / 3;
    
    // Draw gauge background
    circle(buffer, center_x, center_y, radius, C_BLACK);
    circle(buffer, center_x, center_y, radius - 2, C_GRAY);
    
    // Calculate needle angle (-135 to +135 degrees)
    float range = series->max_value - series->min_value;
    if (range <= 0) range = 1.0f;
    
    float normalized = (series->current_value - series->min_value) / range;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;
    
    float angle = -135.0f + (normalized * 270.0f);
    float angle_rad = angle * M_PI / 180.0f;
    
    // Draw needle
    int needle_x = center_x + (int)(cos(angle_rad) * (radius - 5));
    int needle_y = center_y + (int)(sin(angle_rad) * (radius - 5));
    
    line(buffer, center_x, center_y, needle_x, needle_y, C_RED);
    circlefill(buffer, center_x, center_y, 3, C_BLACK);
    
    // Draw scale marks
    for (int i = 0; i <= 10; i++) {
        float mark_angle = -135.0f + (i * 27.0f);
        float mark_rad = mark_angle * M_PI / 180.0f;
        
        int x1 = center_x + (int)(cos(mark_rad) * (radius - 8));
        int y1 = center_y + (int)(sin(mark_rad) * (radius - 8));
        int x2 = center_x + (int)(cos(mark_rad) * (radius - 2));
        int y2 = center_y + (int)(sin(mark_rad) * (radius - 2));
        
        line(buffer, x1, y1, x2, y2, C_BLACK);
    }
    
    // Display current value
    textprintf_centre_ex(buffer, font, center_x, center_y + radius + 10, 
                        C_BLACK, C_WHITE, "%.1f %s", 
                        series->current_value, series->units);
}

void render_bar_chart(REALTIME_CHART *chart, BITMAP *buffer)
{
    if (!chart->series_count || !chart->series[0]) return;
    
    CHART_DATA_SERIES *series = chart->series[0];
    
    int bar_x = chart->x + 20;
    int bar_y = chart->y + 30;
    int bar_w = chart->width - 40;
    int bar_h = chart->height - 60;
    
    // Draw bar background
    rect(buffer, bar_x, bar_y, bar_x + bar_w, bar_y + bar_h, C_BLACK);
    
    // Calculate bar fill
    float range = series->max_value - series->min_value;
    if (range <= 0) range = 1.0f;
    
    float normalized = (series->current_value - series->min_value) / range;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;
    
    int fill_height = (int)(normalized * bar_h);
    
    // Draw filled portion
    if (fill_height > 0) {
        rectfill(buffer, bar_x + 1, bar_y + bar_h - fill_height, 
                bar_x + bar_w - 1, bar_y + bar_h - 1, series->color);
    }
    
    // Display current value
    textprintf_centre_ex(buffer, font, chart->x + chart->width / 2, 
                        chart->y + chart->height - 15, 
                        C_BLACK, C_WHITE, "%.1f %s", 
                        series->current_value, series->units);
}

void render_digital_display(REALTIME_CHART *chart, BITMAP *buffer)
{
    if (!chart->series_count || !chart->series[0]) return;
    
    CHART_DATA_SERIES *series = chart->series[0];
    
    // Draw digital display background
    rectfill(buffer, chart->x + 5, chart->y + 20, 
             chart->x + chart->width - 5, chart->y + chart->height - 5, C_BLACK);
    
    // Display value in large text
    textprintf_centre_ex(buffer, font, chart->x + chart->width / 2, 
                        chart->y + chart->height / 2 - 10, 
                        C_GREEN, C_BLACK, "%.1f", series->current_value);
    
    textprintf_centre_ex(buffer, font, chart->x + chart->width / 2, 
                        chart->y + chart->height / 2 + 10, 
                        C_WHITE, C_BLACK, "%s", series->units);
}

int start_data_recording(const char *filename)
{
    if (chart_manager.recording_enabled) {
        stop_data_recording();
    }
    
    chart_manager.recording_file = fopen(filename, "w");
    if (!chart_manager.recording_file) {
        return 0;
    }
    
    // Write CSV header
    fprintf(chart_manager.recording_file, "Timestamp");
    for (int i = 0; i < chart_manager.chart_count; i++) {
        REALTIME_CHART *chart = chart_manager.charts[i];
        for (int j = 0; j < chart->series_count; j++) {
            fprintf(chart_manager.recording_file, ",%s (%s)", 
                   chart->series[j]->label, chart->series[j]->units);
        }
    }
    fprintf(chart_manager.recording_file, "\n");
    
    strncpy(chart_manager.recording_filename, filename, 
            sizeof(chart_manager.recording_filename) - 1);
    chart_manager.recording_enabled = 1;
    
    return 1;
}

void stop_data_recording(void)
{
    if (chart_manager.recording_enabled && chart_manager.recording_file) {
        fclose(chart_manager.recording_file);
        chart_manager.recording_file = NULL;
    }
    
    chart_manager.recording_enabled = 0;
    chart_manager.recording_filename[0] = '\0';
}