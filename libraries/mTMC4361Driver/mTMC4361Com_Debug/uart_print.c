#include "stm32f4xx_hal.h"
#include <stdarg.h>
#include <stdio.h>

#if 1

#define PRINTF_BUF_MAX   25

#pragma import(__use_no_semihosting)


extern UART_HandleTypeDef huart1; 

struct __FILE     //标准库需要的支持函数   
{ 
	int handle; 
}; 

FILE __stdout;       

void _sys_exit(int x)   // 定义_sys_exit()以避免使用半主机模式    
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)   //重定义fputc函数 
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
	return ch;
}

void _ttywrch(int ch)
{
    uint8_t data = ch;
    HAL_UART_Transmit(&huart1,&data,1,0xffff);
}

//void printf_dma(const char *format, ...)
//{
//	  __align(4) uint8_t print_buffer[PRINTF_BUF_MAX];				//打印缓存
//		uint32_t length;
//		__va_list args; 

//		va_start(args, format);
//		length = vsnprintf((char*)print_buffer, sizeof(print_buffer), (char*)format, args);//格式化内容
//		va_end(args);
//		
//		HAL_UART_Transmit_DMA(&huart1,print_buffer,length);
//}


#endif 



