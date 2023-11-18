#include <stdlib.h>
#include "color_cfg.h"
#include "stm32f4xx_hal.h"
#include "color.h"
#include "ws2812b.h"

ColorMod_t color[COLOR_GROUP_CNT];

Ws2812b_color_u led[LED_CNT];


static void ws0_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (GPIO_PinState)value);
}

void color_hd_init(void) {
	Ws2812b_color_u default_color;
	default_color.value = 0x0;
	memset(led, 0, sizeof(led));
	color[0].ws = ws2812b_new(ws0_write_pin, led, LED_CNT, default_color);
	
#if 0
	{
		WS2812B_t* self = color[0].ws;
		Ws2812b_color_u color;
		color.value = 0;
		color.rgb.r = 255;
		color.rgb.g = 0;
		color.rgb.b = 0;
		self->color_select_write(self, 0x07, color);
	}
#endif
}



