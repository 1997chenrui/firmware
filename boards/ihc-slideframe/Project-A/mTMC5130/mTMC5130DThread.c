#include "mTMC5130DThread.h"
#include "Protocols.h"
#include "cmd.h"
#include "ErrCode.h"
#include "mTMC5130Device.h"

#define MOTOR_THEARD_CNT    MOTOR_CNT

MOTOR motors[MOTOR_CNT];

MotorThread MotorThread_OS[MOTOR_THEARD_CNT];
extern TIM_HandleTypeDef htim4;
SeqQueue_Struct   queue[MOTOR_THEARD_CNT];        //队列成员应该是命令码和send对象

///<电机参数配置表
motor_cfg_struct motors_cfg[MOTOR_CNT]=
{
	{
		. zero_cordinate = 100,	    //零位距离
		. max_cordinate = 1000000, 		//最大行程
		. lost_endure_continue = 6,	//失步忍受,继续运动,返回时通知上位机
		. lost_endure_stop = 200,		//失步忍受,停止报错
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 0,
		. enb_vol = 1,
		. half_vol = 0,
		. enable_zero_makeup = 0,        //是不是旋转电机
		. back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=20,
		.rs_cordinate=200,
	},
	{
		. zero_cordinate = 100,	    //零位距离
		. max_cordinate = 1000000, 		//最大行程
		. lost_endure_continue = 6,	//失步忍受,继续运动,返回时通知上位机
		. lost_endure_stop = 200,		//失步忍受,停止报错
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 1,
		. half_vol = 0,
		. enable_zero_makeup = 0,        //是不是旋转电机
		. back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=20,
		.rs_cordinate=200,
	}
};

DRIVER_REG motors_reg[MOTOR_CNT]=
{
	{
		.CHOPCONF=0x101D5,
		.COOLCONF=1<<24|0<<16 | 10<<8 | 0,
		.TPWMTHRS=20,
		.TCOOLTHRS=120,
		.THIGH=0,
		.VDCMIN=500<<8,
		.PWMCONF=0x504c8,
	},
	{
		.CHOPCONF=0x101D5,
		.COOLCONF=1<<24|0<<16 | 10<<8 | 0,
		.TPWMTHRS=20,
		.TCOOLTHRS=120,
		.THIGH=0,
		.VDCMIN=500<<8,
		.PWMCONF=0x504c8,
	}
	
};

CREATE_Q_BLOCK(0);
CREATE_Q_BLOCK(1);

MOTOR_STATE_MACHINE(0);
MOTOR_STATE_MACHINE(1);

MOTOR_TMC_RESET(0);
MOTOR_TMC_RESET(1);

MOTOR_TMC_BACKZERO(0);
MOTOR_TMC_BACKZERO(1);

MOTOR_TMC_LISTEN_IO(0);
MOTOR_TMC_LISTEN_IO(1);

MOTOR_TMC_LISTEN_EVENT(0);
MOTOR_TMC_LISTEN_EVENT(1);

// CSN, ENABLE, E_STOP, REFL, REFR
MOTOR_IO_WR_DECLARE(0, GPIOC,GPIO_PIN_10, GPIOC,GPIO_PIN_11, GPIOC,GPIO_PIN_12, GPIOC,GPIO_PIN_8, GPIOC,GPIO_PIN_9);  //cs en em REFL_X  REFR_X
MOTOR_IO_WR_DECLARE(1, GPIOE,GPIO_PIN_13, GPIOE,GPIO_PIN_14, GPIOE,GPIO_PIN_15, GPIOE,GPIO_PIN_9, GPIOE,GPIO_PIN_12);  //cs en em REFL_X  REFR_X
// Zero, SWP, SWN
MOTOR_IO_RD_DECLARE(0, GPIOD,GPIO_PIN_15, GPIOC,GPIO_PIN_7, GPIOC,GPIO_PIN_6);  //光耦接触到时IO中断  比较位置寄存器IO中断		SWN_X
MOTOR_IO_RD_DECLARE(1, GPIOB,GPIO_PIN_12, GPIOE,GPIO_PIN_11, GPIOE,GPIO_PIN_10);  //光耦接触到时IO中断  比较位置寄存器IO中断		SWN_X


