#include "disk_ctrl.h"
#include "AD7699.h"
#include <stdlib.h>
#include "bus_can_f4_hal.h"
#include "Async.h"
#include "autoCai.h"
#include "cmd.h"
#include "Protocols.h"
#include "write_data.h"

#define EPSINON_AD  10
//#define Debug_Printf 
//#define Debug_Printf_1


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;


typedef struct encoder encoder;
struct encoder
{
    int count_z;
    unsigned int count_ab;
    bus_os_sem      sem;
};
encoder encoder_t;


static DiskCtrl_t Ths;
//#define Debug_Printf 

static PROCESS  result;

const unsigned int EPSINON_AD_ONE=5;

u16  mix_first_minute_t=0;
bool isTimeTrigger=0;
bool isTimeStart=0;

#define OPEN_IIMEx(x,time) \
do{\
	x->ARR =time;\
	x->CNT=0;\
	x->DIER |= TIM_IT_UPDATE;\
	x->CR1|=TIM_CR1_CEN;\
}while(0);\

	
//static void  Mc500Light_SendData(void )
//{
//	u32 rt;
//	u32 rt_len;
//	u32 code = bus_ita_hb_send(CAN_ADAPTER_ID,0,Ths.disk.send_hole_cnt,(u8*)&Ths.disk.buff[0],(u8*)(&rt),&rt_len);
//	if(code!=0)
//	{
//		printf("c=%x\r\n",code);
//		NVIC_SystemReset();
//	}
//}


extern TIM_HandleTypeDef htim1;


void EXTI9_5_IRQHandler(void)//
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    
}
//bool test =false;
//void EXTI15_10_IRQHandler(void)//中断函数  Z
//{
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//    
//   // encoder_t.count_ab = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
//    __HAL_TIM_SET_COUNTER(&htim1,0);//清零计数
//    encoder_t.count_z ++;
//    //test = true;
//   // bus_os_post_sem(&encoder_t.sem);	
//}


void TIM1_UP_TIM10_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
  //  encoder_t.over_flow ++;
}

void EXTI2_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);	
    if(AutoCali_IsStart())
    {
        AutoCali_Now();
    }
	else if(Ths.is_start)
    {
		Ths.disk.state = STATE_ZERO_TRIG;
		Mc500Light_StateMachine();				
    }
}


//void TIM2_IRQHandler(void)
//{
//	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
//		
//    if(AutoCali_IsStart())
//    {
//        AutoCali_TimerHandle();
//    }
//    else if(Ths.is_start)
//    {		
//        Mc500Light_StateMachine();
//    }
//}

static inline void Mc500Light_StateMachine()
{
    switch(Ths.disk.state)
    {
        case STATE_ZERO_TRIG:
        {       
			OPEN_IIMEx(TIM2,Ths.disk.zero2shole);
            lightCtrlBase.stop_exit2();		
			Ths.disk.read_idx=0;	
			Ths.disk.node_n=1;
			Ths.disk.state=STATE_READ;	
        }break;			
        case STATE_READ:
        {				
			TIM2->ARR=Ths.disk.StartLinkList[Ths.disk.node_n].off;
			if(isTimeStart == true)
			{
				if(isTimeTrigger == true)
				{
					mix_first_minute_t = OSTime;//OSTimeGet();								
				}
				else
				{					
					mix_first_minute_t = 0;												
				}
				isTimeStart=false;
				isTimeTrigger=false;
				bus_os_post_sem(&AD_Detect_Thread_Cfg.sem);	
			}
			if(Ths.disk.StartLinkList[Ths.disk.node_n].puplue_offset <= EPSINON_AD_ONE) 			
			{	
				Ths.disk.buff[1].m1_data[Ths.disk.StartLinkList[Ths.disk.node_n].purple_idx] = averageRead(MPURPLE);
				#ifdef Debug_Printf
				printf("read %d purple  %d\r\n",Ths.disk.StartLinkList[Ths.disk.node_n].purple_idx,Ths.disk.buff[1].m1_data[Ths.disk.StartLinkList[Ths.disk.node_n].purple_idx]);
				#endif
			}
			if(Ths.disk.StartLinkList[Ths.disk.node_n].yellow_offset <= EPSINON_AD_ONE) 
			{
				Ths.disk.buff[1].m1_data[Ths.disk.StartLinkList[Ths.disk.node_n].yellow_idx+ Ths.disk.hold_max] = averageRead(MYELLOW);
				#ifdef Debug_Printf
				printf("read %d yellow  %d\r\n",Ths.disk.StartLinkList[Ths.disk.node_n].yellow_idx,Ths.disk.buff[1].m1_data[Ths.disk.StartLinkList[Ths.disk.node_n].yellow_idx+ Ths.disk.hold_max]);
				#endif
			}
			
			Ths.disk.read_idx++;
			Ths.disk.node_n++;		
            if(Ths.disk.read_idx >= Ths.disk.pre_hole_cnt)
            {        
				//printf("Ths.disk.pre_hole_cnt=%d\r\n",Ths.disk.cur_cir_idx);
				lightCtrlBase.start_exit2();				
                lightCtrlBase.stop_timer();
								
                Ths.disk.buff[0].cmd = 0x32;
                Ths.disk.buff[0].len = Ths.disk.hold_max * 2;	
				Ths.disk.buff[0].m2_current_detect_circle=Ths.disk.cur_cir_idx;
				
				for(char i=0;i<Ths.disk.hold_max * 2;i++) 
				{
					Ths.disk.buff[0].m1_data[i]= Ths.disk.buff[1].m1_data[i];
					Ths.disk.buff[1].m1_data[i]=0;
				}
				
				Ths.disk.buff[0].m1_data[Ths.disk.hold_max * 2]=Ths.disk.cur_cir_idx;
				
                Ths.disk.cur_cir_idx ++;				
				//Async_Excute(1,Mc500Light_SendData);
				bus_write_data((u8 *)&Ths.disk.buff[0], Ths.disk.send_hole_cnt, CAN_ADAPTER_ID, 0);
            }

            if(Ths.disk.cur_cir_idx >= Ths.disk.need_cir_cnt)
            {              
                Ths.is_start = false;
                lightCtrlBase.stop_timer(); 
                lightCtrlBase.stop_exit2();	
				printf("cbli over\r\n"); 				
            } 
            break;
        }		
    }
}

