#ifndef BUS_CAN_F4_HAL_H_
#define BUS_CAN_F4_HAL_H_

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdio.h>

	
typedef union {
    u32 expId;
    struct{
        u32  sessionId : 8;
        u32  funcId    : 5;
        u32  srcId     : 8;
        u32  desId     : 8;
        u32  not_use   : 3;
    }atr;
}exp_id_union;    

#define CAN_ADAPTER_ID            0
#define CAN1_BAUD 				  1000000
#define CAN1_BD_ID				  0xff

extern  bus_adapter CanAdapter;

u32 can_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 can_adapter_init(void *ths, u8 localId)	;


#endif


