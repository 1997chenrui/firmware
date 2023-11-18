#ifndef _IO_THREAD_CFG_H_
#define _IO_THREAD_CFG_H_
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_WRITABLE_CNT_0 6

#define WRITE_GROUP_CNT 1 //写通道组数

void writable_io_init(void );

#endif