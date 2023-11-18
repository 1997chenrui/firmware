#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define SENSOR_CNT 4

typedef struct _Sensor_t{
    u16 (*read)(u8 index);
    void (*read_report)(bus_packet* packet);
}Sensor_t;

void sensor_init(void);

Sensor_t* sensor_get_as_ref(void);

#endif