u32 check_flg=24;
unsigned char motor_type=0x2;

//typedef struct _DriverInfo  //计算第一个采值孔与光源的位置  所需的参数
//{
//	float m_n2y;//两个光源孔的距离
//	float m_z2s;//豁口到第一个孔的距离
//	float m_l2l;//扇区之间的距离
//	float m_s2s;//扇区内孔之间的距离
//	float m_l2s;//豁口到最后一个孔的距离
//	char  m_max_hole;//孔的数量
//	char  m_sector_hole;//每个扇区多少个孔
//}DriverInfo

static void CreateProcessLinkList(unsigned int a)
{	
	DriverInfo Info;
	
	// 长爪中轴线与缺口中轴线偏差0.34度，因此需要矫正
	// 当盘片逆时针旋转时，左加右减；反之，左减右加
	
	if(check_flg == 24) // 24孔(镜像和非镜像)，测试OK
	{
		// 缺口在两孔中间：左加右减
//		const DriverInfo Driver={18.947f, 7.161f + 0.34f, 0.0f, 15.0f, 7.839f - 0.34f, 24, 0}; // WZC
		
		const DriverInfo Driver={18.947f, 7.161f + 0.34f  +0.42f, 0.0f, 15.0f, 7.839f - 0.34f - 0.42f, 24, 0}; // WZC +0.42
		
//		const DriverInfo Driver={18.947f, 7.161f + 0.34f - 0.21f, 0.0f, 15.0f, 7.839f - 0.34f + 0.21f, 24, 0}; // WZC
//		const DriverInfo Driver={18.947f, 7.161f + 0.34f + 0.21f, 0.0f, 15.0f, 7.839f - 0.34f - 0.21f, 24, 0}; // WZC
//		const DriverInfo Driver={18.947f, 7.161f + 0.34f + 0.61f, 0.0f, 15.0f, 7.839f - 0.34f - 0.61f, 24, 0}; // WZC
		Info = Driver;
	}
	else if(check_flg == 30) // 30孔(全血四项)，测试OK
	{
		// 缺口在两孔中间：左加右减
		//const DriverInfo Driver={18.947, 13.619, 22.105, 9.474, 9.166, 30, 5}; // ZHR: 13.279 8.826
		const DriverInfo Driver={18.947f, 13.279f + 0.34f, 22.105f, 9.474f, 8.826f - 0.34f, 30, 5}; // WZC
		Info = Driver;
	}
	else if(check_flg == 361) // 通用36孔无扇区，待调试
	{
		// 缺口在孔的正上方：左加右减
		//const DriverInfo Driver={18.947, 8, 0, 10, 2, 36, 0}; // ZHR
		const DriverInfo Driver={18.947f, 0.0f + 0.34f, 0.0f, 10.0f, 10.0f - 0.34f, 36, 0}; // WZC
		Info=Driver;
	}
	else if(check_flg == 362) // 36孔(全血五项，三个扇区)，测试OK
	{
		const DriverInfo Driver={18.947, 8.02, 15.79, 9.474, 7.772, 36, 12}; // ZHR
		Info=Driver;
	}
	
	result=CreateProceeList(a,Info,motor_type);
		
    Ths.disk.hold_max=Info.m_max_hole;
	Ths.disk.pre_hole_cnt=result.lenght; 
	Ths.disk.StartLinkList=result.Phead;
	Ths.disk.start_idx=result.start_idx + 1;
	Ths.disk.less=result.less;
	
	isTimeStart=true;
	mix_first_minute_t=0;
	
    Ths.disk.send_hole_cnt=((unsigned int *)&Ths.disk.buff[0].m1_data[Ths.disk.hold_max *2] -(unsigned int *)&Ths.disk.buff[0].cmd) * 4 +4;
    
	Ths.disk.zero2shole=result.Phead->off - 75;
	
    memset(Ths.disk.buff,0,sizeof(Ths.disk.buff));

	printf("Ths.disk.pre_hole_cnt=%d\r\nsend_hole_cnt=%d\r\nThs.disk.zero2shole=%d\r\n",\
				Ths.disk.pre_hole_cnt,Ths.disk.send_hole_cnt,Ths.disk.zero2shole); 
	printf("Ths.disk.less=%d\r\nThs.disk.start_idx=%d\r\n",Ths.disk.less,Ths.disk.start_idx);
}


