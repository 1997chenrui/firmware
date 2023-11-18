#ifndef __COLOR_CFG_H__
#define __COLOR_CFG_H__
#include "uart_thread.h"

#define COLOR_GROUP_CNT 3
//设备精度条
#define PROGRESS_BAR_LED_CNT 17
//抽屉
#define DRAWER_LED_CNT 5
//玻片架
#define SLIDE_LED_CNT 4
void color_hd_init(void);
#endif
