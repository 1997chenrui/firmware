#include "TurnTablesMotorHandle.h"
#include "cmd.h"
#include "Protocols.h"
#include "cmdExcute.h"

#define DECLARE_SET_QUEUE_ELE(obj)            Motor_Queue_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;

void TurnTablesCmdExcute(u32 func_code  ,  bus_packet * packet )
{
    switch(func_code)
    {
        case CMD_TurnTableMove:
        {
            
            TurnTable_Move_Send *mv = New_TurnTable_Move_Send();
            mv->GetFromBytes(mv, packet->data,4);
//             printf("mv->m1_cali_location  = %d \r\n",mv->m1_cali_location);
// 			printf("mv->m2_idx = %d \r\n",			 mv->m2_idx);
//             printf("mv->m3_steps = %d \r\n",		 mv->m3_steps);
// 			printf("mv->m4_speed = %d \r\n",		 mv->m4_speed);
// 			printf("mv->m5_acce = %d \r\n",			 mv->m5_acce);
            DECLARE_SET_QUEUE_ELE(mv);
            MotorThread_EnQueue(mv->m2_idx ,&ele);
            break;
        }        
    }
}
