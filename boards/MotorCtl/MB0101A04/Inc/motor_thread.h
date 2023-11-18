#ifndef MOTOR_THREAD_H_
#define MOTOR_THREAD_H_
#include "bytesBuffer.h"
#include "data_struct.h"
#include "stm32f4xx_hal.h"

#define MOTOR_THREAD_STACK_SIZE         256

#define CE_ASSERT( exp ) ce_assert((exp),__LINE__)
//prio 6,7,8,9
#define  MOTOR_THRAD_CREATE(prio, idx)        \
bus_create_thread(&MotorThread_OS[idx].tcb,Motor_Thread,         \
				MotorThread_OS[idx].stack, MOTOR_THREAD_STACK_SIZE, prio, NULL);\
bus_os_create_sem(&MotorThread_OS[idx].sem,0);            \
SeqQueue_Init(&queue[idx],sizeof(Motor_queue_ele_t),&queue##idx##_pend,&queue##idx##_post)

#define CREATE_Q_BLOCK(idx)                                            \
static bool queue##idx##_pend()            \
{                                        \
	bus_os_pend_sem(&MotorThread_OS[idx].sem,0);    \
	return true;\
}                                        \
static bool queue##idx##_post()         \
{                                        \
	bus_os_post_sem(&MotorThread_OS[idx].sem);    \
	return true;                        \
}


typedef struct
{
	bus_os_thread               tcb;
	bus_os_sem                  sem;
	u32                         stack[MOTOR_THREAD_STACK_SIZE]; 
}MotorThread;

typedef struct{
    //²½Êý±ÈÀý
    float step_ratio;
    float zero;
    float max;
    float lost_stop;
    float lost_continue;
    GPIO_TypeDef * cs_port;
    u16 cs_pin;
    GPIO_TypeDef * en_port;
    u16 en_pin;
    GPIO_TypeDef * em_port;
    u16 em_pin;
    GPIO_TypeDef * rl_port;
    u16 rl_pin;
    GPIO_TypeDef * rr_port;
    u16 rr_pin;
    GPIO_TypeDef * zero_port;
    u16 zero_pin;
    GPIO_TypeDef * swp_port;
    u16 swp_pin;
    GPIO_TypeDef * swn_port;
    u16 swn_pin;
}Motor_cfg_extern_t;

typedef struct{
    u32         cmd_code;
    bus_packet  packet_head;
    void * p_cmd;
}Motor_queue_ele_t;

void motor_cmd_run(u32 itf_code, bus_packet* packet);
void Motor_Thread(void *arg);
#endif

