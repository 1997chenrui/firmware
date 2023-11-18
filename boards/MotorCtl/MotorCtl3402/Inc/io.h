#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"



#define IO_READABLE_CNT_0 3






#define READABLE_CHANGE_ENABLE_VALUE 0x00

#define WRITE_GROUP_CNT 0

#define READ_GROUP_CNT 1
void writable_io_init(void );
void readable_io_init(void );
#endif
