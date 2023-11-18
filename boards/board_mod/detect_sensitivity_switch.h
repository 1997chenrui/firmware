#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__
#include "macro.h"
#include "bus.h"

void detect_sen_switch(u8 ctrl_switch);
void detect_sensitivity_cmd_run(u32 itf_code, bus_packet* packet);
	
#endif
