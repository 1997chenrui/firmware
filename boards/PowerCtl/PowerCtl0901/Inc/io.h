#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_WRITABLE_CNT 3
#define IO_READABLE_CNT 5

#define IO_WRITABLE_CNT_0 3

#define IO_READABLE_CNT_0 5

#define WRITE_GROUP_CNT 1 //读通道组数

#define READ_GROUP_CNT 1 //写通道组数

#define READABLE_CHANGE_ENABLE_VALUE 0x13


#define WRITE_GROUP_CNT 1 //私

#define READ_GROUP_CNT 1 //私



void writable_io_init(void );
void readable_io_init(void );

#endif
