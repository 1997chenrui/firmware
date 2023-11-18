#include <stdlib.h>
#include "color_cfg.h"
#include "stm32f4xx_hal.h"
#include "color.h"
#include "ws2812b.h"

ColorMod_t color[COLOR_GROUP_CNT];//一共1组灯带

Ws2812b_color_u progress_bar_led[PROGRESS_BAR_LED_CNT];//进度条灯数组


static void ws0_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)value);//进度条灯单总线
}

void color_hd_init(void) {
	Ws2812b_color_u default_color;
	default_color.value = 0x00;//默认全灭
	memset(progress_bar_led, 0, sizeof(progress_bar_led));//进度条灯 全灭
	//将灯带组 和其总线控制函数、灯 绑定到一起，并且初始化
	color[0].ws = ws2812b_new(ws0_write_pin, progress_bar_led, PROGRESS_BAR_LED_CNT, default_color);
	
#if 1
	{
		WS2812B_t* self = color[0].ws;
		self->color_all_default_write(self);
		
	}
#endif
}



