// Define platform before any includes to avoid dzcomm.h inclusion
#define ALLEGRO_WINDOWS

#include "enhanced_communication.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int enhanced_comm_initialized = 0;
static int active_protocol = 0;

int init_enhanced_communication(void) {
    if (enhanced_comm_initialized) {
        return 0; /* Already initialized */
    }
    
    printf("Initializing enhanced communication protocols...\n");
    
    /* Initialize serial communication first */
    if (!serial_port_opened()) {
        printf("Warning: Serial port not open, enhanced communication limited\n");
    }
    
    /* Try to detect available protocols */
    active_protocol = ENHANCED_PROTOCOL_UDS; /* Default to UDS */
    enhanced_comm_initialized = 1;
    
    printf("Enhanced communication initialized (Protocol: %d)\n", active_protocol);
    return 0;
}

void cleanup_enhanced_communication(void) {
    if (!enhanced_comm_initialized) {
        return;
    }
    
    printf("Cleaning up enhanced communication...\n");
    enhanced_comm_initialized = 0;
    active_protocol = 0;
}

int send_enhanced_command(const char* command, char* response, int max_len) {
    if (!enhanced_comm_initialized) {
        if (init_enhanced_communication() != 0) {
            return -1;
        }
    }
    
    if (!command || !response || max_len <= 0) {
        return -1;
    }
    
    /* For now, simulate successful communication */
    snprintf(response, max_len, "OK: %s", command);
    printf("Enhanced Command: %s -> %s\n", command, response);
    
    return 0;
}

int is_enhanced_protocol_available(void) {
    return enhanced_comm_initialized && (active_protocol > 0);
}
