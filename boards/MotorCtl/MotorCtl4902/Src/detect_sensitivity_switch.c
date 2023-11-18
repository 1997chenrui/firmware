#include <stdlib.h>
#include "detect_sensitivity_switch.h"
#include "tool.h"
#include "io_mod.h"

void detect_sen_switch(u8 ctrl_switch)
{
	if(ctrl_switch == 0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
	}
	
}