void Mc500Light_ReadContinuous(u32 detect_circle_cnt,u32 z2f,MC_AD_Ele_t* ele)
{
	lightCtrlBase.stop_exit2();		
	z2f -= 10417; //定标值减去豁口到第一个孔的距离
    Ths.disk.need_cir_cnt = detect_circle_cnt;
    Ths.disk.cur_cir_idx = 0;
    memmove(&Ths.rev_ele,ele,sizeof(MC_AD_Ele_t));
    
	printf("need_cir_cnt = %d \r\n",Ths.disk.need_cir_cnt);
	printf("zero2shole          = %d \r\n",z2f);
    
	FreeLinkList();
	CreateProcessLinkList(z2f);
	
	lightCtrlBase.start_exit2();
    Ths.is_start = true; 		
}



void Mc500Light_Init(void)
{
//    lightCtrlBase.stop_timer(); 
//    lightCtrlBase.stop_exit2();
    
	//bus_write_init(); // 初始化数据发送模块
    EXTI->IMR &= (~GPIO_PIN_13);
     bus_os_create_sem(&encoder_t.sem,0);
   
    bus_os_create_sem(&AD_Detect_Thread_Cfg.sem,0);
	bus_create_thread(&AD_Detect_Thread_Cfg.tcb,AD_Detect_Thread,AD_Detect_Thread_Cfg.stack,AD_DETECT_THREAD_STACK_SIZE,AD_Detect_Thread_Prio,NULL);	
}


void AD_Detect_Thread(void *data)
{
    EXTI->IMR |= (GPIO_PIN_13);//打开Z中断
	while(1)
	{
       // bus_os_pend_sem(&encoder_t.sem,0);
       
        encoder_t.count_ab = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
        
        printf("encoder_t.count_z =%d         code =%d\r\n",encoder_t.count_z,encoder_t.count_ab);
       
        #if 0
		bus_os_pend_sem(&AD_Detect_Thread_Cfg.sem,0);
		switch(Ths.rev_ele.cmd)
		{
			case CMD_ADDataRead :
			{
				printf("CMD_ADDataRead\r\n");
				ADDataRead_Send* send = (ADDataRead_Send * )Ths.rev_ele.send;
				ADDataRead_Return* rt = New_ADDataRead_Return();
                
				//等待执行完毕
				rt->m1_val_Length = 2;
				rt->m1_val = malloc(rt->m1_val_Length*sizeof(u16));				
				rt->m1_val[0]=0;
				rt->m1_val[1]=0;				

				rt->m1_val[0]=mix_first_minute_t;
				rt->m1_val[1]=Ths.disk.start_idx;
				
//				WriteU16((u8*)rt->m1_val++,mix_first_minute_t);
//				WriteU16((u8*)rt->m1_val,Ths.disk.start_idx);

				BytesBufferNode * node =  rt->ToBytes(rt);
				
				u8 *data = NULL;
				int len = BytesBufferNode_Destroy(node,&data);
				u32 code = bus_ita_return(Ths.rev_ele.adapterId,Ths.rev_ele.srcId,Ths.rev_ele.sessionId,len,data);
				
				free(data);
				free(rt->m1_val);
				Delete_ADDataRead_Send(send);
				Delete_ADDataRead_Return(rt);
				printf("return OK...\r\n");
				break;
			}
		}
		Ths.rev_ele.send = NULL;
        #endif
	} 
}


