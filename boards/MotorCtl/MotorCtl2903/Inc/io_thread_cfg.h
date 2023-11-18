#ifndef _IO_THREAD_CFG_H_
#define _IO_THREAD_CFG_H_
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_READABLE_CNT_0 3



#define READABLE_CHANGE_ENABLE_VALUE 0x1F

#define READ_GROUP_CNT 1 

void readable_io_init(void );

#endif