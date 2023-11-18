#ifndef __WS2812B_H
#define __WS2812B_H

#include "gpio.h"
typedef void (*WS2812B_PIN_WRITE)(unsigned char value);

typedef struct _WS2812B_t{
    WS2812B_PIN_WRITE m_write_pin;
    void (*write)(struct _WS2812B_t* self, unsigned int * color, unsigned char count);
}WS2812B_t;

void ws2812b_init(WS2812B_t* self, WS2812B_PIN_WRITE write_pin);

#endif
