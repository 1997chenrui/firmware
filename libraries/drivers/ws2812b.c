#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "ws2812b.h"
#include "delayUs.h"

/**
 * @brief 延时函数 
 * 
 * @param count 时钟周期数 
 */
static void ws2812b_delay(unsigned int count)
{
	while(count--)
	{
		__NOP();
	}
}

/**
 * @brief ws2812b 写字节
 * 
 * @param wbyte 
 */
static void ws2812b_write_byte(WS2812B_t* self, u8 wbyte)
{
	for(u8 i=0; i<8; ++i)
	{
		if(wbyte & (0x80 >> i))
		{
			self->m_write_pin(1);
			ws2812b_delay(20);
			self->m_write_pin(0);
			ws2812b_delay(15);

		}
		else
		{
			self->m_write_pin(1);
			ws2812b_delay(1);
			self->m_write_pin(0);
			ws2812b_delay(15);
		}
	}
}

/**
 * @brief 写颜色
 * 
 * @param color 颜色
 */
static void ws2812b_write_color(WS2812B_t* self, Ws2812b_color_u color)
{
	ws2812b_write_byte(self, (unsigned char)(color.rgb.g));
	ws2812b_write_byte(self, (unsigned char)(color.rgb.r));
	ws2812b_write_byte(self, (unsigned char)(color.rgb).b);
}


/**
 * @brief 控制多个led颜色, 直接使用外部传入的值控制led颜色
 * 
 * @param color 颜色数组
 * @param count 颜色数组长度
 */
static void color_extend_write(WS2812B_t* self, Ws2812b_color_u* color, unsigned char count)
{
	// 注: 该总线时序要求特别严格,所以必须要加中断保护
	__disable_irq();
	for(u8 i=0; i<count; ++i)
	{
		ws2812b_write_color(self, color[i]);
	}
	__enable_irq();
	// 每帧数据的间隔必须大于50us
	delayUs(50);
}

/**
 * @brief 控制多个led颜色, 将外部传入的值将保存到内部
 * 传入的count应该与内部长度一致，否则将触发error
 * 
 * @param color 颜色数组
 * @param count 颜色数组长度
 */
static void color_write(WS2812B_t* self, Ws2812b_color_u* color, unsigned char count)
{
	ASSERT(self->color_array_len != count, "color write len not equal");
	for(u8 i=0; i < self->color_array_len; ++i)
	{
		self->color_array[i] = color[i];
	}
	color_extend_write(self, color, count);
}


/**
 * @brief 控制多个led颜色, 直接使用外部传入的值控制led颜色
 * 
 * @param color 颜色
 */
static void color_all_write(WS2812B_t* self, Ws2812b_color_u color)
{
	for(u8 i=0; i < self->color_array_len; ++i)
	{
		self->color_array[i] = color;
	}
	color_extend_write(self, self->color_array, self->color_array_len);
}


/**
 * @brief 控制单个led颜色
 * 
 * @param color 颜色
 */
static void color_single_write(WS2812B_t* self, u8 idx, Ws2812b_color_u color)
{
	// 注: 该总线时序要求特别严格,所以必须要加中断保护
	ASSERT(idx > self->color_array_len-1, "color_single_write idx error");
	self->color_array[idx] = color;
	color_extend_write(self, self->color_array, self->color_array_len);
}


/**
 * @brief 控制多个led颜色, 直接使用外部传入的值控制led颜色
 * 此函数要求color_array_len小于等于32，否则直接进入error
 *
 * @param select [i]:代表第i个led， 0无操作 1写入颜色值
 * @param color 颜色
 */
static void color_select_write(WS2812B_t* self, u32 select, Ws2812b_color_u color)
{
	// 注: 该总线时序要求特别严格,所以必须要加中断保护
	ASSERT(self->color_array_len > 32, "color_select_write array len > 32\r\n");
	for(u8 i=0; i<self->color_array_len; ++i)
	{
		if((select >> i) & 0x01) {
			self->color_array[i] = color;
		}
		else
		{
			self->color_array[i] =  self->default_color;
		}
	}
	color_extend_write(self, self->color_array, self->color_array_len);
}

/**
 * @brief 控制多个led颜色, 将所有led设置为默认颜色
 * 
 * @param color 颜色数组
 * @param count 颜色数组长度
 */
static void color_all_default_write(WS2812B_t* self)
{
	color_all_write(self, self->default_color);
}

WS2812B_t* ws2812b_new(WS2812B_PIN_WRITE write_pin, Ws2812b_color_u* color_array, u8 color_array_len, Ws2812b_color_u default_color)
{
	WS2812B_t* self = (WS2812B_t*)malloc(sizeof(WS2812B_t));
	memset(self, 0, sizeof(WS2812B_t));
    self->m_write_pin =  write_pin;
    self->color_array = color_array;
	self->color_array_len = color_array_len;
	self->default_color = default_color;
	self->color_all_default_write = color_all_default_write;
	self->color_all_write = color_all_write;
	self->color_extend_write = color_extend_write;
	self->color_write = color_write;
	self->color_select_write = color_select_write;
	self->color_single_write = color_single_write;
	return self;
}











