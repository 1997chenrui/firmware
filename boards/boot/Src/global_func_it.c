#include "uart_byte_to_frame.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "delayus.h"
#include "bus_uart3_f4_hal.h"
#include "bus_uart6_f4_hal.h"

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_rx;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		uart3_rx_data();
	}else if(huart == &huart6)
	{
		uart6_rx_data();
	}
}


 void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		uart3_rx_half_data();
	}else if(huart == &huart6)
	{
		uart6_rx_half_data();
	}
}

void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}



void Power_Close()
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);//启动按钮(指示灯)关
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);//温控供电控制 关
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);//运动供电控制 关
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);//屏幕供电控制  关
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//清除中断标志位
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
	
	//屏蔽GPIO_PIN_11的中断
	EXTI->IMR &= (~GPIO_PIN_11);
	
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11) != GPIO_PIN_SET)//启动按钮
	{
		EXTI->IMR |= GPIO_PIN_11; // 开中断
		return;
	}
	
	unsigned char delay_cnt = 100;
	for(unsigned char i = 0; i < 10; i++)
	{
		delayMs(delay_cnt);
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11) != GPIO_PIN_SET)
		{
			EXTI->IMR |= GPIO_PIN_11; // 开中断
			return;
		}
	}

	Power_Close();
	EXTI->IMR |= GPIO_PIN_11; // 开中断
}


void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
}





