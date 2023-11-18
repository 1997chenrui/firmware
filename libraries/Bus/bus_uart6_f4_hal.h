#ifndef BUS_UART6_F4_HAL_H_
#define BUS_UART6_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdio.h>
#include "uart_byte_to_frame.h"



#define UART6_ADAPTER_ID            9
	
void uart6_rx_half_data(void);
void uart6_rx_data(void);
u32 uart6_adapter_send(void *this, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 uart6_adapter_init(void *this, u8 localId)	;


extern bus_adapter Uart6Adapter;

#endif


