#ifndef BUS_UART_F4_HAL_H_
#define BUS_UART_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdio.h>
#include "errCode.h"
#include "uart_byte_to_frame.h"



#define UART4_ADAPTER_ID            4
	

u32 uart4_adapter_send(void *this, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 uart4_adapter_init(void *this, u8 localId)	;


extern bus_adapter Uart4Adapter;

#endif


