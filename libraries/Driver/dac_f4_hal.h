#ifndef DAC_F4_HAL_H_
#define DAC_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
void DAC_Open(uint8_t channelId,uint16_t data);
void DAC_Close(uint8_t channelId);

#endif

