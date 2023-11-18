#ifndef __UASRT__
#define __UASRT__

#include "stm32f4xx_hal.h"
#include "ucos_ii.h"
#include "os_implement.h"

typedef struct sUARTRERECEIVE
{
		uint8_t  data[8];
		uint8_t IS_RCV_FLAG;//串口接收数据标示
		uint16_t lenght;//接收一帧数据的长度
	  OS_EVENT *sem; 
	  bus_os_sem 			data_sem;
}UARTRERECEIVE;

void UART_EnableUart(void);
uint8_t UART_Send(uint8_t *pdata,uint8_t len);
uint8_t UART_Getdata(uint8_t *pdata);


#endif

