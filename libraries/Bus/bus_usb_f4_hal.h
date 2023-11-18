#ifndef BUS_USB_F4_HAL_H_
#define BUS_USB_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdio.h>
#include "errCode.h"
#include "uart_byte_to_frame.h"



#define USB_ADAPTER_ID            10
	

u32 usb_adapter_send(void *this, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 usb_adapter_init(void *this, u8 localId)	;
void CDC_Receive_FS_Callback (uint8_t* Buf, uint32_t *Len);

extern bus_adapter usbAdapter;

#endif


