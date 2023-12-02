#include "uart_config.h"
#include <string.h>
#include <stdlib.h>
#include "async.h"

struct _uart_cfg usart_array[UART_TRANSPND_CNT] = {
    {
        .is_enable = 1,
        .huart = &huart3  
    },
    {
        .is_enable = 1,
        .huart = &huart4 
    }
};

struct _uart_transpond uart_transpond[UART_TRANSPND_CNT];

static void UartTransCheck(struct _uart_transpond *uart_transpond);
static void ByteTranspond(u8 data, struct _uart_transpond *uart_transpond);
static void UartSend(u8 *data, struct _uart_cfg *uart_array, u16 data_len);
    
void UartTranspndInit(void)
{
    for(int i=0; i<UART_TRANSPND_CNT; i++)
    {
        uart_transpond[i].time = 0;
        uart_transpond[i].UartTransCheck = UartTransCheck;
        uart_transpond[i].uart_cfg = &usart_array[i];
        uart_transpond[i].ByteTranspond = ByteTranspond;
        uart_transpond[i].UartSend = UartSend;
        struct _fifo *self = &uart_transpond[i].uart_cfg->fifo;
        self->empty = 0;
        self->full = 0;
        self->r_s = 0;
        self->w_s = 0;
        self->size = 0;
    } 
}


static bool FifoRead(u8 *buff, struct _fifo *fifo, u16 length)
{
    if(fifo->size < length)
    {
        fifo->empty = 1;
        printf("fifo buff is empty\r\n");
        return 1;
    }
    else
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
        fifo->size -= length;
        return 0;
    }
}

static bool FifoWrite(u8 *buff, struct _fifo *fifo, u16 length)
{
    if(fifo->size + length > FIFO_SIZE_MAX - 1)
    {
        fifo->size = FIFO_SIZE_MAX;
        fifo->full = 1; 
        printf("fifo buff is full\r\n");
        return 1;
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
        if(FIFO_SIZE_MAX == fifo->size)
        {
            fifo->full = 1;
            printf("fifo buff is full too\r\n");
            return 1;
        }
        return 0;
    }
}

static void ByteTranspond(u8 data, struct _uart_transpond *uart_transpond)
{
    FifoWrite(&data, &uart_transpond->uart_cfg->fifo, 1);
    uart_transpond->time = 0;
}

void USART3_IRQHandler(void)
{
    int data = 0;
	data = USART3->SR;
	if(data & USART_SR_RXNE )
	{
		data = USART3->DR;
        uart_transpond[0].ByteTranspond(data, &uart_transpond[0]);
	}
	data = USART3->DR;
}

void USART4_IRQHandler(void)
{
    int data = 0;
	data = UART4->SR;
	if(data & USART_SR_RXNE )
	{
		data = UART4->DR;
        uart_transpond[0].ByteTranspond(data, &uart_transpond[0]);
	}
	data = UART4->DR;
}
static void UartSend(u8 *data, struct _uart_cfg *uart_array, u16 data_len)
{
    USART_TypeDef *UARTX = uart_array->huart->Instance;
    for (u16 i = 0; i < data_len; i++) {
        UARTX->DR = (data[i] & (u16)0x01FF);
        while ((UARTX->SR & UART_FLAG_TC) == (u16)RESET) {
        }
    }
}

static void UartTransCheck(struct _uart_transpond *uart_transpond)
{
    struct _fifo *self = &uart_transpond->uart_cfg->fifo;
    if(uart_transpond->time > UART_TIME_THRESHOLD || self->size > FIFO_SIZE_THRESHOLD)
    {
        if(self->size > FIFO_SIZE_THRESHOLD)
        {
            u8 *buff = (u8 *)malloc(FIFO_SIZE_THRESHOLD);
            FifoRead(buff, self, FIFO_SIZE_THRESHOLD);
            uart_transpond->UartSend(buff, uart_transpond->uart_cfg, FIFO_SIZE_THRESHOLD);
            u8 data[5] = {'0', '0', '5', '0', '0'};
            FifoWrite(data, self, 5);
            FifoRead(buff, self, self->size);
            uart_transpond->UartSend(buff, uart_transpond->uart_cfg, self->size);
        }
        else if(uart_transpond->time > UART_TIME_THRESHOLD && self->size > 0)
        {
            u8 data[5] = {'0', '0', '5', '0', '0'};
            FifoWrite(data, self, 5);
            u8 *buff = (u8 *)malloc(self->size);
            FifoRead(buff, self, self->size);
            uart_transpond->UartSend(buff, uart_transpond->uart_cfg, self->size);
        }       
    }
    else
    {
         uart_transpond->time += UART_TIME_SPAN;
    }
}
static void UartTransRevThread(void)
{
    printf("UartTransRevThread create successfully\r\n");
    while(1)
    {
        for(int i=0; i<UART_TRANSPND_CNT; i++)
        {
            if(uart_transpond[i].uart_cfg->is_enable)
            {
                uart_transpond[i].UartTransCheck(&uart_transpond[i]);
            }
        }
        OSTimeDly(UART_TIME_SPAN);
    } 
}
