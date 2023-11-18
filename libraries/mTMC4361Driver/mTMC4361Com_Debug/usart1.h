#ifndef _USART1_H
#define _USART1_H

#include "stm32f4xx_hal.h"

#define UART1TxDMA_TIMEOUT		5000	
#define USART_REC_LEN  50

void USART1_DMA_send(uint8_t* sendbuf,uint8_t length);

void Usart1_Start(void);


#endif
