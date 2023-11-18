#include "auto_cali_client.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "write_data.h"
#include "bus_can_f4_hal.h"
#include "cmd.h"
#include "Protocols.h"
#include "signal_det.h"

static inline void mc800_light_statemachine(void);
extern TIM_HandleTypeDef htim1;
extern light_time light_Table[LNODE_BUFF_MAX];
extern Collection_t collection_table;

auto_client auto_client_t;
const unsigned int EPSINON_AD_ONE=5;
u16  mix_first_minute_t=0;
bool isTimeTrigger=0;
bool isTimeStart=0;
static DiskCtrl_t Ths;

// PA5连接到52号板的PB11
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	OSTime=0; //清零时间计数器  开始计时
	isTimeTrigger=true;
}

#define OPEN_IIMEx(x,time) \
do{\
	x->ARR =time;\
	x->CNT=0;\
	x->DIER |= TIM_IT_UPDATE;\
	x->CR1|=TIM_CR1_CEN;\
}while(0)\


unsigned int light2hole(void)//取平均值
{
    return (auto_client_t.encoder.c_time1 + auto_client_t.encoder.c_time2) >> 1;//(ALL_ENCODER_NUM -((auto_client_t.encoder.c_time1 + auto_client_t.encoder.c_time2)>>1));
}

static void cali_start(auto_client *me, ecali method)
{
    singelRead(n_800);//采集一次AD数据
    me->tick = 0;//清空记录时间
    me->method = method;//根据参数设定 粗/细 定标
    if(method == eC_CALI)//粗
		{
					printf("now start c cali\r\n");
					me->state = eCali_ready;//修改状态为 eCali_ready
					set_tim_cnt(100);//设定定时器初值
		}
		else if(method == eD_CALI)//细
		{
					printf("now start d cali\r\n");
					set_tim_cnt(50);//设定定时器初值
					me->state = eCali_ready;//修改状态为 eCali_ready
		}
    open_exti();////打开外部中断
		
    bus_os_pend_sem(&me->sem,0);//等待阻塞结束（调用cali_it_timeFSM后可能释放信号）无限等待，不设超时时间
		
    if(method == eC_CALI)////粗定标 数据处理
    {
        // // 记录上下光源的上下脉冲数
			auto_client_t.encoder.c_time1 = auto_client_t.encoder_event[0].encoder_value1;
			auto_client_t.encoder.c_time2 = auto_client_t.encoder_event[1].encoder_value1;
			auto_client_t.dlt_time = auto_client_t.encoder.c_time1 - auto_client_t.encoder.c_time2;////逆时针旋转  向下递减
    }
    else////精定标 数据处理
    {
        auto_client_t.light_to_hole = light2hole(); 
        printf("light_to_hole = %d\r\n",auto_client_t.light_to_hole);
    }
    me->method = eIDLE_CALI;////恢复空闲状态
}
void cali_client_Iint(auto_client *me)////初始化 cali客户端
{
    me->time_off_on = false;
    me ->errcode = 0;
    me ->samplingPeriod = 100;//设定定标周期 单位us
    me ->method = eIDLE_CALI;//设定 定标客户端状态为空闲
    bus_os_create_sem(&me->sem,0);//生成 定标客户端 信号量
    auto_encoder_Iint(&me->encoder);//初始化 编码器
    close_exti();////关闭中断
}

unsigned int appcali_start(void)//定标开始
{
    cali_start(&auto_client_t,eC_CALI);//先粗定标
    if(auto_client_t.errcode == 0)//如果粗定标没有错误，代表粗定标完成，即开始精定标
        cali_start(&auto_client_t,eD_CALI);  //精定标
		
		
    return auto_client_t.errcode;//把错误代码返回

}


/*
1213 :1. 初始的时候判断光源大于 15000
       2.记录光源小于15000的时候tick   脉冲数
        3.记录光源重新大于15000的时候 tick  脉冲数
        4.打印tick  脉冲        计算 脉冲差（>=676）
开始细定标

1214 ：完成定标   系统固有误差3脉冲
*/

