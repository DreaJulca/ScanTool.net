#ifndef ENHANCED_COMMUNICATION_H
#define ENHANCED_COMMUNICATION_H

#include "scantool.h"
#include "serial.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Enhanced communication protocols */
int init_enhanced_communication(void);
void cleanup_enhanced_communication(void);
int send_enhanced_command(const char* command, char* response, int max_len);
int is_enhanced_protocol_available(void);

/* Protocol definitions */
#define ENHANCED_PROTOCOL_UDS    1
#define ENHANCED_PROTOCOL_KWP    2
#define ENHANCED_PROTOCOL_CAN    3

#ifdef __cplusplus
}
#endif

#endif /* ENHANCED_COMMUNICATION_H */
