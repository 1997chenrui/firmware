#ifndef BOOT_LOADER_CALL_BACK_H_
#define BOOT_LOADER_CALL_BACK_H_
#include "stm32f4xx_hal.h"
#include "bus.h"

u32 bus_ita_func_callback(bus_packet * packet);
u32 bus_ita_func_broadcast(bus_packet * packet);




#endif
