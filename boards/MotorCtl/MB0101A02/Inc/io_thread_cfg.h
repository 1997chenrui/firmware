#ifndef _IO_THREAD_CFG_H_
#define _IO_THREAD_CFG_H_
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define READABLE_CHANGE_ENABLE_VALUE 0x03ff

#define IO_WRITABLE_CNT_0 2
#define IO_READABLE_CNT_0 10

#define WRITE_GROUP_CNT 1 //写通道组数

#define READ_GROUP_CNT 1 //读通道组数

void writable_io_init(void );
void readable_io_init(void );

#endif