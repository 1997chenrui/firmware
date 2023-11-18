#ifndef __CLIENT_H
#define __CLIENT_H
#include "auto_caliPKG.h"
#include "auto_cali_encoder.h"
#include "auto_cali_it.h"
#include "cal.h"
#define COVER        15000
#define C_EVNET_IT_CNT      8
#define OFF_T           3
#define ERR_CALIBRATE_ERR_T   0x722 			//光值小于设定值(15000)
#define ALL_ENCODER_NUM       32768

#define   STATE_ZERO_TRIG      0
#define   STATE_ZERO_TRIG_1    1

#define   STATE_READ         5


#define AD_DETECT_THREAD_STACK_SIZE       512
#define AD_Detect_Thread_Prio             14

#define DiskInstallDetectPeriod 20
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
	char    pre_hole_cnt;// 一圈有多少采值步聚
	char    hold_max;
	u8      read_idx;	
	char    driver_type;
	u8      node_n;
    char    start_idx;
	char    buffidx;
	struct _Buff
	{
		u32   cmd;
		u32   len;
		u16   m1_data[LNODE_BUFF_MAX +1];
	}buff[2];
}mc800light_t;

typedef struct
{
    u8             first_time;
    bool           is_start;
    u32 		   err_code;
    MC_AD_Ele_t    rev_ele;
    mc800light_t    disk;
}DiskCtrl_t;

typedef enum {eIDLE_CALI,eC_CALI,eD_CALI}ecali;
typedef enum 
{
    eCali_ready,
    eC_SIGNAL_STATE_HIGHT,
    eC_SIGNAL_STATE_LOW,
    eD_CALI_STATE_LOW,
    eD_CALI_CHECK,
}FSM_state;


typedef struct auto_client auto_client;
struct auto_client
{
    bool time_off_on;
    unsigned char method;//粗 细定标
    unsigned char state;//定标时候的FSM（状态机）
    unsigned char cnt_t;
    unsigned short dlt_time;
    uint32_t c_zero_to_edge_center;//粗定标零位到第一个光源
    uint32_t light_to_hole;
    uint32_t errcode;//返回上位机的错误码
    uint32_t samplingPeriod;//定标周期 100us
    uint32_t tick;//记录时间
    auto_encoder encoder;
    auto_encoder encoder_event[C_EVNET_IT_CNT];
    auto_it auto_it_t;
    bus_os_sem      sem;
};
void cali_client_Iint(auto_client *me);
unsigned int appcali_start(void);
unsigned int light2hole(void);
void AD_Detect_Thread(void *data);
static inline void Mc500Light_StateMachine(void);
extern auto_client auto_client_t;
void mc800_light_init(void);
void DiskInstalledReadWork(u16 crt,u16 span_ms,u16 channel,MC_AD_Ele_t* ele);
#endif

