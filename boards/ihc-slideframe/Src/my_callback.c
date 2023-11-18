#include "my_callback.h"
#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "flash.h"

#include "CommonHandle.h"
#include "IOHandle.h"
//#include "TempCtrlHandle.h"
#include "IOHandle.h"
#include "protoTest.h"
#include "mTMC5130MotorHandle.h"
#include "sensors.h"
#include "tempctrl_thread.h"


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
			break;
        }

        case CMD_FirmWareConfig:
        case CMD_FirmWareVersionGet:
		case CMD_FirmWareTransmitFile:
        {
            FirmWare_Excute(func_code,packet);
            break;
        }
		case CMD_IoSimple:
        case CMD_IoOperation:
		{
			IOCmdExcute(func_code, packet);
			break;
		}
		case CMD_MotorReset:
        case CMD_MotorMultiMove:      //MotorMultiMove
        case CMD_MotorMove:
        case CMD_MotorRotate :
        case CMD_MotorStatetGet :
        case CMD_MotorRotateStart :
        case CMD_MotorRotateStop :
        case CMD_MotorAbort :
        case CMD_MotorMoveSeq :
        case CMD_MotorConfigSet: 
        case CMD_MotorMoveEven: 
        case CMD_MotorMoveGPIOEvent:
        case CMD_SeqMotorMultiMove :
		case CMD_MotorConfigSetNew:
			
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
		case CMD_TemperatureGet:
            temperaturesensor_get_as_ref()->buffer_report(packet);
            break;
		case CMD_TemperatureOpen:
            tempctrl_get_as_ref()->open_report(packet);
            break;
		case CMD_TemperatureClose:
		{
			tempctrl_get_as_ref()->close_report(packet);
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



