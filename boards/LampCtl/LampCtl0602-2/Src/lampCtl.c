#include "lampCtl.h"
#include "ucos_ii.h"
#include "procfg.h"
extern TIM_HandleTypeDef htim6;

void HAL_Delay_us(uint16_t us)
{
	uint16_t differ=0xffff-us-5;
	__HAL_TIM_SET_COUNTER(&htim6,differ);
	HAL_TIM_Base_Start(&htim6);
	while(differ<0xffff-6)
	{
		differ=__HAL_TIM_GET_COUNTER(&htim6);
	}
	HAL_TIM_Base_Stop(&htim6);
}

void LampCtl(void)
{

		//全灭 1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);//全灭
	    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(4);
		HAL_Delay_us(120);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		OSTimeDly(4);
		HAL_Delay_us(260);
	    //HAL_Delay_us(400);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(4);
		HAL_Delay_us(120);
	
		//全灭 2
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);//全灭
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		HAL_Delay_us(1500);
		
		//全灭 3
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);//全灭
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		HAL_Delay_us(1500);
		
		//亮紫蓝光 405nm 4
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//紫光
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11|GPIO_PIN_10, GPIO_PIN_SET);
		
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		HAL_Delay_us(1500);
		
		//全灭 5
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);//全灭


		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		HAL_Delay_us(1500);
		
		//575nm 6
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);//黄光
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		
		
		//670nm 7
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);//红光
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_SET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);	

		HAL_Delay_us(1500);
		
		
				
		//全灭 8
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);//全灭
		OSTimeDly(5);
		HAL_Delay_us(825);
		//拉高同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay_us(850);
		//拉低同步
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
		OSTimeDly(5);
		HAL_Delay_us(825);
		
		HAL_Delay_us(1500);
	
	
}

