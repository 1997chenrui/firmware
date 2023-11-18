#ifndef _AD7699_TASK_H_
#define _AD7699_TASK_H_
#include "itf_00040020.h"
#include "bus.h"
#include "stm32f4xx.h"

#include "AD7699_task.h"
void Ad_cmd_run(u32 func_code, bus_packet* packet);

#endif
