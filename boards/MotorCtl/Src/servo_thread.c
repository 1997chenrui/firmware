#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servo_thread.h"
#include "data_struct.h"
#include "Protocols.h"
#include "stm32f4xx_hal.h"
#include "EPOS4Device.h"
#include "errCode.h"
#include "cmd.h"
#include "delayus.h"

#define SERVO_MOTOR_THREAD_STACK_SIZE         512
#define Servo_Motor_Thread_Prio               16
#define SERVO_MOTOR_THEARD_CNT				  1
#define SERVOMOTOR_CNT						  1

extern UART_HandleTypeDef huart3;

#define SERVO_MAX_STEP			32768

servo_driver_struct 		servo_motors[SERVOMOTOR_CNT];
SeqQueue_Struct             servomotor_queue[SERVO_MOTOR_THEARD_CNT];        //队列成员应该是命令码和send对象

static void ServoMotor_Thread(void *arg);
void Syn_Signal_Hight(void);
void Syn_Signal_Low(void);
//这里要用到队列 队列在data_struct.c 里面有封装好的 
static struct
{
    bus_os_thread               tcb;
    bus_os_sem                  sem;
    u32                         stack[SERVO_MOTOR_THREAD_STACK_SIZE];
    
}ServoMotorThread_Cfg[SERVO_MOTOR_THEARD_CNT];

#define CREATE_Q_BLOCK(idx)                                            \
static bool queue##idx##_pend()            \
{                                        \
	bus_os_pend_sem(&ServoMotorThread_Cfg[idx].sem,0);    \
	return true;                        \
}                                        \
static bool queue##idx##_post()         \
{                                        \
	bus_os_post_sem(&ServoMotorThread_Cfg[idx].sem);    \
	return true;                        \
}


#define  SERVOMOTOR_THRAD_CREATE(idx)        \
bus_create_thread(&ServoMotorThread_Cfg[idx].tcb,ServoMotor_Thread,         \
		ServoMotorThread_Cfg[idx].stack, SERVO_MOTOR_THREAD_STACK_SIZE, (Servo_Motor_Thread_Prio+idx),NULL);\
