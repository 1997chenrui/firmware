#include "servoMotor.h"

#include "stdio.h"


#include <string.h>
#include <math.h>

#include "port_0x00012005.h"
#include "port_0x00012006.h"
#include "port_0x00012010.h"
#include "port_0x00012011.h"
#include "port_0x00012100.h"
#include "port_0x00012101.h"
#include "port_0x00012120.h"
#include "port_0x00012122.h"
#include "port_0x00012400.h"
#include "port_0x00012150.h"
#include "port_0x00012153.h"
#include "port_0x00012160.h"
#include "port_0x00012161.h"
#include "port_0x00012200.h"
#include "port_0x00012210.h"

ADD_PORT_0X00012005();
ADD_PORT_0X00012006();
ADD_PORT_0X00012010();
ADD_PORT_0X00012011();
ADD_PORT_0X00012100();
ADD_PORT_0X00012101();
ADD_PORT_0X00012120();
ADD_PORT_0X00012122();
ADD_PORT_0X00012400();
ADD_PORT_0X00012150();
ADD_PORT_0X00012153();
ADD_PORT_0X00012160();
ADD_PORT_0X00012161();
ADD_PORT_0X00012200();
ADD_PORT_0X00012210();

#if SERVOMOTOR_EXTEND == 1
#include "mTMC5130Device.h"
#include "mTMC5130Driver.h"
#include "port_0x00012201.h"
#include "port_0x00012211.h"
ADD_PORT_0X00012201();
ADD_PORT_0X00012211();

extern int motor_step_calc(u8 idx, float value);
extern f32 motor_distance_calc(u8 idx, int value);

#endif

	
#define DEBUG_LOG_DEBUG(s,args...) 
#define DEBUG_LOG_INFO(s,args...)
#define DEBUG_LOG_WARN(s,args...)
#define DEBUG_LOG_ERROR(s,args...) printf(s,##args)


//osSemaphoreAcquire(SMotorTask_Static.spiMutex,osWaitForever);

//osSemaphoreRelease(SMotorTask_Static.spiMutex);

#define PutSPI();   osSemaphoreRelease(SMotorTask_Static.spiMutex);

#define GetSPI();   osSemaphoreAcquire(SMotorTask_Static.spiMutex,osWaitForever);

//这里放置了不耗时操作
#define NOTIMECONSUMING (\
task->funCode==0X00012005|| \
task->funCode==0x00012006|| \
task->funCode==0x00012010|| \
task->funCode==0x00012011|| \
task->funCode==0x00012150|| \
task->funCode==0x00012160|| \
task->funCode==0x00012161)

//对于任务来讲 简单理解为一个静态类 
volatile SMotorTask SMotorTask_Static;

//伺服电机任务需要用到的线程：
//任务监听线程
 static osThreadId_t servoMotorMonitorHandle;
//线程栈
static uint32_t servoMotorMonitor[512];
//线程信息
 const static osThreadAttr_t servoMotorMonitor_attributes = {
  .priority = (osPriority_t) osPriorityRealtime1,	
  .stack_size = sizeof(servoMotorMonitor),
  .stack_mem=&servoMotorMonitor[0],
  .name="servoMotorMonitor",
};
#if	SERVOMOTOR_REAL == 1
//事件扫描线程
 static osThreadId_t servoMotorEventScanHandle;
//线程栈
static uint32_t servoMotorEventScan[256];
//线程信息
 const static osThreadAttr_t servoMotorEventScan_attributes = {
  .priority = (osPriority_t) osPriorityHigh,	
  .stack_size = sizeof(servoMotorEventScan),
  .stack_mem=&servoMotorEventScan[0],
  .name="servoMotorEventScan",
};
#endif

//任务监听间隔事件 ms
#define SERVOMOTOR_SPANTIMEMS 20

//电机运行精度
#define SERVOMOTOR_PRECISION 0.1
//待做任务队列
#define MAXSMOTOR_TASK_LIST 4

static osMessageQueueId_t sMotor_task_q;   
static SMotor_task_q_t_p servoMotorReply_QMem[MAXSMOTOR_TASK_LIST];
const static osMessageQueueAttr_t sMotor_task_q_Attr =
{
	.mq_mem=&servoMotorReply_QMem[0],
	.mq_size=sizeof(SMotor_task_q_t_p)*MAXSMOTOR_TASK_LIST
};

//接收任务的内存池定义
#define MAXTASKPARSE 4
osMemoryPoolId_t taskParseMemory;
uint8_t taskParseMemorymem[MAXTASKPARSE][sizeof(SMotor_task_q_t)];
const static  osMemoryPoolAttr_t taskParseMemorymemAttr_t={
	.mp_mem=&taskParseMemorymem[0][0],
	.mp_size=sizeof(taskParseMemorymem)
};


//非耗时任务处理函数
static void NoTimeConsumingTaskHandle(SMotor_task_q_t_p task,SMotor_return *rt);
//耗时任务处理函数
static SMotorRunState TimeConsumingTaskHandle(SMotor_task_q_t_p task,SMotor_return* report);
//返回电机的监听状态函数
static SMotorRunState ServoMotorMonitorHandleFunc(SMotor_task_q_t_p task,SMotor_return* report);
//电机空闲时刻的事件监听函数
static void ServoMotorMonitorHandleFunc_FreeTime(void);
//估算一条指令的超时时间函数
static int32_t TaskTimeOutCalculate(SMotor_task_q_t_p task);

//距离（mm）转换为微步数（脉冲数量）
static int32_t DistanceToPulseCount_Conversion(uint8_t motorId ,float distance);
//微步数（脉冲数量）转换为距离（mm）
static float PulseCountToDistance_Conversion(uint8_t motorId ,int32_t pulse);

//
void Cheak_sMotor_id(uint8_t id);
/*
*********************************************************************************************************
*                                     解析任务线程相关
*********************************************************************************************************
*/ 
#include "RWByte.h"

void ServoMotorTest_Reset(uint8_t id,float acc,float speed,int32_t cal)
{
	SMotor_task_q_t_p task=NULL;
	uint8_t data[16];
	
	data[0]=id;
	WriteF32(&data[1],acc);
	WriteF32(&data[5],speed);
	WriteS32(&data[9],cal);
	
	task=osMemoryPoolAlloc(taskParseMemory,0);
	//申请失败的话 直接总线返回错误
	if(task==NULL)
	{
		printf("osMemoryPoolAlloc err\r\n");
		return;
	}
	task->adapterId=0xFF;
	task->srcId=0xFF;
	task->sessionId=0xFF;
	task->funCode=0x00012120;
	
	task->timeOut=0;
	task->step=0;
	
	//把总线 数据包拷贝到 新申请的内存块上
	memcpy(&task->parameterMem, &data[0],14);
	
	osMessageQueuePut(sMotor_task_q, 
								&task,         
								NULL,              
								NULL               
						); 
	
}


//添加一个任务到 待完成的任务链表中 或者直接完成非耗时操作
void ServoMotorParse(uint32_t func_code,bus_packet* packet)
{
	SMotor_return parsert;
	SMotor_task_q_t_p task=NULL;
	
#if SERVOMOTOR_DEBUG ==1		
	printf("new servoMotor cmd\r\n");
#endif
	//ucos ii 内核的队列实现是靠保存指针实现的 没有对数据进行 深拷贝
	//而bus总线回调函数执行结束后 就会回收掉内存 为了保留队列数据 这里分配一个内存池 
	//从内存池中申请一个SMotor_task_q_t
	task=osMemoryPoolAlloc(taskParseMemory,0);
	//申请失败的话 直接总线返回错误
	if(task==NULL)
	{
		printf("osMemoryPoolAlloc err\r\n");
		parsert.returnMem[0]=ServoMotor_ERR_MEMFULL;
		parsert.returnSize=1;
		BUS_ITA_RTN((*packet), (uint8_t*)&parsert.returnMem, (parsert.returnSize) * sizeof(uint8_t));
		return;
	}
	//参数预解包
	uint8_t offset=4;
	task->adapterId=packet->adapterId;
	task->srcId=packet->srcId;
	task->sessionId=packet->sessionId;
	task->funCode=func_code;
	
	task->timeOut=0;
	task->step=0;

#if SERVOMOTOR_DEBUG ==1		
	printf("new sevroMotor Cmd\r\n");
	printf("cmd len byte=%d\r\n",packet->data_len);
	
#endif
	//把总线 数据包拷贝到 新申请的内存块上
	memcpy(&task->parameterMem, &packet->data[offset],packet->data_len-4);
	
	/*
	在这里可以对task->funCode进行判断
	如果是不耗时操作 直接在这里完成 
	并且通过SMotor_return 进行返回
	是耗时操作就加入到任务队列
	*/
	//符合这个条件的要马上执行 不放入队列
	if(NOTIMECONSUMING)
	{
		//涉及到spi操作 使用互斥量保护
		//osSemaphoreAcquire(SMotorTask_Static.spiMutex,osWaitForever);
		//调用非耗时函数去执行命令
		NoTimeConsumingTaskHandle(task,&parsert);
		//osSemaphoreRelease(SMotorTask_Static.spiMutex);
		//执行完随着返回立即上传
#if SERVOMOTOR_DEBUG ==1		
		printf("rtsize%d\r\n",parsert.returnSize);
#endif
		BUS_ITA_RTN((*packet), (uint8_t*)&parsert.returnMem, (parsert.returnSize) * sizeof(uint8_t));			
		//释放掉内存池
		osMemoryPoolFree(taskParseMemory,task);
	}
	else//尝试放入队列 
	{
		//尾插 到任务链表
		uint8_t err;

		err=osMessageQueuePut(sMotor_task_q, 
								&task,         
								NULL,              
								NULL               
						); 
		//插入队列失败 就返回错误
		if(err!=osOK)
		{
			printf("new QueuePut Err\r\n");
			parsert.returnMem[0]=ServoMotor_ERR_MEMFULL;
			parsert.returnSize=1;
			BUS_ITA_RTN((*packet), (uint8_t*)&parsert.returnMem, (parsert.returnSize) * sizeof(uint8_t));
			return;			
		}
		else
		{
			printf("new QueuePut, sMotor_task_q free count=%d\r\n",osMessageQueueGetSpace(sMotor_task_q));
		}
		//在这里不能直接 释放内存 因为ucosii内核的队列不是真正的队列 其只保留数据地址而没深拷贝
		//osMemoryPoolFree(taskParseMemory,task);
	}
}


