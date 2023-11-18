#include "autoCai.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "async.h"
#include "errCode.h"
#include <math.h>

extern light_ctrl_base_t 		lightCtrlBase;

AutoCali_t	cali = 
{
	.cfg.edge_width_us = 10320,
	.cfg.offsetus_once = 20,	//20us
	.cfg.hole_to_edge_center = 10416,
	.event_cnt = 0,
	.samplingPeriod = 100,	//100us
	.err_code = 0
};

#define CALI_CHECK_SKIP_US_LONG    ((cali.cfg.edge_width_us>>2)*3)	//棱宽度的四分之三
#define CALI_CHECK_SKIP_US_SHORT    (((cali.cfg.edge_width_us>>1)>>2)*3)	//棱宽度的一半的四分之三

//a=(a1+a2)/2
void get_zero_2_refer_center_time(AutoCali_t *ths)
{
	for(int i = 0;i<8;i++)
	{
		printf("%d\r\n",(cali.ccali.evens[i].timeSpan));
	}
	printf("\r\n");
	
	cali.ccali.c_zero_to_edge_center = ((cali.ccali.evens[0].timeSpan+cali.ccali.evens[1].timeSpan)>>1) * cali.samplingPeriod;	
}

void AutoCali_Init(void)
{
	u32 code = 0;
    cali.method = IDLE_CALI;
    code = bus_os_create_sem(&cali.sem,0);
	if(code != 0)
	{
		printf("pend error\r\n");
	}
}

//获取补偿时间
static u32 get_compen_time(AutoCali_t *ths)
{
	int left = 0;
	int right = 0;
	left = cali.dcali.events[0].timeSpan*cali.cfg.offsetus_once + CALI_CHECK_SKIP_US_SHORT;
	right = (cali.dcali.events[1].timeSpan - cali.dcali.events[0].timeSpan)*cali.cfg.offsetus_once + CALI_CHECK_SKIP_US_LONG - left;
    
	cali.dcali.d_zero_to_edge_center = cali.ccali.c_zero_to_edge_center +((right - left)>>1);
	cali.ccali.c_zero_to_edge_center = cali.dcali.d_zero_to_edge_center;
	cali.dcali.dyn_zero_to_edge_center = cali.dcali.d_zero_to_edge_center;

	printf("etailed_cali_left=%d,etailed_cali_right=%d\r\n",left,right);
    printf(" cali.dcali.events[0].timeSpan=%d,cali.dcali.events[1].timeSpan=%d\r\n",cali.dcali.events[0].timeSpan,cali.dcali.events[1].timeSpan);
	return (cali.dcali.d_zero_to_edge_center + cali.cfg.hole_to_edge_center);
}

void AutoCali_Start(AutoCali_t *ths, CaliType_e method)
{
    singelRead(MPURPLE);
	cali.timerTick = 0;
    cali.method = method;
	if(method == C_CALI)
	{
		cali.dcali.d_zero_to_edge_center = 0;
		cali.ccali.c_zero_to_edge_center = 0;
		cali.dcali.dyn_zero_to_edge_center = 0;
		printf("now start c cali\r\n");
		cali.state = CALI_START;
    lightCtrlBase.set_timer(cali.samplingPeriod);
	}
	else if(method == D_CALI)
	{
		printf("now start d cali\r\n");
		cali.state = CALI_START;
	}
	lightCtrlBase.start_exit2();
    bus_os_pend_sem(&cali.sem,0);       //等待采样结束
	
    if(method == 1)
	{
		//开始计算粗定标点
 		for(u8 i = 0;i<C_EVNET_CNT;i++)
 		{		
 			printf("edgeKind=%d,timeSpan=%d,data=%d\r\n",cali.ccali.evens[i].edgeKind, cali.ccali.evens[i].timeSpan,cali.ccali.evens[i].data);	
 		}
		get_zero_2_refer_center_time(ths);
		cali.dcali.dyn_zero_to_edge_center = cali.ccali.c_zero_to_edge_center;
		printf("dyn_zero_to_edge_center=%d\r\n",cali.dcali.dyn_zero_to_edge_center);
    }
	else if(method == 2)
	{
 	    for(u8 i = 0;i<10;i++)
 	    {
 			printf("time_span1=%d,time_span2=%d\r\n",cali.dcali.events[0].timeSpan, cali.dcali.events[1].timeSpan);
 	    }
		cali.light_to_hole = get_compen_time(ths);
		printf("light_to_hole = %d\r\n",cali.light_to_hole);
	}
    cali.method = IDLE_CALI;
}
extern u32 check_flg;

u32 DiskAuto_Start(void)
{
	cali.err_code=0;
	AutoCali_Start(&cali,C_CALI);//粗定标
    if(cali.err_code == 0)
    {      
        AutoCali_Start(&cali,D_CALI); //精定标
    }
    return cali.err_code;
}

