#include "SensorValueOperationHandle.h"

#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"
#include "motor_thread.h"
#include "simpleBus.h"

extern SimpleBus_t SmpBus;

void SensorValueOperationCmdExcute(u32 cmd  ,  bus_packet * packet )
{
    switch(cmd)
    {
        case CMD_SensorValueOperation :
        {
			BytesBufferNode* rt_node = NULL;
            SensorValueOperation_Send *sensor = New_SensorValueOperation_Send();
            SensorValueOperation_Return *rt = New_SensorValueOperation_Return(); 
            u32 rt_buff_len = 0;
            u8 *rt_buff = NULL;

            sensor->GetFromBytes(sensor, packet->data,4);
			
            printf("sensor->m1_reg  = %d \r\n",sensor->m1_reg);
            printf("sensor->m2_op = %d \r\n",sensor->m2_op);
            printf("sensor->m3_value = %d \r\n",sensor->m3_value);

            if(sensor->m2_op == 1)
            {
                motor_event_start(&SmpBus,sensor->m1_reg,sensor->m3_value);
            }

            rt->m1_errCode = 0;
            
            rt_node = rt->ToBytes(rt);
            rt_buff_len = BytesBufferNode_Destroy(rt_node , &rt_buff);
    
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,rt_buff_len,rt_buff);
            free(rt_buff);
            
            Delete_SensorValueOperation_Send(sensor);
            Delete_SensorValueOperation_Return(rt);
        }
    }
   
}
