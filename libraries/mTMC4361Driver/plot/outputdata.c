#include "stm32f4xx.h"

#define CMD_WARE 3

extern UART_HandleTypeDef huart6;

void Usart_SendByte(USART_TypeDef * pUSARTx,char ch)
{
    USART1->DR = (ch & (uint16_t)0x01FF);
    while( !(USART1->SR & USART_SR_TC));  
}

void Plot(unsigned int value)
{
	Usart_SendByte(USART1, 0xAA);
	Usart_SendByte(USART1, 0xFF);
	Usart_SendByte(USART1, CMD_WARE);
	Usart_SendByte(USART1, ~CMD_WARE);
	Usart_SendByte(USART1, 0xFF & (value>>0));
	Usart_SendByte(USART1, 0xFF & (value>>8));
	Usart_SendByte(USART1, 0xFF & (value>>16));
	Usart_SendByte(USART1, 0xFF & (value>>24));
	Usart_SendByte(USART1, ~CMD_WARE);
	Usart_SendByte(USART1, CMD_WARE);
}

