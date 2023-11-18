#include <stdlib.h>
#include "uart_cfg.h"
//#include "tool.h"
//#include "io_mod.h"
#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart4;


Uart_thread_t uart_array[UART_TRANSPOND_CNT] = {
    {
        .uart = &huart5,
        .enable = 1
    },
    {
        .uart = &huart4,
        .enable = 1
    }
		
};

void uart_thread_hd_init(void){
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);

}

void UART5_IRQHandler(void)
{
	volatile int data = 0;
	data = UART5->SR;
	if(data & USART_SR_RXNE )
	{
		data = UART5->DR;
        Uart_transpond_t* transpond = uart_array[0].transpond;
        transpond->byte_rev(transpond, data);
	}
	data = UART5->DR;
}

void UART4_IRQHandler(void)
{
	volatile int data = 0;
	data = UART4->SR;
	if(data & USART_SR_RXNE )
	{
		data = UART4->DR;
        Uart_transpond_t* transpond = uart_array[1].transpond;
        transpond->byte_rev(transpond, data);
	}
	data = UART4->DR;
}





