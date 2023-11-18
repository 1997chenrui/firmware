#include "UartTransmitHandle.h"
#include "cmd.h"

#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"
#include "uart_transfer.h"



void UartTransmitCmdExcute(u32 func_code  ,  bus_packet * packet )
{
    switch(func_code)
    {
		case CMD_UartTransmit:
		{
			//printf("CMD_UartTransmit\r\n");
			UartTransmit_Send *ut = New_UartTransmit_Send();
			ut->GetFromBytes(ut,packet->data,4);
			
			UartSend_Queue_Ele_t ele;                    
            ele.adapterId = packet->adapterId;        
            ele.cmd = func_code;                    
            ele.srcId = packet->srcId;  
            ele.sessionId = packet->sessionId;              
            ele.send = ut;       
			
			UartSend_Queue_Enter(&ele);
			
			break;
		}
		default:break;
    }

}

