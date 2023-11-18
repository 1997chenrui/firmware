/*
    盘片控制 基础接口
*/
#include "disk_ctrl_base.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "delayus.h"

extern TIM_HandleTypeDef htim2;

light_ctrl_base_t 		lightCtrlBase;

void adc_start_exit2_it(void)
{
	EXTI->IMR |= GPIO_PIN_2;
}
void adc_stop_exit2_it(void)
{
	EXTI->IMR &= (~GPIO_PIN_2);
}

void adc_set_tim2_cnt(uint32_t value)
{                                                      
    TIM2->ARR = value;                     
}

void adc_start_tim2(void)                        
{                                                           
	TIM2->CNT = 0;                                          
	HAL_TIM_Base_Start_IT(&htim2);                      
}         

void adc_stop_tim2(void)                         
{                                                           
	HAL_TIM_Base_Stop(&htim2);                             
}

void disk_ctrl_base_init(void)
{
	lightCtrlBase.set_timer = adc_set_tim2_cnt;
	lightCtrlBase.start_timer = adc_start_tim2;
	lightCtrlBase.stop_timer = adc_stop_tim2;
	lightCtrlBase.start_exit2 = adc_start_exit2_it;
	lightCtrlBase.stop_exit2 = adc_stop_exit2_it;
	
	lightCtrlBase.stop_exit2();
}


