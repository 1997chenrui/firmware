#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__
#include "macro.h"
#include "bus.h"
void io_cmd_run(u32 itf_code, bus_packet* packet);
void io_thread_init(void (*thread_create)(void (*thread)(void* data)));

#endif
