#ifndef __PC_REBOOT_H__
#define __PC_REBOOT_H__
#include "macro.h"
#include "bus.h"

void reboot_pc_cmd(u32 func_code, bus_packet* packet);

#endif
