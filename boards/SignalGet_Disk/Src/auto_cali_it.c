#include "auto_cali_it.h"

 void open_exti(void)
{
	EXTI->IMR |= GPIO_PIN_13;
}
 void close_exti(void)
{
	EXTI->IMR &= (~GPIO_PIN_13);
}

 void set_tim_cnt(uint32_t value)
{           
    TIM2->ARR = value ;                     
}

 void open_tim(void)                        
{                                                           
	TIM2->CNT = 0;                                          
	HAL_TIM_Base_Start_IT(&htim2);                      
}         

 void close_tim(void)                         
{                                                           
	HAL_TIM_Base_Stop(&htim2);                             
}

void auto_it_Init(auto_it *me,void(*opentim)(void),void(*closetim)(void),void(*openexti)(void),void(*closeexti)(void),void(*settime)(uint32_t us))
{
    me->close_exit = closeexti;
    me->close_timer = closetim;
    me->open_exit = openexti;
    me->open_timer = opentim;
    me->set_timer = settime;
}