//置位伺服电机的外部事件
__attribute__((always_inline))  void ServoMotorEventSet_external(uint8_t motorId,volatile uint32_t flag)
{
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		
		DEBUG_LOG_ERROR("ServoMotorEventSet_external motorId overflow \r\n");
		while(1);
	}
	//根据实时操作标志 执行需要实时的操作
	if(((SMotorTask_Static.sMotorList[motorId].event.relEvents_exter_Flag)&flag))
	{

		switch(SMotorTask_Static.sMotorList[motorId].event.relOper)
		{
			case 0:
			{
				GetSPI();
				
				SMotorTask_Static.sMotorList[motorId].event.relEventPosition=
									TMC4361_GetEncPosition(SMotorTask_Static.sMotorList[motorId].drive);
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[motorId].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[motorId].drive);		
				PutSPI();			
			
			break;
			}
			
			default:
			{
			break;
			}
		}
		SMotorTask_Static.sMotorList[motorId].event.relEvents_exter_Flag&=(~flag);	
	}
	//同步到事件列表
	
	SMotorTask_Static.sMotorList[motorId].event.externalEvent|=flag;
}

//置位伺服电机的内部事件
__attribute__((always_inline))  void ServoMotorEventSet_internal(uint8_t motorId,uint32_t flag)
{
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR("ServoMotorEventSet_internal motorId overflow \r\n");
		while(1);
	}
	
	if(((SMotorTask_Static.sMotorList[motorId].event.relEvents_inter_Flag)&flag))
	{

		switch(SMotorTask_Static.sMotorList[motorId].event.relOper)
		{
			case 0:
			{
				GetSPI();
				SMotorTask_Static.sMotorList[motorId].event.relEventPosition=
									TMC4361_GetEncPosition(SMotorTask_Static.sMotorList[motorId].drive);
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[motorId].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[motorId].drive);		
				PutSPI();			
				
			break;
			}
			
			default:
			{
			break;
			}
		}
		SMotorTask_Static.sMotorList[motorId].event.relEvents_inter_Flag&=(~flag);	
	}
	//同步到事件列表

	SMotorTask_Static.sMotorList[motorId].event.internalEvent|=flag;
}

//得到伺服电机的外部事件


__attribute__((always_inline))  static void ServoMotorEventClear_external(uint8_t motorId,uint8_t eId)
{
	if(motorId>=SMotorTask_Static.sMotorCount||eId>32)
	{
		DEBUG_LOG_ERROR(" ServoMotorEventClear_external motorId overflow \r\n");
		while(1);
	}
	SMotorTask_Static.sMotorList[motorId].event.externalEvent&=~(0x01<<eId);
}

__attribute__((always_inline))  static void ServoMotorEventClear_external_all(uint8_t motorId)
{
		SMotorTask_Static.sMotorList[motorId].event.externalEvent=0;
}

__attribute__((always_inline))  static void ServoMotorEventClear_internal(uint8_t motorId,uint8_t eId)
{
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR(" ServoMotorEventClear_internal motorId overflow \r\n");
		while(1);
	}
	SMotorTask_Static.sMotorList[motorId].event.internalEvent&=~(0x01<<eId);
}

__attribute__((always_inline))  static void ServoMotorEventClear_internal_all(uint8_t motorId)
{
		SMotorTask_Static.sMotorList[motorId].event.internalEvent=0;
}


__attribute__((always_inline))  static void ServoMotorEventClear_ScanEvent()
{
	uint32_t exflag=0;
	uint8_t  infalg=0;

	for(uint8_t i=0;i<SMotorTask_Static.sMotorCount;i++)
	{
		ServoMotorEventClear_external_all(i);
		ServoMotorEventClear_internal_all(i);
		SMotorTask_Static.sMotorList[i].event.relOper=0;
		SMotorTask_Static.sMotorList[i].event.relEvents_exter_Flag=0;
		SMotorTask_Static.sMotorList[i].event.relEvents_inter_Flag=0;
		exflag=0;
		infalg=0;
		
		if(SMotorTask_Static.sMotorList[i].event.scanEvent==NULL)
			continue;
		
		SMotorTask_Static.sMotorList[i].event.scanEvent(&exflag,&infalg);

	}
}


__attribute__((always_inline))  static bool ServoMotorEventGet_external(uint8_t motorId,uint8_t eId)
{
	bool flag=false;
	if(motorId>=SMotorTask_Static.sMotorCount||eId>32)
	{
		DEBUG_LOG_ERROR("maxid:%d\r\n currentid:%d\r\n",SMotorTask_Static.sMotorCount,motorId);
		DEBUG_LOG_ERROR("ServoMotorEventGet_external motorId overflow \r\n");
		while(1);
	}
	
	if((SMotorTask_Static.sMotorList[motorId].event.externalEvent&(0x01<<eId)))
	{
		flag=true;
	}
	return flag;
}
//得到伺服电机的内部事件
__attribute__((always_inline))  static bool ServoMotorEventGet_internal(uint8_t motorId,uint8_t eId)
{
	bool flag=false;
	if(motorId>=SMotorTask_Static.sMotorCount||eId>32)
	{
		DEBUG_LOG_ERROR("ServoMotorEventGet_internal motorId overflow \r\n");
		while(1);
	}
	if((SMotorTask_Static.sMotorList[motorId].event.internalEvent&(0x01<<eId)))
	{
		flag=true;
	}
	return flag;
}

//设定 实时操作标记和操作类型
#define SetRelFlagandOper(eid,oper); {\
		SMotorTask_Static.sMotorList[dataPack.idx].event.relEvents_exter_Flag|=(0x01<<eid);\
		SMotorTask_Static.sMotorList[dataPack.idx].event.relOper=oper;\
		}

//空闲时间段 对事件的监听
static void ServoMotorEventScan_Free(void *p_arg)
{

	for(;;)
	{
		osDelay(100);	
	} 	
}
static void ServoMotorEventScan(void *p_arg)
{
	 uint32_t exflag=0;
	 uint8_t  infalg=0;
	
	for(;;)
	{
		osDelay(2);
		
		for(uint8_t i=0;i<SMotorTask_Static.sMotorCount;i++)
		{

			exflag=0;
			infalg=0;
			
			if(SMotorTask_Static.sMotorList[i].event.scanEvent==NULL)
				continue;
			
			SMotorTask_Static.sMotorList[i].event.scanEvent(&exflag,&infalg);

			if(exflag)
			{
				ServoMotorEventSet_external(i,exflag);
			}
			if(infalg)
			{	
				uint32_t inevent=0;
				
				//在这里实际的获取 驱动内部的事件
				GetSPI();
				TMC4361_Event_Get(SMotorTask_Static.sMotorList[i].drive,&inevent);
				PutSPI();
				//printf("new inevent =%x\r\n",inevent);
				ServoMotorEventSet_internal(i,inevent);
			}

		}
		
	}
}
static void ServoMotorMonitor(void *p_arg)
{
	SMotor_task_q_t_p task;	
	uint16_t step;
	SMotorRunState rt;
	SMotor_return report;
	
	step=SMotorTask_Static.sMotorMonitorStep;
	
	printf("ServoMotorMonitor\r\n");
	for(;;)
	{
		
		osDelay(SERVOMOTOR_SPANTIMEMS);
		
		switch(step)
		{
			case 0:
			printf("one queue over,continue\r\n");
			case 100:
			{
/*
*********************************************************************************************************
*                       每次进来要等待消息队列 以等到一个任务
*						等到任务后 去执行 只下发命令，不监听
*********************************************************************************************************
*/ 		
				printf("wait new Queue\r\n");
				osStatus_t result= osOK;
				
				result=osMessageQueueGet(sMotor_task_q, 
				  &task,         
				  NULL,              
				  NULL               
				 ); 
				
				if(result!=osOK)
				{
					DEBUG_LOG_ERROR("osMessageQueueGet err is %d\r\n",result);
				    step=0;
					break;
				}
				
				printf("funcCode=%x\r\n",task->funCode);
				printf("Run ServoMotor Cmd\r\n");
		
				report.returnSize=0;
				//在开始运行前先把任务的超时时间计算出来
				task->timeOut=TaskTimeOutCalculate(task);
				//这里其实只下发任务并没有去监听 监听任务在step300
			
				rt=TimeConsumingTaskHandle(task,&report);
				
				//也有可能由于任务冲突 或者使用的参数有误 造成 任务执行失败 这里就直接返回错误给使用者
				if(rt!=SMotorRunState_Complete)
				{
					osThreadSuspend(servoMotorEventScanHandle);
					printf("任务执行异常 不需要监听\r\n");
					if(!(task->adapterId==0xFF&&task->srcId==0xFF&&task->sessionId==0xFF))
						bus_ita_return(task->adapterId, task->srcId, task->sessionId, report.returnSize*sizeof(uint8_t), (uint8_t*)report.returnMem);
					
					osMemoryPoolFree(taskParseMemory,task);
					step=0;				
				}
				else //任务执行成功 就去监听
				{
					step=200;
				
					osThreadResume(servoMotorEventScanHandle);
					printf("start Monitor---------------------------\r\n");
				}
				break;
			}
/*
*********************************************************************************************************
*                       有些多步骤的任务在这里执行
*********************************************************************************************************
*/ 					
			case 110://这里执行多步骤任务
			{	

				//这里其实只下发任务并没有去监听 监听任务在step300

				
				rt=TimeConsumingTaskHandle(task,&report);

				
				//也有可能由于任务冲突 或者使用的参数有误 造成 任务执行失败 这里就直接返回错误给使用者
				if(rt!=SMotorRunState_Complete)
				{
					osThreadSuspend(servoMotorEventScanHandle);
					
					if(!(task->adapterId==0xFF&&task->srcId==0xFF&&task->sessionId==0xFF))
						bus_ita_return(task->adapterId, task->srcId, task->sessionId, report.returnSize*sizeof(uint8_t), (uint8_t*)report.returnMem);
					
					osMemoryPoolFree(taskParseMemory,task);
					step=0;				
				}
				else
				{
					step=200;
					osThreadResume(servoMotorEventScanHandle);
					printf("start Monitor---------------------------\\r\n");
				}
				break;
			}
			case 200:
			{
/*
*********************************************************************************************************
*                             监听到任务结束 并且回复上位机
*********************************************************************************************************
*/ 					
					report.returnSize=0;
					
					//执行一次监听
					printf("*");
					rt=ServoMotorMonitorHandleFunc(task,&report);
				
				    //通过监听看看任务是不是 还在运行状态
					if(rt!=SMotorRunState_Runing)
					{	
						printf("\n end Monitor-----------\r\n");
						//上一个步骤执行成功 但是步骤没有全部执行完 继续去执行
						if(task->step!=0&&rt==SMotorRunState_Complete)
						{
							osThreadSuspend(servoMotorEventScanHandle);
							step=110;
							break;
						}
						//如果任务的步骤执行完了或者出现了错误 就马上回复
						else
						{
							osThreadSuspend(servoMotorEventScanHandle);
							/* 开始回复消息 并清理内存 */	
							if(!(task->adapterId==0xFF&&task->srcId==0xFF&&task->sessionId==0xFF))
								bus_ita_return(task->adapterId, task->srcId, task->sessionId, report.returnSize*sizeof(uint8_t), (uint8_t*)report.returnMem);
#if SERVOMOTOR_DEBUG ==1		
							printf("nomal return %d byte\r\n",report.returnSize);
							
							for(uint8_t t=0;t<report.returnSize;t++)
								printf("%d byte:%x\r\n",report.returnSize,report.returnMem[t]);
#endif						
							osMemoryPoolFree(taskParseMemory,task);
						
							printf("servoMotorThread FreeStack=%d\r\n",osThreadGetStackSpace(servoMotorMonitorHandle));
							printf("one queue over\r\n");
							step=0;
				
						}
					}
				break;
			}
			
		}
	}
}

