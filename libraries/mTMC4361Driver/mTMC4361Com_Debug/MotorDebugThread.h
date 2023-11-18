#ifndef __MOTROTREAD__H__
#define __MOTROTREAD__H__

#include "ucos_ii.h"
#include "stm32f4xx.h"
#include "os_implement.h"

#define MOTORTHRAD_QUEUE_SIZE  5
#define DEBUG_THREAD_STACK_SIZE 512
#define DEBUG_THREAD_PRIO             50

typedef enum uMOTORCMD
{
	Rotator=1,//旋转
	Stop,//停止
	MoveTO,//绝对位移
	MoveBY,//相对位移
	GetState=5,//获取状态
	GetPostion,//获取位置
	SetParameter,//设置参数
	ReadWrieteRegister,//读寄存器
	Oscillation=9,//振荡试 
	ResetMotor=10,//电机复位
	SetCurrent=11,//
	SetMicoStep=12,//
	SystemReset=13,
	OverZero=14,
	SetDefaultRegister=15,
}MOTORCMD;

typedef struct sMotorThread
{
	  OS_EVENT *Queue;
	  void  *QueueTbl[MOTORTHRAD_QUEUE_SIZE];
		bus_os_thread               tcb;
	  bus_os_sem                  sem;
	  u32                               stack[DEBUG_THREAD_STACK_SIZE]; 	
	  MOTORCMD  cmd;
}MOTOR_DEBUG_THRAD;

void MotorDebug_ThreadMain(void *p_arg);
void MOTOR_THRAD_DEBUG_CREATE(void);

#endif