static void cali_it_timeFSM(void)//定标状态机 由定时器回调函数执行
{
    auto_client_t.tick ++;//记录时间值++
    switch(auto_client_t.state)//根据客户端状态进行状态机操作
    {
        case eCali_ready://在准备状态下
        {
            auto_client_t.encoder.light_value = singelRead(n_800);//进行一次采集
					
            if(auto_client_t.encoder.light_value < COVER) // 检测光是否被挡住
            {
								//如果 光被挡住
								//且是粗定标
                if(auto_client_t.method == eC_CALI)
                {
                    auto_client_t.errcode = ERR_CALIBRATE_ERR_T;//返回 “校准”错误
                    close_exti();//关闭外部中断
					close_tim();//关闭时钟
					bus_os_post_sem(&auto_client_t.sem);//释放客户端信号量
                }
                else if(auto_client_t.method == eD_CALI)
                {
					// 不做处理...
                }
            }
            else//如果光没有被挡住
            {
                auto_client_t.state = (auto_client_t.method == eC_CALI)? eC_SIGNAL_STATE_HIGHT : eD_CALI_STATE_LOW;
								//根据当前定标模式 设定客户端状态
								//粗定标 eC_SIGNAL_STATE_HIGHT  精定标 eD_CALI_STATE_LOW
            }
            auto_client_t.time_off_on = false;
            close_tim();//关闭定时器
			break;
        }
        case eC_SIGNAL_STATE_HIGHT://在粗定标后，初始状态 光没有被挡住
        {
            auto_client_t.encoder.light_value = singelRead(n_800);//采集一次数据
            if(auto_client_t.encoder.light_value < COVER)//如果光被挡住�
            {
								
                auto_client_t.encoder_event[auto_client_t.cnt_t].encoder_value1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
							// 记录第一次小于15000的状态
                auto_client_t.encoder_event[auto_client_t.cnt_t].light_value = auto_client_t.encoder.light_value;
							//记录当前采集数据
                auto_client_t.encoder_event[auto_client_t.cnt_t].tim_value1 = auto_client_t.tick;
							//记录从开始到现在总共时间。
                auto_client_t.state = eC_SIGNAL_STATE_LOW;//开始检测下次 棱 转出传感器
                auto_client_t.cnt_t++;
            }
			break;
        }
        case eC_SIGNAL_STATE_LOW ://在粗定标后，初始状态 光被挡住。
        {
				auto_client_t.encoder.light_value = singelRead(n_800);//采集一次数据
				if(auto_client_t.encoder.light_value > COVER )////如果光没被挡住，意味 棱 已 转出 传感器
				{
					// 记录再次大于15000的状态
					auto_client_t.encoder_event[auto_client_t.cnt_t].encoder_value1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
					
					auto_client_t.encoder_event[auto_client_t.cnt_t].light_value = auto_client_t.encoder.light_value;
					auto_client_t.encoder_event[auto_client_t.cnt_t].tim_value1 = auto_client_t.tick;
					auto_client_t.state = eC_SIGNAL_STATE_HIGHT;//开始检测下次 棱 转入传感器
					
					if(++auto_client_t.cnt_t > C_EVNET_IT_CNT)//粗定标完成
					{
						auto_client_t.cnt_t = 0;
						auto_client_t.tick = 0;
						close_exti();
						close_tim();
						bus_os_post_sem(&auto_client_t.sem);//释放信号，使得cali_start解除阻塞
					}
				}
			break;
        }
        case eD_CALI_STATE_LOW://精定标后的 初始状态 光被挡住。
        {
            unsigned short light_value = singelRead(n_800);//采集一次数据
            unsigned short encoder_value1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
            if(encoder_value1 < (auto_client_t.encoder.c_time1 + auto_client_t.dlt_time))
            {
                if(light_value < COVER)//光被挡住
                {
                    auto_client_t.encoder.c_time1 = encoder_value1;
                    auto_client_t.state = eD_CALI_CHECK;
                }
                else//光没有被挡住 等待电机下一周
                {
                    auto_client_t.encoder.c_time1 -= OFF_T;
                    auto_client_t.time_off_on = false;
                    close_tim();
                }
            }
			break;
        }
        case eD_CALI_CHECK:
        {
            auto_encoder event;
            event.light_value = singelRead(n_800);//读取一次数据
            event.encoder_value1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim1));
            if(event.encoder_value1 < (auto_client_t.encoder.c_time2 - (auto_client_t.dlt_time >>1)))//此处决定暂时无法360安装
            {
                if(event.light_value > COVER) //光没有被挡住 等待电机下一周
                {
                    auto_client_t.encoder.c_time2 += OFF_T;
                    auto_client_t.time_off_on = false;
                    close_tim();
                }
                else//光被挡住 完成精定标
                {
                    auto_client_t.encoder.c_time2 = event.encoder_value1;
                    close_exti();
										close_tim();
										bus_os_post_sem(&auto_client_t.sem);//释放信号，使得cali_start解除阻塞
                }
            }
			break;
        }
    }
}

