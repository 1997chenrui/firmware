#ifndef __COLOR_H__
#define __COLOR_H__
#include "macro.h"
#include "stm32f4xx_hal.h"
#include "bus.h"
#include "ws2812b.h"


typedef struct
{
	WS2812B_t* ws;
}ColorMod_t;

void colormod_init(void);
ColorMod_t* colormod_get_as_ref(u8 idx);
void ws_led_cmd_run(u32 itf_code, bus_packet* packet);
#endif
