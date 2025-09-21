#ifndef REALTIME_CHARTS_H
#define REALTIME_CHARTS_H

#include "allegro_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Real-time data visualization functions */
int display_realtime_charts(void);
int init_charts_system(void);
void cleanup_charts_system(void);
void update_chart_data(int sensor_id, float value);

#ifdef __cplusplus
}
#endif

#endif /* REALTIME_CHARTS_H */
