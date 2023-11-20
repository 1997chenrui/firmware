#include "uart_config.h"
#include "macro.h"
#include <string.h>
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#define FIFO_SIZE_MAX 512
#define FIFO_SIZE_THRESHOLD 256

#define UART_TIME_SPAN 20
#define UART_TIME_THRESHOLD 500

#define UART_TRANSPND_CNT 2

struct _fifo{
	u16 size;
	u16 w_s;
	u16 r_s;
	u8 *buff;
	bool full;
	bool empty;
};

struct _uart_cfg{
    struct _fifo *fifo;
    bool is_enable;
    UART_HandleTypeDef *huart;
};

struct _uart_transpond{
    struct _uart_cfg *uart_cfg;
    u16 time;
};

struct _uart_transpond uart_transpond[UART_TRANSPND_CNT];

static void FifoRead(u8 *buff, struct _fifo *fifo, u16 length)
{
	if(fifo->r_s + length > FIFO_SIZE_MAX-1)
	{
        memmove(buff, fifo->buff + fifo->r_s, FIFO_SIZE_MAX-1-fifo->r_s);
        memmove(buff + FIFO_SIZE_MAX-1-fifo->r_s, fifo->buff, length -(FIFO_SIZE_MAX-1-fifo->r_s));
        fifo->r_s = FIFO_SIZE_MAX-1-fifo->r_s;
	}
	else
    {
        memmove(buff, fifo->buff, length);
        fifo->r_s += length;
    }
}

static void FifoWrite(u8 *buff, struct _fifo *fifo, u16 length)
{
    if(fifo->size + length > FIFO_SIZE_MAX - 1)
    {
        fifo->size = FIFO_SIZE_MAX - 1;
        fifo->full = 1;      
    }
    else
    {
        if(fifo->w_s + length > FIFO_SIZE_MAX-1)
        {
            memmove(fifo->buff + fifo->w_s, buff, FIFO_SIZE_MAX-1-fifo->w_s);
            memmove(fifo->buff, buff + FIFO_SIZE_MAX-1-fifo->w_s, length -(FIFO_SIZE_MAX-1-fifo->w_s));
            fifo->w_s = FIFO_SIZE_MAX-1-fifo->w_s;
        }
        else
        {
            memmove(fifo->buff, buff, length);
            fifo->w_s += length;
        }
        fifo->size += length;
    }
}

static void ByteTranspond(u8 data)
{
    FifoWrite(&data, uart_transpond[1].uart_cfg->fifo, 1);
}

void USART1_IRQHandler(void)
{
    int data = 0;
	data = USART1->SR;
	if(data & USART_SR_RXNE )
	{
		data = USART1->DR;
//        Uart_transpond_t* transpond = uart_array[0].transpond;
//        transpond->byte_rev(transpond, data);
        ByteTranspond(data);
	}
	data = USART1->DR;
}

void USART2_IRQHandler(void)
{
    int data = 0;
	data = USART2->SR;
	if(data & USART_SR_RXNE )
	{
		data = USART2->DR;
//        Uart_transpond_t* transpond = uart_array[0].transpond;
//        transpond->byte_rev(transpond, data);
        ByteTranspond(data);
	}
	data = USART2->DR;
}
static void UartSend(u8 *data)
{
    
}

static void UartTransCheck(struct _uart_transpond *uart_transpond)
{
    struct _fifo *self = uart_transpond->uart_cfg->fifo;
    if(uart_transpond->time > UART_TIME_THRESHOLD || self->size > FIFO_SIZE_THRESHOLD)
    {
        if(self->size > FIFO_SIZE_THRESHOLD)
        {
            u8 *buff = (u8 *)malloc(FIFO_SIZE_THRESHOLD);
            FifoRead(buff, self, FIFO_SIZE_THRESHOLD);
            UartSend(buff);
            u8 data[5] = {'0', '0', '5', '0', '0'};
            FifoWrite(data, self, 5);
            FifoRead(buff, self, self->size);
            UartSend(buff);
        }
        else if(uart_transpond->time > UART_TIME_THRESHOLD && self->size > 0)
        {
            u8 data[5] = {'0', '0', '5', '0', '0'};
            FifoWrite(data, self, 5);
            u8 *buff = (u8 *)malloc(self->size);
            FifoRead(buff, self, self->size);
            UartSend(buff);
        }
        uart_transpond->time += UART_TIME_SPAN;
    }
}
