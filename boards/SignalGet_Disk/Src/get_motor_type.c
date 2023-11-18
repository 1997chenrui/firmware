#include "get_motor_type.h"

//extern unsigned char motor_type;

void Get_MotorType_Excute(int cmd,bus_packet *packet )
{
    switch(cmd)
	{
 		case CMD_GetEPOSType://获取电机类型
		{
			GetEPOSType_Send* cfg = New_GetEPOSType_Send();
			GetEPOSType_Return* rt = New_GetEPOSType_Return();
			cfg->GetFromBytes(cfg,packet->data,4);
					
			rt->m2_errCode=0;
			
			//rt->m1_motorType=motor_type;
			
			BytesBufferNode * node =  rt->ToBytes(rt);		
			u8 *data =NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			
			free(data);
			Delete_GetEPOSType_Send(cfg);
			Delete_GetEPOSType_Return(rt);
            break;
		}	
		case CMD_SetEPOSType://设置电机类型
		{
			SetEPOSType_Send* cfg = New_SetEPOSType_Send();
			SetEPOSType_Return* rt = New_SetEPOSType_Return();
			cfg->GetFromBytes(cfg,packet->data,4);
					
			//motor_type=cfg->m2_typeId;
			
			BytesBufferNode * node =  rt->ToBytes(rt);		
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			
			free(data);
			Delete_SetEPOSType_Send(cfg);
			Delete_SetEPOSType_Return(rt);
            break;
		}		
	}
}

