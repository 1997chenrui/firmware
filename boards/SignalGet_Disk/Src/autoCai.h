#ifndef AUTO_CAI_H_
#define AUTO_CAI_H_
#include <stdint.h>
#include <stdbool.h>
#include <os_implement.h>
#include "signal_det.h"
#include "disk_ctrl_base.h"

#define TRIGGER_THRESHOLD_HIGHT      15000         //加个软件施密特的功能
#define TRIGGER_THRESHOLD_LOW        15000         
#define C_EVNET_CNT                     8           //定标先采8圈 粗定标
#define D_EVNET_CNT		                	10			//定标先扫描一次 精定标

#define CALI_START			            		0
#define C_SIGNAL_STATE_HIGHT            1
#define C_SIGNAL_STATE_LOW              2
#define D_CALI_STATE_LOW                3
#define D_CALI_CHECK_SKIP 	            4
#define D_CALI_CHECK 	                	5

#define  SIGNAL_RISE_EDGE           0
#define  SIGNAL_FALL_EDGE           1

typedef enum
{
    IDLE_CALI = 0,      //空闲
    C_CALI,             //粗定标
    D_CALI              //精定标
}CaliType_e;

typedef struct
{
    uint8_t     edgeKind;       //边缘种类   降沿or升沿
    uint32_t    timeSpan;       //降沿or升沿触发的时候经过多少时间片段 
	uint16_t    data;			//当前ad值
}AutoCaliEvent_t;
//单个扫描
typedef struct
{
	int			 offsetus_once;         //us每步
	int			 edge_width_us;         // 棱的宽度(us)
	int			 hole_to_edge_center;	//第一个孔 到 棱中心的时间（此时棱中心与零位重合）  机械配置
}AutoCaliCfg;

//粗定标结构
typedef struct
{
	u32		        c_zero_to_edge_center;		//粗定标 零位到棱中心
    AutoCaliEvent_t evens[C_EVNET_CNT];
}CCali_Struct;
//精定标结构
typedef struct
{
	AutoCaliEvent_t events[D_EVNET_CNT];
	uint8_t  is_tri;
    int  	 dyn_zero_to_edge_center;			//动态偏移的f2rc,扫描时用
	int	 	 dny_us_offset;	                //动态调整的时间偏移
	int      d_zero_to_edge_center;				//精定标 零位到棱中心宽度(us)
}DCali_Struct;

typedef struct
{
	CCali_Struct	ccali;
	DCali_Struct	dcali;
	AutoCaliCfg  	cfg;	
	uint16_t        data;
    uint16_t        event_cnt;
    uint32_t        timerTick;	//当前tick
    uint8_t         state;        // 施密特的状态
    CaliType_e      method;		//0:无 1:粗定标 2:精定标
    uint32_t        samplingPeriod;
    bus_os_sem      sem;	
    uint32_t        light_to_hole;     //定标完成后，光到第一个孔的时间
    uint32_t        err_code;
}AutoCali_t;

void AutoCali_Init(void);
void AutoCali_Now(void);

bool AutoCali_IsStart(void);
u32 AutoCali_GetZero2SHole(void);
void AutoCali_TimerHandle(void);
u32 DiskAuto_Start(void);
#endif


