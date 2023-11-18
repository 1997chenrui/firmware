#include "pwm_set.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "ErrCode.h"
#include <stdbool.h>
//通道数   +   Tim 
//bit 0-3 定时器  bit 4-7 通道
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim4;
#define TIM_ADDR_SPAN 				0x0400U  //TIM_x和TIM_x+1之间的跨度


//TIM2至TIM7
static uint32_t time_channel_2_T(uint8_t time_channle)
{
	uint8_t tid = time_channle & 0x0f;
//	uint8_t cid = time_channle >> 4;
	
	uint32_t t;
	
	switch(tid)
	{
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			t = APB1PERIPH_BASE + TIM_ADDR_SPAN * (tid-2);
			break;
		case 1:
			t = (uint32_t)TIM1;
			break;
		case 8:
			t = (uint32_t)TIM8;
			break;
		case 9:
		case 10:
		case 11:
			t = APB2PERIPH_BASE + 0x4000U + TIM_ADDR_SPAN * (tid-9);
			
			break;
		default:break;
	}
//	APB1PERIPH_BASE + TIM_ADDR_SPAN * (tid-2);
	return t;
}

//0x34:捕获/比较寄存器 1 偏移地址
static uint32_t time_channel_2_TxCy(uint8_t time_channle)
{
	uint8_t cid = (time_channle >> 4)&0x0f;
	
	uint32_t t = time_channel_2_T(time_channle);
	return (t + 0x34 + ((cid-1)<<2));
}

//0x2C:自动重装寄存器偏移地址
static uint32_t time_channel_2_TxAy(uint8_t time_channle)
{
	
	uint32_t t = time_channel_2_T(time_channle);
	
	return t+0x2c;
}

void updatePluse(uint8_t time_channle, uint32_t dutyCycle)
{
	
	uint32_t* cy = (uint32_t*)time_channel_2_TxCy(time_channle);
	
	*cy = dutyCycle;
}

void updateARR(uint8_t time_channle,uint32_t period)
{
	uint32_t* ay = (uint32_t*)time_channel_2_TxAy(time_channle);
	
	*ay = period;
}

uint32_t updatePwm(uint8_t time_channle,uint32_t period,uint32_t dutyCycle)
{
	if((time_channle & 0x0f) > 12 || (time_channle >> 4) > 4)
		return ERR_APP_PRAM_ERR;
	updateARR(time_channle,period);
	updatePluse(time_channle,dutyCycle);
	return 0;
}

bool startPumpPwm(uint8_t time_channle)
{	
	TIM_TypeDef* tx = (TIM_TypeDef*)time_channel_2_T(time_channle);
	
	uint8_t cid = (time_channle >> 4)&0x0f;
	
	if(cid > 4)
		return false;
	TIM_CCxChannelCmd(tx, (cid-1)<<2, TIM_CCx_ENABLE);// 使能捕获/比较通道Channle_x
	tx->CR1 |=(TIM_CR1_CEN);//使能定时器
	
//	printf("	tx->CR1 addr = %x,cid = %d\r\n",(int)(&(tx->CR1)),cid);
	
	return true;
	
//     TIM8->CCER &= ~TIM_CCER_CC2E;
//     TIM8->CCER &= ~TIM_CCER_CC3E;
//   
//     TIM8->CCER |= TIM_CCER_CC2NE;
//     TIM8->CCER |= TIM_CCER_CC3NE;
}