void EXTI9_5_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void EXTI15_10_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


#define motor_reach(motor,pin)\
{\
    if((motor)->is_reset == 0) {\
        mTMC5130ProcessSemPost((motor),WAIT_REACH_SEM);\
        mTMC5130ProcessSemPost((motor),WAIT_REACH_FLG);\
    } else {\
        __HAL_GPIO_EXTI_CLEAR_IT((pin));\
    }\
}

#define MOTOR_ZERO_TOUCH(motor)                                 \
{                                                               \
    MOTOR* ths = &motor;                                        \
    if(ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) != ths->mot_cfg->zero_trig_vol)\
    {                                                           \
        ths->read_io(PIN_ID_SET_TOUCH_ISR,ths->zeor_isr_id,ths->mot_cfg->zero_trig_vol);\
        return;                                                 \
    }                                                           \
    mTMC5130Motor_ZeroTouch(ths);                               \
    mTMC5130ProcessSemPost(ths,WAIT_TOUCH_SEM);                 \
}

#define MOTOR_POSITION_REACH(motor)     \
{                                   \
    MOTOR* ths = &motor;            \
    /*位置到达信号均为下降沿触发*/        \
    if(ths->read_io(PIN_ID_SET_REACH_SENSOR,ths->reach_isr_id,1) != 0)\
    {                               \
        ths->read_io(PIN_ID_SET_REACH_ISR,ths->reach_isr_id,0);\
        return;                     \
    }                               \
    mTMC5130ProcessSemPost(ths,WAIT_REACH_SEM);\
    mTMC5130ProcessSemPost(ths,WAIT_REACH_FLG);\
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("pin = %x\r\n", GPIO_Pin);
	switch(GPIO_Pin)
	{
		case GPIO_PIN_7:
		{
			MOTOR_POSITION_REACH(motors[0]);
			break;
		}
        case GPIO_PIN_11:
		{
			MOTOR_POSITION_REACH(motors[1]);
			break;
		}
        case GPIO_PIN_12:
		{
			MOTOR_ZERO_TOUCH(motors[1]);
			break;
		}
		case GPIO_PIN_15:
		{
			MOTOR_ZERO_TOUCH(motors[0]);
			break;
		}
		default:break;
	}
}

void MotorInit()
{
	MOTOR_TMC_INIT(0);
	MOTOR_TMC_INIT(1);

	MOTOR_THRAD_CREATE(0);
	MOTOR_THRAD_CREATE(1);
}


void MotorThread_EnQueue(int idx,Motor_Queue_Ele_t *data)
{
    SeqQueue_Enter(&queue[idx],data);
}

