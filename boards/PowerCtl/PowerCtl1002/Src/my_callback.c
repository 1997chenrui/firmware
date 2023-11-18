#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "flash.h"
#include "voltage.h"

#include "CommonHandle.h"
#include "io_thread.h"
#include "uart_thread.h"
#include "rv_thread.h"
#include "project_cfg.h"
#include "power.h"
#include "color.h"
#include "pc_reboot.h"

#if AD_SIGNAL_ENABLE == 1
#include "ad_signal.h"
#endif

//extern UART_HandleTypeDef huart2;

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
        case CMD_McuReset:
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
		case 0x01000c60:
		{
			reboot_pc_cmd(func_code,packet);
			break;
		}
#if IO_CMD_ENABLE == 1	
        case 0x00020000:
        case 0x00020010:
        case 0x00030000:
        case 0x00030010:
			
		case 0x00020001:
		case 0x00030011:
 		{
 			io_cmd_run(func_code, packet);
 			break;
 		}
#endif		
#if AD_SIGNAL_ENABLE == 1
		case 0x00040000:
		case 0x00040001:	
		case 0x00040010:
		case 0x00040020:
		case 0x00040030:{
			signal_cmd_run(func_code, packet);
			break;
		}
#endif
		
#if UART_TRANSPOND_ENABLE == 1
		case 0x00050000:
		{
			uart_transpond_cmd_run(packet);
			break;
		}
#endif
#if RUNZE_ENABLE == 1        
        case 0x01000007:
        case 0x01000008:
        case 0x01000009:{
            rv_cmd_run(func_code, packet);
            break;
        }
#endif
#if WS_LED_ENABLE == 1
		case 0x00080010:
		case 0x00080011:
		case 0x00080012:
		ws_led_cmd_run(func_code, packet);
		break;
#endif        
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
