#include "protocols.h"
#include "cmd.h"
#include "flash.h"
#include "CommonHandle.h"
#include "AD7699.h"
#include "stm32f4xx_hal.h"
#include "sp_light.h"
#include "tempctrl_thread.h"
#include "project_cfg.h"

__weak u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
	printf("未实现\r\n");
    return 1;
}

#define DECLARE_SET_QUEUE_ELE(obj)            Motor_Queue_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;
												



u32 bus_ita_func_callback(bus_packet * packet)
{
    u32 func_code = 0;
    func_code = ReadU32(packet->data);
    printf("func_code = %x\r\n",func_code);
    switch(func_code)
    {
		case CMD_Ping : 
        {
            CommonCmdExcute(func_code,packet);
            break;
        }   
        case CMD_FirmWareConfig:
        case CMD_FirmWareVersionGet:
        {
            FirmWare_Excute(func_code,packet);
            break;
        }
        case CMD_DCAdRead :
        {
		//	printf("read time1 = %dms\r\n", OSTimeGet());
			const unsigned int countconst = 6; // 每次上传数据的组数

            DCAdRead_Send *send = New_DCAdRead_Send();
            DCAdRead_Return *rt = New_DCAdRead_Return();
            rt->m2_adValue = malloc(sizeof(Light_Ad_t) * countconst);
			rt->m2_adValue_Length = 0;
//			rt->m2_adValue_Length = sizeof(Light_Ad_t) / sizeof(u16) * countconst;
            send->GetFromBytes(send,packet->data, 4);
            switch(send->m1_cmd )
            {
				case 0:		//清除所有数据
				{
					Light_ClearData();
					break;
				}
				case 1:		//读取数据
				{
					Light_Ad_t dt[countconst] = {0};
					//printf("m2_channel = %d\r\n", send->m2_channel);
					unsigned char readcount = Light_GetData(dt, countconst, send->m2_channel);
					if(readcount == 0)
                    printf("readcount = %d\r\n", readcount);
					rt->m1_remainCnt = Ligrh_RemianDataCountGet();
	
					for(unsigned char i = 0; i < readcount; i++)
					{
						rt->m2_adValue[rt->m2_adValue_Length++] = dt[i].channelIdx;
						rt->m2_adValue[rt->m2_adValue_Length++] = dt[i].mix2FirstAdTime;
					//	printf("channelIdx = %d, mix2FirstAdTime = %d\r\n", dt[i].channelIdx, dt[i].mix2FirstAdTime);
						for(unsigned char k = 0; k < LIGHT_CHANNEL_SIZE; k++)
						{
							rt->m2_adValue[rt->m2_adValue_Length++] = dt[i].data[k];
						}
					}
					
					break;
				}
            }
			
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
			//printf("read time1 = %dms\r\n", OSTimeGet());
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			
            free(data);
            free(rt->m2_adValue);
            Delete_DCAdRead_Return(rt);
            Delete_DCAdRead_Send(send);
//			printf("read time2 = %dms\r\n", OSTimeGet());
            break;
        }
		case 0x00060000:
		case 0x00060010:
		case 0x00060020:
		{
			temperature_cmd_run(func_code,packet);
			break;
		}
        default :
        {
            //若此符号未被覆盖 说明指令未找到
            //若此符号被覆盖，说明存在指令扩展，若指令扩展返回1，说明扩展指令中仍未找到该指令
            if(bus_ita_func_callback_extern(func_code, packet)){  
                bus_ita_return_unkonw_cmd(packet);
            }
            break;
        }
		
    }

    return 0;
}

u32 bus_ita_func_broadcast(bus_packet * packet)
{
    printf("bus_ita_func_broadcast\r\n");
    return 0;
}
