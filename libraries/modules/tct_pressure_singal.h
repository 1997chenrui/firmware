#ifndef __TCT_PRESSURE_SINGAL_H__
#define __TCT_PRESSURE_SINGAL_H__
#include <stdbool.h>
#include "macro.h"
#include "sensor.h"
#include "os_implement.h"
#include "bus.h"

#define BUFFER_CAPACITY 2048

typedef struct{
    u32 value;
    u16 time_delay;
}Pressure_IoTiming_t;

typedef struct _Pressure_Cfg_t{
    //上报结果
    bool report_result_enable;
    bool report_pressure_enable;
    bool threshold_enable;
    bool pressure_range_enable;
    bool is_pressure_bigger; //压力曲线向增大方向
    u8 sensor_idx; //传感器id
    u32 diff_threthold;//触发阈值
    u16 listen_time_span;//监听时间间隔
    u16 low_pressure;//阈值低范围
    u16 high_prsspure;//阈值高范围
    u16 data_cnt; //监听总时间 
    Pressure_IoTiming_t* io_timing_array;
    u8 io_timing_len;
}Pressure_Cfg_t;

typedef struct _Pressure_Entity_t{
    Pressure_Cfg_t* cfg;
    u8 error_code;
    u16 pressure;
    u16 data_len;
    bus_packet packet;
    u8 buffer[BUFFER_CAPACITY];
}Pressure_Entity_t;

typedef struct _Pressure_t
{
    bool is_busy;
    Pressure_Entity_t* entity;
    bus_os_sem sem;
    void (*add)(Pressure_Cfg_t*);
    void (*parse)(bus_packet*);
}Pressure_t;

typedef u16 (*SENSOR_READ)(u8);
typedef void (*IO_TIMING_WRITE)(void* ,u8 );

Pressure_t* pressure_get_as_ref(void);
void pressure_singal_init(void (*thread_create)(void (*thread)(void* data)),
        void (sem_create)(bus_os_sem* sem),
        SENSOR_READ sensor_read,
        IO_TIMING_WRITE io_timing_write);
#endif
