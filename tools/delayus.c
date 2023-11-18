#include "delayus.h"


void delayUs(u16 us)
{
	u32 delay_i = 0,delay_j = 0;
	for(delay_j = 0; delay_j< us; delay_j++)
		for(delay_i = 0;delay_i<39;delay_i++);
}

void delayMs(u32 ms)
{
	for(u32 i = 0; i < ms; i++)
		delayUs(1000);
}
