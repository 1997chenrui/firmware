#ifndef BUS_UART3_F4_HAL_H_
#define BUS_UART3_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdio.h>
#include "errCode.h"
#include "uart_byte_to_frame.h"



#define UART5_ADAPTER_ID            6
	
void uart5_rx_half_data(void);
void uart5_rx_data(void);
u32 uart5_adapter_send(void *this, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 uart5_adapter_init(void *this, u8 localId)	;


extern bus_adapter Uart5Adapter;

#endif


