#include "uart_byte_to_frame.h"
#include "stm32f4xx_hal.h"
#include "uart_byte_to_frame.h"
#include "SP_DR100CtrlHandle.h"

extern u8 uart3_rx_buff[DR100DATALENGTH] ;
extern UART_HandleTypeDef huart3;
extern  DMA_HandleTypeDef hdma_usart3_rx;
extern ubtf_struct ubtf3;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	char i=0;
	if(huart == &huart3)
	{
//        ubtf_notify_byte_received(&ubtf3,uart3_rx_buff[1]);	
		    for(i=0;i<DR100DATALENGTH;i++)
				{
						ubtf3.buff[i]=uart3_rx_buff[i];
				}
				ubtf3.rx_call( (uart_frame_struct *) (ubtf3.buff) );
	}
	
}


 void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
        ubtf_notify_byte_received(&ubtf3,uart3_rx_buff[0]);
	}	
}



void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}
