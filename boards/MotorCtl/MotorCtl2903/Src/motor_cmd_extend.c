#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_000100a5.h"
#include "itf_000100a6.h"
#include "io_mod.h"
#include "listenandsync_cfg.h"
#include "bus_can_f4_hal.h"
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);

extern TIM_HandleTypeDef htim2;
u8 sendBuf[32];
//////////上料进程
u32 FunnelMotor_ThreadSTk[256];
u8 FunnelMotor_Work_step=0;
OS_EVENT *FunnelMotor_Sem;
bool ForcedWorkingEndFlag=false;

 u8 FunnelMotor_Run(void);
void FunnelMotor_Thread(void *arg)
{
	u8 err;
	u8 zeroCount=0;
	u32 rt = 0;
	u32 len = 0;

	
	FunnelMotor_Sem=OSSemCreate(0);
	printf("创建 漏斗电机任务成功\r\n");
	FunnelMotor_Work_step=0;
	for(;;)
	{

		switch(FunnelMotor_Work_step)
		{
			case 0://默认 停止状态 需要下达命令
			{
				
				break;	
			}
			case 1://正常工作状态
			{
				//printf("进料一次\r\n");
//				queue_ele_enter(0, 0x01000a05, 0, 0);//让电机执行一步
//				OSSemPend(FunnelMotor_Sem, 0, &err);//阻塞到电机执行完
				FunnelMotor_Run();
				OSTimeDly(500);//这里决定了运行速度（料斗进料速度）
				if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==GPIO_PIN_RESET)//说明料已经满了
				{
					FunnelMotor_Work_step=2;//物料满了 也需要停止
				}
			break;	
			}
			
			case 2://料满状态 	
			{
				
				if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==GPIO_PIN_SET)//说明料已经不满了
				{
					FunnelMotor_Work_step=1;
				}
				break;
			}
			
			case 10://强制工作状态
			{
//				queue_ele_enter(0, 0x01000a05, 0, 0);//让电机执行一步
//				OSSemPend(FunnelMotor_Sem, 0, &err);//阻塞到电机执行完
				FunnelMotor_Run();
				OSTimeDly(500);//这里决定了运行速度（料斗进料速度）
				
				if(ForcedWorkingEndFlag==true)
				{
					ForcedWorkingEndFlag=false;
					FunnelMotor_Work_step=2;//恢复普通模式
				}
				break;
			}
				
		}
		OSTimeDly(10);	
	}
}



//
//进行一步操作
 u8 FunnelMotor_Run(void)
{
	u8 error_code =0;
	

	printf("移动一定距离\r\n");
	motors[0].id = 0;
	motors[0].postion = motor_step_calc(0, 5);
	motors[0].speed = motor_step_calc(0, 20);
	motors[0].acc = motor_step_calc(0, 10);
	motors[0].isBackZero = 0;
	motors[0].isRelative = 1;
    motors[0].mTMC5130MotorMove(&motors[0]);	
	
	for(int i = 0;i < 3; i++)
    {
		motors[0].id = 0;
		motors[0].postion = motor_step_calc(0, -1);
		motors[0].speed = motor_step_calc(0, 500);
		motors[0].acc = motor_step_calc(0, 300);
		motors[0].isBackZero = 0;
		motors[0].isRelative = 1;
       	motors[0].mTMC5130MotorMove(&motors[0]);
        motors[0].postion = motor_step_calc(0, 1);
        motors[0].mTMC5130MotorMove(&motors[0]);
    }
	
	
			
	return error_code;
}

volatile static bool takeCup_flag=false;

u8 motor_thread_exe_extend(Motor_queue_ele_t *ele)
{
	switch (ele->cmd_code)
	{
		case 0x01000a05:
		{
//			uint8_t err=0;
//			err=FunnelMotor_Run();

//			
//			if(FunnelMotor_Work_step==1)//在正常状态下
//			{

//			}
//			else if(FunnelMotor_Work_step==10)//在强制工作状态下
//			{

//			}
//			
//			OSSemPost(FunnelMotor_Sem);
			break;
		}
		case 0x01000a06:
		{
			u8 last=0;
			u8 current=0;
			Itf_000100a6_t *cmd = ele->p_cmd;		
		
			u8 idx = 1;
			motors[idx].acc=motor_step_calc(idx, 100);
			motors[idx].speed=motor_step_calc(idx, 30);
		
			takeCup_flag=false;
			motors[idx].postion=motor_step_calc(idx,3);
			motors[idx].isRelative=1;
			motors[idx].isBackZero = 0;
			
			HAL_TIM_Base_Start_IT(&htim2);
			motors[idx].mTMC5130MotorMove(&motors[idx]);
			HAL_TIM_Base_Stop_IT(&htim2);
		
		
			motors[idx].id=idx;

			motors[idx].postion=motor_step_calc(idx,300);
			motors[idx].isRelative=1;
			int coordinate = 0;
			u8 error_code=motors[idx].mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
		
			if(error_code != 0)
			{	
			   
				cmd->rtn_and_free(cmd, &ele->packet_head, 1,0,0);
				break;
			}
			if(takeCup_flag==true)
				last=1;
			
			takeCup_flag=false;
			motors[idx].postion=motor_step_calc(idx,cmd->content.cal_distance);
			printf("cmd->content.cal_distance=%f\r\n",cmd->content.cal_distance);
			motors[idx].isRelative=1;
			motors[idx].isBackZero = 0;
			HAL_TIM_Base_Start_IT(&htim2);
			motors[idx].mTMC5130MotorMove(&motors[idx]);
			HAL_TIM_Base_Stop_IT(&htim2);
			
			if(takeCup_flag==true)
				current=1;
			
			cmd->rtn_and_free(cmd, &ele->packet_head, 0,last,current);
			break;
		}
	default:
		return 1;
	}
	return 0;
}