//根据任务 来推算超时时间
static int32_t TaskTimeOutCalculate(SMotor_task_q_t_p task)
{
	int32_t timeOut;

	switch(task->funCode)
	{
		case 0x00012010://电机使能 
		case 0x00012005:
		case 0x00012006:
		case 0x00012011:
		case 0x00012150:
		case 0x00012160:
		case 0x00012161://电机急停 
			timeOut=2000;
		break;
		
		case 0x00012100://电机简单移动耗时 要计算时间
		{
			Port_0X00012100_t dataPack;
			PORT_UNPACK_0X00012100(&dataPack,task->parameterMem);
			int32_t tick=0;
			
			if(dataPack.isRelative==false)
			{
				float distance=PulseCountToDistance_Conversion(dataPack.idx,TMC4361_GetPosition(SMotorTask_Static.sMotorList[dataPack.idx].drive));
				tick=dataPack.distance-distance;
				tick=abs(tick);			
			}
			else
			{
				tick=dataPack.distance;//=DistanceToPulseCount_Conversion(dataPack.idx,dataPack.distance);
				tick=abs(tick);
			}
			
	
			timeOut=(tick/dataPack.speed*1000)+5000;
			break;
		}
		case 0x00012120:
		{
			Port_0X00012120_t dataPack;
			PORT_UNPACK_0X00012120(&dataPack,task->parameterMem);
			
			float distance=0;
			
			distance=(SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance>SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance_reverse)?
					(SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance):(SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance_reverse);
			
			if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance==0&&SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance==0)
			{
				timeOut=30000;
			}
			else
			{
				timeOut=(distance/dataPack.speed*1000)+5000;	
			}
			
			break;
		}
		default:
		{
			timeOut=30000;
			break;
		}
		
	}
	printf("cmd=%x timeOut=%d\r\n",task->funCode,timeOut);
	return timeOut;
}

//距离（mm）转换为微步数（脉冲数量）
static int32_t DistanceToPulseCount_Conversion(uint8_t motorId ,float distance)
{
	float  step_ratio;
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR("DistanceToPulseCount_Conversion motorId overflow \r\n");
		while(1);
		//return 0;
	}
	step_ratio=SMotorTask_Static.sMotorList[motorId].cfg->step_ratio;
	
	//printf("distance=%f,step_ratio=%f,\r\n",distance,step_ratio);
	return (int32_t)((distance*step_ratio*256)+0.5);
}
//微步数（脉冲数量）转换为距离（mm）
static float PulseCountToDistance_Conversion(uint8_t motorId ,int32_t pulse)
{
	float  step_ratio;
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR("PulseCountToDistance_Conversion motorId overflow \r\n");
		while(1);
		//return 0;
	}
	step_ratio=SMotorTask_Static.sMotorList[motorId].cfg->step_ratio;
	
	
	return (float)((pulse*1.0)/(256*1.0)/step_ratio);
}

//速度（mm\s）转换为速度（bpp\s）
static int32_t SpeedMMsToSpeedBpps_Conversion(uint8_t motorId ,float SpeedMMs)
{
	float  step_ratio;
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR("SpeedMMsToSpeedBpps_Conversion motorId overflow \r\n");
		while(1);
		//return 0;
	}
	step_ratio=SMotorTask_Static.sMotorList[motorId].cfg->step_ratio;
	
	return (int32_t)((SpeedMMs*step_ratio*256)+0.5);
}
//速度（bpp\s）转换为 速度（mm\s）
static float SpeedBppsToSpeedMMs_Conversion(uint8_t motorId ,int32_t SpeedBpps)
{
	float  step_ratio;
	if(motorId>=SMotorTask_Static.sMotorCount)
	{
		DEBUG_LOG_ERROR("SpeedBppsToSpeedMMs_Conversion motorId overflow \r\n");
		while(1);
		//return 0;
	}
	step_ratio=SMotorTask_Static.sMotorList[motorId].cfg->step_ratio;
	
	
	return (float)(SpeedBpps/256/step_ratio);
}


void Cheak_sMotor_id(uint8_t id)
{
	if(id>=SMotorTask_Static.sMotorCount)
	{
		while(1)
		{
			DEBUG_LOG_ERROR("sMotorCount is %d,but  sMotor is %d\r\n",SMotorTask_Static.sMotorCount,id);
			
			for(unsigned short nms = 0; nms < 1000; nms++)
			{
				for(unsigned short ms = 0; ms < 41750; ms++)
				{
					// 大致延时1s
				}
			}
		}
	}
	
}

#if SERVOMOTOR_EXTEND == 1	
void Cheak_Motor_id(uint8_t id)
{
	if(id>=SMotorTask_Static.extend.tmc5130MotorCount)
	{
		while(1)
		{
			DEBUG_LOG_ERROR("MotorCount is %d,but  Motor is %d\r\n",SMotorTask_Static.extend.tmc5130MotorCount,id);
			
			for(unsigned short nms = 0; nms < 1000; nms++)
			{
				for(unsigned short ms = 0; ms < 41750; ms++)
				{
					// 大致延时1s
				}
			}
		}
	}
	
}
#endif

