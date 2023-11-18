#include "dac.h"
#include "stm32f4xx_hal.h"

#define CHANNEL DAC_CHANNEL_1
extern DAC_HandleTypeDef hdac;

void dac_write(u32 value){
	HAL_DAC_Stop(&hdac, CHANNEL);
	HAL_DAC_SetValue(&hdac, CHANNEL, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, CHANNEL);
}
