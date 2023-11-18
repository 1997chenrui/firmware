#ifndef __UART_TRANSPOND_H__
#define __UART_TRANSPOND_H__
#include "bus.h"
#include "macro.h"

#define UARTFIFO_CNT 512
#define DAEMON_TIME_CNT 100
#define DAEMON_TIME_SPACE 20
#define DAEMON_SEND_THRESHOLD 256

typedef void (*UART_SEND_BYTES)(u8 id, u8 *data, u32 data_len);

typedef struct {
    u8 buff[UARTFIFO_CNT];
    u16 rd_p;
    u16 wr_p;
    u16 size;
    u8 full;
    u8 empty;
} Uart_fifo_t;

typedef struct _Uart_transpond_t {
    u8 id;
    volatile u32 daemon_time;
    Uart_fifo_t fifo;
    void (*byte_rev)(struct _Uart_transpond_t *, u8 data);
    void (*data_send)(struct _Uart_transpond_t*, u8 *, u32);
    void (*pack_rev_check)(struct _Uart_transpond_t *);
} Uart_transpond_t;

Uart_transpond_t *uart_transpond_new(u8 id, UART_SEND_BYTES send);

#endif
