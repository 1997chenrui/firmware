#ifndef _IO_THREAD_CFG_H_
#define _IO_THREAD_CFG_H_
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_WRITABLE_CNT 2
#define IO_READABLE_CNT 2
#define READABLE_CHANGE_ENABLE_VALUE 0x0001

#define IO_WRITABLE_CNT_0 2

#define IO_READABLE_CNT_0 6

#define WRITE_GROUP_CNT 1 //��ͨ������

#define READ_GROUP_CNT 1 //дͨ������

void writable_io_init(void );
void readable_io_init(void );

#endif
