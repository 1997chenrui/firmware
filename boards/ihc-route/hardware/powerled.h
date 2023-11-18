#ifndef __POWERLED_H__
#define __POWERLED_H__
#include "macro.h"
#include "bus.h"
#define CHANNEL_CNT 2

typedef struct
{
    u32 channel[CHANNEL_CNT];
    void (*write)(u8 idx, f32 voltage);
    void (*write_report)(bus_packet* packet);
}PowerLed_t;
void powerled_init(void);

PowerLed_t* powerled_get_as_ref(void);
/** ²âÊÔº¯Êý **/
void powerled_write_3_3_test(void);
void powerled_write_0_test(void);
#endif
