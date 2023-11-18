#ifndef __ROTATE_VALVE_THREAD_H__
#define __ROTATE_VALVE_THREAD_H__
#include "macro.h"
#include "bus.h"
#include "data_struct.h"
#include "runze_sv04_new.h"
#include "bus.h"

#define RV_THREAD_STACK_SIZE 512

typedef struct{
    u32         cmd_code;
    bus_packet  packet_head;
    void * p_cmd;
}Rotatevalve_queue_ele_t;

typedef struct{
    Runze_t *runze;
    SeqQueue_Struct queue;
    bus_os_sem queue_sem;
    bus_os_thread thread_tcb;
    u32 thread_prio;
	u32 thread_stack[RV_THREAD_STACK_SIZE]; 
    void (*uart_bytes_send)(u8 *data, u8 len);
    bool (*queue_pend)(void);
    bool (*queue_post)(void);
    
}Rv_thread_t;

void rv_cmd_run(u32 itf_code, bus_packet* packet);
void rv_thread_init(void);

#endif
