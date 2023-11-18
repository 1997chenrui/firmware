#include "usart1.h"
#include "MotorDebugThread.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
			  		
__align(4) uint8_t UART1_RECEIVE_DATA[USART_REC_LEN];   

void Uart1Callback(UART_HandleTypeDef *huart)
{  	  
	extern MOTOR_DEBUG_THRAD MotorDebug_Thread;

	for(uint8_t i=USART_REC_LEN-huart->hdmarx->Instance->NDTR;i<USART_REC_LEN;i++)
	{
		UART1_RECEIVE_DATA[i]=0;	
	}	

	OSQPost(MotorDebug_Thread.Queue,(void *)UART1_RECEIVE_DATA);
}

void Usart1_Start()
{  
	HAL_UART_Receive_DMA(&huart1,UART1_RECEIVE_DATA,USART_REC_LEN);						//启动发送
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);		//开启接收中断	开启中断后 会先出发一次中断
}


void MYDMA1_Enable(UART_HandleTypeDef *huart,uint16_t ndtr)
{
	huart->hdmarx->Instance->CR=~(1<<0); //停止DMA
	while(huart->hdmarx->Instance->CR&0X1); 
	huart->hdmarx->Instance->NDTR=ndtr;
	huart->hdmarx->Instance->CR |= 1<<0;
}


void USART1_IRQHandler(void)                	
{
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!=RESET))
	{    				
		 Uart1Callback(&huart1);								
		 MYDMA1_Enable(&huart1,USART_REC_LEN);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);				
	}	
} 

void DMA2_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}


void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //当接收到的数据超过缓冲数组时  调用
//{
//}

