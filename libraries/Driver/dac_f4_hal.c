#include "dac_f4_hal.h"

void DAC_Open(uint8_t adcId,uint16_t data)
{
	switch(adcId)
	{
		case 1:
		{
			DAC->CR |= 1;
			DAC->DHR12R1 = data;
			DAC->SWTRIGR |= 1;
			break;
		}
		case 2:
		{
			
			DAC->CR |= 1<<16;
			DAC->DHR12R2 = data;
			DAC->SWTRIGR |= 1<<1;
			break;
		}
		default:break;
	}
}
void DAC_Close(uint8_t adcId)
{
	switch(adcId)
	{
		case 0:
		{
			DAC->CR &= ~(1<<0);
			break;
		}
		case 1:
		{
			DAC->CR &= ~(1<<16);
			break;
		}
		default:break;
	}
}