void AutoCali_Now(void)
{
    if(cali.method == C_CALI)
    {//粗定标
        lightCtrlBase.start_timer();
    }
    else if(cali.method == D_CALI)
    {//精定标
        cali.dcali.dyn_zero_to_edge_center += cali.dcali.dny_us_offset;
        lightCtrlBase.set_timer(cali.dcali.dyn_zero_to_edge_center);
        lightCtrlBase.start_timer();
    }   
}

bool AutoCali_IsStart(void)
{
    return (cali.method != IDLE_CALI);
}

u32 AutoCali_GetZero2SHole(void)
{
    return cali.light_to_hole;
}

static void cali_over(void)
{
    lightCtrlBase.stop_exit2();
    lightCtrlBase.stop_timer();
    bus_os_post_sem(&cali.sem);
}

//开始都是高
//反转后要记录当前时间tick
void AutoCali_TimerHandle(void)
{
    cali.timerTick ++;
    switch(cali.state)
    {		
		case CALI_START://先拿到前两个点的初始值
		{
			cali.data = singelRead(MPURPLE);
			if(cali.data < TRIGGER_THRESHOLD_LOW)
			{
                if(cali.method == C_CALI)
                {
                    Async_Excute(1,printf,"e\r\n");
                    cali.err_code = ERR_CALIBRATE_ERR;
                    cali_over();
                }
                else if(cali.method == D_CALI)
                {
                    cali.state = D_CALI_STATE_LOW;
                    cali.dcali.dny_us_offset = -CALI_CHECK_SKIP_US_SHORT;
                }
			}
			else if(cali.data > TRIGGER_THRESHOLD_HIGHT)
			{
                if(cali.method == C_CALI)
                {
                     cali.state = C_SIGNAL_STATE_HIGHT;
                }
                else
                {
                    Async_Excute(1,printf,"e\r\n");
                    cali.err_code = ERR_CALIBRATE_ERR;
                    cali_over();
                }
			}
			lightCtrlBase.stop_timer();
			break;
		}
        case C_SIGNAL_STATE_HIGHT :
        {
			cali.data = singelRead(MPURPLE);
			if(cali.data < TRIGGER_THRESHOLD_LOW)
			{
				//有一个下降沿
				AutoCaliEvent_t event;
				event.edgeKind = SIGNAL_FALL_EDGE;
				event.timeSpan = cali.timerTick;
				event.data = cali.data;
				cali.state = C_SIGNAL_STATE_LOW;//更新状态
				cali.ccali.evens[cali.event_cnt++] = event;   // 添加一个事件
			}		
        }break;
        case C_SIGNAL_STATE_LOW :
        {
			cali.data = singelRead(MPURPLE);
			if(cali.data > TRIGGER_THRESHOLD_HIGHT )
			{
				//有一个上升沿
				AutoCaliEvent_t event;
				event.edgeKind = SIGNAL_RISE_EDGE;
				event.timeSpan = cali.timerTick;
				event.data = cali.data;
				cali.state = C_SIGNAL_STATE_HIGHT;//更新状态
				cali.ccali.evens[cali.event_cnt++] = event;   // 添加一个事件

				if(cali.event_cnt >= C_EVNET_CNT)
				{
					//粗定标完成
					cali.timerTick = 0;
					cali.event_cnt = 0;
					cali_over();
				}
			}            
        }break;
		case D_CALI_STATE_LOW:
		{
			 cali.data = singelRead(MPURPLE); 
			 if(cali.data > TRIGGER_THRESHOLD_HIGHT)
			 {
			 	AutoCaliEvent_t event;
				event.edgeKind = SIGNAL_RISE_EDGE;
				event.data = cali.data;
				event.timeSpan = cali.timerTick;
				cali.dcali.events[cali.event_cnt++] = event;
				cali.state = D_CALI_CHECK_SKIP;
			 }
			 else
			 {
				 cali.dcali.dny_us_offset = -abs(cali.cfg.offsetus_once);
			 }
			 lightCtrlBase.stop_timer();
		}break;
		case D_CALI_CHECK_SKIP:
		{
			cali.dcali.dny_us_offset = CALI_CHECK_SKIP_US_LONG;
			cali.state = D_CALI_CHECK;
			lightCtrlBase.stop_timer();
		}break;
		case D_CALI_CHECK:
		{
			cali.data = singelRead(MPURPLE);

			if(cali.data > TRIGGER_THRESHOLD_HIGHT)
			{
				AutoCaliEvent_t event;
				event.edgeKind = SIGNAL_RISE_EDGE;
				event.data = cali.data;
				event.timeSpan = cali.timerTick;
				cali.dcali.events[cali.event_cnt++] = event;

				{
				cali.timerTick = 0;
				cali.event_cnt = 0;
				cali_over();
				}
			}
			else
			{
				cali.dcali.dny_us_offset = abs(cali.cfg.offsetus_once);
			}
			lightCtrlBase.stop_timer();
		}break;
    }
}

