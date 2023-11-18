#ifndef __ROTATE_VALVE_THREAD_H__
#define __ROTATE_VALVE_THREAD_H__
#include "macro.h"
#include "bus.h"
#include "data_struct.h"

typedef struct{
    u32         cmd_code;
    bus_packet  packet_head;
    void * p_cmd;
}Rotatevalve_queue_ele_t;

void rv_cmd_run(u32 itf_code, bus_packet* packet);
void rv_thread_init(void (*thread_create)(void (*thread)(void* data)));

#endif
