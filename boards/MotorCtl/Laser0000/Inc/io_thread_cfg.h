#ifndef _IO_THREAD_CFG_H_
#define _IO_THREAD_CFG_H_
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_WRITABLE_CNT 11
#define IO_READABLE_CNT 4
#define READABLE_CHANGE_ENABLE_VALUE 0x000F
//                                        1110(上报三个浮子开关状态)
#endif