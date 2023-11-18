#ifndef __A2F_H
#define __A2F_H

#include "hole2lightPKG.h"
#include "z2f.h"
//#define LIGHT_CNT    4



//#define   STATE_ZERO_TRIG      0
//#define   STATE_ZERO_TRIG_1    1

//#define   STATE_READ         5


#define AD_DETECT_THREAD_STACK_SIZE       512
#define AD_Detect_Thread_Prio             14

typedef struct
{
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void       *send;            //send指针
    u32         cmd;
}MC_AD_Ele_t;

static struct
{
    bus_os_thread               tcb;
    u32                         stack[AD_DETECT_THREAD_STACK_SIZE];
    bus_os_sem                  sem;
}AD_Detect_Thread_Cfg;



#define OPEN_IIMEx(x,time) \
do{\
	x->ARR =time;\
	x->CNT=0;\
	x->DIER |= TIM_IT_UPDATE;\
	x->CR1|=TIM_CR1_CEN;\
}while(0);\

enum eDisk
{
    eIde,
    eReady,
    eTiny,
    eBuff,
};

typedef struct a2f a2f;
struct a2f
{
    char disk_node;
    bool state;
    char cali_step;
    char disk_type;
    char disk_idx;
    z2f *disk;
    L4node_arr *light_data;
};

extern a2f     ths;
//void AD_Detect_Thread(void *data);

void a2f_Init(a2f *me,L4node_arr *lightdata,z2f *pz2f,char disktype);
void a2f_diskFSM(void);
void AD_Detect_Thread(void *data);


















#endif

