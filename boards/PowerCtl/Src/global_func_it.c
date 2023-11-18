#include "uart_byte_to_frame.h"
#include "stm32f4xx_hal.h"
#include "bus_uart6_f4_hal.h"
#include "bus_uart3_f4_hal.h"
#include "uart_thread.h"
#include "runze_sv04.h"
#include "project_cfg.h"


#if UART3_TO_PC == 1
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;

void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}
#endif

#if UART6_TO_PC == 1
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_rx;

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}

#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if UART6_TO_PC == 1
	if(huart == &huart6)
	{
        uart6_rx_data();
	}	
#endif
#if UART3_TO_PC == 1
    if(huart == &huart3)
	{
        uart3_rx_data();
	}
#endif

}
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
#if UART6_TO_PC == 1
	if(huart == &huart6)
	{
		uart6_rx_half_data();
	}
#endif
#if UART3_TO_PC == 1
    if(huart == &huart3)
	{
		uart3_rx_half_data();
    }
#endif
	
}



