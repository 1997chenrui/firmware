#include "ServoMotorHandle.h"

#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"

#include "servo_thread.h"

#define SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(obj)       ServoMotor_Queue_Ele_t ele;                    \
                                               					 ele.adapterId = packet->adapterId;        \
                                               					 ele.cmd = func_code;                    \
                                               					 ele.srcId = packet->srcId;                \
                                               					 ele.send = obj;                        \
                                                				 ele.sessionId = packet->sessionId;

void ServoMotorCmdExcute(u32 func_code,  bus_packet * packet )
{
	printf("ServoMotorCmdExcute: fun=0x%x\r\n", func_code);
	switch(func_code)
	{
		case CMD_ServoMotorMultiRotate:
		{
			ServoMotorMultiRotate_Send *servo = New_ServoMotorMultiRotate_Send();
            servo->GetFromBytes(servo,packet->data,4);
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(servo);
			
            ServoMotorThread_EnQueue(0,&ele);
			break;
		}
		case CMD_ServoMotorRotateShakeRotateDetect:
		{
			ServoMotorRotateShakeRotateDetect_Send *servo = New_ServoMotorRotateShakeRotateDetect_Send();
            servo->GetFromBytes(servo, packet->data, 4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(servo);
			
            ServoMotorThread_EnQueue(servo->m1_idx, &ele);
			break;
		}
		case CMD_ServoMotorRotateShakeDetect:
		{
			ServoMotorRotateShakeDetect_Send *servo = New_ServoMotorRotateShakeDetect_Send();
            servo->GetFromBytes(servo,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(servo);
			
            ServoMotorThread_EnQueue(servo->m1_idx,&ele);
			break;
		}
		case CMD_ServoMotorRotateTwoDirOnTime:
		{
			ServoMotorRotateTwoDirOnTime_Send *rotateTwoDir = New_ServoMotorRotateTwoDirOnTime_Send();
            rotateTwoDir->GetFromBytes(rotateTwoDir,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(rotateTwoDir);
			
            ServoMotorThread_EnQueue(rotateTwoDir->m1_idx,&ele);
			break;
		}
		case CMD_ServoStateGet:
	   {
			ServoMotorStatetGet_Send *state = New_ServoMotorStatetGet_Send();
            state->GetFromBytes(state,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(state);
			
            ServoMotorThread_EnQueue(state->m1_idx,&ele);
			break;
	   }
		case CMD_ServoRotateStart:
		{
			ServoMotorRotateStart_Send *rotate_start = New_ServoMotorRotateStart_Send();
            rotate_start->GetFromBytes(rotate_start,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(rotate_start);
			
            ServoMotorThread_EnQueue(rotate_start->m1_idx,&ele);
			break;
		}
		case CMD_ServoRotateStop:
		{
			ServoMotorRotateStop_Send *rotate_stop = New_ServoMotorRotateStop_Send();
            rotate_stop->GetFromBytes(rotate_stop,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(rotate_stop);
			
            ServoMotorThread_EnQueue(rotate_stop->m1_idx,&ele);
			break;
		}
		case CMD_ServoReset:
		{
			printf("reset\r\n");
			ServoMotorReset_Send *reset = New_ServoMotorReset_Send();
            reset->GetFromBytes(reset,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(reset);
			
            ServoMotorThread_EnQueue(reset->m1_idx,&ele);
			break;
		}
		case CMD_ServoAutoPID:
		{
			break;
		}
		case CMD_ServoShake:
		{
			ServoMotorShake_Send *shake = New_ServoMotorShake_Send();
			shake->GetFromBytes(shake,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(shake);
			
            ServoMotorThread_EnQueue(shake->m1_idx,&ele);
			break;
		}
		case CMD_ServoBaseCtrl:
		{
			ServoMotorBaseCtrl_Send *base_ctrl = New_ServoMotorBaseCtrl_Send();
			base_ctrl->GetFromBytes(base_ctrl,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(base_ctrl);
			
            ServoMotorThread_EnQueue(base_ctrl->m1_idx,&ele);
			break;
		}
		case CMD_ServoMove:
		{
			ServoMotorMove_Send* move = New_ServoMotorMove_Send();
			move->GetFromBytes(move,packet->data,4);
			
			SERVO_MOTOR_DECLARE_SET_QUEUE_ELE(move);
			
			ServoMotorThread_EnQueue(move->m1_idx,&ele);
			break;
		}
		default:break;
	}
}

