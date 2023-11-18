#ifndef __UART_THREAD_H__
#define __UART_THREAD_H__
#include "macro.h"
#include "bus.h"
#include "uart_transpond.h"
#include "stm32f4xx.h"

typedef struct {
    Uart_transpond_t *transpond;
    UART_HandleTypeDef *uart;
    u8 enable;
} Uart_thread_t;


void uart_transpond_cmd_run(bus_packet *packet);
void uart_rev_thread_init(void (*thread_create)(void (*thread)(void *data)));
void uart_send_test(void);

#endif