bus_os_create_sem(&ServoMotorThread_Cfg[idx].sem,0);            \
SeqQueue_Init(&servomotor_queue[idx],sizeof(ServoMotor_Queue_Ele_t),&queue##idx##_pend,&queue##idx##_post)

                    
/********************************* 阻塞队列定义        ****************************************************/                    
CREATE_Q_BLOCK(0);
/******************************** 电机线程如队列    ****************************************************/

void ServoMotorThread_EnQueue(int idx,ServoMotor_Queue_Ele_t *data)
{
    SeqQueue_Enter(&servomotor_queue[idx],data);
}
/*********************************** 所有命令 ******************************************/

void ServoMotor_Mod_Init(void)
{
	printf("ServoMotor_Mod_Init \r\n");
	Syn_Signal_Low();
	
	EPOS4Driver_Init(&servo_motors[0]);
//	Servo_motor_reset(&servo_motors[0]);
//	Servo_motor_rotate_start(&servo_motors[0], 1, 400, 400);
	
	
//	delayMs(500);
//	Servo_motor_move(&servo_motors[0],8888,444,444,18100,1,1);
//	// profileVelocity:速度, profileAcceleration:加速度, targetPosition:目标位置, absolute:是否是绝对运动, immediately:是否立即运动
	SERVOMOTOR_THRAD_CREATE(0);
}

static int thread_idx = 0;
static void ServoMotor_Thread(void *arg)
{
    BytesBufferNode* temp_node = NULL;
    int td_idx = thread_idx++;
	printf("servo motor thread %d start \r\n",td_idx);
	
    while(1)
    {
        SeqQueue_Waite(&servomotor_queue[td_idx]);
        ServoMotor_Queue_Ele_t ele;
        SeqQueue_Delete(&servomotor_queue[td_idx],&ele);
		
		printf("Servo Motor Thread: fun=0x%x\r\n", ele.cmd);
        switch(ele.cmd)
        {
			case CMD_ServoMotorMultiRotate:
			{
				ServoMotorMultiRotate_Send* servo = (ServoMotorMultiRotate_Send *)ele.send;
				ServoMotorMultiRotate_Return* rt = New_ServoMotorMultiRotate_Return();
				
				servo_multi_rotate_args_t * margs = malloc(sizeof(servo_multi_rotate_args_t) * servo->m1_MoveArgs_Length);
				for(int i = 0;i<servo->m1_MoveArgs_Length;i++)
				{
					margs[i].motor = &servo_motors[0];
					margs[i].dir = (servo->m1_MoveArgs[i].m2_dir == 0)?false:true;
					margs[i].acce = servo->m1_MoveArgs[i].m4_acce;
					margs[i].speed = servo->m1_MoveArgs[i].m3_speed;
					margs[i].time = servo->m1_MoveArgs[i].m5_delayms;
					margs[i].isSendSyncSignal = servo->m1_MoveArgs[i].m6_isSendSyncSigle;
				}
				rt->m1_errCode = Servo_motor_multi_rotate(servo->m1_MoveArgs_Length,margs);
				
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}				
				free(margs);				
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_ServoMotorMultiRotate_Send(servo);
				Delete_ServoMotorMultiRotate_Return(rt);				
				break;
			}
			case CMD_ServoMotorRotateShakeDetect:
			{
				printf("CMD_ServoMotorRotateShakeDetect_thread\r\n");
				ServoMotorRotateShakeDetect_Send* servo = (ServoMotorRotateShakeDetect_Send *)ele.send;
				ServoMotorRotateShakeDetect_Return* rt = New_ServoMotorRotateShakeDetect_Return();

				Servo_motor_rotate_shake_rotate(&servo_motors[servo->m1_idx], 
					servo->m5_isForward,servo->m3_speed,servo->m2_acce,servo->m4_time,
					SERVO_MAX_STEP,servo->m8_amplitude,servo->m7_frequency,servo->m6_time,
					servo->m11_isForward,servo->m10_speed,servo->m9_acce);
				
				rt->m1_errCode = servo_motors[servo->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorRotateShakeDetect_Send(servo);
				Delete_ServoMotorRotateShakeDetect_Return(rt);

				break;
			}
			case CMD_ServoMotorRotateShakeRotateDetect: //旋转  振动 旋转 采取  四合一指令
			{
				uint32_t time = OSTimeGet(); // 3757ms
				printf("ServoMotorRotateShakeRotateDetect\r\n");
				ServoMotorRotateShakeRotateDetect_Send* servo = (ServoMotorRotateShakeRotateDetect_Send *)ele.send;
				ServoMotorRotateShakeRotateDetect_Return* rt = New_ServoMotorRotateShakeRotateDetect_Return();
				
				servo_motors[servo->m1_idx].errCode=0;
				servo_motor_rotate_shake_rotate_detect(&servo_motors[servo->m1_idx],
														SERVO_MAX_STEP,
														servo->m2_acce,
														servo->m3_speed,
														servo->m4_time,
														servo->m5_isForward,
														servo->m6_time,
														servo->m7_frequency,
														servo->m8_amplitude,
														servo->m9_acce,
														servo->m10_speed,
														servo->m11_isForward,
														servo->m12_time,
														servo->m13_acce,
														servo->m14_speed,
														servo->m15_isForward );	// 3682		
				rt->m1_errCode = servo_motors[servo->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorRotateShakeRotateDetect_Send(servo);
				Delete_ServoMotorRotateShakeRotateDetect_Return(rt);
				printf("time=%d\r\n", OSTimeGet()-time);
				break;					
			}						
			case CMD_ServoMotorRotateTwoDirOnTime:
			{
				printf("ServoMotorRotateTwoDirOnTime_Send\r\n");
				ServoMotorRotateTwoDirOnTime_Send* rotate_start = (ServoMotorRotateTwoDirOnTime_Send *)ele.send;
				ServoMotorRotateTwoDirOnTime_Return* rt = New_ServoMotorRotateTwoDirOnTime_Return();
				
				servo_motors[rotate_start->m1_idx].errCode=0;
				
				Servo_motor_rotate_two_dir_on_time(&servo_motors[rotate_start->m1_idx], rotate_start->m4_isForward,  rotate_start->m3_speed,  rotate_start->m2_acce,rotate_start->m5_time);
				
				rt->m1_errCode = servo_motors[rotate_start->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
								int len = BytesBufferNode_Destroy(temp_node,&data);
								bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorRotateTwoDirOnTime_Send(rotate_start);
				Delete_ServoMotorRotateTwoDirOnTime_Return(rt);

				break;
			}
			case CMD_ServoStateGet:
			{			
				int step = 0;
				uint8_t err=0;
				
				ServoMotorStatetGet_Send* state = (ServoMotorStatetGet_Send *)ele.send;
				ServoMotorStatetGet_Return* rt = New_ServoMotorStatetGet_Return();
				
				servo_motors[state->m1_idx].errCode=0;
				
				step = Servo_base_ctrl(&servo_motors[state->m1_idx],SERVO_CMD_GET_POSTION,&err);
				
				rt->m1_errCode = err;
				
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				rt->m3_currentCordinate = step<0?SERVO_MAX_STEP+(step%SERVO_MAX_STEP):step%SERVO_MAX_STEP;
				
				rt->m4_currentSpeed = Servo_base_ctrl(&servo_motors[state->m1_idx],SERVO_CMD_GET_VELOCITY,&err);
			
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_ServoMotorStatetGet_Send(state);
				Delete_ServoMotorStatetGet_Return(rt);				
				break;
			}
			case CMD_ServoRotateStart://添加错误码
			{
				printf("ServoMotorRotateStart_Send\r\n");
				ServoMotorRotateStart_Send* rotate_start = (ServoMotorRotateStart_Send *)ele.send;
				ServoMotorRotateStart_Return* rt = New_ServoMotorRotateStart_Return();
				printf("m1_idx=%d, m4_isForward=%d, m3_speed=%d, m2_acce=%d\r\n",rotate_start->m1_idx,rotate_start->m4_isForward,rotate_start->m3_speed,rotate_start->m2_acce);
				if(rotate_start->m5_isSendSyncSigle)
				{
					Syn_Signal_Hight();
				}
				
//				Syn_Signal_Hight();
//				bus_os_sleep(10);
//				Syn_Signal_Low();
				
				Servo_motor_rotate_start(&servo_motors[rotate_start->m1_idx], rotate_start->m4_isForward,  rotate_start->m3_speed,  rotate_start->m2_acce);
				
				rt->m1_errCode = servo_motors[rotate_start->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorRotateStart_Send(rotate_start);
				Delete_ServoMotorRotateStart_Return(rt);
				Syn_Signal_Low();

				break;
			}
			case CMD_ServoRotateStop:
			{
				uint8_t err;
				printf("ServoMotorRotateStop_Send\r\n");
				ServoMotorRotateStop_Send* rotate_stop = (ServoMotorRotateStop_Send *)ele.send;
				ServoMotorRotateStop_Return* rt = New_ServoMotorRotateStop_Return();
		
//				Syn_Signal_Hight();
//				bus_os_sleep(10);
//				Syn_Signal_Low();
				
				Servo_base_ctrl(&servo_motors[rotate_stop->m1_idx],SERVO_CMD_QUICK_STOP,&err);
				
				rt->m1_errCode = servo_motors[rotate_stop->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
								int len = BytesBufferNode_Destroy(temp_node,&data);
								bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorRotateStop_Send(rotate_stop);
				Delete_ServoMotorRotateStop_Return(rt);

				break;
			}
			case CMD_ServoReset:
			{
				printf("ServoMotorReset_Send\r\n");
				ServoMotorReset_Send* reset = (ServoMotorReset_Send *)ele.send;
				ServoMotorReset_Return* rt = New_ServoMotorReset_Return();

				Servo_motor_reset(&servo_motors[reset->m1_idx]);
				
				rt->m1_errCode = servo_motors[reset->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				else
				{
					printf("ServoMotorReset succ\r\n");
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);

				Delete_ServoMotorReset_Send(reset);
				Delete_ServoMotorReset_Return(rt);
				break;
			}
			case CMD_ServoShake:
			{
				printf("ServoMotorShake_Send\r\n");
				ServoMotorShake_Send* shake = (ServoMotorShake_Send *)ele.send;
				ServoMotorShake_Return* rt = New_ServoMotorShake_Return();
				
				Servo_motor_shake(&servo_motors[shake->m1_idx], SERVO_MAX_STEP,shake->m4_amplitude,shake->m3_frequency,shake->m2_time,&rt->m1_maxCordinate);//振幅 频率 时间

				rt->m2_errCode = servo_motors[shake->m1_idx].errCode;
				if(rt->m2_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m2_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_ServoMotorShake_Send(shake);
				Delete_ServoMotorShake_Return(rt);
				break;
			}
			case CMD_ServoBaseCtrl:
			{
				uint8_t err=0;
				ServoMotorBaseCtrl_Send *base = (ServoMotorBaseCtrl_Send *)ele.send;
				ServoMotorBaseCtrl_Return * rt = New_ServoMotorBaseCtrl_Return();
				
				Servo_base_ctrl(&servo_motors[base->m1_idx],(SERVO_CMD)base->m2_option,&err);

				rt->m1_errCode = servo_motors[base->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);

				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_ServoMotorBaseCtrl_Send(base);
				Delete_ServoMotorBaseCtrl_Return(rt);
				break;
			}
			case CMD_ServoMove:
			{				
				ServoMotorMove_Send* move = (ServoMotorMove_Send *)ele.send;
				ServoMotorMove_Return* rt = New_ServoMotorMove_Return();
										
				Servo_motor_move(&servo_motors[move->m1_idx],move->m3_speed,move->m4_acce,move->m4_acce,move->m2_steps,move->m5_isAbsolute,1);
				rt->m1_errCode = servo_motors[move->m1_idx].errCode;
				if(rt->m1_errCode != 0)
				{
					printf("thread errCode=%d\r\n",rt->m1_errCode);
				}
				temp_node =  rt->ToBytes(rt);
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_ServoMotorMove_Send(move);
				Delete_ServoMotorMove_Return(rt);
				break;
			}
			default:printf("servo motor cmd error\r\n");
        }
    }
}



//同步信号 高
void Syn_Signal_Hight(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
}


void Syn_Signal_Low(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
}

