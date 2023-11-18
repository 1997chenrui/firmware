#include "my_callback.h"
#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "flash.h"

#include "CommonHandle.h"
#include "IOHandle.h"
#include "MotorHandle.h"
#include "IOHandle.h"
#include "protoTest.h"
#include "sensor.h"
#include "io_ctrl_event.h"
#include "tct_pressure_singal.h"
#include "io.h"
#include "project_cfg.h"




extern UART_HandleTypeDef huart2;
extern u16 sensor_read(u8 idx);


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
    switch(func_code)
    {
		case CMD_Ping : 
        {
            CommonCmdExcute(func_code,packet);
            break;
        }
        case CMD_ProtoTest :
        {
            ProtoTestExcute(func_code,packet);
        }
        
        case CMD_FirmWareConfig:
        case CMD_FirmWareVersionGet:
        {
            FirmWare_Excute(func_code,packet);
            break;
        }	
		case  CMD_MotorMultiMoveTmc: 
		case  CMD_MotorMoveTmc:
		case  CMD_MotorStatetGetTmc :
		case  CMD_MotorRotateStartTmc:
		case  CMD_MotorRotateStopTmc:
		case  CMD_MotorRotateTmc:
		case  CMD_MotorResetTmc:
		case  CMD_MotorConfigSetTmc:	
		case  CMD_MotorMoveGPIOEventTmc:
		{
			MotorCmdExcute(func_code,packet);
			break;
		}
        case CMD_IoOperation:
        case CMD_IoSimple:
		{
			IOCmdExcute(func_code, packet);
			break;
		}
#if PRESSURE_SENSOR_ENABLE == 1
        case CMD_SensorReadSingle:
        {
            sensor_get_as_ref()->read_report(packet);
            break;
        }
#endif
#if TCT_PRESSURE_SINGAL_ENABLE == 1
        case CMD_PressureListen:
        {
            pressure_get_as_ref()->parse(packet);
            break;
        }
#endif
#if IO_CTRL_EVENT_ENABLE == 1
        case CMD_IOCtrlByEvent:
        {
            ioctrl_get_as_ref()->parse(packet);
            break;
        }
#endif
        case CMD_IOWrite:
        {
            io_get_as_ref()->write_report(packet);
            break;
        }
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



