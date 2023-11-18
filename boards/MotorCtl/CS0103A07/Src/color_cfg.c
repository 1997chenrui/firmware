#include <stdlib.h>
#include "color_cfg.h"
#include "stm32f4xx_hal.h"
#include "color.h"
#include "ws2812b.h"

ColorMod_t color[COLOR_GROUP_CNT];//一共1组灯带

Ws2812b_color_u slide_led[SLIDE_LED_CNT];////玻片架 灯数组

static void ws2_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)value);//玻片架灯单总线
}
//*/
void color_hd_init(void) {
	Ws2812b_color_u default_color;
	default_color.value = 0x0;//默认全灭

	memset(slide_led, 0, sizeof(slide_led));//玻璃夹灯全灭
	printf("color init\r\n");
	//将灯带组 和其总线控制函数、灯 绑定到一起，并且初始化
	color[0].ws = ws2812b_new(ws2_write_pin, slide_led, SLIDE_LED_CNT, default_color);
	
#if 1
	{
		WS2812B_t* self = color[0].ws;
		Ws2812b_color_u color;
		color.value = 0;
		color.rgb.r = 0;
		color.rgb.g = 0;
		color.rgb.b = 0;
		self->color_all_write(self, color);
	}
#endif
}



