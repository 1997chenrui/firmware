#ifndef __TCT_MIXING_H__
#define __TCT_MIXING_H__
#include "macro.h"
#include "bus.h"

typedef void (*VOLTAGE_WRITE)(f32);

typedef struct  _TCTMixing_t{
    f32 start_voltage;
    f32 stop_voltage;
    f32 run_voltage;
    u16 start_ms;
    u16 run_ms;
    bool is_busy;
    bool is_report;
    bus_packet packet;
    bus_os_sem sem;
    void (*add)(f32 run_voltage, u16 run_ms);
    void (*parse)(bus_packet* packet);
}TCTMixing_t;

void tctmixing_init(f32 start_voltage, u16 start_ms, VOLTAGE_WRITE voltage_write, void (*power)(u8 on), void (*thread_create)(void (*thread)(void* data)), void (sem_create)(bus_os_sem* sem));
TCTMixing_t* tctmixing_get_as_ref(void);

#endif
