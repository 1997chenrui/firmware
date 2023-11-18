#ifndef __ITF_00020010_H__
#define __ITF_00020010_H__
#include "macro.h"
#include "bus.h"

u8 Itf_00020010_parse(bus_packet* packet);
void Itf_00020010_rtn(bus_packet* packet, u32 state);
#endif


