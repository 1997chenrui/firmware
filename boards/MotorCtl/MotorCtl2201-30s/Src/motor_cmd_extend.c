#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "bus_uart3_f4_hal.h"
#include "itf_01000f10.h"
#include "ad_signal.h"
#include "stm32f4xx_hal.h"
#include "AD7699.h"
#include "ad_signal_cfg.h"
#include "write_data.h"

u32 Samping_cmd_extend_ThreadSTk[256];

extern TIM_HandleTypeDef htim2;

extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);

static void signal_read(u8 idx, u16 time_ms, u16 time_span_ms);

static Itf_01000f10_content_t SampingThreadArg;
static u8 sampingtaskBusyFlag=0;

static uint16_t singNalData[13];
static uint16_t singNalDataCount;
void TIM2_IRQHandler(void)
{	
	uint16_t value;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	value=AD7699_GetSingleData(&ad7699[0], 1);
	WriteU16((u8*)&singNalData[singNalDataCount+3],value);
	printf("%d\n",value);
	if(++singNalDataCount>=10)
	{
		singNalDataCount=0;
		WriteU32((u8*)&singNalData[0],0x1000f10);
		WriteU32((u8*)&singNalData[0+2],(10));
		
		bus_write_data((u8 *)&singNalData, 13*sizeof(u16), UART3_ADAPTER_ID, 0);
		
	}
	
	
}

void Samping_cmd_extend_Thread(void *arg)
{
	Itf_01000f10_content_t p=*(Itf_01000f10_content_t*)arg;
	
	
	printf("创建采值任务成功\r\n");
	sampingtaskBusyFlag=1;
	printf("加速度：%f\r"\
		   "速度%f\r"\
	       "距离%f\r"\
		   "采值时间间隔%d\rn",
			p.acc,p.speed,p.distance,p.time_spam_ms);
	
	
	

	singNalDataCount=0;
	//设定采值间隔时间
	__HAL_TIM_SET_AUTORELOAD(&htim2,(p.time_spam_ms*1000)-1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);//开灯
	
	for(;;)
	{			
		//配置电机运动
		motors[4].id = 4;
        motors[4].postion = motor_step_calc(4, p.distance);
	    motors[4].speed = motor_step_calc(4, p.speed);
        motors[4].acc = motor_step_calc(4, p.acc);
        motors[4].isBackZero=0;
        motors[4].isRelative=1;
	    u8 error_code = motors[4].mTMC5130MotorMove(&motors[4]);
        //阻塞至电机完成
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);//关灯
		//看下上组数据是否完整 否则全部补0
		if(singNalDataCount!=0)
		{
			for(uint16_t i=singNalDataCount;i<10;i++)
			{
				WriteU16((u8*)&singNalData[singNalDataCount+3],0);
				WriteU32((u8*)&singNalData[0],0x1000f10);
				WriteU32((u8*)&singNalData[0+2],(10));
			}
			WriteU32((u8*)&singNalData[0],0x1000f10);
			WriteU32((u8*)&singNalData[0+2],(10));
			bus_write_data((u8 *)&singNalData, 13*sizeof(u16), UART3_ADAPTER_ID, 0);			
		}
		
		//发送结束数据
		for(uint16_t i=0;i<10;i++)
		{
			WriteU16((u8*)&singNalData[singNalDataCount+3],0xFFEE);	
		}
		
		WriteU32((u8*)&singNalData[0],0x1000f10);
		WriteU32((u8*)&singNalData[0+2],(10));
		bus_write_data((u8 *)&singNalData, 13*sizeof(u16), UART3_ADAPTER_ID, 0);
		
		printf("检测完成\r\n");
		sampingtaskBusyFlag=0;
		OSTaskDel(6);
		

	}
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
	switch(func_code){
		case 0x01000f10: {
			Itf_01000f10_t * cmd = Itf_01000f10_new();
			cmd->packet_parse(cmd,packet);
			
			if(sampingtaskBusyFlag==1)
			{
				    printf("上次采值未结束\r\n");
					cmd->rtn_and_free(cmd,packet,ITF_01000F10_UNCLOSED);
					return 0;
			}
			if(cmd->content.acc<=0||cmd->content.distance==0)
			{
				    printf("参数非法\r\n");
					cmd->rtn_and_free(cmd,packet,ITF_01000F10_UNCLOSED);
					return 0;			
			}
				
			SampingThreadArg=cmd->content;
			
			u8 rt= OSTaskCreateExt(Samping_cmd_extend_Thread, 
				&SampingThreadArg,
				&Samping_cmd_extend_ThreadSTk[256 - 1],
				6,
				6,
				&Samping_cmd_extend_ThreadSTk[0],
				256,
				0,
				(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
			
			if(rt!=0)//如果线程创建失败 就返回数组长度为0
			{
				printf("任务创建失败\r\n");
				cmd->rtn_and_free(cmd,packet,ITF_01000F10_ERR);
			}
			else
			{
				printf("任务创建成功\r\n");
				cmd->rtn_and_free(cmd,packet,ITF_01000F10_UNCLOSED);
			}		
			break;
		}
		default:return 1;
	}
    return 0;
}



