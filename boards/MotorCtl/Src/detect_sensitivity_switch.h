#ifndef __IO_H__
#define __IO_H__
#include "stm32f4xx_hal.h"
#include "macro.h"
#include "bus.h"
#endif

void detect_sen_switch(u8 ctrl_switch);
void detect_sensitivity_cmd_run(u32 itf_code, bus_packet* packet);

