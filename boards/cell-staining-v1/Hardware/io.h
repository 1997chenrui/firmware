#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_CNT 9

typedef struct{
    GPIO_TypeDef* port;
    u16 pin;
    u8 state;
    u8 feature; // [0]:可读 [1]:可写 
}Io_t;

typedef struct{
    //写io组的值
    u32 io_value;
    //写完后的延时时间
    u16 time_delay;
}Io_timing_t;

/*
state : [0:1]每两个为一组 0 无效 1 写低 2 写高
*/
typedef struct{
    Io_t io_array[IO_CNT];
    void (*write)(u32);
    void (*write_report)(bus_packet*);
    void (*timing_io_write)(void* ,u8);
}Io_Group_t;

void io_init(void);
Io_Group_t* io_get_as_ref(void);

#endif
