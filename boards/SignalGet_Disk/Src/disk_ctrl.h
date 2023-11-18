#ifndef      MC500_LIGHT_H_
#define      MC500_LIGHT_H_


#include "stm32f4xx_hal.h"
#include "bus.h"
#include "disk_ctrl_base.h"
#include "cal.h"


//#define MAX_BUFF 70

#define   STATE_ZERO_TRIG      0
#define   STATE_ZERO_TRIG_1    1

#define   STATE_READ         5


#define AD_DETECT_THREAD_STACK_SIZE       512
#define AD_Detect_Thread_Prio             14


typedef struct
{
    u32         cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void       *send;            //send指针

}MC_AD_Ele_t;


//--------------------------结构声明--------------------------------------

static struct
{
    bus_os_thread               tcb;
    u32                         stack[AD_DETECT_THREAD_STACK_SIZE];
    bus_os_sem                  sem;
}AD_Detect_Thread_Cfg;


typedef struct
{
    int     state;
    int     zero2shole;     //零位距离到第一个样本孔的距离
    int     cur_cir_idx;    //当前圈数  
    int     need_cir_cnt;   //需要采样多少圈 
	unsigned int send_hole_cnt;
	
	DATA_LNode *StartLinkList;
	
	char    pre_hole_cnt;// 一圈有多少采值步聚
	char    hold_max;
	char    read_idx;	
	char    driver_type;
	char    node_n;
    char    start_idx;
	char    less;
	struct _Buff
	{
		u32   cmd;
		u32   len;
		u16   m1_data[LNODE_BUFF_MAX];	
		u16   m2_current_detect_circle;	//当前检测到第几圈
	}buff[2];
	
}Mc500Light_t;

typedef struct
{
    bool           is_start;
    MC_AD_Ele_t    rev_ele;
    Mc500Light_t   disk;
    u32 		   err_code;
}DiskCtrl_t;



void AD_Detect_Thread(void *data);
void adc_send_thread(void *arg);
static inline void Mc500Light_StateMachine(void);


void Mc500Light_Init(void);
void Mc500Light_Config(u16*amp,u8 fileterCnt,int lightOffset);
//连续采集
void Mc500Light_ReadContinuous(u32 detect_circle_cnt,u32 z2f,MC_AD_Ele_t* ele);

void Mc500Light_Loop(void);


#endif





