#ifndef __UART_CONFIG_H
#define __UART_CONFIG_H

#include "stm32f4xx_hal.h"
#include "macro.h"

#define FIFO_SIZE_MAX 512
#define FIFO_SIZE_THRESHOLD 256

#define UART_TIME_SPAN 20
#define UART_TIME_THRESHOLD 500

#define UART_TRANSPND_CNT 2
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

struct _fifo{
	volatile u16 size;
	volatile u16 w_s;
	u16 r_s;
	u8 buff[FIFO_SIZE_MAX];
	bool full;
	bool empty;
};

struct _uart_cfg{
    struct _fifo fifo;
    bool is_enable;
    UART_HandleTypeDef *huart;
};

struct _uart_transpond{
    struct _uart_cfg *uart_cfg;
    void (*UartTransCheck)(struct _uart_transpond * );
    void (*ByteTranspond)(u8 data, struct _uart_transpond *uart_transpond);
    void (*UartSend)(u8 *data, struct _uart_cfg *uart_array, u16 data_len);
    u16 time;
};

void UartTranspndInit(void);
#endif
