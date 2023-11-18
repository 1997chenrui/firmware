#include <stdlib.h>
#include "color_cfg.h"
#include "stm32f4xx_hal.h"
#include "color.h"
#include "ws2812b.h"

ColorMod_t color[COLOR_GROUP_CNT];

Ws2812b_color_u progress_bar_led[PROGRESS_BAR_LED_CNT];
Ws2812b_color_u drawer_led[DRAWER_LED_CNT];
Ws2812b_color_u slide_led[SLIDE_LED_CNT];


static void ws0_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)value);
}

static void ws1_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (GPIO_PinState)value);
}

static void ws2_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (GPIO_PinState)value);
}

void color_hd_init(void) {
	Ws2812b_color_u default_color;
	default_color.value = 0x0;
	memset(progress_bar_led, 0, sizeof(progress_bar_led));
	memset(drawer_led, 0, sizeof(drawer_led));
	memset(slide_led, 0, sizeof(slide_led));
	color[0].ws = ws2812b_new(ws0_write_pin, progress_bar_led, PROGRESS_BAR_LED_CNT, default_color);
	color[1].ws = ws2812b_new(ws1_write_pin, drawer_led, DRAWER_LED_CNT, default_color);
	color[2].ws = ws2812b_new(ws2_write_pin, slide_led, SLIDE_LED_CNT, default_color);
	
#if 0
	{
		WS2812B_t* self = color[0].ws;
		Ws2812b_color_u color;
		color.value = 0;
		color.rgb.r = 0;
		color.rgb.g = 255;
		color.rgb.b = 0;
		self->color_select_write(self, 2, color);
	}
#endif
}



