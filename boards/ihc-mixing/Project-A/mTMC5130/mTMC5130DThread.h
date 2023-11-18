#ifndef MOTOR_THREAD_H_
#define MOTOR_THREAD_H_
#include "bytesBuffer.h"
#include "data_struct.h"
#include "stm32f4xx_hal.h"

#define     MOTOR_CNT             2  //总电机数目
#define MOTOR_THEARD_CNT    MOTOR_CNT
#define MOTOR_THREAD_STACK_SIZE         512

#define CE_ASSERT( exp ) ce_assert((exp),__LINE__)

#define Motor0Thread_Prio                6
#define Motor1Thread_Prio                7


#define  MOTOR_THRAD_CREATE(idx)        \
bus_create_thread(&MotorThread_OS[idx].tcb,Motor_Thread,         \
				MotorThread_OS[idx].stack, MOTOR_THREAD_STACK_SIZE, (6+idx),NULL);\
bus_os_create_sem(&MotorThread_OS[idx].sem,0);            \
SeqQueue_Init(&queue[idx],sizeof(Motor_Queue_Ele_t),&queue##idx##_pend,&queue##idx##_post)

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
    u32         cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void        *send;            //send指针
}Motor_Queue_Ele_t;

typedef struct
{
	bus_os_thread               tcb;
	bus_os_sem                  sem;
	u32                         stack[MOTOR_THREAD_STACK_SIZE]; 
}MotorThread;

void  Motor_Thread(void *arg);
void MotorThread_EnQueue(int idx,Motor_Queue_Ele_t *data);
void Motors_Cfg_Init(void);
void MotorInit(void);
#endif

