#ifndef __SENSORS_H__
#define __SENSORS_H__
#include "macro.h"
#include "TempSensor_DS18B20.h"
#include "bus.h"

typedef struct{
    TempSensor_Struct sensor;
    f32 (*read)(void);
    void (*buffer_report)(bus_packet* packet);
}Temperature_sensor_t;

void temperature_sensor_init(void);
Temperature_sensor_t* temperaturesensor_get_as_ref(void);

#endif