void EXTI15_10_IRQHandler(void)////中断函数(电机零位中断，该信号来自于电机的编码器)
{
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5); // Debug
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13); // PE13
    __HAL_TIM_SET_COUNTER(&htim1,32768);//清零计数
    if(auto_client_t.auto_it_t.z_count < 10)//如果圈数小于10 （先使电机转10周，使电机进入到一个稳定的工作状态）
        auto_client_t.auto_it_t.z_count ++;////进行一次圈数 计数
    if(auto_client_t.method != eIDLE_CALI && auto_client_t.auto_it_t.z_count >= 10)
        open_tim();			//如果客户端不处于空闲状态（即处于粗/精 定标状态） 且 没有转够 10圈 那么开启定时器
    else if(Ths.is_start) ////如果采集任务开启
		{
			//在客户端计算时间表的任务完成后 没当转一圈 就触发一次 使传感器状态机 进行一次工作
			Ths.disk.state = STATE_ZERO_TRIG; //// 零位触发
			mc800_light_statemachine();//传感器状态机工作一次
    }
}

void TIM2_IRQHandler(void)////时钟2 中断回调函数
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    if(auto_client_t.method != eIDLE_CALI)//如果客户端不处于空闲状态 继续/进行 状态机工作
        cali_it_timeFSM();  // 定标步骤执行 继续计算时间表 
		
    else if(Ths.is_start) // 如果时间表计算完成 并且采集任务开启 那么就开始进行传感器状态机工作
        mc800_light_statemachine();//传感器状态机工作一次
}

	
static void cal_task(collection_light_ele_t* ele)
{
	if(isTimeStart == true)//每次生成 新的数据表后 只会进来一次
	{
		
		if(isTimeTrigger == true)
			mix_first_minute_t = OSTime;
		else
			mix_first_minute_t = 0;
		
		isTimeStart=false;//关闭
		isTimeTrigger=false;
		bus_os_post_sem(&AD_Detect_Thread_Cfg.sem);//释放AD采集线程信号
	}
	
	
	switch(ele->type)
	{
		case n_800: Ths.disk.buff[Ths.disk.buffidx].m1_data[light_Table[Ths.disk.read_idx].hole + 0*Ths.disk.hold_max] = averageRead(n_800); 
		
		break;
		
		case n_660: Ths.disk.buff[Ths.disk.buffidx].m1_data[light_Table[Ths.disk.read_idx].hole + 1*Ths.disk.hold_max] = averageRead(n_660);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);		
		break;
		
		case n_405: Ths.disk.buff[Ths.disk.buffidx].m1_data[light_Table[Ths.disk.read_idx].hole + 2*Ths.disk.hold_max] = averageRead(n_405); 
		break;
		
		case n_570: Ths.disk.buff[Ths.disk.buffidx].m1_data[light_Table[Ths.disk.read_idx].hole + 3*Ths.disk.hold_max] = averageRead(n_570);
		break;
		
		default: break;
	} // // 7次采值, switch需要420us
	Ths.disk.read_idx++;
	if(Ths.disk.read_idx >= Ths.disk.pre_hole_cnt)//如果达到最大的采集步骤
	{
		open_exti();//开启外部中断 
		close_tim();//关闭定时器
		Ths.disk.buff[Ths.disk.buffidx].cmd = 0x32;
		Ths.disk.buff[Ths.disk.buffidx].len = Ths.disk.hold_max * 4;
		Ths.disk.buff[Ths.disk.buffidx].m1_data[Ths.disk.hold_max * 4]=Ths.disk.cur_cir_idx;
			//在m1_data最后两个字节记录 当前圈数
		Ths.disk.cur_cir_idx ++;	
		//数据上传格式  4字节cmd + 4字节长度 + 2字节当前圈数 + 数据   每个数据2个字节
		bus_write_data((u8 *)&Ths.disk.buff[Ths.disk.buffidx++], Ths.disk.send_hole_cnt, CAN_ADAPTER_ID, 0);

		if(Ths.disk.buffidx >= sizeof(Ths.disk.buff) / sizeof(Ths.disk.buff[0]))
		{
			Ths.disk.buffidx = 0;
		}
	}
	if(Ths.disk.cur_cir_idx >= Ths.disk.need_cir_cnt)
		//如果当前圈数到达需要的采集圈数
	{
		Ths.is_start = false;//关闭传感器采集
		close_tim();////关闭定时器
		close_exti();////关闭外部中断
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);//采值完成后 关闭指示灯
		printf("cbli over\r\n");
	}
}

