#include "RGBSensorHandle.h"
#include "cmd.h"

#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"



void RGBSensorCmdExcute(u32 func_code, bus_packet *packet)
{
    switch(func_code)
    {
		case CMD_RGBSensorGet:
		{
			BytesBufferNode* rt_node = NULL;
			u32 rt_buff_len = 0;
			u8 *rt_buff = NULL;
			RGBSensorGet_Send *rs = New_RGBSensorGet_Send();
			RGBSensorGet_Return *rt = New_RGBSensorGet_Return();
			rs->GetFromBytes(rs,packet->data,4);

			RGBDATA_Get(rs->m1_idx,(u16*)(&rt->m5_C),(u16*)(&rt->m2_R),(u16*)(&rt->m3_G),(u16*)(&rt->m4_B));
			rt_node = rt->ToBytes(rt);
			rt_buff_len = BytesBufferNode_Destroy(rt_node , &rt_buff);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,rt_buff_len,rt_buff);
			
			free(rt_buff);
			Delete_RGBSensorGet_Send(rs);
			Delete_RGBSensorGet_Return(rt);
			break;
		}
		default: break;
    }
}

