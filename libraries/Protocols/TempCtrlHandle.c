#include "TempCtrlHandle.h"
#include "cmd.h"
#include "tempctrl_thread.h"

#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"

//static float current_temp_old =0.0;
void TempCtrlCmdExcute(u32 func_code, bus_packet *packet)
{
	switch(func_code)
	{
		case CMD_TemperatureGet:
		{
//			printf("CMD_TemperatureGet\r\n");
			TemperatureGet_Send *temp = New_TemperatureGet_Send();
			TemperatureGet_Return *rt = New_TemperatureGet_Return();
			temp->GetFromBytes(temp, packet->data, 4);

//			rt->m1_state = temp_ctrl_mod[temp->m1_channelId].is_start;
//			rt->m2_current_temp = TempCtrl_GetTemp(temp->m1_channelId, true);
//			if(rt->m2_current_temp> 100.0f||rt->m2_current_temp < 0.0f)
//            {
//                rt->m2_current_temp = current_temp_old;
//            }
//            else
//            {
//                current_temp_old  = rt->m2_current_temp;
//            }
			printf("state: %d, channelId: %d, current_temp: %.2f\r\n", rt->m1_state, temp->m1_channelId, rt->m2_current_temp);
			
			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_TemperatureGet_Send(temp);
			Delete_TemperatureGet_Return(rt);

			break;
		}
		case CMD_TemperatureOpen:
		{
			printf("CMD_TemperatureOpen\r\n");
			TemperatureOpen_Send* open = New_TemperatureOpen_Send();
			TemperatureOpen_Return* rt = New_TemperatureOpen_Return();

			open->GetFromBytes(open,packet->data,4);

//			temp_ctrl_mod[open->m1_channelId].is_start = 1;
//			temp_ctrl_mod[open->m1_channelId].PID_Parm.Target = open->m2_targetTemp;
//			temp_ctrl_mod[open->m1_channelId].PID_Parm.Proportion = open->m3_p;
//			temp_ctrl_mod[open->m1_channelId].PID_Parm.Integral = open->m4_i;
//			temp_ctrl_mod[open->m1_channelId].PID_Parm.Derivative = open->m5_d;
//			temp_ctrl_mod[open->m1_channelId].PID_Parm.Offset = open->m6_offset;
//			printf("open: %d\r\n",open->m1_channelId);
			
 			TempCtrl_Open(open->m1_channelId);
			rt->m1_errCode = 0;

			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_TemperatureOpen_Send(open);
			Delete_TemperatureOpen_Return(rt);
			break;
		}
		case CMD_TemperatureClose:
		{
			printf("CMD_TemperatureClose, time=%dms\r\n", OSTimeGet());
			TemperatureClose_Send* close = New_TemperatureClose_Send();
			TemperatureClose_Return* rt = New_TemperatureClose_Return();
			close->GetFromBytes(close,packet->data,4);

//			temp_ctrl_mod[close->channelId].is_start = 0;
//			printf("close: %d\r\n",close->m1_channelId);
			TempCtrl_Close(close->m1_channelId);
			rt->m1_errCode = 0;

			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_TemperatureClose_Send(close);
			Delete_TemperatureClose_Return(rt);
			break;
		}
		default:printf("TempCtrlCmdExcute Parameter Error\r\n");break;
	}
}


