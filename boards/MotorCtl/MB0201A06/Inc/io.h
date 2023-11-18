#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_READABLE_CNT 3
#define IO_WRITABLE_CNT 6
#define READABLE_CHANGE_ENABLE_VALUE 0x07

#define READ_GROUP_CNT 1
#define WRITE_GROUP_CNT 1
void io_hd_init(void);

#endif
