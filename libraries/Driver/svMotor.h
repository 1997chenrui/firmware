#ifndef  SV_MOTOR_H_
#define   SV_MOTOR_H_

#include "bus.h"




void SvMotor_Init(void (*send)(u8 *data,int len));
void SvMotor_NotifyByte(u8 data);
u32  SvMotor_DataExchange(u8 opCode,u16 *dataIn,int len,u16* dataOut,int *rxLen);




















#endif




