#ifndef _SERVOMOTOR_H_
#define _SERVOMOTOR_H_
#include "cmsis_os2.h"  
#include  "TMC4361.h"//TMC4361驱动
#include "bus.h"
#include "servoMotor_CFG.h"
#include "mTMC5130Device.h"

//配置宏函数
#if 1

#define SetServoMotorExEvent(e) *exflag|=(0x01<<e)
#define SetServoMotorInEvent() *inflag=1


#define Add_Servomotor_Flag(x); \
volatile static bool servomotor_##x##_ZeroFlg=false;\
volatile static bool servomotor_##x##_TargetFlg=false;\
volatile static bool servomotor_##x##_EventFlg=false;

#define Touch_Servomotor_Target(x); \
TMC4361_Target_Flush(&tmc4361List[x]);\
servomotor_##x##_TargetFlg=true;\

#define Touch_Servomotor_Event(x); \
servomotor_##x##_EventFlg=true;\


#define Touch_Servomotor_Zero(x); \
servomotor_##x##_ZeroFlg=true;\



#define Update_Servomotor_Target(x); \
if(servomotor_##x##_TargetFlg==true)    \
{									\
	servomotor_##x##_TargetFlg=false; \
	SetServoMotorExEvent(0);	\
}


#define Update_Servomotor_Event(x); \
if(servomotor_##x##_EventFlg==true)    \
{									\
	servomotor_##x##_EventFlg=false; \
	SetServoMotorInEvent();\
}


#define Update_Servomotor_Zero(x); \
if(servomotor_##x##_ZeroFlg==true)    \
{									\
	servomotor_##x##_ZeroFlg=false; \
	SetServoMotorExEvent(1);\
}

#define Update_Servomotor_UserEvent(event,eid); \
if(event) \
{\
	SetServoMotorExEvent(eid);\
}


#endif

#define SERVOMOTOR_ENABLE 1

#define EMERGENCY_STOP_ACC 5000
/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
#define ServoMotor_ERR_NONE                 0u

// 1-99 单独接口专项错误

// 32-63 电机事件错误
#define ServoMotor_DRIVE_ENC_VLE0 			32+26
#define ServoMotor_DRIVE_CL_MAX 			32+27
#define ServoMotor_DRIVE_CL_FIT 			32+28
#define ServoMotor_HINDER		 			32+31
//100-200 通用错误
#define ServoMotor_ERR_INITERR              101u
#define ServoMotor_ERR_PARAMETER            102u
#define ServoMotor_ERR_MEMFULL              103u
#define ServoMotor_ERR_EMPTY                104u

#define ServoMotor_ERR_TIMEOUT              100u

#define ServoMotor_TERMINATE 				200				



//201-255 特殊错误


//队列里数组的最大字节
#define MAX_SMoto_q_t_size 256
//接收的待做任务队列数据结构
typedef struct sMotor_task_q_t 
{
	//记录这些信息 方便执行完成后进行回复
	uint8_t     adapterId;
	uint8_t 	srcId;
	uint8_t		sessionId;
	
	uint32_t funCode;//功能id
	
	uint8_t parameterMem[MAX_SMoto_q_t_size];
	
	//时间管理 仅供内部超时判断使用 
	int32_t timeOut;
	//动作步骤管理； 有些动作可能需要很多步骤 也仅供内部使用
	uint16_t step;
	
#if SERVOMOTOR_EXTEND == 1	
	uint8_t tmc5130MotorCount;
#endif

}SMotor_task_q_t,*SMotor_task_q_t_p;


#define MAX_SMotor_return_size 32

typedef struct sMotor_return
{
	uint8_t returnSize;//返回总字节的大小
	uint8_t returnMem[MAX_SMotor_return_size];
}SMotor_return;

typedef enum MotorDeviceType
{
	TMC4361,
	TMC5240
}MotorDeviceType;

typedef struct sMotorCfg
{
	MotorDeviceType motorDeviceType;//电机类别
	bool restEventPol;//复位触发电平

	bool defaultDir;//电机默认方向 也是电机复位方向 
	float step_ratio;//1mm对应的整步数
	
	float max_distance;//该电机允许的反向最大行程 mm 为0 不开启此功能
	float max_distance_reverse;//该电机允许的正向最大的行程 mm 为0 不开启此功能
	
	float zero_distance;//复位后还需要 移动的距离 
	
	uint32_t  toleranceDeviation;//可容忍的偏差 为0 不开启此功能
}SMotorCfg;

//事件定义
typedef struct sMotorEvent
{
	/*
	要区分内部事件和外部事件 
	外部事件可以直接获取 而内部事件需要通过读取寄存器来获取（不能在中断里读取寄存器）
	*/
	volatile uint32_t externalEvent;//和驱动器无关的外部事件，诸如零位开关触发 定标触发 探液触发
	volatile uint32_t internalEvent;//和驱动器有直接关系的事件，电机位置到达事件 偏差和换向角的事件

	volatile uint32_t relEvents_inter_Flag;//用来标志 需要实时进行反馈操作的内部事件
	volatile uint32_t relEvents_exter_Flag;//用来标志 需要实时进行反馈操作的外部事件
	
	//最近一次事件触发时 电机所处的位置
	volatile int32_t relEventPosition;	
	
	//需要用户外部实现的事件扫描函数
	void  (*scanEvent)(uint32_t * exflag,uint8_t * inter);
	
	
	uint8_t relOper;//用来区分 一个事件触发后 需要立刻操作的步骤
}SMotorEvent;


typedef struct sMotor
{
	volatile SMotorEvent event;//事件监听组
	
	const SMotorCfg *cfg;//伺服电机的配置
	
	TMC4361TypeDef *drive;//驱动
	
	//外部实现 得到零位开关状态
	bool (*sMotorGetRestSwitchState)(void);
	
}SMotor;

typedef enum sMotorRunState
{
	SMotorRunState_timeOut,
	SMotorRunState_Runing,
	SMotorRunState_Error,
	SMotorRunState_Complete,
	SMotorRunState_CompleteAndNotRun
}SMotorRunState;

typedef struct sMotorExtend
{
#if SERVOMOTOR_EXTEND == 1	
	uint8_t tmc5130MotorCount;
	MOTOR *tmc5130MotorList;
#endif
}SMotorExtend;

typedef enum sMotorState
{
	SMotorState_Free=0,
	SMotorState_Busy,
	
}sMotorState;
//静态类 
//这里可以把伺服电机任务理解为一个静态类 其实所有任务都可以说是一个静态类 
typedef struct sMotorTask
{
	uint8_t sMotorCount;
	SMotor *sMotorList;
	
	uint16_t sMotorMonitorStep;
	
	SMotorExtend extend;//扩展功能
	
	osSemaphoreId_t spiMutex;//应该用互斥量的 但是目前使用信号量来模拟
	
	sMotorState state;
	SMotor_return lastResult;
}SMotorTask;

extern volatile SMotorTask SMotorTask_Static;

void ServoMotorEventSet_external(uint8_t motorId,uint32_t flag);
void ServoMotorEventSet_internal(uint8_t motorId,uint32_t flag);

void ServoMotorParse(uint32_t func_code,bus_packet* packet);
uint8_t ServoMotorTaskInit(void);


uint8_t ServoMotorParseInter(uint32_t func_code,uint8_t* data,uint8_t datatLen,int32_t *timeout);
uint8_t ServoMotorWait(uint32_t timeout);

#endif