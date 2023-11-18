#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "autoCai.h"
#include "flash.h"
#include "CommonHandle.h"
#include "ADCDetectHandle.h"
#include "get_motor_type.h"

//extern light_sampler_struct light_sampler;
//extern light_gen_struct 	light_sigle_gen;


//#define DECLARE_SET_QUEUE_ELE(obj)            Motor_Queue_Ele_t ele;                    \
//                                                ele.adapterId = packet->adapterId;        \
//                                                ele.cmd = func_code;                    \
//                                                ele.srcId = packet->srcId;                \
//                                                ele.send = obj;                        \
//                                                ele.sessionId = packet->sessionId;



u32 bus_ita_func_callback(bus_packet * packet)//上位机命令接受 回调
{
    u32 func_code = 0;
    func_code = ReadU32(packet->data);//把命令代码从数据包里拿出来
    
    switch(func_code)
    {
    case CMD_Ping :
        {
            CommonCmdExcute(func_code,packet);//返回芯片信息和唯一ID
            break;
        }
    case CMD_SetEPOSType:
    case CMD_GetEPOSType: 
				{
					Get_MotorType_Excute(func_code,packet);//返回电机的数据
	  break;
				}		
				
    case CMD_FirmWareConfig:
    case CMD_FirmWareVersionGet:
		case CMD_McuReset:
        {
            FirmWare_Excute(func_code,packet);//固件升级命令处理，包含软件复位命令
    break;
        }
		case CMD_ADAutoCaliStart://
		case CMD_ADSampleRead:
		case CMD_ADDetectConfigSet:
		case CMD_ADDataRead:
		case 0x137:
				{
					ADCDetectCmdExcute(func_code  ,packet);//进行传感器采样工作处理
		break;
				}
		default:
        {
			//bus_ita_return_unkonw_cmd(packet);
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

//u8 send_data[200] = {
//    1,2,3,4,5,6,7,8,9,10,111,222,

//};


