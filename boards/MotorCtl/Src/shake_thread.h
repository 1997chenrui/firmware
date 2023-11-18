#ifndef __SHAKE_THREAD_H__
#define __SHAKE_THREAD_H__
#include "macro.h"
#include "bus.h"
#include "itf_00070011.h"

typedef struct{
    u8 is_cfg;
    bus_packet packet;
    f32 voltage;
    u16 time_ms;
}Shake_content_t;

typedef struct{
    volatile u8 is_busy;
    Shake_content_t content;
    bus_os_sem sem;
    void (*shake)(void);
    void (*shake_time)(u16);
    void (*shake_cfg)(Shake_content_t*);
    void (*shake_asyn)(Shake_content_t*);
}Shake_t;

void shake_thread_init(void (*thread_create)(void (*thread)(void* data)));
void shake_cmd_run(bus_packet* packet);
Shake_t* shake_get_as_ref(void);

#endif
