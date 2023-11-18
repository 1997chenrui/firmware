#include "my_callback.h"
#include "protocols.h"
#include "cmd.h"
#include "flash.h"
#include "motor_thread.h"

#include "CommonHandle.h"
#include "MotorHandle.h"
#include "procfg.h"


#if VOLTAGE_ENABLE == 1
#include "voltage.h"
#endif

extern UART_HandleTypeDef huart2;



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
#if MOTOR_CMD_ENABLE == 1
        case 0x00010000:
        case 0x00010010:
        case 0x00010020:
        case 0x00010030:
        case 0x00010040:
        case 0x00010050:
        case 0x00010060:
        case 0x00010070:
        case 0x00010080:
        {
            motor_cmd_run(func_code, packet);
            break;
        }
#endif
#if VOLTAGE_ENABLE == 1
        case 0x00070000:
        case 0x00070001:
		case 0x00070002:
		case 0x00070003:
		case 0x00070011:{
            voltage_cmd_run(func_code, packet);
            break;
        }
#endif
		default :
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