//对于非耗时任务 直接执行
void NoTimeConsumingTaskHandle(SMotor_task_q_t_p task,SMotor_return *rt)
{
	//osSemaphoreAcquire(SMotorTask_Static.spiMutex,osWaitForever); 
	switch(task->funCode)
	{
		case 0x00012005:
		{
			Port_0X00012005_t dataPack;
			PORT_UNPACK_0X00012005(&dataPack,task->parameterMem);	
			
			Port_0X00012005_rt data;
			
			data.errcode=0;
			data.drive_type=SMotorTask_Static.sMotorList[dataPack.idx].cfg->motorDeviceType;
			data.maxI=SMotorTask_Static.sMotorList[dataPack.idx].drive->cfg->cl_imax;
			data.minI=SMotorTask_Static.sMotorList[dataPack.idx].drive->cfg->cl_imin;
			data.step_per_fs=SMotorTask_Static.sMotorList[dataPack.idx].drive->cfg->setp_per_fs;
			data.step_ratio=SMotorTask_Static.sMotorList[dataPack.idx].cfg->step_ratio;
			data.zero_distance=SMotorTask_Static.sMotorList[dataPack.idx].cfg->zero_distance;
			data.restEventPol=SMotorTask_Static.sMotorList[dataPack.idx].cfg->restEventPol;
			data.defaultDir=SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir;
			data.encoderAccuracy =SMotorTask_Static.sMotorList[dataPack.idx].drive->cfg->enc_in_res;
			data.max_distance=SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance;
			data.max_distance_re=SMotorTask_Static.sMotorList[dataPack.idx].cfg->max_distance_reverse;
			
			PORT_PACK_0X00012005(rt,&data);
			break;
		}
		case 0x00012006:
		{
			Port_0X00012006_t dataPack;
			PORT_UNPACK_0X00012006(&dataPack,task->parameterMem);	
			
			Port_0X00012006_rt data;
			int32_t speedtemp;
			int32_t positiontemp;
			data.errcode=0;
			GetSPI();
			speedtemp=TMC4361_GetEnc_Speed(SMotorTask_Static.sMotorList[dataPack.idx].drive);
			positiontemp=TMC4361_GetEnc_Speed(SMotorTask_Static.sMotorList[dataPack.idx].drive);
			PutSPI();
			data.speed=SpeedBppsToSpeedMMs_Conversion(dataPack.idx,speedtemp);
			data.position=PulseCountToDistance_Conversion(dataPack.idx,positiontemp);
			if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==true)
				data.position*=-1;
			
				
		
			PORT_PACK_0X00012006(rt,&data);
			break;
		}
		case 0x00012010://电机失能
		{
			Port_0X00012010_t dataPack;
			PORT_UNPACK_0X00012010(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			
//			SMotorTask_Static.sMotorList[dataPack.idx].drive->TMC4361A_NFREEZE();
			GetSPI();	
			TMC4361A_SetCL(SMotorTask_Static.sMotorList[dataPack.idx].drive,
				0,
				0);
			PORT_PACK_0X00012010(rt,ServoMotor_ERR_NONE);	
			PutSPI();
			break;
		}
		case 0x00012011://电机使能
		{
			Port_0X00012011_t dataPack;
			PORT_UNPACK_0X00012011(&dataPack,task->parameterMem);
	
			Cheak_sMotor_id(dataPack.idx);

			PORT_PACK_0X00012011(rt,ServoMotor_ERR_NONE);
	
			break;
		}
	
		case 0x00012150://电机旋转
		{
			Port_0X00012150_t dataPack;
			PORT_UNPACK_0X00012150(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			
			
			
			GetSPI();
			
			TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
								(dataPack.acce),
								(dataPack.acce));
			
			PutSPI();
			
			if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==true)
			{
				if(dataPack.dir==true)
				{
					GetSPI();
					TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				else
				{
					GetSPI();
					TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				
			}
			else
			{
				if(dataPack.dir==false)
				{
					GetSPI();
					TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				else
				{
					GetSPI();
					TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
			}
			
			PORT_PACK_0X00012150(rt,ServoMotor_ERR_NONE);
			break;
		}
		case 0x00012160://电机停止
		{
			Port_0X00012160_t dataPack;
			PORT_UNPACK_0X00012160(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			ServoMotorEventSet_external(dataPack.idx,0x01<<2);
			
			GetSPI();
			TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
					dataPack.dece,
					dataPack.dece);
			TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
			PutSPI();

			PORT_PACK_0X00012160(rt,ServoMotor_ERR_NONE);
			break;
		}
		case 0x00012161://电机急停
		{
			Port_0X00012161_t dataPack;
			PORT_UNPACK_0X00012161(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			GetSPI();
			TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
					EMERGENCY_STOP_ACC,
					EMERGENCY_STOP_ACC);
			
			TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
			PutSPI();
			ServoMotorEventSet_external(dataPack.idx,0x01<<2);
			PORT_PACK_0X00012161(rt,ServoMotor_ERR_NONE);

			break;
		}
		
	}
	//osSemaphoreRelease(SMotorTask_Static.spiMutex);
}


//对于耗时任务 只是下发执行 不需要等到任务结束  

#ifndef ServoMotorMove_Def
#define ServoMotorMove_Def(src_data,port); \
		GetSPI();\
		TMC4361A_SetRamp(SMotorTask_Static.sMotorList[src_data.idx].drive,\
					src_data.acce,\
					src_data.acce);\
		int32_t currentDistance_Pulse=TMC4361_GetEncPosition(SMotorTask_Static.sMotorList[src_data.idx].drive);\
		PutSPI();\
		float   currentDistance=PulseCountToDistance_Conversion(src_data.idx,currentDistance_Pulse);\
		int32_t targetDistance_Pulse=DistanceToPulseCount_Conversion(src_data.idx,src_data.distance);\
		float   targetDistance=src_data.distance;\
		if(src_data.isRelative==true)\
		{			\
			if(targetDistance_Pulse!=0)\
			{\
				if(SMotorTask_Static.sMotorList[src_data.idx].cfg->defaultDir==false)\
				{\
					targetDistance_Pulse=targetDistance_Pulse*-1;\
					currentDistance=currentDistance*-1;\
				}\
				float RunAfterDistance=currentDistance+targetDistance;\
				if(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance&&targetDistance>0)\
				{\
					if(RunAfterDistance>SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance)\
					{\
						PORT_PACK_##port(report,2);\
						return SMotorRunState_Error;\
					}\
				}\
				else if(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance_reverse)\
				{\
					if(RunAfterDistance<(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance_reverse*-1))\
					{\
						PORT_PACK_##port(report,2);\
						return SMotorRunState_Error;\
					}						\
				}	\
				int32_t speed=SpeedMMsToSpeedBpps_Conversion(src_data.idx,src_data.speed);\
				GetSPI();\
				TMC4361A_MoveBy(SMotorTask_Static.sMotorList[src_data.idx].drive,&targetDistance_Pulse,speed);\
				PutSPI();\
				ServoMotorEventClear_external_all(src_data.idx);\
				ServoMotorEventClear_internal_all(src_data.idx);\
			}\
			else\
			{\
				ServoMotorEventClear_external_all(src_data.idx);\
				ServoMotorEventClear_internal_all(src_data.idx);\
				ServoMotorEventSet_external(src_data.idx,0x01);\
			}\
		}\
		else\
		{\
			if(SMotorTask_Static.sMotorList[src_data.idx].cfg->defaultDir==false)\
			{\
				targetDistance_Pulse=targetDistance_Pulse*-1;\
				currentDistance=currentDistance*-1;\
			}\
			printf("cd=%f;td=%f,out=%f\r\n",currentDistance,targetDistance,fabs(currentDistance-targetDistance));\
			if(fabs(currentDistance-targetDistance)>SERVOMOTOR_PRECISION)\
			{\
				if(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance&&targetDistance>0)\
				{\
					if(((targetDistance))>SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance)\
					{\
						PORT_PACK_##port(report,2);\
						return SMotorRunState_Error;\
					}\
				}\
				else if(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance_reverse)\
				{\
					if((targetDistance)<(SMotorTask_Static.sMotorList[src_data.idx].cfg->max_distance_reverse*-1))\
					{\
						PORT_PACK_##port(report,2);\
						return SMotorRunState_Error;\
					}\
				}\
				int32_t speed=SpeedMMsToSpeedBpps_Conversion(src_data.idx,src_data.speed);\
				ServoMotorEventClear_external_all(src_data.idx);\
				GetSPI();\
				TMC4361A_MoveTo(SMotorTask_Static.sMotorList[src_data.idx].drive,targetDistance_Pulse,speed);\
				PutSPI();\
			}\
			else\
			{\
				ServoMotorEventClear_external_all(src_data.idx);\
				ServoMotorEventClear_internal_all(src_data.idx);\
				ServoMotorEventSet_external(src_data.idx,0x01);\
			}\
		}

#endif		
static SMotorRunState TimeConsumingTaskHandle(SMotor_task_q_t_p task,SMotor_return* report)
{
	SMotorRunState rt=SMotorRunState_Complete;
	
	//这里需要清理一下 用户的事件扫描 防止瞬间得到信号
	ServoMotorEventClear_ScanEvent();
	
	switch(task->funCode)
	{
		//简单移动
		case 0x00012100:
		{
			Port_0X00012100_t dataPack;
			PORT_UNPACK_0X00012100(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			
#if SERVOMOTOR_DEBUG ==1
			printf("new move：————————————————————————————————————————————————————\n");
			printf("id：%d #",dataPack.idx);
			printf("isRel：%d #",dataPack.isRelative);
			printf("acc：%f #",dataPack.acce);
			printf("distance：%f #",dataPack.distance);
			printf("speed：%f\r\n",dataPack.speed);
			printf("new move end：————————————————————————————————————————————————————\n");
#endif			
		
			ServoMotorMove_Def(dataPack,0X00012100);
		break;
		}
		//进阶的移动
		case 0x00012101:
		{
			Port_0X00012101_t dataPack;
			PORT_UNPACK_0X00012101(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);

#if SERVOMOTOR_DEBUG ==1	
			printf("new Advanced move：————————————————————————————————————————————————————\n");
			printf("id：%d\r\n",dataPack.idx);
			printf("isRel：%d\r\n",dataPack.isRelative);
			printf("speed：%f\r\n",dataPack.speed);
			printf("distance：%f\r\n",dataPack.distance);
			printf("acc：%d\r\n",dataPack.acce);
			printf("dcc：%d\r\n",dataPack.dcce);
			printf("b1：%d\r\n",dataPack.b1);
			printf("b2：%d\r\n",dataPack.b2);
			printf("b3：%d\r\n",dataPack.b3);
			printf("b4：%d\r\n",dataPack.b4);
			printf("new move end：————————————————————————————————————————————————————\n");
#endif			
			GetSPI();
			TMC4361A_SetRamp_advanced(SMotorTask_Static.sMotorList[dataPack.idx].drive,
								(int32_t)dataPack.acce*50,
								(int32_t)dataPack.dcce*50,
								(int32_t)dataPack.b1*50,
								(int32_t)dataPack.b2*50,
								(int32_t)dataPack.b3*50,
								(int32_t)dataPack.b4*50);
			 
			PutSPI();
			if(dataPack.isRelative==true)//相对移动
			{
				int32_t tick=DistanceToPulseCount_Conversion(dataPack.idx,dataPack.distance);
				
				if(tick!=0)
				{
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						tick=tick*-1;
					}
					int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
					//清除一下 对应的事件标志组 防止意外的提前触发
					ServoMotorEventClear_external_all(dataPack.idx);
					GetSPI();
					TMC4361A_MoveBy(SMotorTask_Static.sMotorList[dataPack.idx].drive,&tick,speed);
					PutSPI();
					
				}
				else
				{
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventSet_external(dataPack.idx,0x01);
					break;
				}
			}
			else//绝对移动
			{
					int32_t tick=DistanceToPulseCount_Conversion(dataPack.idx,dataPack.distance);

					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						tick=tick*-1;
					}
					GetSPI();
					int32_t distance=TMC4361_GetPosition(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					if(tick==distance)
					{
						ServoMotorEventClear_external_all(dataPack.idx);
						ServoMotorEventSet_external(dataPack.idx,0x01);
					}
					else
					{

						int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
						//清除一下 对应的事件标志组 防止意外的提前触发
						ServoMotorEventClear_external_all(dataPack.idx);
						GetSPI();
						TMC4361A_MoveTo(SMotorTask_Static.sMotorList[dataPack.idx].drive,tick,speed);
						PutSPI();
					}
			}
			break;
		}
		case 0x00012120://电机复位
		{
			Port_0X00012120_t dataPack;
			PORT_UNPACK_0X00012120(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			
			if(task->step==0)//说明第一次进来
			{
#if SERVOMOTOR_DEBUG ==1
				printf("motor %d rest----------------------------------\r\n",dataPack.idx);
#endif		
				if(SMotorTask_Static.sMotorList[dataPack.idx].drive->TMC4361A_RST!=NULL)
				{
					SMotorTask_Static.sMotorList[dataPack.idx].drive->TMC4361A_RST();
				}
				else
				{
					GetSPI();
					TMC4361_SoftRest(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
				}

				osDelay(100);
				GetSPI();
				TMC4361A_CFG_Init(SMotorTask_Static.sMotorList[dataPack.idx].drive);
				PutSPI();
				osDelay(100);

				//设定速度斜坡
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
						dataPack.acce,
						5000);
				PutSPI();
				bool restSwitch;
				//看看电机是不是触发
				restSwitch=SMotorTask_Static.sMotorList[dataPack.idx].sMotorGetRestSwitchState();
				
				//允许在运动过程中 被外部事件停止
				
				if(restSwitch==SMotorTask_Static.sMotorList[dataPack.idx].cfg->restEventPol)
				{
					//触发了   就需要先往复位方向的 反方向移动直到复位开关不触发
#if SERVOMOTOR_DEBUG ==1
					printf("Zero Triggered\r\n");
#endif

					SMotorTask_Static.sMotorList[dataPack.idx].event.relEvents_exter_Flag|=(0x01<<1);
					SMotorTask_Static.sMotorList[dataPack.idx].event.relOper=0;
					
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						GetSPI();
						TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
											SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));	
						PutSPI();
					}
					else
					{
						GetSPI();
						TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
											SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
						PutSPI();
					}
					task->step=2; //如果复位前就触发了 复位开关 那么至多需要这么多步数就可以复位成功
				}
				else
				{
#if SERVOMOTOR_DEBUG ==1
					printf("Zero No Trigger\r\n");
#endif
					//没有触发 就直接向着 复位方向移动  直到复位开关触发

					SMotorTask_Static.sMotorList[dataPack.idx].event.relEvents_exter_Flag|=(0x01<<1);
					SMotorTask_Static.sMotorList[dataPack.idx].event.relOper=0;

					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==true)
					{
						GetSPI();
						TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
											SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));	
						PutSPI();
					}
					else
					{
						GetSPI();
						TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
											SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
						PutSPI();
					}
					task->step=1;
				}
				
			}
			else if(task->step==2)//再次进来step=2 说明上次是 向着复位方向的反方向移动的
			{
				//允许在运动过程中 被外部事件停止

				SMotorTask_Static.sMotorList[dataPack.idx].event.relEvents_exter_Flag|=(0x01<<1);
				SMotorTask_Static.sMotorList[dataPack.idx].event.relOper=0;

				ServoMotorEventClear_external_all(dataPack.idx);
				ServoMotorEventClear_internal_all(dataPack.idx);
				//那么这次就 直接向着复位方向移动
				if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==true)
				{
					GetSPI();
					TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
										SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();					
				}
				else
				{
					
					GetSPI();
					TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
										SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				task->step=1;//
					
			}

			else if(task->step==1)//复位完成后 还需要走一个 回零距离
			{
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
								dataPack.acce,
								1000);
				PutSPI();
#if SERVOMOTOR_DEBUG ==1
				printf("Zero Distance\r\n");
				printf("motor %d rest end----------------------------------\r\n",dataPack.idx);
#endif				
				float distance=SMotorTask_Static.sMotorList[dataPack.idx].cfg->zero_distance;
				int32_t tick=DistanceToPulseCount_Conversion(dataPack.idx,distance);
			
				task->step=0;
				
				if(tick!=0)
				{
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						tick=tick*-1;
					}
					int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
					//清除一下 对应的事件标志组 防止意外的提前触发
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					GetSPI();
					TMC4361A_MoveBy(SMotorTask_Static.sMotorList[dataPack.idx].drive,&tick,speed);
					PutSPI();
				}
				else//如果是0距离就直接跳过
				{
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);

					ServoMotorEventSet_external(dataPack.idx,0x01);
				}
				
			}
			break;
		}
		case 0x00012122://电机定标
		{
			Port_0X00012122_t dataPack;
			PORT_UNPACK_0X00012122(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			int32_t offset=0;
	
			switch(SMotorTask_Static.sMotorList[dataPack.idx].cfg->motorDeviceType)
			{
				case TMC4361:
				{
					TMC4361TypeDef *drive;
					drive=SMotorTask_Static.sMotorList[dataPack.idx].drive;
					GetSPI();
					TMC4361A_SetCL(drive,200,200);
					TMC4361_CloseCL(drive);
		
					TMC4361A_MoveToNextFullstep(drive);
					PutSPI();
					osDelay(200);
					ServoMotorEventClear_external_all(dataPack.idx);
					GetSPI();
					TMC4361_SimpleCali_Start(drive);
					PutSPI();
					osDelay(500);
					GetSPI();
					offset=TMC4361_SimpleCali_End(drive);
					TMC4361_OpenCL(drive);
					TMC4361A_SetCL(drive,drive->cfg->cl_imin,drive->cfg->cl_imax);
					PutSPI();
					break;
				}
				case TMC5240:
				{
					break;
				}
				default:
					break;
			}
			rt=SMotorRunState_CompleteAndNotRun;
			PORT_PACK_0X00012122(report,ServoMotor_ERR_NONE,offset);
			break;
		}		
		//移动到事件触发后再移动一段距离
		case 0x00012400:
		{
	
			Port_0X00012400_t dataPack;
			PORT_UNPACK_0X00012400(&dataPack,task->parameterMem);
			Cheak_sMotor_id(dataPack.idx);
			if(task->step==0)//说明第一次进来
			{
#if SERVOMOTOR_DEBUG ==1
				printf("Event move start----------------------------------------------------\r\n");
				printf("motor is %d ,event id %d distance=%f afterdistance=%f speed=%f add=%f\r\n",dataPack.idx,dataPack.event,dataPack.overDistance,dataPack.afterDistance,dataPack.speed,dataPack.acce);
				printf("Event move end----------------------------------------------------\r\n");
#endif		
		
				int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
				

				int32_t tick=DistanceToPulseCount_Conversion(dataPack.idx,dataPack.overDistance);
				
				if(tick!=0)
				{
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						tick=tick*-1;
					}
				
					int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
					
					
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
						dataPack.acce,
						dataPack.acce);
					TMC4361A_MoveBy(SMotorTask_Static.sMotorList[dataPack.idx].drive,&tick,speed);
					PutSPI();
					
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					//这里 挂上一些实时操作

					SetRelFlagandOper(dataPack.event,0);
				
				}
				else
				{
					
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					ServoMotorEventSet_external(dataPack.idx,0x01);
					break;
				}		
				task->step=1;
			}
			else if(task->step==1)//第二次进来
			{

				
				task->step=0;
				if(dataPack.afterDistance!=0)
				{   //触发后 再移动一段
					int32_t tick=DistanceToPulseCount_Conversion(dataPack.idx,dataPack.afterDistance);
					
					
		
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						tick=tick*-1;
					}
					int32_t speed=SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed);
						
					osDelay(500);
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,dataPack.acce,dataPack.acce);
					TMC4361A_MoveBy(SMotorTask_Static.sMotorList[dataPack.idx].drive,&tick,speed);
					PutSPI();
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
				}
				else//如果是0距离就直接跳过
				{
					ServoMotorEventClear_external_all(dataPack.idx);
					ServoMotorEventClear_internal_all(dataPack.idx);
					
					ServoMotorEventSet_external(dataPack.idx,0x01);
					break;
				}
				
			}
			break;
		}
		
		
		case 0x00012153://电机旋转 一定的时间
		{
			Port_0X00012153_t dataPack;
			PORT_UNPACK_0X00012153(&dataPack,task->parameterMem);		
			Cheak_sMotor_id(dataPack.idx);
			
			GetSPI();
			TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
								dataPack.acce,
								dataPack.acce);
			PutSPI();
			if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==true)
			{
				if(dataPack.dir==true)
				{
					GetSPI();
					TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				else
				{
					GetSPI();
					TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				
			}
			else
			{
				if(dataPack.dir==false)
				{
					GetSPI();
					TMC4361A_Left(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
				else
				{
					GetSPI();
					TMC4361A_Right(SMotorTask_Static.sMotorList[dataPack.idx].drive,
									SpeedMMsToSpeedBpps_Conversion(dataPack.idx,dataPack.speed));
					PutSPI();
				}
			}
			dataPack.time=dataPack.time;
			
			break;
		}
		case 0x00012200://多电机同时运动
		{
			Port_0X00012200_t dataPack;
			PORT_UNPACK_0X00012200(&dataPack,task->parameterMem);		
			
#if SERVOMOTOR_DEBUG ==1
			printf("most motor move start :%d----------------------------------------------------\r\n",dataPack.count);
			for(uint8_t i=0;i<dataPack.count;i++)
			{
				printf("motor %d :----",i);
				Port_0X00012200_one_t currentData;
				PORT_UNPACK_0X00012200_ONE(&currentData,&dataPack.data[i*Port_0X00012200_one_t_size]);
				
				printf("currentId：%d #",currentData.idx);
				printf("isRel：%d #",currentData.isRelative);
				printf("acc：%f #",currentData.acce);
				printf("distance：%f #",currentData.distance);
				printf("speed：%f #\n",currentData.speed);
			
			}
		
			printf("most motor move end---------------------------------------------------------------\r\n");
#endif	
			for(uint16_t i=0;i<dataPack.count;i++)
			{
				Port_0X00012200_one_t currentData;
				PORT_UNPACK_0X00012200_ONE(&currentData,&dataPack.data[i*Port_0X00012200_one_t_size]);
				Cheak_sMotor_id(currentData.idx);
				
				
				ServoMotorMove_Def(currentData,0X00012200);
			}
			break;
		}

		case 0x00012210://序列化多电机运动
		{
			Port_0X00012210_t dataPack;
			PORT_UNPACK_0X00012210(&dataPack,task->parameterMem);		
			
			static uint16_t addresOffset=0;
			if(task->step==0)//说明第一次进来  赋值序列数量
			{
				task->step=dataPack.count;
				addresOffset=0;
			}

			//解包序列数组
			Port_0X00012210_Arg_t argsPack;
			PORT_UNPACK_0X00012210_Arg(&argsPack,&dataPack.data[addresOffset]);
			addresOffset=addresOffset+4+(argsPack.count*Port_0X00012210_one_t_size);
	
		
			for(uint16_t i=0;i<argsPack.count;i++)
			{
				
				Port_0X00012210_one_t currentData;
				PORT_UNPACK_0X00012210_ONE(&currentData,&argsPack.data[i*Port_0X00012210_one_t_size]);
				Cheak_sMotor_id(currentData.idx);
				
				ServoMotorMove_Def(currentData,0X00012210);
				
			}
			//减少一个序列数量
			task->step--;
			break;
		}	
		
		
		
#if SERVOMOTOR_EXTEND == 1		
		case 0x00012201://扩展版本的多电机同时运动 可以搭配普通电机
		{
			task->step=0;
			task->tmc5130MotorCount=0;
			Port_0X00012201_t dataPack;
			PORT_UNPACK_0X00012201(&dataPack,task->parameterMem);	
			
#if SERVOMOTOR_DEBUG ==1
			printf("most motor move start :%d----------------------------------------------------\r\n",dataPack.count);
			for(uint8_t i=0;i<dataPack.count;i++)
			{
				printf("motor %d :----",i);
				Port_0X00012201_one_t currentData;
				PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
				printf("step=%d #",dataPack.count);
				printf("currentId：%d #",currentData.idx);
			    printf("currentType：%d #",currentData.type);
				printf("isRel：%d #",currentData.isRelative);
				printf("acc：%f #",currentData.acce);
				printf("distance：%f #",currentData.distance);
				printf("speed：%f #",currentData.speed);
			
			}
		
			printf("most motor move end---------------------------------------------------------------\r\n");
#endif	
			//先下达伺服电机的指令
			for(uint16_t i=0;i<dataPack.count;i++)
			{
				Port_0X00012201_one_t currentData;
				PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
				Cheak_sMotor_id(currentData.idx);
				//判断电机类型 是普通电机就先不执行
				if(currentData.type==1)
				{
					task->tmc5130MotorCount++;//借助 tmc5130MotorCount 记录普通电机的数量 
					continue;
					
				}
				
				ServoMotorMove_Def(currentData,0X00012201);
			}
			
			if(task->tmc5130MotorCount!=0)
			{
				//这里阻塞的执行 普通电机的运动
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012201_one_t currentData;
					PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
					
					//判断电机类型 执行普通电机的动作
					if(currentData.type==1)
					{
						Cheak_Motor_id(currentData.idx);
						
						
						uint8_t  result;
						if(currentData.isRelative==1)
						{
							result=mTMC5130DevicMoveby(
												&SMotorTask_Static.extend.tmc5130MotorList[currentData.idx],
												motor_step_calc(currentData.idx,currentData.speed),
												motor_step_calc(currentData.idx,currentData.acce),
												motor_step_calc(currentData.idx,currentData.distance),
												0,
												SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->subdivision,
												SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->currentLevel
									);	
							if(result!=1)
							{
								SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].wait_type=WAIT_REACH_FLG;
								SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].evnt_group|=1<<currentData.idx;
							}
						}
						else
						{
							result=mTMC5130DevicMoveto(
												&SMotorTask_Static.extend.tmc5130MotorList[currentData.idx],
												motor_step_calc(currentData.idx,currentData.speed),
												motor_step_calc(currentData.idx,currentData.acce),
												motor_step_calc(currentData.idx,currentData.distance),
												0,
												SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->subdivision,
												SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->currentLevel
									);
				
						}
						
						if(result!=1)
						{
							SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].wait_type=WAIT_REACH_FLG;
							SMotorTask_Static.extend.tmc5130MotorList[0].evnt_group|=1<<currentData.idx;
						}
					}	
						
				}
				//在这里 阻塞普通电机运行完成
				uint16_t err=0;
				if(SMotorTask_Static.extend.tmc5130MotorList[0].evnt_group>0)
				{
				
					err=mTMC5130DevicWait_Sem(&SMotorTask_Static.extend.tmc5130MotorList[0],WAIT_REACH_FLG,0);
					
					for(uint8_t i=0;i<dataPack.count;i++)
					{
						Port_0X00012201_one_t currentData;
						PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
						
						if(currentData.isRelative==1)
						{
							SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].status=MOTOR_STATUS_MOVE_FINISH;
						}
					}
				}
				
				if(err!=MOTOR_STATUS_STOP)
				{
					rt=SMotorRunState_CompleteAndNotRun;
					PORT_PACK_0X00012201(report,ServoMotor_ERR_MEMFULL);
					break;
				}
			}
			break;
		}
		case 0x00012211://扩展版本的电机序列化运动 可以搭配普通电机
		{

			Port_0X00012211_t dataPack;
			PORT_UNPACK_0X00012211(&dataPack,task->parameterMem);		
			

			static uint16_t addresOffset=0;
			
			if(task->step==0)//说明第一次进来  赋值序列数量
			{
				
#if SERVOMOTOR_DEBUG ==1
				printf("new seq start %d --------------------------------------------------------\n",dataPack.count);
				task->tmc5130MotorCount=0;
				addresOffset=0;
				for(uint8_t i=0;i<dataPack.count;i++)
				{
					
					Port_0X00012211_Arg_t argsPack;
					PORT_UNPACK_0X00012211_Arg(&argsPack,&dataPack.data[addresOffset]);
					addresOffset=addresOffset+4+(argsPack.count*Port_0X00012211_one_t_size);

					printf("seq %d :---\n",i);
					for(uint16_t i=0;i<argsPack.count;i++)
					{
						Port_0X00012211_one_t currentData;
						PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);		
						
						printf("idx=%d  ",currentData.idx);
						printf("acce=%f  ",currentData.acce);
						printf("distance=%f  ",currentData.distance);
						printf("isRelative=%d  ",currentData.isRelative);
						printf("speed=%f ",currentData.speed);
						printf("type=%d \n",currentData.type);
					}
					
				}
				printf("new seq end --------------------------------------------------------\n");
#endif			

				
				task->step=dataPack.count;
				task->tmc5130MotorCount=0;
				addresOffset=0;
				
			}
		
			//解包序列数组
			Port_0X00012211_Arg_t argsPack;
			PORT_UNPACK_0X00012211_Arg(&argsPack,&dataPack.data[addresOffset]);
			addresOffset=addresOffset+4+(argsPack.count*Port_0X00012211_one_t_size);
	
		
			//先下达伺服电机的指令
			for(uint16_t i=0;i<argsPack.count;i++)
			{
				Port_0X00012211_one_t currentData;
				PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);
				
				Cheak_sMotor_id(currentData.idx);
				//判断电机类型 是普通电机就先不执行
				if(currentData.type==1)
				{
					task->tmc5130MotorCount++;//借助 tmc5130MotorCount 记录普通电机的数量 
					continue;
				}
				
				ServoMotorMove_Def(currentData,0X00012211);
					
			}
			
			//printf("Start Run SimpleMotor, in all %d\r\n",task->tmc5130MotorCount);
			
			if(task->tmc5130MotorCount!=0)
			{
					//这里阻塞的执行 普通电机的运动
					for(uint16_t i=0;i<argsPack.count;i++)
					{
						Port_0X00012211_one_t currentData;
						PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);	
						//判断电机类型 执行普通电机的动作
						if(currentData.type==1)
						{
							Cheak_Motor_id(currentData.idx);
							uint8_t  result;
							if(currentData.isRelative==1)
							{
								result=mTMC5130DevicMoveby(
													&SMotorTask_Static.extend.tmc5130MotorList[currentData.idx],
													motor_step_calc(currentData.idx,currentData.speed),
													motor_step_calc(currentData.idx,currentData.acce),
													motor_step_calc(currentData.idx,currentData.distance),
													0,
													SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->subdivision,
													SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->currentLevel
										);	
								if(result!=1)
								{
									SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].wait_type=WAIT_REACH_FLG;
									SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].evnt_group|=1<<currentData.idx;
								}
							}
							else
							{
								result=mTMC5130DevicMoveto(
													&SMotorTask_Static.extend.tmc5130MotorList[currentData.idx],
													motor_step_calc(currentData.idx,currentData.speed),
													motor_step_calc(currentData.idx,currentData.acce),
													motor_step_calc(currentData.idx,currentData.distance),
													0,
													SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->subdivision,
													SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].mot_cfg->currentLevel
										);
					
							}
							
							if(result!=1)
							{
								SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].wait_type=WAIT_REACH_FLG;
								SMotorTask_Static.extend.tmc5130MotorList[0].evnt_group|=1<<currentData.idx;
							}
						}	
							
					}
					//在这里 阻塞普通电机运行完成
					uint16_t err=0;
					if(SMotorTask_Static.extend.tmc5130MotorList[0].evnt_group>0)
					{
						err=mTMC5130DevicWait_Sem(&SMotorTask_Static.extend.tmc5130MotorList[0],WAIT_REACH_FLG,0);
						for(uint8_t i=0;i<dataPack.count;i++)
						{
							Port_0X00012211_one_t currentData;
							PORT_UNPACK_0X00012211_ONE(&currentData,&dataPack.data[i*Port_0X00012211_one_t_size]);
							
							if(currentData.isRelative==1)
							{
								SMotorTask_Static.extend.tmc5130MotorList[currentData.idx].status=MOTOR_STATUS_MOVE_FINISH;
							}
						}
					}
					
					if(err!=MOTOR_STATUS_STOP)
					{
						rt=SMotorRunState_CompleteAndNotRun;
						PORT_PACK_0X00012211(report,ServoMotor_ERR_MEMFULL);
						break;
					}
			}
			//减少一个序列数量
			task->step--;		
			break;
		}