static inline void mc800_light_statemachine(void)//在定标完成后 从电机中断或 定时器中断 回调函数进入
{
    switch(Ths.disk.state)//根据 盘 的状态 进行操作
    {
        case STATE_ZERO_TRIG:////如果是零位触发状态进入
        {
			close_exti();
			Ths.disk.node_n=0;
			OPEN_IIMEx(TIM2,collection_table.tick_ele[Ths.disk.node_n].ticks);
			Ths.disk.read_idx=0;
			Ths.disk.state=STATE_READ;
			break;
        }
        case STATE_READ://如果是STATE_READ状态进入
        {
		#if defined(CAL_SINGLE)
			TIM2->ARR = light_Table[Ths.disk.node_n++].encode_value;
			cal_task();
		#else
			if(Ths.disk.node_n >= collection_table.cnt)
			{
				close_tim();
			}else{
				//设置下一个定时器中断时间
				TIM2->ARR = collection_table.tick_ele[(Ths.disk.node_n+1)%collection_table.cnt].ticks;
				//采值
				collection_tick_ele_t* tick = &collection_table.tick_ele[Ths.disk.node_n];
				for(int i = 0; i < tick->cnt; i++)
				{
					cal_task(&tick->light_ele[i]);
				}
				Ths.disk.node_n++;
			}
		#endif
			break;
        }
    }
}

u32 check_flg=28;
double offangle = 0.0f;
unsigned char motor_type=0x2;
unsigned int cal_totaltime = 0;

