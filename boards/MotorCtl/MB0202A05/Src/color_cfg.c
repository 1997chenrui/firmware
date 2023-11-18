#include <stdlib.h>
#include "color_cfg.h"
#include "stm32f4xx_hal.h"
#include "color.h"
#include "ws2812b.h"

ColorMod_t color[COLOR_GROUP_CNT];//一共1组灯带

Ws2812b_color_u drawer_led[DRAWER_LED_CNT];//抽屉 灯数组




static void ws1_write_pin(u8 value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (GPIO_PinState)value);//抽屉灯单总线
}


void color_hd_init(void) {
	Ws2812b_color_u default_color;
	default_color.value = 0x00;//默认全灭
	
	memset(drawer_led, 0, sizeof(drawer_led));//抽屉 灯 全灭
	
	color[0].ws = ws2812b_new(ws1_write_pin, drawer_led, DRAWER_LED_CNT, default_color);

#if 1
//	while(1)
	{	
			WS2812B_t* self = color[0].ws;
			self->color_all_default_write(self);	
//			HAL_Delay(500);
//			default_color.value = 0xff;
//			self->color_all_default_write(self);
//			self->color_all_write(self,default_color);
//			HAL_Delay(500);
//			default_color.value = 0xff0000;
//			self->color_all_write(self,default_color);	
//			HAL_Delay(500);	
		
	}
#endif
}