#endif
		//未实现的指令
		default:
		{
			break;
		}	
	}
	
	
	
	return rt;
}



//监听任务结果 返回 任务执行情况
static SMotorRunState ServoMotorMonitorHandleFunc(SMotor_task_q_t_p task,SMotor_return* report)
{
	//每次监听都需要把任务的超时往下递减

	task->timeOut=task->timeOut-SERVOMOTOR_SPANTIMEMS;
	

	switch(task->funCode)
	{
		//简单移动
		case 0x00012100:
		{
			Port_0X00012100_t dataPack;
			PORT_UNPACK_0X00012100(&dataPack,task->parameterMem);
				
			switch(SMotorTask_Static.sMotorList[dataPack.idx].cfg->motorDeviceType)
			{
				case TMC4361:
				{
					//完成
					if(ServoMotorEventGet_external(dataPack.idx,0))
					{
						PORT_PACK_0X00012100(report,ServoMotor_ERR_NONE);
						return SMotorRunState_Complete;
					}
//通用判断					
#if 1					
					else if(task->timeOut<=0)
					{
						GetSPI();
						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
						TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
						PutSPI();
						PORT_PACK_0X00012100(report,ServoMotor_ERR_TIMEOUT);
						return SMotorRunState_timeOut;
					}
					else if(ServoMotorEventGet_external(dataPack.idx,2))//被终止
					{
						PORT_PACK_0X00012100(report,ServoMotor_TERMINATE);
						return SMotorRunState_Complete;
					}

#endif
				break;
				}
				default:
				break;
			}
		break;
		}
		//进阶的移动
		case 0x00012101:
		{
			Port_0X00012101_t dataPack;
			PORT_UNPACK_0X00012101(&dataPack,task->parameterMem);
			
	
		
			//完成
//			if(ServoMotorEventGet_external(dataPack.idx,0))
//			{
//				PORT_PACK_0X00012101(report,ServoMotor_ERR_NONE);
//				return SMotorRunState_Complete;
//			}
//			else if(ServoMotorEventGet_external(dataPack.idx,2))//被终止
//			{
//				PORT_PACK_0X00012101(report,ServoMotor_TERMINATE);
//				return SMotorRunState_Complete;
//			}
//			else if(ServoMotorEventGet_internal(dataPack.idx,26))
//			{
//				PORT_PACK_0X00012101(report,ServoMotor_DRIVE_ENC_VLE0);
//				return SMotorRunState_Error;
//			}
//			else if(ServoMotorEventGet_internal(dataPack.idx,27))
//			{
//				PORT_PACK_0X00012101(report,ServoMotor_DRIVE_CL_MAX);
//				return SMotorRunState_Error;				
//			}
//			else if(ServoMotorEventGet_internal(dataPack.idx,28))
//			{
//				PORT_PACK_0X00012101(report,ServoMotor_DRIVE_CL_FIT);
//				return SMotorRunState_Error;				
//			}
			//超时
			if(task->timeOut<=0)
			{
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
				PutSPI();
				PORT_PACK_0X00012101(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;
			}
			break;
		}
		case 0x00012120://电机复位
		{
			Port_0X00012120_t dataPack;
			PORT_UNPACK_0X00012120(&dataPack,task->parameterMem);
			
			if(task->step==2)//这里是 需要监听 复位开关 从触发 到不触发
			{
				if(ServoMotorEventGet_external(dataPack.idx,1))
				{
					//如果零位开关触发 就先让电机停下来
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					PORT_PACK_0X00012120(report,ServoMotor_ERR_NONE);
					return SMotorRunState_Complete;					
				}	
			}

			else if(task->step==1)//这里是 需要监听 复位开关 从不触发 到触发
			{
				if(ServoMotorEventGet_external(dataPack.idx,1))
				{
					//停止被外部事件停止
					
					//先让电机停下来
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					PORT_PACK_0X00012120(report,ServoMotor_ERR_NONE);
					return SMotorRunState_Complete;					
				}			
			}

			else if(task->step==0)//说明 是监听最后一步 只需要监听电机到达即刻
			{
				
				if((ServoMotorEventGet_external(dataPack.idx,0))||SMotorTask_Static.sMotorList[dataPack.idx].cfg->zero_distance==0)
				{
					//这里电机复位完成 需要把驱动器内部外部的 位置都清零

					if(SMotorTask_Static.sMotorList[dataPack.idx].drive->TMC4361A_RST!=NULL)
					{
						SMotorTask_Static.sMotorList[dataPack.idx].drive->TMC4361A_RST();
					}
					else
					{
						GetSPI();
						TMC4361_SoftRest(SMotorTask_Static.sMotorList[dataPack.idx].drive);
						PutSPI();
					}
					osDelay(200);
	

					GetSPI();
					TMC4361A_CFG_Init(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					osDelay(200);
					GetSPI();
					TMC4361_Set_Cl_Offset(SMotorTask_Static.sMotorList[dataPack.idx].drive,dataPack.calibrate);
					TMC4361_ClearEncAndPosition(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					TMC4361_OpenCL(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					osDelay(200);
					
					PORT_PACK_0X00012100(report,ServoMotor_ERR_NONE);
					printf("rest success\r\n");
					return SMotorRunState_Complete;
				}				
			}
			
			//如果在复位过程中 有任何特殊事件 进行处理
			if(ServoMotorEventGet_external(dataPack.idx,2))//被终止
			{
				PORT_PACK_0X00012120(report,ServoMotor_TERMINATE);
				return SMotorRunState_Complete;
			}

			//超时
			if(task->timeOut<=0)
			{
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
				PutSPI();
				PORT_PACK_0X00012120(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;
			}
			break;
		}
		//移动到事件触发后再移动一段距离
		case 0x00012400:
		{
			static float rtpostion;
			static uint8_t monitor=0;
			Port_0X00012400_t dataPack;
			PORT_UNPACK_0X00012400(&dataPack,task->parameterMem);
		
			//第一次监听
			if(task->step==1)
			{
				//等待事件触发
				
				if(ServoMotorEventGet_external(dataPack.idx,dataPack.event))
				{
					monitor=0;
					PORT_PACK_0X00012100(report,ServoMotor_ERR_NONE);
					//先确保电机停下来
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					//获取当前位置
					int32_t position=TMC4361_GetEncPosition(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					PutSPI();
					//position=TMC4361_GetPosition(SMotorTask_Static.sMotorList[dataPack.idx].drive);
					if(SMotorTask_Static.sMotorList[dataPack.idx].cfg->defaultDir==false)
					{
						position=position*-1;
						
					}
					rtpostion=PulseCountToDistance_Conversion(dataPack.idx,position);
						
					return SMotorRunState_Complete;
				}
				//如果走完第一段后还没有触发事件 就报错
				else if((ServoMotorEventGet_external(dataPack.idx,0))||monitor)
				{
				
					//printf("\r\n exter Envent=%x\r\n",exterFlag);
					if(monitor>5)
					{
						
						monitor=0;
						PORT_PACK_0X00012400(report,1,0);
					
						return SMotorRunState_Error;					
					}
					else
					{	
						monitor++;
						break;
					}

				}
				
			}
			//第二次监听
			else if(task->step==0)
			{

				if(dataPack.afterDistance!=0)
				{
					//等待电机执行完毕
					if(ServoMotorEventGet_external(dataPack.idx,0))
					{
						
						PORT_PACK_0X00012400(report,ServoMotor_ERR_NONE,rtpostion);
						rtpostion=0;
						return SMotorRunState_Complete;
					}
					
								
				}
				else	//触发后移动的距离为0 就直接完成
				{
				
						PORT_PACK_0X00012400(report,ServoMotor_ERR_NONE,rtpostion);
						rtpostion=0;
						return SMotorRunState_Complete;
				}
			}
#if 1			
			if(task->timeOut<=0)
			{
			
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
				PutSPI();
				PORT_PACK_0X00012400(report,ServoMotor_ERR_TIMEOUT,0);
				return SMotorRunState_timeOut;
			}	
#endif
			break;
		}
		
		case 0x00012153://电机旋转
		{
			static uint32_t timeCount12153=0;
			
			Port_0X00012153_t dataPack;
			PORT_UNPACK_0X00012153(&dataPack,task->parameterMem);		

			
			
			if(dataPack.time>timeCount12153)
			{
				timeCount12153=timeCount12153+10;
			}
			else
			{
				timeCount12153=0;
				//到时间就让电机停下来
				GetSPI();
				TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,
					0,
					400000);
				TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
				PutSPI();
				PORT_PACK_0X00012153(report,ServoMotor_ERR_NONE);
				return SMotorRunState_Complete;
			}
			break;
		}
		case 0x00012200://多电机同时运动
		{
			Port_0X00012200_t dataPack;
			PORT_UNPACK_0X00012200(&dataPack,task->parameterMem);		
			
			//如果被置位，说明置位的位所代表的电机已完成动作 
			static uint32_t motorFlag=0;
			
			uint16_t eventId=0;
			int16_t motorFlagCount;//用来记录完成的电机数量
			
			motorFlagCount=0;
			eventId=0;
		
			for(uint16_t i=0;i<dataPack.count;i++)
			{
				//如果被置过位 就不需要再判断了 直接看下一个电机
				if((motorFlag&(0x01<<i))!=0)
				{
					motorFlagCount++;
					continue;
				}
				
				Port_0X00012200_one_t currentData;
				PORT_UNPACK_0X00012200_ONE(&currentData,&dataPack.data[i*Port_0X00012200_one_t_size]);	
				

				if(ServoMotorEventGet_external(currentData.idx,0))
				{
					motorFlagCount++;
					eventId=0;
					motorFlag=motorFlag|(0x01<<i);//如果完成就在这里置位
				}
							
			}
			
			//判断运动过程是否有异常事件发生
			if(eventId==0)//如果没有异常事件发生
			{
				//看看所有电机是否都执行完毕
				printf("\n motorFlagCount is%d;count is %d\r\n",motorFlagCount,dataPack.count);
				if(motorFlagCount>=dataPack.count)
				{
					motorFlag=0;
					motorFlagCount=0;
					PORT_PACK_0X00012200(report,ServoMotor_ERR_NONE);
					return SMotorRunState_Complete;
				}
			}
			else//如果有异常事件发送
			{
				motorFlag=0;
				motorFlagCount=0;
				//先停下所有电机动作
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012200_one_t currentData;
					PORT_UNPACK_0X00012200_ONE(&currentData,&dataPack.data[i*Port_0X00012200_one_t_size]);
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
					0,
					400000);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);	
					PutSPI();
				}	
				
				PORT_PACK_0X00012200(report,eventId);
				return SMotorRunState_Error;
			}
			
			//判断是否超时
			if(task->timeOut<=0)
			{
				motorFlag=0;
				motorFlagCount=0;
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012200_one_t currentData;
					PORT_UNPACK_0X00012200_ONE(&currentData,&dataPack.data[i*Port_0X00012200_one_t_size]);
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
					0,
					400000);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);	
					PutSPI();
				PORT_PACK_0X00012200(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;					
				}	
				
			}
			break;
		}	
		
		case 0x00012210://序列化电机运动
		{
			Port_0X00012210_t dataPack;
			PORT_UNPACK_0X00012210(&dataPack,task->parameterMem);	
			
			static uint8_t delayFlag=0;
			static uint32_t delayMs=0;
			
			//如果被置位，说明置位的位所代表的电机已完成动作 
			static uint32_t motorFlag=0;
			uint16_t eventId=0;
			int16_t motorFlagCount;
			motorFlagCount=0;
			eventId=0;
			
			static uint16_t addresOffset=0;
			
			//当任务执行完成后 进行指定的延时
			if(delayFlag==1)
			{
				delayMs=delayMs+10;
				
				if(delayMs>=dataPack.delayMs)
				{
					
					delayMs=0;
					delayFlag=0;
					PORT_PACK_0X00012210(report,ServoMotor_ERR_NONE);
//					printf("序列%d完成\r\n",(dataPack.count-task->step-1));
					return SMotorRunState_Complete;				
				}
				else
				{
					return SMotorRunState_Runing;
				}
			}
			
			//解析当前序列的数据 得到执行组
			Port_0X00012210_Arg_t argsPack;
			PORT_UNPACK_0X00012210_Arg(&argsPack,&dataPack.data[addresOffset]);
			
			for(uint16_t i=0;i<argsPack.count;i++)
			{
				//如果被置过位 就不需要再判断了 直接看下一个电机
				if((motorFlag&(0x01<<i))!=0)
				{
					eventId=0;
					motorFlagCount++;
					continue;
				}
				
				Port_0X00012210_one_t currentData;
				PORT_UNPACK_0X00012210_ONE(&currentData,&argsPack.data[i*Port_0X00012210_one_t_size]);
			
			
			
				if(ServoMotorEventGet_external(currentData.idx,0))
				{
					motorFlagCount++;
					eventId=0;
					motorFlag=motorFlag|(0x01<<i);//如果完成就在这里置位
//					printf("%d完成\r\n",motorFlagCount);
				}
				//遇到下面的这些情况 需要直接报错 并停止所有动作
	
				
			}
			
			//判断运动过程是否有异常事件发生
			if(eventId==0)//如果没有异常事件发生
			{
				//看看本次序列所有电机是否都执行完毕
				if(motorFlagCount>=argsPack.count)
				{
					motorFlagCount=0;
					motorFlag=0;
					addresOffset=addresOffset+4+(argsPack.count*Port_0X00012210_one_t_size);
					
					
					if(task->step==0)
					{
//							printf("所有序列执行完毕\r\n");
							addresOffset=0;//最后一次肯定从这里走
							delayMs=0;
							delayFlag=0;
		
							PORT_PACK_0X00012210(report,ServoMotor_ERR_NONE);
							return SMotorRunState_Complete;						
					}
					else
					{
						if((dataPack.delayMs<10))//看看需不需要延时
						{
//							printf("序列%d执行完毕，不需要延时\r\n",(dataPack.count-task->step-1));
							delayMs=0;
							delayFlag=0;
							return SMotorRunState_Complete;
						}
						else
						{
//							printf("序列%d执行完毕，开始延时\r\n",(dataPack.count-task->step-1));
							delayMs=0;
							delayFlag=1;
							//本来应该这里直接返回任务完成的 但是由于存在任务间的延时 所以要等待延时结束
							return SMotorRunState_Runing;
						}	
					}
				}
			}
			else//如果有异常事件发送
			{
				addresOffset=0;
				motorFlag=0;
				motorFlagCount=0;
				//先停下所有电机动作
				for(uint16_t i=0;i<argsPack.count;i++)
				{
					Port_0X00012210_one_t currentData;
					PORT_UNPACK_0X00012210_ONE(&currentData,&argsPack.data[i*Port_0X00012210_one_t_size]);
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
					0,
					400000);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);	
					PutSPI();
				}	
				
				PORT_PACK_0X00012210(report,eventId);
				return SMotorRunState_Error;
			}
			
			//判断是否超时
			if(task->timeOut<=0)
			{
				addresOffset=0;
				motorFlag=0;
				motorFlagCount=0;
				//停下所有动作
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012210_one_t currentData;
					PORT_UNPACK_0X00012210_ONE(&currentData,&argsPack.data[i*Port_0X00012210_one_t_size]);
					GetSPI();
					TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
					0,
					400000);
					TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);		
					PutSPI();
				}
				
				PORT_PACK_0X00012210(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;		
			}
			break;
		}