static void CreateProcessLinkList(unsigned int a,f32 time_per_degree)
{
	DriverInfo *Info;
// 长爪中轴线与缺口中轴线偏差0.34度，因此需要矫正
// 当盘片逆时针旋转时，左加右减；反之，左减右加
// 补偿角度为负表示起始角度减小(时间表总时间也减小)，反之增加

	//check_flg=28;
	//printf("flg = %d\r\n",check_flg);
	if(check_flg == 24) // 24孔(镜像和非镜像)
	{
		// 缺口在两孔中间：左加右减
		const DriverInfo Driver={check_flg,18.95f, 18.95f+37.90f, 18.95f+37.90f+18.95f, 7.161f, 0.0f, 15.0f, 7.839f, 24, 0}; // WZC
		Info = (DriverInfo *)&Driver;
	}
	else if(check_flg == 30) // 30孔(全血四项)
	{
		// 缺口在两孔中间：左加右减�
		const DriverInfo Driver={check_flg,18.95f, 18.95f+37.90f, 18.95f+37.90f+18.95f, 13.279f, 22.105f, 9.474f, 8.826f, 30, 5}; // WZC
		Info = (DriverInfo *)&Driver;
	}
	else if(check_flg == 361) // 通用36(孔无扇区)
	{
		// 缺口在孔的正上方：左加右减(该孔作为最后一个孔)
		const DriverInfo Driver={check_flg,18.95f, 18.95f+37.90f, 18.95f+37.90f+18.95f, 10.0f, 0.0f, 10.0f, 0.0f, 36, 0}; // WZC
		Info = (DriverInfo *)&Driver;
	}
	else if(check_flg == 362)// 36孔(全血五项，三个扇区)
	{
		const DriverInfo Driver={check_flg,18.95f, 18.95f+37.90f, 18.95f+37.90f+18.95f, 8.02f, 15.79f, 9.474f, 7.772f, 36, 12}; // WZC
		Info = (DriverInfo *)&Driver;
	}
    else if(check_flg == 32)// 32孔(APTT纠正，四个扇区)
    {
		offangle += 0.34f+0.24f;
		const DriverInfo Driver={check_flg,18.95f, 18.95f+37.90f, 18.95f+37.90f+18.95f, 7.38f, 15.0f, 10.0f, 7.62f, 32, 8}; // WZC
		Info = (DriverInfo *)&Driver;
    }
    else if(check_flg == 28)
    {//APTT2.0
		const DriverInfo Driver={
			.type=check_flg,
			.m_n2y=18.947f,
			.m_n3y=18.947f+37.894f,//1-3
			.m_n4y=18.947f+37.894f+18.947f,//1-4
			.m_z2s = 9.5f,//
			.m_l2l= 18.5f,//
			.m_s2s = 9.0f,//
			.m_l2s = 9.0f,//
			.m_max_hole= 28,//
			.m_sector_hole= 7,//
		};
		Info = (DriverInfo *)&Driver;
    }
    else if(check_flg == 281)//7
    {
		const DriverInfo Driver={
			.type=check_flg,
			.m_n2y=18.947f,
			.m_n3y=18.947f+37.894f,//1-3
			.m_n4y=18.947f+37.894f+18.947f,//1-4
			.m_z2s = 6.671f,//
			.m_l2l= 18.1f,//
			.m_s2s = 12.8f,//
			.m_l2s = 11.429,//
			.m_max_hole= 28,//
			.m_sector_hole= 7,//
		};
		Info = (DriverInfo *)&Driver;
    }

	else if(check_flg == 282)//7
    {
		const DriverInfo Driver={
			.type=check_flg,
			.m_n2y=18.947f,
			.m_n3y=18.947f+37.894f,//1-3
			.m_n4y=18.947f+37.894f+18.947f,//1-4
			.m_z2s = 6.436f,//
			.m_l2l= 12.857f,//
			.m_s2s = 12.857,//
			.m_l2s = 6.436,//
			.m_max_hole= 28,//
			.m_sector_hole= 7,//
		};
		Info = (DriverInfo *)&Driver;
    }
	Info->m_z2s += offangle;
	Info->m_l2s -= offangle;
    cal_totaltime = off_time(a,time_per_degree,Info);
    Ths.disk.hold_max = Info->m_max_hole;
	Ths.disk.pre_hole_cnt = LIGHT_COUNT * Info->m_max_hole;
    Ths.disk.zero2shole = light_Table[0].ticks;
    Ths.disk.start_idx=light_Table[0].hole + 1;
	
	//printf("hold_max=%d, pre_hole_cnt=%d, zero2shole=%d, start_idx=%d, offangle=%f\r\n",\
	Ths.disk.hold_max, Ths.disk.pre_hole_cnt, Ths.disk.zero2shole, Ths.disk.start_idx, offangle);
	
	isTimeStart=true;//开启时钟
	mix_first_minute_t=0;
    //发送数据长度 数据上传格式  4字节cmd + 4字节长度 + 2字节当前圈数 + 数据位字节   每个数据2个字节
    Ths.disk.send_hole_cnt= 4 + 4 + 2 + Info->m_max_hole * LIGHT_COUNT * 2;
    memset(Ths.disk.buff, 0, sizeof(Ths.disk.buff));
	Ths.disk.buffidx = 0;
}
void Mc500Light_ReadContinuous(u32 detect_circle_cnt, u32 z2f,f32 time_per_degree , MC_AD_Ele_t* ele)
{
    close_exti();
    Ths.disk.need_cir_cnt = detect_circle_cnt;//设置采集的圈数
    Ths.disk.cur_cir_idx = 0;//当前圈数复位为0
    memmove(&Ths.rev_ele,ele,sizeof(MC_AD_Ele_t));//将参数块拷贝到 ths.rev_ele
		FreeLinkList();//先清空况传感器数据数据表
		CreateProcessLinkList(z2f,time_per_degree);//根据定标值重新生成一个传感器数据表
		open_exti();//打开外部中断
		Ths.is_start = true;//置位
}