static int thread_idx = 0;
void  Motor_Thread(void *arg)
{
   BytesBufferNode* temp_node = NULL;
    int td_idx = thread_idx++;

	while(1)
    {
		SeqQueue_Waite(&queue[td_idx])	;
		Motor_Queue_Ele_t ele;
		BUS_ASSERT(!SeqQueue_Delete(&queue[td_idx],&ele),"Motor_Thread Seq Delete");
        BUS_ASSERT(SeqQueue_GetSize(&queue[td_idx])!=0,"Motor_Thread Seq Size");	
        switch(ele.cmd)
        {
            case CMD_MotorResetTmc:
            {
				printf("thread %d reset \r\n",td_idx);
				MotorResetTmc_Send *reset  = (MotorResetTmc_Send *)ele.send;
				BUS_ASSERT(reset->m1_idx != td_idx, "m1_idx != td_idx");
                
                MotorResetTmc_Return * rt = New_MotorResetTmc_Return();		


				motors[reset->m1_idx].id = reset->m1_idx;
				motors[reset->m1_idx].speed = reset->m3_speed;
				motors[reset->m1_idx].acc=reset->m2_acce;
				motors[reset->m1_idx].id=reset->m1_idx;
				motors[reset->m1_idx].i_run=reset->m4_currentLevel;
				motors[reset->m1_idx].micostep=reset->m5_subdivision;
				motors[reset->m1_idx].isBackZero=false;
				rt->m1_errCode=motors[reset->m1_idx].mTMC5130MotorReset(&motors[reset->m1_idx]);
				temp_node =    rt->ToBytes(rt);
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);														
				Delete_MotorResetTmc_Send(reset);
				Delete_MotorResetTmc_Return(rt);
				break;
            }            
            case CMD_MotorMoveTmc ://己测试
            {
				printf("CMD_MotorMove\r\n");
                MotorMoveTmc_Send *mv = (MotorMoveTmc_Send *)ele.send;
                BUS_ASSERT(mv->m1_idx != td_idx, "m1_idx != td_idx");
                
                MotorMoveTmc_Return *rt = New_MotorMoveTmc_Return();

                motors[mv->m1_idx].id=mv->m1_idx;
                motors[mv->m1_idx].speed=mv->m3_speed;
                motors[mv->m1_idx].acc=mv->m2_acce;
                motors[mv->m1_idx].isBackZero=mv->m6_isBackZero;
                motors[mv->m1_idx].isRelative=mv->m5_isRelative;
                motors[mv->m1_idx].i_run=mv->m7_currentLevel;
                motors[mv->m1_idx].micostep=mv->m8_subdivision;
//					motors[mv->m1_idx].status=MOTOR_STATUS_STOP;
                motors[mv->m1_idx].postion=mv->m4_distance;	
                motors[mv->m1_idx].backzero_position=mv->m9_backzero_position;//回零时停止的位置
                rt->m1_errCode=motors[mv->m1_idx].mTMC5130MotorMove(&motors[mv->m1_idx]);
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                
                motors[mv->m1_idx].isBackZero = 0; // 运动结束后清空回零标志
                free(data);
                Delete_MotorMoveTmc_Send(mv);
                Delete_MotorMoveTmc_Return(rt);																		 
							
                break;								
            }
			
			case CMD_MotorRotateStartTmc :
            {
				printf("CMD_MotorRotateStart\r\n");
				MotorRotateStartTmc_Send *send = (MotorRotateStartTmc_Send *)ele.send;
                BUS_ASSERT(send->m1_idx != td_idx, "m1_idx != td_idx");
				MotorRotateStartTmc_Return *rt = New_MotorRotateStartTmc_Return();

				motors[send->m1_idx].id=send->m1_idx;
				motors[send->m1_idx].speed=send->m3_speed;
				motors[send->m1_idx].acc=send->m2_acce;
				motors[send->m1_idx].i_run=send->m5_currentLevel;
				motors[send->m1_idx].micostep=send->m6_subdivision;
				motors[send->m1_idx].dir=send->m4_isForward;
//				motors[send->m1_idx].status=MOTOR_STATUS_STOP;							
				rt->m1_errCode=motors[send->m1_idx].mTMC5130MotorRotate(&motors[send->m1_idx]);
				temp_node = rt->ToBytes(rt);
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_MotorRotateStartTmc_Send(send);
				Delete_MotorRotateStartTmc_Return(rt);
                break;
            }            
			case CMD_MotorRotateStopTmc :
            {
				printf("CMD_MotorRotateStop\r\n");
				MotorRotateStopTmc_Send *send = (MotorRotateStopTmc_Send *)ele.send;
                BUS_ASSERT(send->m1_idx != td_idx, "m1_idx != td_idx");
				MotorRotateStopTmc_Return *rt = New_MotorRotateStopTmc_Return(); 
				
				motors[send->m1_idx].id=send->m1_idx;
//				motors[send->m1_idx].status=MOTOR_STATUS_STOP;												
				rt->m1_errCode=motors[send->m1_idx].mTMC5130MotorStop(&motors[send->m1_idx]);				
				temp_node = rt->ToBytes(rt);
				u8 *data =NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_MotorRotateStopTmc_Send(send);
				Delete_MotorRotateStopTmc_Return(rt);
                break;
            }
			case CMD_MotorStatetGetTmc:
			{
				printf("CMD_MotorStatetGetTmc\r\n");
				MotorStatetGetTmc_Send *send = (MotorStatetGetTmc_Send *)ele.send;
                BUS_ASSERT(send->m1_idx != td_idx, "m1_idx != td_idx");
				MotorStatetGetTmc_Return *rt = New_MotorStatetGetTmc_Return();

				motors[send->m1_idx].id=send->m1_idx;
				motors[send->m1_idx].mTMC5130MotorGetStatus(&motors[send->m1_idx]);	
				rt->m1_isZeroTriggered = ( motors[send->m1_idx].read_io(READ_PIN_ID_TOUCH_ISR,motors[send->m1_idx].zeor_isr_id,0)==motors[send->m1_idx].mot_cfg->zero_trig_vol );
				printf("motor%d:rt->m1_isZeroTriggered  :%d\r\n",motors[send->m1_idx].id,rt->m1_isZeroTriggered);			
				BytesBufferNode * node =  rt->ToBytes(rt);
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(node,&data);
				bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
				free(data);
				Delete_MotorStatetGetTmc_Send(send);
				Delete_MotorStatetGetTmc_Return(rt); 																						
			break;
			}		           
			case CMD_MotorMultiMoveTmc  :
            {
				printf("CMD_MotorMultiMove\r\n");							
				MotorMultiMoveTmc_Send *mmv = (MotorMultiMoveTmc_Send *)ele.send;
				MotorMultiMoveTmc_Return *rt = New_MotorMultiMoveTmc_Return(); 
				if(mmv->m1_MoveArgs_Length > 0 )
				{
					motor_multi_move_args_t * margs = malloc(sizeof(motor_multi_move_args_t)*mmv->m1_MoveArgs_Length);
					if(margs == NULL)
					{
						rt->m1_errCode = ERR_MOTOR_ARGS_ERROR;
					}
					else
					{
						memset(margs,0,sizeof(motor_multi_move_args_t)*mmv->m1_MoveArgs_Length);                  
						for(char i=0;i<mmv->m1_MoveArgs_Length;i++)
						{										
                            BUS_ASSERT(mmv->m1_MoveArgs[i].m1_idx > MOTOR_CNT, "m1_idx != td_idx");
							margs[i].motor=&motors[mmv->m1_MoveArgs[i].m1_idx];
							margs[i].m1_idx=mmv->m1_MoveArgs[i].m1_idx;
							margs[i].m2_acce=mmv->m1_MoveArgs[i].m2_acce;
							margs[i].m3_speed=mmv->m1_MoveArgs[i].m3_speed;
							margs[i].m4_distance=mmv->m1_MoveArgs[i].m4_distance;
							margs[i].m5_isRelative=mmv->m1_MoveArgs[i].m5_isRelative;
							margs[i].m6_delayms=mmv->m1_MoveArgs[i].m6_delayms;
							margs[i].m7_currentLevel=mmv->m1_MoveArgs[i].m7_currentLevel;
							margs[i].m8_subdivision=mmv->m1_MoveArgs[i].m8_subdivision;						
							margs[i].motor->id=mmv->m1_MoveArgs[i].m1_idx;	
							margs[i].motor->wait_type=0;
							margs[i].motor->evnt_group=0;
                            margs[i].motor->isBackZero=mmv->m2_isBackZero;
						}
						rt->m1_errCode = margs[0].motor->mTMC5130Motor_MultiMove(mmv->m1_MoveArgs_Length,margs,0);
						
					}
					free(margs);
				}
                else
                {
                    rt->m1_errCode = ERR_MOTOR_ARGS_ERROR;
                }
				
                BytesBufferNode * node =  rt->ToBytes(rt);
                u8 *data =NULL;
                int len = BytesBufferNode_Destroy(node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);  				
                free(data);
                Delete_MotorMultiMoveTmc_Send_m1_MoveArgs(mmv->m1_MoveArgs);
                Delete_MotorMultiMoveTmc_Send(mmv );
                Delete_MotorMultiMoveTmc_Return(rt);
                break;
            }  
			//运动IO事件
            case CMD_MotorMoveGPIOEventTmc : 
            {
//				printf("CMD_MotorMoveGPIOEventTmc\r\n");
                MotorMoveGPIOEventTmc_Send *mv = (MotorMoveGPIOEventTmc_Send*)ele.send;
                BUS_ASSERT(mv->m1_idx != td_idx, "m1_idx != td_idx");
                MotorMoveGPIOEventTmc_Return *rt = New_MotorMoveGPIOEventTmc_Return();
                GPIO_TypeDef *GPIO_PORT=NULL; 
				GPIO_InitTypeDef InitStruct;
				switch(mv->m6_state_port_pin >>4 & 0x7)
				{
					case 0: __HAL_RCC_GPIOA_CLK_ENABLE(); GPIO_PORT=GPIOA;break;
					case 1: __HAL_RCC_GPIOB_CLK_ENABLE(); GPIO_PORT=GPIOB;break;
					case 2: __HAL_RCC_GPIOC_CLK_ENABLE(); GPIO_PORT=GPIOC;break;
					case 3: __HAL_RCC_GPIOD_CLK_ENABLE(); GPIO_PORT=GPIOD;break;
					case 4: __HAL_RCC_GPIOE_CLK_ENABLE(); GPIO_PORT=GPIOE;break;
					case 5: __HAL_RCC_GPIOF_CLK_ENABLE(); GPIO_PORT=GPIOF;break;
					case 6: __HAL_RCC_GPIOG_CLK_ENABLE(); GPIO_PORT=GPIOG;break;
					case 7: __HAL_RCC_GPIOH_CLK_ENABLE(); GPIO_PORT=GPIOH;break;
				}
				InitStruct.Pin =1<<(mv->m6_state_port_pin &0xf);;
				InitStruct.Mode = GPIO_MODE_INPUT;
				InitStruct.Pull = GPIO_NOPULL;
				InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(GPIO_PORT, &InitStruct);

				motors[mv->m1_idx].id=mv->m1_idx;
				motors[mv->m1_idx].acc=mv->m2_acce;
				motors[mv->m1_idx].speed=mv->m3_speed;
				motors[mv->m1_idx].postion=mv->m4_distance;
				motors[mv->m1_idx].isRelative=mv->m5_isRelative;
				motors[mv->m1_idx].Listen_pin=mv->m6_state_port_pin &0xf;
				motors[mv->m1_idx].Listen_type=mv->m6_state_port_pin >>7 & 0x3;
//				motors[mv->m1_idx].i_run=mv->m7_currentLevel;
				motors[mv->m1_idx].micostep=mv->m8_subdivision;
				motors[mv->m1_idx].Listen_port=GPIO_PORT;
	
				u8 *data = NULL;
                
                rt->m1_errCode=motors[mv->m1_idx].mTMC5130MotorMoveGPIOEvent(&motors[mv->m1_idx],&rt->m2_trigger_pos);
                
                
                if(rt->m1_errCode != 0)
                {	
                    rt->m2_trigger_pos=0;
                }
                temp_node = rt->ToBytes(rt);
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);

                free(data);
                Delete_MotorMoveGPIOEventTmc_Send(mv);
                Delete_MotorMoveGPIOEventTmc_Return(rt);  			
                break;
            } 
		}
    }
}