#if SERVOMOTOR_EXTEND == 1	
		
		case 0x00012201://扩展多电机同时运动
		{
			Port_0X00012201_t dataPack;
			PORT_UNPACK_0X00012201(&dataPack,task->parameterMem);		
			
			//如果被置位，说明置位的位所代表的电机已完成动作 
			static uint32_t motorFlag=0;
			
			uint16_t eventId=0;
			int16_t motorFlagCount;
			
			motorFlagCount=0;
			eventId=0;
			for(uint16_t i=0;i<dataPack.count;i++)
			{
				//如果被置过位 就不需要再判断了 直接看下一个电机
				if((motorFlag&(0x01<<i))!=0)
				{
					motorFlagCount++;
					continue;
				}
				
				Port_0X00012201_one_t currentData;
				PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);	
				
				if(currentData.type==1)
				{
					motorFlagCount++;
					continue;
				}
	
				
				if(ServoMotorEventGet_external(currentData.idx,0))
				{
					
					motorFlagCount++;
					eventId=0;
					motorFlag=motorFlag|(0x01<<i);//如果完成就在这里置位
				}
				{
			
				}		
			}
			
			//判断运动过程是否有异常事件发生
			if(eventId==0)//如果没有异常事件发生
			{
				if(motorFlagCount>=(dataPack.count))
				{
					motorFlag=0;
					motorFlagCount=0;
					task->tmc5130MotorCount=0;
					PORT_PACK_0X00012201(report,ServoMotor_ERR_NONE);
					return SMotorRunState_Complete;
				}
			}
			else//如果有异常事件发送
			{
				motorFlag=0;
				motorFlagCount=0;
				//先停下所有电机动作
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012201_one_t currentData;
					PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
					if(currentData.type==0)
					{	
						GetSPI();
						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
						0,
						400000);
						TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);	
						PutSPI();
					}
				}	
				task->tmc5130MotorCount=0;
				PORT_PACK_0X00012201(report,eventId);
				return SMotorRunState_Error;
			}
			
			//判断是否超时
			if(task->timeOut<=0)
			{
				motorFlag=0;
				motorFlagCount=0;
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012201_one_t currentData;
					PORT_UNPACK_0X00012201_ONE(&currentData,&dataPack.data[i*Port_0X00012201_one_t_size]);
					
					if(currentData.type==0)
					{
						GetSPI();
						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
						0,
						400000);
						TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);		
						PutSPI();
					}
					
				}
				task->tmc5130MotorCount=0;
				PORT_PACK_0X00012201(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;				
			}
			break;
		}	
		
		case 0x00012211://扩展序列化电机运动
		{
			Port_0X00012211_t dataPack;
			PORT_UNPACK_0X00012211(&dataPack,task->parameterMem);	
			
			static uint8_t delayFlag=0;
			static uint32_t delayMs=0;
			
			//如果被置位，说明置位的位所代表的电机已完成动作 
			static uint32_t motorFlag=0;
			uint16_t eventId=0;
			int16_t motorFlagCount;
			motorFlagCount=0;
			eventId=0;
			
			static uint16_t addresOffset=0;
			
			//当任务执行完成后 进行指定的延时
			if(delayFlag==1)
			{
				delayMs=delayMs+10;
				
				if(delayMs>=dataPack.delayMs)
				{
					
					delayMs=0;
					delayFlag=0;
					PORT_PACK_0X00012211(report,ServoMotor_ERR_NONE);
					return SMotorRunState_Complete;				
				}
				else
				{
					return SMotorRunState_Runing;
				}
			}
			
			//解析当前序列的数据 得到执行组
			Port_0X00012211_Arg_t argsPack;
			PORT_UNPACK_0X00012211_Arg(&argsPack,&dataPack.data[addresOffset]);
			
			for(uint16_t i=0;i<argsPack.count;i++)
			{
				//如果被置过位 就不需要再判断了 直接看下一个电机
				if((motorFlag&(0x01<<i))!=0)
				{
					motorFlagCount++;
					continue;
				}
				
				Port_0X00012211_one_t currentData;
				PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);
			
				if(currentData.type==1)
				{
					motorFlagCount++;
					continue;
				}

				
				if(ServoMotorEventGet_external(currentData.idx,0))
				{
					
					motorFlagCount++;
					eventId=0;
					motorFlag=motorFlag|(0x01<<i);//如果完成就在这里置位
				}
				else
				{
					
				}
			
			
			}
			
			//判断运动过程是否有异常事件发生
			if(eventId==0)//如果没有异常事件发生
			{
				
				if(motorFlagCount>=(argsPack.count))
				{
					motorFlagCount=0;
					motorFlag=0;
					addresOffset=addresOffset+4+(argsPack.count*Port_0X00012211_one_t_size);
	
					
					if(task->step==0)
					{
							//printf("所有序列执行完毕\r\n");
							addresOffset=0;//最后一次肯定从这里走
							delayMs=0;
							delayFlag=0;
							task->tmc5130MotorCount=0;
							PORT_PACK_0X00012211(report,ServoMotor_ERR_NONE);
							return SMotorRunState_Complete;						
					}
					else
					{
						if((dataPack.delayMs<10))//看看需不需要延时
						{
							//printf("序列%d执行完毕，不需要延时\r\n",(dataPack.count-task->step-1));
							delayMs=0;
							delayFlag=0;
							task->tmc5130MotorCount=0;
							return SMotorRunState_Complete;
						}
						else
						{
							//printf("序列%d执行完毕，开始延时\r\n",(dataPack.count-task->step-1));
							delayMs=0;
							delayFlag=1;
							task->tmc5130MotorCount=0;
							//本来应该这里直接返回任务完成的 但是由于存在任务间的延时 所以要等待延时结束
							return SMotorRunState_Runing;
						}	
					}

				}
			}
			else//如果有异常事件发送
			{
				addresOffset=0;
				motorFlag=0;
				motorFlagCount=0;
				//先停下所有电机动作
				for(uint16_t i=0;i<argsPack.count;i++)
				{
					Port_0X00012211_one_t currentData;
					PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);
					
					if(currentData.type==0)
					{
						GetSPI();
						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
						0,
						400000);
						TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);
						PutSPI();
					}
					
				}	
				task->tmc5130MotorCount=0;
				PORT_PACK_0X00012211(report,eventId);
				return SMotorRunState_Error;
			}
			
			//判断是否超时
			if(task->timeOut<=0)
			{
				addresOffset=0;
				motorFlag=0;
				motorFlagCount=0;
				//停下所有动作
				for(uint16_t i=0;i<dataPack.count;i++)
				{
					Port_0X00012211_one_t currentData;
					PORT_UNPACK_0X00012211_ONE(&currentData,&argsPack.data[i*Port_0X00012211_one_t_size]);
					if(currentData.type==0)
					{
						GetSPI();
						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[currentData.idx].drive,
						0,
						400000);
						TMC4361A_Stop(SMotorTask_Static.sMotorList[currentData.idx].drive);	
						PutSPI();
					}
				task->tmc5130MotorCount=0;
				PORT_PACK_0X00012211(report,ServoMotor_ERR_TIMEOUT);
				return SMotorRunState_timeOut;					
				}	
				
			}
			break;
		}

