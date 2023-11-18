#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"

#define IO_READABLE_CNT 4
#define IO_WRITABLE_CNT 0
#define READABLE_CHANGE_ENABLE_VALUE 0x04

#define WRITE_GROUP_CNT 0 //ׁͨրة˽

#define READ_GROUP_CNT 1 //дͨրة˽



//void writable_io_init(void );
void readable_io_init(void );

#endif
