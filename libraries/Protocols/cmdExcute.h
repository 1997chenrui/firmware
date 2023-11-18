#ifndef CMD_EXCUTE_H_
#define CMD_EXCUTE_H_


#include "bus.h"
#include "cmd.h"
#include "bytesBuffer.h"
#include "Protocols.h"
#include "data_struct.h"



#define                         MOTOR_THEARD_CNT                5
extern SeqQueue_Struct          queue[MOTOR_THEARD_CNT];


typedef struct
{
    u32            cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void       *send;            //send指针

}Motor_Queue_Ele_t;


void MotorThread_EnQueue(int idx,Motor_Queue_Ele_t *data);














#endif
































