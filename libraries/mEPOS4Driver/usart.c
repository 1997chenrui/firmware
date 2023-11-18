#include "usart.h"
#include <stdint.h>
#include <stdlib.h>

#define USART_BUFF_LEN   20

extern void MX_USART2_UART_Init(void);

extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

__align(4) uint8_t UART2_RECEIVE_DATA[USART_BUFF_LEN];     			
   		

static UARTRERECEIVE UARTTxReceive; //串口接收

#define my_printf(f,s,n)    do\
							{\
								f==0 ? printf("Send: "):printf("Rcv: ");\
								for(uint8_t i=0;i<n;i++)\
									printf("0x%x ",s[i]);\
								printf("\r\n");\
							}while(0)


void UART_EnableUart()
{	    
	//UARTTxReceive.sem=OSSemCreate(0);
	bus_os_create_sem(&UARTTxReceive.data_sem,0);
	HAL_UART_Receive_DMA(&huart2, UART2_RECEIVE_DATA, USART_BUFF_LEN);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);			
}

uint8_t UART_Send(uint8_t *pdata,uint8_t len)
{
	uint8_t result=1;
	uint8_t stick=0;
	uint32_t err=0;    	    
	UARTTxReceive.IS_RCV_FLAG=0;
	do
	{					
		result=HAL_UART_Transmit_DMA(&huart2,pdata,len);
		
		stick++;
			
		if(result != HAL_OK) 
		{
			MX_USART2_UART_Init();
			printf("uart err  init.....\r\n");
		}
		else
		{
			err=bus_os_pend_sem(&UARTTxReceive.data_sem ,500);
			if(err == 0)
			{		
				result=0;
				break; //接收到返回信息说明发送成功
			}
			else
			{
				bus_os_sleep(10);
				printf("uart err / angin send %d\r\n",stick);
			}
		}
	}while(stick<10); 
	
	return result;
}

uint8_t UART_Getdata(uint8_t *pdata)
{
	uint8_t result=1;
	if(UARTTxReceive.IS_RCV_FLAG)
	{
		for(uint8_t i=0;i<4;i++)
		{
			pdata[i]=UARTTxReceive.data[i];
		}
		result=0;
	}
	return result;
}


void UartCallback(UART_HandleTypeDef *huart)
{  		
	/*//接收到的数据帧头  读数据的时侯返回的数据帧长度都是14   
	其它的都是10  使用简单帧过滤  不能用长度等于14来简单校验，有时接收到的长度会大于14个字节 */
	if(huart->pRxBuffPtr[0] == 0x90 && huart->pRxBuffPtr[1] == 0x02)//&& (USART_BUFF_LEN-huart->hdmarx->Instance->NDTR == 14))  
	{
		for(uint8_t i=8,j=0;i<USART_BUFF_LEN-huart->hdmarx->Instance->NDTR-2;j++)
		{
			if(huart->pRxBuffPtr[i] == 0x90 && huart->pRxBuffPtr[i+1] == 0x90)
			{
				UARTTxReceive.data[j]=0x90;			
				i+=2;
			}		
			else
			{
				UARTTxReceive.data[j]=huart->pRxBuffPtr[i];			
				i+=1;											
			}
		}					 
		UARTTxReceive.lenght=USART_BUFF_LEN-huart->hdmarx->Instance->NDTR;
		UARTTxReceive.IS_RCV_FLAG=1;		
		bus_os_post_sem(&UARTTxReceive.data_sem);						
	}			
	//		my_printf(1,huart->pRxBuffPtr,USART_BUFF_LEN-huart->hdmarx->Instance->NDTR);			
	for(uint8_t i=0;i<USART_BUFF_LEN;i++) huart->pRxBuffPtr[i]=0;		   			
}

void MYDMA_Enable(UART_HandleTypeDef *huart,uint16_t ndtr)
{
	huart->hdmarx->Instance->CR=~(1<<0); 
	while(huart->hdmarx->Instance->CR&0X1); 
	huart->hdmarx->Instance->NDTR=ndtr;
	huart->hdmarx->Instance->CR |= 1<<0;
}


void USART2_IRQHandler(void)
{	    
	OSIntEnter();	
	HAL_UART_IRQHandler(&huart2);
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!=RESET))
	{    										
		UartCallback(&huart2);
		MYDMA_Enable(&huart2,USART_BUFF_LEN);
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);				
	}			
	huart2.Instance->SR;
	huart2.Instance->DR;
	OSIntExit(); 
}

void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
}


void DMA1_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

