#include <stdlib.h>
#include "uart_cfg.h"
#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart4;


Uart_thread_t uart_array[UART_TRANSPOND_CNT] = {
	{
        .uart = &huart4,
        .enable = 1
    }
};

void uart_thread_hd_init(void){
	__HAL_UART_CLEAR_FLAG(&huart4,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
}


//void USART3_IRQHandler(void)
//{
//	volatile int data = 0;
//	data = USART3->SR;

//	if(data & USART_SR_RXNE )
//	{
//		data = USART3->DR;
//        Uart_transpond_t* transpond = uart_array[1].transpond;
//        transpond->byte_rev(transpond, data);
//	}
//	data = USART3->DR;
//}

void UART4_IRQHandler(void)
{
	volatile int data = 0;
	data = UART4->SR;
	if(data & USART_SR_RXNE )
	{
		data = UART4->DR;
        Uart_transpond_t* transpond = uart_array[0].transpond;
        transpond->byte_rev(transpond, data);
	}
}

