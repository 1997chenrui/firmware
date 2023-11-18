#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "flash.h"
#include "voltage.h"
#include "CommonHandle.h"
#include "power.h"

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
    printf("func_code = 0x%x\r\n", func_code);
    
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
        case 0x00000040:
        {
            power_cmd(packet);
            break;
        }
		case 0x00070000:{
			voltage_cmd_run(func_code,packet);
			break;
		}      
        default:
        {
            bus_ita_return_unkonw_cmd(packet);
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