void mc800_light_init(void)//mc800设备光传感器初始化函数
{
	close_tim();
	close_exti();
	//关外部中断和定时器
	
	bus_write_init(); // 初始化数据发送模块 ？
	bus_os_create_sem(&AD_Detect_Thread_Cfg.sem,0);//生成 AD采集信号标记
	bus_create_thread(&AD_Detect_Thread_Cfg.tcb,AD_Detect_Thread,AD_Detect_Thread_Cfg.stack,AD_DETECT_THREAD_STACK_SIZE,AD_Detect_Thread_Prio,NULL);
	//生成AD采集任务线程
}

static u16 DiskInstalledDetect_crt;//检测是否安装盘的 次数
static u16 DiskInstalledDetect_span_ms;//检测是否安装盘的 检测间隔时间
static u16 DiskInstalledDetect_m3_channel;//检测是否安装盘的 传感器通道


void DiskInstalledReadWork(u16 crt,u16 span_ms,u16 channel,MC_AD_Ele_t* ele)
{
	 close_exti();
	 close_tim();//关闭定时器
	 memmove(&Ths.rev_ele,ele,sizeof(MC_AD_Ele_t));//将参数块拷贝到 ths.rev_ele
	
	 DiskInstalledDetect_crt=crt;
	 DiskInstalledDetect_span_ms=span_ms;
	 DiskInstalledDetect_m3_channel=channel;
	 
	 DiskInstalledDetect_span_ms=15;//采集间隔时间 不受上位机控制，固定为15ms
	
		if(DiskInstalledDetect_crt<DiskInstallDetectPeriod)
		{
			DiskInstalledDetect_crt=DiskInstallDetectPeriod;//最少检测DiskInstallDetectPeriod次
		}
		else if(DiskInstalledDetect_crt>DiskInstallDetectPeriod)//检测数最好是DiskInstallDetectPeriod的倍数
		{
			DiskInstalledDetect_crt += (DiskInstallDetectPeriod-(DiskInstalledDetect_crt%DiskInstallDetectPeriod));
		}
	 bus_os_post_sem(&AD_Detect_Thread_Cfg.sem);//释放AD采集线程信号
	
}


