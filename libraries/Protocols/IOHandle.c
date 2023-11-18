#include "IOHandle.h"
#include "cmd.h"

#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"
#include "io_operation.h"



void IOCmdExcute(u32 func_code  ,  bus_packet * packet )
{
    switch(func_code)
    {
		case CMD_IoOperation:
		{
			IoOperation_Send *io = New_IoOperation_Send();
			io->GetFromBytes(io,packet->data,4);
			
			Io_Queue_Ele_t ele;                    
            ele.adapterId = packet->adapterId;        
            ele.cmd = func_code;                    
            ele.srcId = packet->srcId;  
            ele.sessionId = packet->sessionId;              
            ele.send = io;       
			
			Io_Queue_Enter(&ele);
			
			break;
		}
		case CMD_IoSimple:
		{
			BytesBufferNode* rt_node = NULL;
			u32 rt_buff_len = 0;
			u8 *rt_buff = NULL;
			IoSimple_Send *ctrl  = New_IoSimple_Send();
			IoSimple_Return *rt = New_IoSimple_Return(); 
			ctrl->GetFromBytes(ctrl,packet->data,4);
			
			printf("ctrl->m1_port_pin_arg = %d\r\n",ctrl->m1_port_pin_arg);
			
			rt->m1_errCode_value = Io_Sample(ctrl->m1_port_pin_arg);
			
			rt_node = rt->ToBytes(rt);
			rt_buff_len = BytesBufferNode_Destroy(rt_node , &rt_buff);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,rt_buff_len,rt_buff);
			
			free(rt_buff);
			Delete_IoSimple_Send(ctrl);
			Delete_IoSimple_Return(rt);
			break;
		}
		default:break;
    }

}

