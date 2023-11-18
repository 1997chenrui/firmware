#ifndef HANDER_H
#define HANDER_H

#include "bus.h"

#define  			HANDER_CFG_TASK_START_STK_SIZE        512u

typedef enum
{
	eHANDER_NONE,
	eHANDER_RESET,
}eHanderOptType_t;

typedef struct
{
    u32         cmd_code;
    bus_packet packet_head;
    void        *p_cmd;            //send指针
}Hander_Queue_Ele_t;

typedef struct
{
	bus_os_thread               tcb;
	bus_os_sem                  sem;
	u32                         stack[HANDER_CFG_TASK_START_STK_SIZE]; 
}Hander_thread;


// 抓手动作接口
unsigned int hander_opt(unsigned char state);
void hander_cmd_run(u32 func_code  ,  bus_packet * packet );
void hander_init(void);

#endif