void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)==GPIO_PIN_RESET)
	{
		takeCup_flag=true;
	}
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet *packet)
{
	switch (func_code)
	{
		case 0x01000a05:
		{
			
			Itf_000100a5_t * cmd = Itf_000100a5_new();
			cmd->packet_parse(cmd,packet);
			
			printf("0x01000a05 start=%d\r\n",cmd->content.start);

			if(cmd->content.start==0)//停止
			{
				FunnelMotor_Work_step=0;
			}
			else if(cmd->content.start==1)//开启
			{
				FunnelMotor_Work_step=2;
			}
			else if(cmd->content.start==2)//强制工作状态
			{
				ForcedWorkingEndFlag=false;
				FunnelMotor_Work_step=10;
			}
			
			cmd->rtn_and_free(cmd,packet,0);
			break;
		}
		case 0x01000a06:
		{
			Itf_000100a6_t *cmd = Itf_000100a6_new();		
			cmd->packet_parse(cmd, packet);
			//printf("0x01000a06 =%f\r\n",cmd->content.cal_distance);
			queue_ele_enter(0, func_code, packet, cmd);
			
			break;
		}
		default:
			return 1;
		}
	return 0;
}

void TakeCup_Test()
{
		u8 last=0;
		u8 current=0;
	
	
		u8 idx = 1;

		motors[idx].acc=motor_step_calc(idx, 100);
		motors[idx].speed=motor_step_calc(idx, 30);
	
		takeCup_flag=false;
		motors[idx].postion=motor_step_calc(idx,3);
		motors[idx].isRelative=1;
		motors[idx].isBackZero = 0;
		
		HAL_TIM_Base_Start_IT(&htim2);
		motors[idx].mTMC5130MotorMove(&motors[idx]);
		HAL_TIM_Base_Stop_IT(&htim2);
	
	
		motors[idx].id=idx;

		motors[idx].postion=motor_step_calc(idx,300);
		motors[idx].isRelative=1;
		int coordinate = 0;
		u8 error_code=motors[idx].mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
	
	
		if(error_code != 0)
		{	
		   
			printf("move stop event err \r\n");
			return;
		}
		if(takeCup_flag==true)
			last=1;
		
		
		motors[idx].postion=motor_step_calc(idx,1);
		motors[idx].isRelative=1;
		motors[idx].isBackZero = 0;
		takeCup_flag=false;
		
		HAL_TIM_Base_Start_IT(&htim2);
		
		motors[idx].mTMC5130MotorMove(&motors[idx]);
		HAL_TIM_Base_Stop_IT(&htim2);
		
		if(takeCup_flag==true)
			current=1;
		printf("-------------------------------------------\r\n");	
		printf("tagerover last=%d,current=%d\r\n",last,current);
		printf("-------------------------------------------\r\n");	
}

void FunnelMotor_Create()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//漏斗反射五联传感器
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	//拨杯对射传感器
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	u8 rt= OSTaskCreateExt(FunnelMotor_Thread, 
										0,
										&FunnelMotor_ThreadSTk[256 - 1],
										12,
										12,
										&FunnelMotor_ThreadSTk[0],
										256,
										0,
										(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	
	

	
	if(rt!=0)//如果线程创建失败 就返回数组长度为0
	{
		printf("漏斗任务创建失败\r\n");
		
	}
	else
	{
		printf("漏斗任务创建成功\r\n");
		
		
	}



	
	
}



void EXTI2_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
	
	ForcedWorkingEndFlag=true;
}

/*
if(++zeroCount>=5)
				{
					zeroCount=0;
					
					if(zeroTigger>5)
					{
						zeroTigger=0;
					}
					else//卡杯了 需要停止
					{
						zeroTigger=0;
						printf("卡杯\r\n");
						WriteU32((u8*)&sendBuf[0],0x1000a05);
						sendBuf[4]=1;
						
						if(bus_ita_hb_send(CAN_ADAPTER_ID, 5, 10, sendBuf, (u8*)(&rt), &len) == 0)
						{
							printf("卡杯上报成功\r\n");
						}
						else
						{
							printf("卡杯上报失败\r\n");
						}				
						FunnelMotor_Work_step=0;
					}
				}
*/
