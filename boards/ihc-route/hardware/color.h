#ifndef __COLOR_H__
#define __COLOR_H__
#include "macro.h"
#include "stm32f4xx_hal.h"
#include "bus.h"
#include "ws2812b.h"

#define REG_LED_CNT 3
#define SLIDE_LED_CNT 5


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
}Color_u;

typedef struct
{
    u8 led_len;
    u32* led_array;
    WS2812B_t ws;
}ColorChip_t;


typedef struct
{
    ColorChip_t reg_chip;
    ColorChip_t slide_chip;
    u32 (*pack)(u8 r, u8 g, u8 b);
    void (*write)(u8 channel, u32 color);
    void (*write_report)(bus_packet* packet);
}ColorMod_t;


void colormod_write_color_test(u8 channel, u8 r, u8 g, u8 b);
void colormod_init(void);
ColorMod_t* colormod_get_as_ref(void);
#endif
