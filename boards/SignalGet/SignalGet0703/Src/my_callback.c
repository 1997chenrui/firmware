#include "protocols.h"
#include "cmd.h"
#include "flash.h"
#include "CommonHandle.h"
#include "sp_light.h"

//#include "AD7699BySG0703.h"

#include "AD7699.h"
#include "stm32f4xx_hal.h"
#include "sp_light.h"
#include "tempctrl_thread.h"
#include "project_cfg.h"
#include "sensor_single_ctl.h"
#include "lampCtl.h"

u32 tim1;

extern Wave wave[];

extern void sensor_single_cmd_run(u32 itf_code, bus_packet *packet);
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
		case 0x00040040:
		case 0x00040050:
		{
			sensor_single_cmd_run(func_code,packet);
			break;
		}
		case 0x01000c01:
		{
			for(int i=0;i<8;i++)
			{
				wave[i].need_updata = 1;
			}
			u8 error_code = 0;
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,1,&error_code);
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