#endif		
		default:
		{
			break;	
		}
		
	}
	
	
	return SMotorRunState_Runing;
}	


//伺服电机任务初始化
uint8_t ServoMotorTaskInit(void)
{
/*
*********************************************************************************************************
*                                      生成伺服电机任务的线程和其需要的一些元素
*********************************************************************************************************
*/ 
	uint8_t rt;

	//初始化任务接收内存池
	taskParseMemory=osMemoryPoolNew(MAXTASKPARSE,sizeof(SMotor_task_q_t),&taskParseMemorymemAttr_t);
	if(taskParseMemory==NULL)
	{
		DEBUG_LOG_ERROR("taskParseMemory creation Err\r\n");
	}

	//生成待做任务队列
	sMotor_task_q   = osMessageQueueNew(MAXSMOTOR_TASK_LIST,        
									   sizeof(SMotor_task_q_t_p),   
									   &sMotor_task_q_Attr  
									   );
	if(sMotor_task_q==NULL)
	{
		DEBUG_LOG_ERROR("sMotor_task_q creation Err\r\n");
	}

	//生成SPI互斥量
	SMotorTask_Static.spiMutex=NULL;
	
	SMotorTask_Static.spiMutex=osSemaphoreNew(1,1,NULL);
	
	if(SMotorTask_Static.spiMutex==NULL)
	{
		DEBUG_LOG_ERROR("SMotorTask_Static.spiMutex\r\n");
	}

	
    //创建伺服电机监听线程
	servoMotorMonitorHandle=osThreadNew(ServoMotorMonitor,NULL,&servoMotorMonitor_attributes);
	if(servoMotorMonitorHandle!=NULL)
	{
	  DEBUG_LOG_INFO("task_name=%s\r\n",osThreadGetName(servoMotorMonitorHandle));
	  DEBUG_LOG_INFO("task_prio=%d\r\n",osThreadGetPriority(servoMotorMonitorHandle));
	}
	else
	{
	  DEBUG_LOG_ERROR("servoMotorMonitor creation Err\r\n");
	  return ServoMotor_ERR_INITERR;
	}

	servoMotorEventScanHandle=osThreadNew(ServoMotorEventScan,NULL,&servoMotorEventScan_attributes);
	if(servoMotorEventScanHandle!=NULL)
	{
	  DEBUG_LOG_INFO("task_name=%s\r\n",osThreadGetName(servoMotorEventScanHandle));
	  DEBUG_LOG_INFO("task_prio=%d\r\n",osThreadGetPriority(servoMotorEventScanHandle));
	  osThreadSuspend(servoMotorEventScanHandle);
	}
	else
	{
	  DEBUG_LOG_ERROR("servoMotorEventScanHandle creation Err\r\n");
	  return ServoMotor_ERR_INITERR;
	}
	

	return ServoMotor_ERR_NONE;
}







