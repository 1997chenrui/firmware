#include "uart_byte_to_frame.h"
#include "stm32f4xx_hal.h"
#include "uart_transfer.h"
#include "bus_uart6_f4_hal.h"

extern UartReceive_Struct uartrs2;
extern UartReceive_Struct uartrs4;

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_rx;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart == &huart6)
	{
		uart6_rx_data();
	}

}
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart6)
	{
		uart6_rx_half_data();
	}
}

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}

//void USART2_IRQHandler(void)
//{
//	volatile int data = 0;
//	data = USART2->SR;
//	if(data & USART_SR_RXNE )
//	{
//		data = USART2->DR;
//		UartReceiveEvent(&uartrs2,2,data);
//	}
//	data = USART2->DR;
//}

//void UART4_IRQHandler(void)
//{
//	volatile int data = 0;
//	data = UART4->SR;
//	if(data & USART_SR_RXNE )
//	{
//		data = UART4->DR;
//		UartReceiveEvent(&uartrs4,4,data);
//	}
//	data = UART4->DR;
//}
