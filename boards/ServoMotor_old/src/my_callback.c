#include "my_callback.h"
#include "protocols.h"
#include "cmd.h"
#include "flash.h"
#include "CommonHandle.h"
#include "servoMotor.h"

#include "mTMC5130MotorHandle.h"
#include "motor_thread.h"
#include "io_thread.h"
#include "uart_thread.h"
#include "project_cfg.h"

#if TEMP_CTRL_ENABLE == 1
#include "tempctrl_thread.h"
#endif


#if VOLTAGE_ENABLE == 1
#include "voltage.h"
#include "voltage_cfg.h"
#endif

#define DECLARE_SET_QUEUE_ELE(obj)            Motor_Queue_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;
												

#define SEND_SYNC_SIGNAL(val)     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (GPIO_PinState)val);
extern TIM_HandleTypeDef htim5;

__weak u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
	printf("未实现\r\n");
    return 1;
}

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
		case CMD_FirmWareTransmitFile:
        {
            FirmWare_Excute(func_code,packet);
            break;
        }
#if MOTOR_CMD_ENABLE == 1
        case 0x00010000:
        case 0x00010010:
        case 0x00010020:
        case 0x00010030:
        case 0x00010031:
        case 0x00010040:
        case 0x00010050:
        case 0x00010060:
		case 0x00010062:
        case 0x00010070:
        case 0x00010071:
        case 0x00010080:
        case 0x00010082:
        case 0x00010083:
        {
            motor_cmd_run(func_code, packet);
            break;
        }
#if RV_ENABLE == 1
		case 0x000100a0:
		case 0x000100a1:
		case 0x000100a2:
			rv_cmd_run(func_code, packet);
			break;
#endif
#endif                               
#if IO_CMD_ENABLE == 1
        case 0x00020000:
        case 0x00020010:
        case 0x00030000:
        case 0x00030010:
			
		case 0x00020001:
        case 0x00030001:
        {
            io_cmd_run(func_code, packet);
            break;
        }
#endif
#if AD_SIGNAL_ENABLE == 1
		case 0x00040000:
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
		
#if VOLTAGE_ENABLE == 1
        case 0x00070000:
        case 0x00070001:
		case 0x00070002:
		case 0x00070010:	
		case 0x00070011:{
            voltage_cmd_run(func_code, packet);
            break;
        }
#endif
#if LED_ENABLE == 1
        case 0x00080000:
            led_cmd_run(packet);
        break; 
#endif
		
#if WS_LED_ENABLE == 1
		case 0x00080010:
		case 0x00080011:
		case 0x00080012:
			ws_led_cmd_run(func_code, packet);
			break;
#endif       
#if TEMP_CTRL_ENABLE == 1
        case 0x00060000:
        case 0x00060010:
        case 0x00060020:
            temperature_cmd_run(func_code, packet);
            break;
#endif        
#if EPOS_ENABLE == 1
        case CMD_ServoStateGet:
		case CMD_ServoRotateStart:
		case CMD_ServoRotateStop:
		case CMD_ServoReset:
		case CMD_ServoAutoPID:
		case CMD_ServoShake:
		case CMD_ServoBaseCtrl:
		case CMD_ServoMove:
		case CMD_ServoMotorRotateTwoDirOnTime:
		case CMD_ServoMotorRotateShakeDetect:
		case CMD_ServoMotorRotateShakeRotateDetect:
		case CMD_ServoMotorMultiRotate:
		{
			ServoMotorCmdExcute(func_code  , packet );
			break;
		}
#endif

#ifdef SERVOMOTOR_ENABLE

		case 0x00012005:
		case 0x00012006:
		case 0x00012010:
		case 0X00012011:
		case 0X00012100:
		case 0X00012101:
		case 0x00012120:
		case 0x00012122:
		case 0X00012400:
		
		case 0X00012150:
		case 0X00012153:
		case 0X00012160:
		case 0X00012161:
	    case 0X00012200:
		case 0x00012210:
	    case 0X00012201:
		case 0x00012211:
		{
			ServoMotorParse(func_code,packet);
			break;
		}
#endif	
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



