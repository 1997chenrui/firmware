#ifndef TEMP_CTRL_THREAD_H_
#define TEMP_CTRL_THREAD_H_
#include "os_implement.h"
#include "pid.h"
#include <stdbool.h>
#include "bus.h"

void sensor_single_cmd_run(u32 itf_code, bus_packet *packet) ;
void Light_Data_Get_Test(void);
	
#endif
