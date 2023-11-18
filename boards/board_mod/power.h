#ifndef __POWER_H__
#define __POWER_H__
#include "macro.h"
#include "bus.h"

void power_init(void (*thread_create)(void (*thread)(void* data)));
void power_ctrl(u8 on);
void power_cmd(bus_packet* packet);

#endif
