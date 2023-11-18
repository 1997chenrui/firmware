#ifndef __WS2812B_H
#define __WS2812B_H
#include "macro.h"

typedef void (*WS2812B_PIN_WRITE)(u8 value);

typedef union
{
    struct
    {
        u32 b:8;
        u32 r:8;
        u32 g:8;
        u32 invalid:8;
    }rgb;
    u32 value;
}Ws2812b_color_u;

typedef struct _WS2812B_t{
    WS2812B_PIN_WRITE m_write_pin;
	Ws2812b_color_u* color_array;
	u8 color_array_len;
	Ws2812b_color_u default_color;
    void (*color_all_write)(struct _WS2812B_t* self, Ws2812b_color_u color);
	void (*color_all_default_write)(struct _WS2812B_t* self);
    void (*color_extend_write)(struct _WS2812B_t* self, Ws2812b_color_u* color, u8 count);
	void (*color_write)(struct _WS2812B_t* self, Ws2812b_color_u* color, u8 count);
	void (*color_select_write)(struct _WS2812B_t* self, u32 select, Ws2812b_color_u color);
	void (*color_single_write)(struct _WS2812B_t* self, u8 idx, Ws2812b_color_u color);
}WS2812B_t;

WS2812B_t* ws2812b_new(WS2812B_PIN_WRITE write_pin, Ws2812b_color_u* color_array, u8 color_array_len, Ws2812b_color_u default_color);
#endif
