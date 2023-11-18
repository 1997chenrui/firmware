#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_WRITABLE_CNT_0 8

#define IO_WRITABLE_CNT_1 16

#define IO_READABLE_CNT_0 7



#define READABLE_CHANGE_ENABLE_VALUE 0x07

#define WRITE_GROUP_CNT 2 //读通道组数

#define READ_GROUP_CNT 1 //写通道组数

#define PWM_SET_DEFAULT 1

void writable_io_init(void );
void readable_io_init(void );

#endif
