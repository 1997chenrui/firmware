#ifndef UART_TRANSFER_H_
#define UART_TRANSFER_H_

#include "bus.h"
#include "ErrCode.h"
#include "uart_transfer_cfg.h"
typedef struct
{
	u32            cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 
    void       *send;            //send
}UartSend_Queue_Ele_t;

typedef struct
{
	u32            cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        
    void       *send;            
}UartReceive_Queue_Ele_t;

typedef struct
{
	u8  buff[UARTFIFO_CNT];
	u16 rd_p;
	u16 wr_p;
	u16 size;
	u8 full;
	u8 empty;
}UartFifo_Struct;

typedef struct
{
	u8  state;
	volatile u32 daemon_time;
	//bus_os_sem sem;
	UartFifo_Struct fifo;
	u32 errCode;
}UartReceive_Struct;

void UartSend_Queue_Enter(UartSend_Queue_Ele_t *ele);

void UartReceiveEvent(UartReceive_Struct *ths,u8 uartId,u8 data);
void UartTransfer_Init(void);

#endif