int cmp_t(const void* p1, const void* p2) 
{
	return(*(u16*)p1 - *(u16*)p2);
}

	
void AD_Detect_Thread(void *data)
{
	while(1)
	{
		bus_os_pend_sem(&AD_Detect_Thread_Cfg.sem,0);
		//等待信号标记
		switch(Ths.rev_ele.cmd)//根据cmd命令进行操作 	//！Ths.rev_ele.cmd使用Mc500Light_ReadContinuous函数进行传递
		
		{
			case CMD_ADDataRead ://读数据命令
			{
				ADDataRead_Send* send = (ADDataRead_Send * )Ths.rev_ele.send;
				ADDataRead_Return* rt = New_ADDataRead_Return();

				//等待执行完毕
				rt->m1_val_Length = 4;//设定数据长度为4
				rt->m1_val = malloc(rt->m1_val_Length*sizeof(u16));//申请对应数据长度的内存
				rt->m1_val[0]=0;//初始化
				rt->m1_val[1]=0;

				rt->m1_val[0]=mix_first_minute_t;
				rt->m1_val[1]=Ths.disk.start_idx;//记录盘 启动索引值
				//TODO::489000
				WriteU32((u8*)&rt->m1_val[2], 489000); // wangzc
				printf("mix_first_minute_t=%d\r\n", mix_first_minute_t);
				BytesBufferNode * node =  rt->ToBytes(rt);//将rt的数据转换BytesBufferNode
				u8 *data_ = NULL;
				int len = BytesBufferNode_Destroy(node,&data_);//把数据拷贝出来，并销毁原来的数据
				u32 code = bus_ita_return(Ths.rev_ele.adapterId,Ths.rev_ele.srcId,Ths.rev_ele.sessionId,len,data_);
				//上传数据 并阻塞等待应答 
				
				//一次任务完成后 即刻释放内存
				free(data_);
				free(rt->m1_val);
				Delete_ADDataRead_Send(send);
				Delete_ADDataRead_Return(rt);
				break;
			}
			case CMD_DISKInstalledRed://检测是否有 盘
			{
				DiskInstalledRed_Send* send = (DiskInstalledRed_Send * )Ths.rev_ele.send;
				DiskInstalledRed_Return* rt = New_DiskInstalledRed_Return();
				
				u16 i=0;
				u16 t=0;
				u16 tempmax=0;
				u16 tempmin=65535;
				
				u16 tempdata[DiskInstallDetectPeriod];
				for(i=0,t=0;i<DiskInstalledDetect_crt;i++)
				{
					tempdata[t]=singelRead((LIGHT_E)DiskInstalledDetect_m3_channel);//进行一次采样
					if(++t==DiskInstallDetectPeriod)
					{
						//排序 升序
						qsort(tempdata, DiskInstallDetectPeriod, sizeof(tempdata[0]),cmp_t);
						if(tempmax<tempdata[DiskInstallDetectPeriod-2])
						{
							tempmax=tempdata[DiskInstallDetectPeriod-2];
						}
						if(tempmin>tempdata[2])
						{
							tempmin=tempdata[2];
						}
						t=0;
					}
					
					OSTimeDly(DiskInstalledDetect_span_ms);
				}
				
				rt->m1_errCode=0x0000;//如果有错误可以在此设置错误返回代码
				rt->m2_maxval=tempmax;
				rt->m3_minval=tempmin;
				printf("max=%d;min=%d\r\n", tempmax,tempmin);
				
				BytesBufferNode * node =  rt->ToBytes(rt);//将rt的数据转换BytesBufferNode
				u8 *data_ = NULL;
				int len = BytesBufferNode_Destroy(node,&data_);//把数据拷贝出来，并销毁原来的数据
				u32 code = bus_ita_return(Ths.rev_ele.adapterId,Ths.rev_ele.srcId,Ths.rev_ele.sessionId,len,data_);
				
				free(data_);
				Delete_DiskInstalledRed_Send(send);
				Delete_DiskInstalledRed_Return(rt);
				break;
			}
		}
		Ths.rev_ele.send = NULL;
	}
}