/*

					WriteU32((u8*)&signal_array[0],0x1000c32);
					WriteU16((u8*)&signal_array[4],(upcount));//current_count

					WriteU32((u8*)&signal_array[6],10);

					for(uint8_t k=0,d=10;k<10;k++)
					{
						WriteU16((u8*)&signal_array[d],buf_405_650[k][0]);

						d=d+2;
						signal_array[d]=buf_405_650[k][1];
						d=d+1;

						printf("id=%d,%d,%d\r\n",k,buf_405_650[k][0],buf_405_650[k][1]);

					}

					bus_write_data((u8 *)&signal_array, 10+(9*sizeof(u16)*2)+4+4+2, CAN_ADAPTER_ID, 0);
*/

//					else if(ServoMotorEventGet_internal(dataPack.idx,26))
//					{

//						GetSPI();
//						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
//						TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
//						PutSPI();
//						PORT_PACK_0X00012100(report,ServoMotor_DRIVE_ENC_VLE0);
//						return SMotorRunState_Error;
//					}
//					else if(ServoMotorEventGet_internal(dataPack.idx,27))
//					{
//						GetSPI();
//						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
//						TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
//						PutSPI();
//						PORT_PACK_0X00012100(report,ServoMotor_DRIVE_CL_MAX);
//						return SMotorRunState_Error;				
//					}
//					else if(ServoMotorEventGet_internal(dataPack.idx,28))
//					{
//						GetSPI();
//						TMC4361A_SetRamp(SMotorTask_Static.sMotorList[dataPack.idx].drive,EMERGENCY_STOP_ACC,EMERGENCY_STOP_ACC);
//						TMC4361A_Stop(SMotorTask_Static.sMotorList[dataPack.idx].drive);
//						PutSPI();
//						PORT_PACK_0X00012100(report,ServoMotor_DRIVE_CL_FIT);
//						return SMotorRunState_Error;				
//					}