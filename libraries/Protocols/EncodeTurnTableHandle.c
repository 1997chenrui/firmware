#include "EncodeTurnTableHandle.h"
#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"
#include "motor.h"
#include "cmdExcute.h"


#define DECLARE_SET_QUEUE_ELE(obj)            Motor_Queue_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;


static void mc_assert(bool exp,int line)
{
    if(exp)
    {
        while(1)
        {
            printf("cbk err %d \r\n",line);
        }
    }
}

#define MC_ASSERT( exp ) mc_assert((exp),__LINE__)



void TurnTableCmdExcute(u32 func_code  ,  bus_packet * packet )
{
    switch(func_code)
    {
		case CMD_EncodeTurnTableFindZero:
		{
			EncodeTurnTableFindZero_Send *send = New_EncodeTurnTableFindZero_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
			break;
		}
		case CMD_EncodeTurnTableRotateStart:
		{
			EncodeTurnTableRotateStart_Send *send = New_EncodeTurnTableRotateStart_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
			break;
		}
		case CMD_EncodeTurnTableSignalSend:
		{
			EncodeTurnTableSignalSend_Send *send = New_EncodeTurnTableSignalSend_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
			break;
		}
		case CMD_EncodeTurnTableStateGet:
		{
			EncodeTurnTableSignalSend_Send *send = New_EncodeTurnTableSignalSend_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
			break;
		}
        default:printf("TurnTableCmdExcute Error");break;
        
    }
   
}
