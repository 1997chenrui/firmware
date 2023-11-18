#ifndef _LAMPCTL_H_
#define _LAMPCTL_H_
#include "stm32f4xx_hal.h"

#define LIGHT_CHANNEL_SIZE 27

void LampCtl(void);
void LightClearData(void);

typedef struct
{
	unsigned char	 func_start ;
	volatile unsigned char	 need_updata ;
	unsigned short   data[LIGHT_CHANNEL_SIZE];
}Wave;

#endif

