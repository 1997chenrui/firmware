#ifndef __IO_CTRL_BY_EVENT__
#define __IO_CTRL_BY_EVENT__
#include <stdbool.h>
#include <stdlib.h>
#include "link_list.h"
#include "bus.h"
#include "macro.h"
#define QUERY_TIME_SPAN 100

typedef struct{
    //写io组的值
    u32 io_value;
    //写完后的延时时间
    u16 time_delay;
}IOCtrl_timing_t;

typedef union{
    struct{
        //阈值
        u32 threshold:24;
        //传感器id
        u32 sensor_idx:5;
        //触发方式0 等于 1 大于 2 小于
        u32 trigger_way:2;
        //是否使能阈值触发
        u32 threshold_enable:1;
    }split;
    u32 value;
}IOCtrl_threshold_u;

typedef struct{
    //是否上报结果
    bool report;
    u8 before_len;
    u8 after_len;
    //触发前io操作
    IOCtrl_timing_t* before;
    //触发后io操作
    IOCtrl_timing_t* after;
    IOCtrl_threshold_u threshold_u;
    u32 query_cnt;
}IOCtrl_cfg_t;

typedef struct{
    IOCtrl_cfg_t* cfg;
    //轮训次数
    u32 query_time;
    //传感器值
    u32 sensor_value;
    bus_packet packet;
    u8 error_code;
}IOCtrl_entity_t;

typedef struct{
    Link_list_t* list;
    void (*parse)(bus_packet*);
    void (*add)(IOCtrl_cfg_t*);
}IOCtrl_t;

typedef u16 (*SENSOR_READ)(u8);
typedef void (*IO_WRITE)(u32);


void ioctrl_init(void (*thread_create)(void (*thread)(void* data)), SENSOR_READ, IO_WRITE);
IOCtrl_t* ioctrl_get_as_ref(void);

#endif
