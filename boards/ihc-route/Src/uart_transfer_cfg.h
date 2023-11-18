#ifndef UART_TRANSFER_CFG_H_
#define UART_TRANSFER_CFG_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "bus_uart6_f4_hal.h"

#define UARTFIFO_CNT 512
#define DAEMON_TIME_CNT								100
#define DAEMON_TIME_SPACE							20
#define DAEMON_SEND_THRESHOLD						256

#define UART_TRANSFER_SEND(data_len,data,dataout,dataoutLen)	\
		bus_ita_hb_send(UART6_ADAPTER_ID, 0, data_len, data, dataout, dataoutLen)		

#endif
