#ifndef IO_OPERATION_H_
#define IO_OPERATION_H_
#include "bus.h"
#include "ErrCode.h"

typedef struct
{
	u32            cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void       *send;            //send指针
}Io_Queue_Ele_t;


typedef void (*OnSimpleIoCtrlCallBack)(u8 port,u8 pin,u8 fun,u8 arg);
void setSimpleIoCtrlEvent(OnSimpleIoCtrlCallBack callback);

void Io_OperationInit(void);

void Io_Queue_Enter(Io_Queue_Ele_t *ele);
void IO_cfg(u8 port , u16 pin , bool isOut);
u32 Io_Sample(u16 port_pin_arg);
#endif
