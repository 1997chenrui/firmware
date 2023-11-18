
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "cmsis_compiler.h"             // Compiler agnostic definitions

#include "ucos_ii.h"
#include "ucosii_os2.h"
#include "ucosii_os2_tcb.h"


#include <string.h>
#define IS_IRQ_MASKED()           (__get_PRIMASK() != 0U)
#define IS_IRQ_MODE()             (__get_IPSR() != 0U)
#define IS_IRQ()                  IS_IRQ_MODE()

#define SVCall_IRQ_NBR            (IRQn_Type) -5	/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */


#define KERNEL_ID                 ("ucosii " )


/* Limits */
#define MAX_BITS_TASK_NOTIFY      31U
#define MAX_BITS_EVENT_GROUPS     24U

#define THREAD_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_TASK_NOTIFY)  - 1U))
#define EVENT_FLAGS_INVALID_BITS  (~((1UL << MAX_BITS_EVENT_GROUPS) - 1U))


__STATIC_INLINE void SVC_Setup (void) {
#if (__ARM_ARCH_7A__ == 0U)
  /* Service Call interrupt might be configured before kernel start     */
  /* and when its priority is lower or equal to BASEPRI, svc intruction */
  /* causes a Hard Fault.                                               */
  NVIC_SetPriority (SVCall_IRQ_NBR, 0U);//重置SVC中断优先级
#endif
}


/*
ucosii任务优先级是 0————OS_LOWEST_PRIO 其中0为最大优先级 
CMSIS-RTOS接口中 任务优先级为 osPriority_t枚举类型 :0为无优先级 1为最低优先级（空闲任务） osPriorityRealtime7（55）为最高优先级

要把优先级映射过来 最好不修改CMSIS-RTOS源文件和ucosii源文件

*/
static const INT8U prioMapping[OS_LOWEST_PRIO+1]=
{
/*-0-*/ 	64,		//无优先级，错误的优先级
/*-1-*/ 	63,		//IDLE任务优先级
	
/*-2-*/ 	62,		
/*-3-*/ 	61,		
/*-4-*/ 	60,			   
/*-5-*/ 	59,		
/*-6-*/ 	58,		
/*-7-*/ 	57,		
	
/*-8-*/ 	56,		//osPriorityLow
/*-9-*/ 	55,
/*-10-*/ 	54,
/*-11-*/ 	53,
/*-12-*/ 	52,
/*-13-*/ 	51,
/*-14-*/ 	50,
/*-15-*/ 	49,		//osPriorityLow+7
/*-16-*/ 	48,		//osPriorityBelowNormal
/*-17-*/ 	47,
/*-18-*/ 	46,
/*-19-*/ 	45,
/*-20-*/ 	44,
/*-21-*/ 	43,
/*-22-*/ 	42,
/*-23-*/ 	41,		//osPriorityBelowNormal+7
/*-24-*/ 	40,		//Priority: normal
/*-25-*/ 	39,
/*-26-*/ 	38,
/*-27-*/ 	37,
/*-28-*/ 	36,
/*-29-*/ 	35,
/*-30-*/ 	34,
/*-31-*/ 	33,		//Priority: normal+7
/*-32-*/ 	32,		//Priority: above normal
/*-33-*/ 	31,
/*-34-*/ 	30,
/*-35-*/ 	29,
/*-36-*/ 	28,
/*-37-*/ 	27,
/*-38-*/ 	26,
/*-39-*/ 	25,		//Priority: above normal+7
/*-40-*/ 	24,		//Priority: high
/*-41-*/ 	22,
/*-42-*/ 	21,
/*-43-*/ 	20,
/*-44-*/ 	19,
/*-45-*/ 	18,
/*-46-*/ 	17,
/*-47-*/ 	16,		//Priority: high+7		
/*-48-*/ 	15,		//Priority: realtime
/*-49-*/ 	14,
/*-50-*/ 	13,
/*-51-*/ 	12,
/*-52-*/ 	11,
/*-53-*/ 	10,
/*-54-*/ 	9,
/*-55-*/ 	8,		//Priority: realtime+7
/*-56-*/ 	0,		//Reserved for ISR deferred thread.
					//我们把ucos的0-7这8个较高的优先级保留 用作互斥量优先级继承
					//8个互斥量应该够用了吧 (┬┬﹏┬┬)
/*-57-*/ 	7,
/*-58-*/ 	6,
/*-59-*/ 	5,
/*-60-*/ 	4,
/*-61-*/ 	3,
/*-62-*/ 	2,
/*-63-*/ 	1

};

static INT8U PrioToCmsisRtos(INT8U ucosp)
{
	INT8U i=0;
	
	for(i=0;i<OS_LOWEST_PRIO+1;i++)
	{
		if(ucosp==prioMapping[i])
		{
			break;
		}
	}
	return i;
}
/*
因为ucosii相比CMSIS-RTOS缺少了一些必要属性
这些属性必须是实体，为了不暴漏这些属性并使得ucosii接口和CMSIS-RTOS接口完全一致 
需要用到动态内存 但是又不想使用 标准C语言函数malloc/free(会产生内存碎片,且线程不安全)
故使用ucosii内存池管理 
这个内存池-defaultMemPool 仅供cmsis_os2接口内部使用 用户程序无法使用
*/
//分区内存块数量 至少有两个分区
#define defaultMemPoolNblks 32
//分区内存块大小 至少一个指针的大小
#define defaultMemPoolBlksize 128
//这个内存池会在调用osKernelInitialize()时被初始化
OS_MEM * defaultMemPool;

static INT8U defaultMemPoolBuf[defaultMemPoolNblks][defaultMemPoolBlksize];


typedef struct {
  osTimerFunc_t func;
  void         *arg;
} TimerCallback_t;

/*
内核信息和控制
*/


/* Kernel initialization state */
static osKernelState_t KernelState = osKernelInactive;

/*
初始化系统内核
会创建一个defaultMemPool内存池用于接口内部使用
*/
osStatus_t osKernelInitialize (void) {
  osStatus_t stat;
  INT8U rt;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else {
    if (KernelState == osKernelInactive) {
      #if defined(USE_TRACE_EVENT_RECORDER)
        //事件跟踪分析器 暂未实现
        #error EVENT RECORDER Not realized
      #endif
      //此处可以初始化 内存管理功能(如果有需要)
      OSInit();
	  defaultMemPool=OSMemCreate(&defaultMemPoolBuf[0][0],defaultMemPoolNblks,defaultMemPoolBlksize,&rt);
	  if(rt!=OS_ERR_NONE)
	  {
		return	 osError;
	  }
      KernelState = osKernelReady;
      stat = osOK;
    } else {
      stat = osError;
    }
  }

  return (stat);
}

//得到内核信息
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size) {

  if (version != NULL) {
    /* Version encoding is major.minor.rev: mmnnnrrrr dec */
    version->api    = OS_VERSION;
    version->kernel = OS_VERSION;
  }

  if ((id_buf != NULL) && (id_size != 0U)) {
    if (id_size > sizeof(KERNEL_ID)) {
      id_size = sizeof(KERNEL_ID);
    }
    memcpy(id_buf,KERNEL_ID, id_size);
  }

  return (osOK);
}

//得到内核状态
osKernelState_t osKernelGetState (void) {
  osKernelState_t state;

  if(OSRunning==OS_TRUE)
  {
    state = osKernelRunning;
  }
  else
  {
      if (KernelState == osKernelReady) {
        state = osKernelReady;
      } else {
        state = osKernelInactive;
      }
  }
  return (state);
}

//开始启动内核调度
osStatus_t osKernelStart (void) {
  osStatus_t stat;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else {
    if (KernelState == osKernelReady) {
      /* Ensure SVC priority is at the reset value */
      SVC_Setup();
      /* Change state to enable IRQ masking check */
      KernelState = osKernelRunning;
      /* Start the kernel scheduler */
      OSStart();
      stat = osOK;
    } else {
      stat = osError;
    }
  }
  return (stat);
}

//调度器上锁
int32_t osKernelLock (void) {
  int32_t lock;

  if (IS_IRQ()) {
    lock = (int32_t)osErrorISR;
  }
  else {
    if(OSRunning==OS_TRUE)
    {
      lock=0;
      OSSchedLock();
    }
    else
    {
      lock = 1;
      if(KernelState==osKernelInactive)
      {
        lock = (int32_t)osError;
      }
    }
  }
   return (lock);
}

//调度器解锁
int32_t osKernelUnlock (void) {
  int32_t lock;

  if (IS_IRQ()) {
    lock = (int32_t)osErrorISR;
  }
  else {
    if(OSRunning==OS_FALSE)
    {
      if(KernelState==osKernelInactive)
      {
        lock = (int32_t)osError;
      }
      else
      {
       lock=1;
       OSSchedUnlock();
      }

    }
    else
    {
      lock = 0;
    }
  }
  return (lock);
}


//恢复先前的锁状态
int32_t osKernelRestoreLock (int32_t lock) {

  if (IS_IRQ()) {
    lock = (int32_t)osErrorISR;
  }
  else {

    if(OSRunning==OS_TRUE||(OSRunning==OS_FALSE&&KernelState!=osKernelInactive))
    {
        if (lock == 1) {
          OSSchedLock();
        }
        else{
            OSSchedUnlock();
            if(OSRunning!=OS_TRUE)
            {
              lock = (int32_t)osError;  
            }
        }
    }
    else
    {
       lock = (int32_t)osError;  
    }
  }
  return (lock);
}

//得到开机以来系统计数器的值
uint32_t osKernelGetTickCount (void) {
  uint32_t ticks;
  ticks=OSTimeGet();
  return (ticks);
}
//得到系统频率
uint32_t osKernelGetTickFreq (void) {
  return (OS_TICKS_PER_SEC);
}

/* Get OS Tick count value */
static uint32_t OS_Tick_GetCount (void) {
  uint32_t load = SysTick->LOAD;
  return  (load - SysTick->VAL);
}

/* Get OS Tick overflow status */
static uint32_t OS_Tick_GetOverflow (void) {
  return ((SysTick->CTRL >> 16) & 1U);
}

/* Get OS Tick interval */
static uint32_t OS_Tick_GetInterval (void) {
  return (SysTick->LOAD + 1U);
}

uint32_t osKernelGetSysTimerCount (void) {
  uint32_t irqmask = IS_IRQ_MASKED();
  uint32_t ticks;
  uint32_t val;

  __disable_irq();

  ticks = OSTimeGet();
  val   = OS_Tick_GetCount();

  if (OS_Tick_GetOverflow() != 0U) {
    val = OS_Tick_GetCount();
    ticks++;
  }
  val += ticks * OS_Tick_GetInterval();

  if (irqmask == 0U) {
    __enable_irq();
  }

  return (val);
}

//得到系统内核的原始频率
uint32_t osKernelGetSysTimerFreq (void) {
  return (SystemCoreClock);
}

/*
线程管理相关

ucosii在新29300版本中虽然增加了 eventFlag 功能 但是没有单独的threadFlag功能

在CMSIS-RTOS接口中 拥有threadFlag功能 在ucosii中，可以通过eventFlag功能功能来实现，
由于ucosii在OS_TCB任务块中 没有相应的threadFlag属性 可以通过扩展TCB属性来实现。
所以我们统一Ucos_ii_extend_tcb为ucosii的tcb扩展类型，今后tcb的扩展全部在此类型上进行修改
*/

osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
  const char *name;
  INT32U stack;
  INT8U prio;
  INT8U rt;
	
	
  OS_TCB *hhTask;
  Ucos_ii_extend_tcb *extcb;
	
  hhTask=NULL;
  extcb=NULL;
	
  OS_TCB  * currentTcb=NULL;

  if (!IS_IRQ() && (func != NULL)) {
    stack = OS_TASK_MIN_STK_SIZE;
    prio  = (INT8U)osPriorityNormal;
	
    name = NULL;
    rt  = OS_ERR_NONE;

    if (attr != NULL) {
      if (attr->name != NULL) {
        name = attr->name;
      }
      if (attr->priority != osPriorityNone) {
        prio = (INT8U)attr->priority;
      }

      if ((prio < osPriorityIdle) || (prio > osPriorityISR) || ((attr->attr_bits & osThreadJoinable) == osThreadJoinable)) {
        return (NULL);
      }

      if (attr->stack_size > 0U) {
        stack = attr->stack_size / sizeof(OS_STK);
      }
	
	  prio=prioMapping[prio];
	  
	  extcb=(Ucos_ii_extend_tcb*)OSMemGet(defaultMemPool,&rt);
	  
	  if(extcb==NULL)
	  {
		  return NULL;
	  }
	  
	  //栈指针不为空 且栈大小不为零  代表静态创建
//      if ((attr->stack_mem != NULL) && (attr->stack_size >  0U) &&
//		  (attr->cb_mem != NULL) && (attr->cb_size > 0U)) {
		if ((attr->stack_mem != NULL) && (attr->stack_size >  0U)){
#if (OS_TASK_CREATE_EXT_EN == 1)	
			  //将任务控制块转换为 ucosii格式
			  
			//hhTask=(UCos_ii_tcb*)attr->cb_mem;
			  
			rt = OSTaskCreateExt(func, //指向任务代码的指针
							argument,//指向参数区（很多时候为0）
							&((OS_STK*)attr->stack_mem)[stack-1],//堆栈向高地址增长（指向任务堆栈空间的最高地址）
							prio,//优先级
							prio,//id任务的标识，可以设置同优先级
							&((OS_STK*)attr->stack_mem)[0],//指向堆栈底端的指针
							stack,//任务栈的大小
							extcb,//TCB扩展
							(OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP));//允许堆栈检查	
			 if(rt!=OS_ERR_NONE)
			 {
				 OSMemPut(defaultMemPool,extcb);
				 return NULL;
			 }
			 //把创建任务产生的 TCB 给传送出去
			 OS_ENTER_CRITICAL();
			 hhTask = OSTCBPrioTbl[prio];
			 if(hhTask== NULL|| hhTask== OS_TCB_RESERVED){
					OS_EXIT_CRITICAL();
					OSMemPut(defaultMemPool,extcb);
					return NULL;
			 }
			 OS_EXIT_CRITICAL(); 

			//创建一个事件标志组 和扩展tcb绑定 当作任务标志功能使用
			 extcb->task_flag_grp=OSFlagCreate (0x00, &rt);
			 if(rt!=OS_ERR_NONE)
			 {
				
				 OSTaskDel(prio);
				 OSMemPut(defaultMemPool,extcb);
				 return NULL;
			 }
#endif		 //ucosii 29300版本带有name属性
			 OSTaskNameSet(prio,(INT8U *)(void *)name,&rt); 
			 if(rt!=OS_ERR_NONE)
			 {
				  
				 OSTaskDel(prio);
				 OSMemPut(defaultMemPool,extcb);
				 return NULL;
			 }

	  }//否则 动态创建
	  else if((attr->cb_mem == NULL) && (attr->cb_size == 0U) && (attr->stack_mem == NULL)){

		//暂未实现动态任务创建	
		 OSMemPut(defaultMemPool,extcb);
		return NULL;
	  }
	}//attr为空
    else {
		return NULL;
    }
	}//中断或者任务为空
  else {
	return NULL;
  }
  return ((osThreadId_t)hhTask);
}

/*
下列函数大量的使用了
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;

这样的语句，原因在于ucosii大量的系统API都是使用prio作为参数使用的，
我们当然可以直接通过OS_TCB*指针来直接进行修改 这样做速度会更快，但是官方既然已提供了API，还是使用API更安全。
如果觉得使用API较为间接 速度慢 可以使用指针来实现，但要注意资源互斥的问题。
*/
//得到线程名字
const char *osThreadGetName (osThreadId_t thread_id) {

	INT8U *name=NULL;
	INT8U rt;
	INT8U slen;
	
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
	
	if (IS_IRQ() || (hTask == NULL)) {
		name = NULL;
	}
	else
	{
		slen=OSTaskNameGet(hTask,&name,&rt);
		if(slen&&rt==OS_ERR_NONE)
		{
			
		}
		else
		{
			name=NULL;
		}		
	}
  return (char*)(name);
}
//得到线程控制块
osThreadId_t osThreadGetId (void) {
  osThreadId_t id;
  id = (osThreadId_t)&OSTCBCur->OSTCBId;
  return (id);
}
//得到线程状态
osThreadState_t osThreadGetState (osThreadId_t thread_id) {
  OS_TCB  currentTcb={0};
  osThreadState_t state;
	
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  INT8U rt;
	
  if (IS_IRQ() || (hTask == 0)) {
    state = osThreadError;
	
  }
  else
  {
	rt=OSTaskQuery(hTask,&currentTcb);
	if(rt==OS_ERR_NONE)
	{
		if(currentTcb.OSTCBStat==OS_STAT_RDY)
		{
			state=osThreadReady;
		}
		else
		{
			state = osThreadBlocked;
		}
	}
	else
	{
	  state = osThreadError;
	}
  }
  

  return (state);
}

//得到线程栈的空闲大小
uint32_t osThreadGetStackSpace (osThreadId_t thread_id) {
  OS_STK_DATA   pdata={0};
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  uint32_t sz;
  INT8U rt;
	
  if (IS_IRQ() || (hTask == 0)) {
	sz=0;    	
  }
  else
  {
	rt=OSTaskStkChk(hTask,&pdata);
	if(rt==OS_ERR_NONE)
	{
		sz=(pdata.OSFree);
	}
	else
	{
	  sz=0;
	}
  }
  return (sz);
}
//设定线程的优先级
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority) {
	
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
	
  osStatus_t stat;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if ((hTask == NULL) || (priority < osPriorityIdle) || (priority > osPriorityISR)) {
    stat = osErrorParameter;
  }
  else {
			
    if(OSTaskChangePrio (hTask,prioMapping[(INT8U)priority])==OS_ERR_NONE)
	{
		hTask=prioMapping[(INT8U)priority];
		stat = osOK;
	}
	else
	{
		stat = osErrorParameter;
	}
  }

  return (stat);
}
//得到线程的优先级
osPriority_t osThreadGetPriority (osThreadId_t thread_id) {
  OS_TCB  currentTcb={0};
  
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  
  osPriority_t prio;
  INT8U rt;
  if (IS_IRQ() || (hTask == NULL)) {
    prio = osPriorityError;
  } else {
	  rt=OSTaskQuery(hTask,&currentTcb);
	  if(rt==OS_ERR_NONE)
	  {
		prio = (osPriority_t)((int32_t)currentTcb.OSTCBPrio);
		prio=(osPriority_t)PrioToCmsisRtos(prio);
	  }
	  else
	  {
		  prio = (osPriority_t)0;
	  }
    
  }
  return ((prio));
//	//对应ucosii 来讲 优先级和id绑定
//	return *(INT8U*)thread_id;
}

#if (OS_TASK_SUSPEND_EN > 0u)

//线程挂起
osStatus_t osThreadSuspend (osThreadId_t thread_id) {
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  osStatus_t stat;
  INT8U rt;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTask == NULL) {
    stat = osErrorParameter;
  }
  else {
    
    rt=OSTaskSuspend (hTask);
	  
	if(rt==OS_ERR_NONE)
	{
		stat = osOK;
	}
	else
	{
		stat = osErrorParameter;
	}
  }

  return (stat);
}

//线程恢复
osStatus_t osThreadResume (osThreadId_t thread_id) {
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  osStatus_t stat;
  INT8U rt;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTask == NULL) {
    stat = osErrorParameter;
  }
  else {
    
    rt=OSTaskResume (hTask);
	  
	if(rt==OS_ERR_NONE)
	{
		stat = osOK;
	}
	else
	{
		stat = osErrorParameter;
	}
  }

  return (stat);
}
#endif /* (configUSE_OS2_THREAD_SUSPEND_RESUME == 1) */

__NO_RETURN void osThreadExit (void) {
  OSTaskDel(OS_PRIO_SELF);
  for (;;);
}

//这里是针对 静态任务而言可以这样使用
//如果是动态任务或者使用了动态内存 要借助ucosii的OSTaskDelSeq来实现 资源回收 （暂未实现）

//删除一个线程
osStatus_t osThreadTerminate (osThreadId_t thread_id) {
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
  osStatus_t stat;

  INT8U rt;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTask == NULL) {
    stat = osErrorParameter;
  }
  else {
	 //回收我们创建任务时 创建的线程标志
	  OSFlagDel(((Ucos_ii_extend_tcb*)hhTask->OSTCBExtPtr)->task_flag_grp ,OS_DEL_ALWAYS,&rt);
	  //回收我们创建任务时 申请的	OSTCBExtPtr 空间
	 OSMemPut(defaultMemPool,hhTask->OSTCBExtPtr);
     rt= OSTaskDel(hTask);
	 if(rt==OS_ERR_NONE)
	 {
		stat = osOK;
	 }
	 else
	 {
		stat = osErrorParameter;
	 }	
   }
  
  stat = osError;

  return (stat);
}

//得到目前任务数量
uint32_t osThreadGetCount (void) {
  uint32_t count;

  if (IS_IRQ()) {
    count = 0U;
  } else {
    count = OSTaskCtr;
  }

  return (count);
}

//枚举线程列表 （未实现）
//uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items) {
// 
//}


/*
线程标志组 
//置位线程标志组
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags);
//清除线程标志组
uint32_t osThreadFlagsClear (uint32_t flags);
//查询线程标志组
uint32_t osThreadFlagsGet (void);
//等待线程标志组被置位
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout);
*/


		
#if (OS_FLAG_EN == 1)
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags)
{
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_TCB * hhTask = (OS_TCB *)thread_id;
  INT8U hTask=hhTask->OSTCBId;
	uint32_t rflags;
	
	OS_TCB    *ptcb=NULL;
	INT8U rt;
	if ((hTask == NULL||hTask>OS_LOWEST_PRIO) || ((flags & THREAD_FLAGS_INVALID_BITS) != 0U)) {
		rflags = (uint32_t)osErrorParameter;
	}
	else {
		rflags = (uint32_t)osError;
//		if (IS_IRQ()) { //ucosii中OSFlagpost()可在中断中使用
//			rflags=(uint32_t)osErrorISR;
//		}
//		else {
			OS_ENTER_CRITICAL();

			ptcb = OSTCBPrioTbl[hTask];
			
			if(ptcb== (OS_TCB *)0||ptcb==OS_TCB_RESERVED){
				OS_EXIT_CRITICAL(); 
				rflags=(uint32_t)osErrorParameter;
			}
			else{
				OS_EXIT_CRITICAL();	
				rflags=OSFlagPost(((Ucos_ii_extend_tcb*)ptcb->OSTCBExtPtr)->task_flag_grp,flags,OS_FLAG_SET,&rt);
				if(rt!=OS_ERR_NONE)
				{
					rflags=(uint32_t)osError;
				}
			}
		//}
	}
  /* Return flags after setting */
  return (rflags);
}

	

uint32_t osThreadFlagsClear (uint32_t flags)
{
	
	uint32_t rflags;
	INT8U rt;
	if (IS_IRQ()) {
		rflags = (uint32_t)osErrorISR;
	}
	else if ((flags & THREAD_FLAGS_INVALID_BITS) != 0U) {
		rflags = (uint32_t)osErrorParameter;
	}
	else {
		rflags=OSFlagPost(((Ucos_ii_extend_tcb*)OSTCBCur->OSTCBExtPtr)->task_flag_grp,flags,OS_FLAG_CLR,&rt);
		if(rt!=OS_ERR_NONE)
		{
			rflags=(uint32_t)osError;
		}		
	}
	  /* Return flags before clearing */
	 return (rflags);
}

uint32_t osThreadFlagsGet (void) {

	uint32_t rflags;
	INT8U rt;
  if (IS_IRQ()) {
    rflags = (uint32_t)osErrorISR;
  }
  else {
	rflags=OSFlagQuery(((Ucos_ii_extend_tcb*)OSTCBCur->OSTCBExtPtr)->task_flag_grp,&rt);
	if(rt!=OS_ERR_NONE)
	{
		rflags=(uint32_t)osError;
	}
	
  }

  return (rflags);
}

uint32_t osThreadFlagsWait (uint32_t flags,uint32_t options, uint32_t timeout)
{
	uint32_t rflags;
	OS_TCB    *ptcb=NULL;
	INT8U rt;
	if (IS_IRQ()) {
		rflags = (uint32_t)osErrorISR;
	}
	else if ((flags & THREAD_FLAGS_INVALID_BITS) != 0U) {
		rflags = (uint32_t)osErrorParameter;
	}
	else {
			//将CMSIS-RTOS的opt转换为ucosii的opt
			//CMSIS-RTOS的线程标志 等待后默认不清除 而ucos事件组默认清除 需要对应
			if((options&osFlagsWaitAny)==osFlagsWaitAny)
			{
				if((options&=osFlagsNoClear)==osFlagsNoClear)
					options=OS_FLAG_WAIT_SET_ANY;
				else
					options=OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME;
			}
			else if((options&osFlagsWaitAll)==osFlagsWaitAll)
			{
				if((options&=osFlagsNoClear)==osFlagsNoClear)
					options=OS_FLAG_WAIT_SET_ALL;
				else
					options=OS_FLAG_WAIT_SET_ALL|OS_FLAG_CONSUME;
			}
			rflags=OSFlagPend(((Ucos_ii_extend_tcb*)OSTCBCur->OSTCBExtPtr)->task_flag_grp,
																				flags,
																				options,
																				timeout,
																				&rt);
			if(rt!=OS_ERR_NONE)
			{
				if(rt==OS_ERR_PEND_ISR)
					rflags=(uint32_t)osErrorISR;
				else if(rt==OS_ERR_TIMEOUT)
					rflags=(uint32_t)osErrorTimeout;
				else
					rflags=(uint32_t)osError;
			}	
	}
	  /* Return flags before clearing */
	 return (rflags);
}
#endif

osStatus_t osDelay (uint32_t ticks) {
  osStatus_t stat;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else {
    stat = osOK;

    if (ticks != 0U) {
      OSTimeDly(ticks);
    }
  }

  return (stat);
}

//osStatus_t osDelayUntil (uint32_t ticks);


/*
软件定时器相关

ucosii软件定时器较为简易，为了不改动ucosii源代码的前提下和CMSIS-RTOS接口保持一致
定义一个共用的回调函数TimerCallback（） 其参数parg用作 用户的回调函数

*/

#if (OS_TMR_EN == 1)

static void TimerCallback (void *ptmr, void *parg) {
  TimerCallback_t  *callb;

  callb = (TimerCallback_t *)parg;

  if (callb != NULL) {
    callb->func (callb->arg);
  }
}

//创建一个定时器
//注意 需要动态的申请TimerCallback_t结构体
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr) {
  const char *name;
  INT8U rt;
  OS_TMR* hTimer;
  TimerCallback_t *callb;
  INT8U  reload;
  int32_t mem;

  hTimer = NULL;

  if (!IS_IRQ() && (func != NULL)) {
    /* Allocate memory to store callback function and argument */
    callb = (TimerCallback_t*)OSMemGet (defaultMemPool,&rt);

    if (callb != NULL) {
      callb->func = func;
      callb->arg  = argument;

      if (type == osTimerOnce) {
        reload = OS_TMR_OPT_ONE_SHOT;
      } else {
        reload = OS_TMR_OPT_PERIODIC;
      }

      mem  = -1;
      name = NULL;

      if (attr != NULL) {
        if (attr->name != NULL) {
          name = attr->name;
        }

//        if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(OS_TMR))) {
//          mem = 1;
//        }
//        else {
//          if ((attr->cb_mem == NULL) && (attr->cb_size == 0U)) {
//            mem = 0;
//          }
//        }
//      }
//      else {
//        mem = 0;
      }
		//ucosii 定时器都是静态创建
	  mem  = 1;
      if (mem == 1) {
        #if (configSUPPORT_STATIC_ALLOCATION == 1)
          hTimer = xTimerCreateStatic (name, 1, reload, callb, TimerCallback, (StaticTimer_t *)attr->cb_mem);
		 #endif 
		  //创建定时器的时候 不设置dly和period
		  hTimer=OSTmrCreate(1,1,reload,TimerCallback,callb,(INT8U*)name,&rt);
        
      }
      else {
        if (mem == 0) {
			//暂未实现动态定时器创建
        }
      }

      if ((hTimer == NULL) && (callb != NULL)) {
        OSMemPut(defaultMemPool,callb);
      }
    }
  }

  return ((osTimerId_t)hTimer);
}

//得到定时器的名字 （用处不大可以删除此功能 节省资源）
const char *osTimerGetName (osTimerId_t timer_id) {
  OS_TMR* hTimer = (OS_TMR*)timer_id;
  const char *p;
  INT8U rt;
  if (IS_IRQ() || (hTimer == NULL)) {
    p = NULL;
  } else {
	  
	  OSTmrNameGet(hTimer, (INT8U**)&p, &rt);
	 if (rt != OS_ERR_NONE) {
		 return (NULL);
	 }
  }
  return (p);
}


//启动一个定时器
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks) {
	
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_TMR* hTimer = (OS_TMR*)timer_id;
  osStatus_t stat;
  INT8U rt;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTimer == NULL) {
    stat = osErrorParameter;
  }
  else {
	OS_ENTER_CRITICAL();
	//一次
	if(hTimer->OSTmrOpt==OS_TMR_OPT_ONE_SHOT)
	{
		hTimer->OSTmrDly=ticks;
	}
	//周期
	else if(hTimer->OSTmrOpt==OS_TMR_OPT_PERIODIC)
	{
		hTimer->OSTmrDly=ticks;
		hTimer->OSTmrPeriod=ticks;
	}
	OS_EXIT_CRITICAL();	
    if (OSTmrStart (hTimer, &rt) == OS_TRUE) {
      stat = osOK;
    } else {
      stat = osErrorResource;
    }
  }

  return (stat);
}

//停止一个定时器
osStatus_t osTimerStop (osTimerId_t timer_id) {
	
  OS_TMR* hTimer = (OS_TMR*)timer_id;
  osStatus_t stat;
  INT8U rt;
  INT8U  tmrState;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTimer == NULL) {
    stat = osErrorParameter;
  }
  else {

	   tmrState=OSTmrStateGet(hTimer,&rt);
	   if (rt == OS_ERR_NONE) {
		   if (tmrState!=OS_TMR_STATE_RUNNING){
			  stat = osErrorResource;
		   }
		   else
		   {
				OSTmrStop(hTimer,
						  OS_TMR_OPT_NONE,
						  (void *)0,
						  &rt);
				if (rt == OS_ERR_NONE || rt == OS_ERR_TMR_STOPPED) {
					stat = osOK;
				}	
				else{
					stat = osError;
				}
		   }
	   }
	   else
	   {
			stat = osError;
	   }

  }

  return (stat);
}


//得到定时器的运行状态
uint32_t osTimerIsRunning (osTimerId_t timer_id) {
  OS_TMR* hTimer = (OS_TMR*)timer_id;
  INT8U rt;
  INT8U  tmrState;
	
  uint32_t running;

  if (IS_IRQ() || (hTimer == NULL)) {
    running = 0U;
  } else {
		tmrState=OSTmrStateGet(hTimer,&rt);
	    if (rt == OS_ERR_NONE) {
			
			if(tmrState==OS_TMR_STATE_RUNNING)
			{
				running=1;
			}
			else
			{
				running=0;
			}	  
	   }
		else
		{
			running = 0U;
		}
  }

  return (running);
}

//删除一个定时器 注意需要释放创建时候申请的结构体TimerCallback_t
osStatus_t osTimerDelete (osTimerId_t timer_id) {
	
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_TMR* hTimer = (OS_TMR*)timer_id;
  osStatus_t stat;
  INT8U rt;
  TimerCallback_t *callb;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hTimer == NULL) {
    stat = osErrorParameter;
  }
  else {
	OS_ENTER_CRITICAL();  
    callb = (TimerCallback_t *)hTimer->OSTmrCallbackArg;

    if (OSTmrDel (hTimer, &rt) == OS_TRUE) {
		OSMemPut(defaultMemPool,callb);
		stat = osOK;
    } else {
      stat = osErrorResource;
    }
	OS_EXIT_CRITICAL();	
  }

  return (stat);
}
#endif /* (configUSE_OS2_TIMER == 1) */

/*---------------------------------------------------------------------------*/

/*
//普通的事件标志组，线程标志组就是用其实现的，相比消息邮箱 队列 更节省资源
事件标志相关
*/

osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr) {
	
  OS_FLAG_GRP* hEventGroup;
  const char *name=NULL;
  hEventGroup = NULL;
  INT8U rt;

  if (!IS_IRQ()) {

		if (attr != NULL) {
			if (attr->name != NULL) {
				name = attr->name;
			}
	
		}	
		hEventGroup=OSFlagCreate(0x00,&rt);
		if(rt!=OS_ERR_NONE)
		{
			return NULL;
		}
		
		OSFlagNameSet(hEventGroup,(INT8U*)name,&rt);
	}
    else {
		//不支持中断调用
		hEventGroup=NULL;
    }

  return ((osEventFlagsId_t)hEventGroup);
}


uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags) {
  OS_FLAG_GRP* hEventGroup = (OS_FLAG_GRP*)ef_id;
  uint32_t rflags;
  INT8U rt;
  if ((hEventGroup == NULL) || ((flags & EVENT_FLAGS_INVALID_BITS) != 0U)) {
    rflags = (uint32_t)osErrorParameter;
  }
  else  {//OSFlagPost支持ISR调用
	rflags=OSFlagPost(hEventGroup,flags,OS_FLAG_SET,&rt);
	if(rt!=OS_ERR_NONE)
	{
		rflags=(uint32_t)osError;
	}
  }
  return (rflags);
}


uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags) {
  OS_FLAG_GRP* hEventGroup = (OS_FLAG_GRP*)ef_id;
  uint32_t rflags;
  INT8U rt;

  if ((hEventGroup == NULL) || ((flags & EVENT_FLAGS_INVALID_BITS) != 0U)) {
    rflags = (uint32_t)osErrorParameter;
  }
  else {
	rflags=OSFlagPost(hEventGroup,flags,OS_FLAG_CLR,&rt);
	if(rt!=OS_ERR_NONE)
	{
		rflags=0;
	}
  }

  return (rflags);
}
		

uint32_t osEventFlagsGet (osEventFlagsId_t ef_id) {
  OS_FLAG_GRP* hEventGroup = (OS_FLAG_GRP*)ef_id;
  uint32_t rflags;
  INT8U rt;

  if (ef_id == NULL) {
    rflags = 0U;
  }
  else {//OSFlagQuery也可以在ISR中调用
	rflags=OSFlagQuery(hEventGroup,&rt);
	if(rt!=OS_ERR_NONE)
	{
		rflags=(uint32_t)osError;
	}
  }

  return (rflags);
}




uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout) {
	uint32_t rflags;
	OS_FLAG_GRP* hEventGroup = (OS_FLAG_GRP*)ef_id;
	INT8U rt;
	if (IS_IRQ()) {
		rflags = (uint32_t)osErrorISR;
	}
	else if ((flags & THREAD_FLAGS_INVALID_BITS) != 0U) {
		rflags = (uint32_t)osErrorParameter;
	}
	else {
			//将CMSIS-RTOS的opt转换为ucosii的opt
			//CMSIS-RTOS的线程标志 等待后默认不清除 而ucos事件组默认清除 需要对应
			if((options&osFlagsWaitAny)==osFlagsWaitAny)
			{
				if((options&=osFlagsNoClear)==osFlagsNoClear)
					options=OS_FLAG_WAIT_SET_ANY;
				else
					options=OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME;
			}
			else if((options&osFlagsWaitAll)==osFlagsWaitAll)
			{
				if((options&=osFlagsNoClear)==osFlagsNoClear)
					options=OS_FLAG_WAIT_SET_ALL;
				else
					options=OS_FLAG_WAIT_SET_ALL|OS_FLAG_CONSUME;
			}
			rflags=OSFlagPend(hEventGroup,
									flags,
									options,
									timeout,
									&rt);
			if(rt!=OS_ERR_NONE)
			{
				if(rt==OS_ERR_PEND_ISR)
					rflags=(uint32_t)osErrorISR;
				else if(rt==OS_ERR_TIMEOUT)
					rflags=(uint32_t)osErrorTimeout;
				else
					rflags=(uint32_t)osError;
				
			}	
			else
			{
				rflags=osOK;
			}
	}
	  /* Return flags before clearing */
	return (rflags);
}

osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id) {
	OS_FLAG_GRP* hEventGroup = (OS_FLAG_GRP*)ef_id;
	osStatus_t stat;
	INT8U rt;
	if (IS_IRQ()) {
		stat = osErrorISR;
	}
	else if (hEventGroup == NULL) {
		stat = osErrorParameter;
	}
	else {
		
		//OS_DEL_NO_PEND 如果有任务等待该标志 不删除
		OSFlagDel(hEventGroup,OS_DEL_NO_PEND,&rt);
		if(rt!=OS_ERR_NONE)
		{
			if(rt==OS_ERR_TASK_WAITING)
				stat=osErrorResource;
			else
				stat=osError;
		}
		else
		{
			stat = osOK;
		}
	}
	return (stat);
}

//互斥量相关
//使用互斥量的好处是可以解决优先级反转的问题
//在ucosii中 实现优先级继承的原理是 空闲出一定的高优先级的prio留给优先级继承来使用
//在低优先级任务使用互斥资源时 临时的将其优先级调高 完成任务后再恢复
//这里使用了0-7这8个优先级来 供互斥量使用
//每一位代表优先级继承的使用状况 
static uint8_t mutexPrioFlag=0x00;

#if (OS_MUTEX_EN == 1)

//创建一个互斥量
osMutexId_t osMutexNew (const osMutexAttr_t *attr) {
  OS_EVENT * hMutex;
  uint32_t type;
  int8_t prio;
  int32_t  mem;

  hMutex = NULL;
  INT8U rt;
  if (!IS_IRQ()) {
    if (attr != NULL) {
		type = attr->attr_bits;//由于ucosii的互斥量较为简单仅且默认支持优先级继承互斥量
								//所以type对我们来讲没有太大意义 但还是要甄别下
    } else {
      type = 0U;
    }
	
	if((type & osMutexRecursive) == osMutexRecursive)
	{
		return NULL;
	}
	if((type & osMutexRobust) == osMutexRobust)
	{
		return NULL;
	}
	//如果不支持优先级继承
	if((type & osMutexPrioInherit) != osMutexPrioInherit)
	{
		return NULL;
	}		
	//互斥量已满
	if(mutexPrioFlag==0xFF)
	{
		return NULL;
	}
	
	if(!(mutexPrioFlag&0x01))
		prio=0;
	else if(!(mutexPrioFlag&0x02))
		prio=1;
	else if(!(mutexPrioFlag&0x04))
		prio=2;
	else if(!(mutexPrioFlag&0x08))
		prio=3;
	else if(!(mutexPrioFlag&0x10))
		prio=4;
	else if(!(mutexPrioFlag&0x20))
		prio=5;
	else if(!(mutexPrioFlag&0x40))
		prio=6;	
	else if(!(mutexPrioFlag&0x80))
		prio=7;	
	else
	{
		
		return NULL;
	}   
		
	  hMutex=OSMutexCreate(prio, &rt);
	  if(rt!=OS_ERR_NONE)
	  {
		  return NULL;
	  }
  }

  return ((osMutexId_t)hMutex);
}


osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout) {
  OS_EVENT *  hMutex;
  osStatus_t stat;
  INT8U rt;
  hMutex = (OS_EVENT *)(mutex_id);

  stat = osOK;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
    
    OSMutexPend(hMutex,timeout,&rt);
	if(rt!=OS_ERR_NONE)
	{
		if(rt==OS_ERR_TIMEOUT)
			stat=osErrorTimeout;
		else
			stat=osError;		
	}
  }

  return (stat);
}

osStatus_t osMutexRelease (osMutexId_t mutex_id) {
  OS_EVENT *  hMutex;
  osStatus_t stat;
  INT8U rt;
  hMutex = (OS_EVENT *)(mutex_id);

  stat = osOK;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
    rt=OSMutexPost(hMutex);
	if(rt!=OS_ERR_NONE)
	{
		if(rt==OS_ERR_NOT_MUTEX_OWNER)
			stat=osErrorResource;
		else
			stat=osError;		
	}
  }
  return (stat);
}

osThreadId_t osMutexGetOwner (osMutexId_t mutex_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_EVENT *  hMutex;
  osThreadId_t owner;
  OS_MUTEX_DATA mutex_data;
  INT8U rt;
  hMutex = (OS_EVENT *)(mutex_id);

  owner=NULL;
  if (IS_IRQ() || (hMutex == NULL)) {
    owner = NULL;
  } else {
	 
	rt=OSMutexQuery(hMutex,&mutex_data);
	if(rt==OS_ERR_NONE)
	{
		OS_ENTER_CRITICAL();			
		owner=(osThreadId_t)OSTCBPrioTbl[mutex_data.OSOwnerPrio];
		OS_EXIT_CRITICAL();	
	}
    
  }

  return (owner);
}

osStatus_t osMutexDelete (osMutexId_t mutex_id) {
  OS_EVENT * hMutex;
  uint32_t type;
  int8_t prio;
  int32_t  mem;
  osStatus_t stat;
  hMutex = (OS_EVENT *)(mutex_id);
  INT8U rt;
  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
	 prio=(hMutex->OSEventCnt>>8)&0x0F;
	 hMutex = OSMutexDel(hMutex, OS_DEL_ALWAYS, &rt);
	 if(rt!=OS_ERR_NONE)
	 {
			stat=osError;
		 
	 }
	 else
	 {
		 if(prio==0)
			mutexPrioFlag=mutexPrioFlag&(~0x01);
		 else if(prio==1)
			mutexPrioFlag=mutexPrioFlag&(~0x02);
		 else if(prio==2)
			mutexPrioFlag=mutexPrioFlag&(~0x04);
		 else if(prio==3)
			mutexPrioFlag=mutexPrioFlag&(~0x08);
		 else if(prio==4)
			mutexPrioFlag=mutexPrioFlag&(~0x10);
		 else if(prio==5)
			mutexPrioFlag=mutexPrioFlag&(~0x20);
		 else if(prio==6)
			mutexPrioFlag=mutexPrioFlag&(~0x40);
		 else if(prio==7)
			mutexPrioFlag=mutexPrioFlag&(~0x80);
	 }
  }


  return (stat);
}
#endif /* (configUSE_OS2_MUTEX == 1) */

/*---------------------------------------------------------------------------*/

//信号量相关
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  OS_EVENT * hSemaphore;
  hSemaphore = NULL;

  (void)attr;
  if (!IS_IRQ() && (max_count > 0U) && (initial_count <= max_count)) {
			hSemaphore=OSSemCreate(initial_count);
  }
  return ((osSemaphoreId_t)hSemaphore);
}


osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
	
  OS_EVENT * hSemaphore;
  hSemaphore = (OS_EVENT *)semaphore_id;
  osStatus_t stat;
  INT8U rt;
  stat = osOK;

  if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else if (IS_IRQ()) {
      stat = osErrorISR;
  }
  else {
	OSSemPend(hSemaphore,timeout,&rt);
    if (rt != OS_ERR_NONE) {
		  if (rt == OS_ERR_TIMEOUT) {
				stat = osErrorTimeout;
		  } 
		  else {
				stat = osError;
		  }
    }
  }

  return (stat);
}


osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  OS_EVENT * hSemaphore;
  hSemaphore = (OS_EVENT *)semaphore_id;
  osStatus_t stat;
  stat = osOK;

  if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else
  {	  
	  if(OSSemPost(hSemaphore)!=OS_ERR_NONE)
	 {
		stat=(osStatus_t)osError;
	 }
  }
  
  return (stat);
}

uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  OS_EVENT * hSemaphore;
  hSemaphore = (OS_EVENT *)semaphore_id;
  osStatus_t stat;
  INT8U rt;
  OS_SEM_DATA  qdata;
  stat = osOK;
	
  uint32_t count;

  if (hSemaphore == NULL) {
    count = 0U;
  }	 
  else 
  {
	  rt = OSSemQuery(hSemaphore, &qdata);
	   if (rt == OS_ERR_NONE) {
         count=  qdata.OSCnt;
      }
	   else
	   {
		   count=0;
	   }
  }

  return (count);
}


osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id) {
 OS_EVENT * hSemaphore;
  hSemaphore = (OS_EVENT *)semaphore_id;
  osStatus_t stat;
  INT8U rt;

  if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else {
    stat = osOK;
	hSemaphore = OSSemDel(hSemaphore, OS_DEL_ALWAYS, &rt);
     if(rt!=OS_ERR_NONE)
	 {
			stat=osError;
		 
	 }
  }
  return (stat);
}
/*
//消息队列
//这里需要说明下 消息邮箱是队列长度为1的消息队列 所以在CMSIS-RTOSV2中消息邮箱已经被弃用
//但是在ucosii中 对于短消息使用消息邮箱显然比消息队列更节省资源
//所以如果发现队列长度为1就使用消息邮箱方式 否则就使用消息队列 用户无需关心这些 

由于ucosii的消息邮箱和消息队列 的取值没有深拷贝 导致接口与CMSIS_RTOSV2接口无法一致、
并且ucosii所用的事件块结构和邮箱、队列都没有属性去记录消息的字节大小 导致无法容易的拷贝

不过 在事件块结构用于邮箱、队列时候 其OSEventCnt属性 没有被使用 可以使用此属性保存消息的字节大小 方便拷贝

已经弃用深拷贝 原因在于ucosii的队列实现 并不是真正的队列 而是保存了数据的地址，
即使我们在出队的时候做深拷贝也无实际意义，在入队到出队的过程中数据被修改 无法避免
反而如果使用动态内存分配还会因为出队时 数据拷贝丢失了数据的地址 从而无法释放
所以还是采用ucosii的队列风格 单为了保持接口统一 建议为消息队列数据结构使用typedef进行封装:

typedef struct qdata_t
{
	...
}* qdata_t;

并且为了安全起见 只使用动态内存或者全局变量
*/

osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif 
  OS_EVENT * hQueue;
  hQueue = NULL;

  //仅支持静态创建任务
  if (!IS_IRQ() && (msg_count > 0U) && (msg_size > 0U)) {
 
	  //如果只发生一条，就直接使用消息邮箱而不是消息队列
    if(msg_count==1)
	{
		hQueue=OSMboxCreate((void*)0);
	}
	else
	{
		//如果是消息队列 必须从外部传来队列数组
		if ((attr->mq_mem != NULL) && (attr->mq_size >= (msg_count * msg_size)))
		{
			hQueue=OSQCreate(attr->mq_mem,msg_count);
		}
	}
	
	if(hQueue!=NULL)
	{
		OS_ENTER_CRITICAL();
		hQueue->OSEventCnt=msg_size;
		OS_EXIT_CRITICAL();	
	}

  }
  return ((osMessageQueueId_t)hQueue);
}


osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  void **ptr=NULL;
  osStatus_t stat;
  INT8U rt;
  (void)timeout;//超时时间被忽略

   stat = osOK;
  
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
		ptr=(void**)msg_ptr;
		
		//如果是消息邮箱
		if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
		{
			rt= OSMboxPost(hQueue, (void *)*ptr);
		}
		else
		{
			//如果存在优先级 就后进先出
				if(msg_prio!=0)
				{
				
					rt= OSQPost(hQueue, (void *)*ptr);
				}
				else
				{
					rt= OSQPostFront(hQueue, (void *)*ptr);
				}
				
				
		}
		
		if(rt!=OS_ERR_NONE)
		{
			if(rt==OS_ERR_MBOX_FULL||rt==OS_ERR_Q_FULL)
			{
				stat = osErrorResource;
			}
			else
			{
				stat=osError;
			}
		}
    }
 
  return (stat);
}

osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  void **ptr=NULL;
  void *pt=NULL;
  osStatus_t stat;
  INT8U rt;

  (void)msg_prio; /* Message priority is ignored */

  stat = osOK;
	if(IS_IRQ())
	{
		stat = osErrorISR;
	}
	else
	{
		if ((hQueue == NULL) || (msg_ptr == NULL)) {
				  stat = osErrorParameter;
		}
		else {
			//如果是消息邮箱
				
			if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
			{
				ptr=(void**)msg_ptr;
				*ptr=OSMboxPend(hQueue, timeout, &rt);
				
				
//				pt=OSMboxPend(hQueue, timeout, &rt);
//				OS_ENTER_CRITICAL();	
//				memcpy(msg_ptr, pt, hQueue->OSEventCnt);
//				OS_EXIT_CRITICAL();	
			}
			else
			{
				
				ptr=(void**)msg_ptr;
				*ptr=OSQPend(hQueue, timeout, &rt);
				
				
				//printf("pend=%d\r\n",((pQueuetest)*ptr)->d);
//				pt=OSQPend(hQueue, timeout, &rt);
//				OS_ENTER_CRITICAL();	
//				memcpy(msg_ptr, pt, hQueue->OSEventCnt);
//				OS_EXIT_CRITICAL();	
				
			}
			
			if(rt!=OS_ERR_NONE)
			{
				if(rt==OS_ERR_TIMEOUT)
				{
					stat = osErrorTimeout;
				}
				else
				{
					stat=osError;
				}
			}
		}		
		
	}
	
  return (stat);
}

// 输入
// mq_id : 消息队列ID
// 输出
//  消息队列所能容纳的最大消息个数
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  uint32_t capacity;
  INT8U rt;
  if (hQueue == NULL) {
     capacity = 0U;
  }
  else {
	OS_ENTER_CRITICAL();
	if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
	{
		capacity=1;
	}	
	else
	{
		OS_Q_DATA q_data;
		rt= OSQQuery(hQueue, &q_data);
		if(rt==OS_ERR_NONE)
		{
			capacity=q_data.OSQSize;
		}	
	}
	OS_EXIT_CRITICAL();
  }

  return (capacity);
}
// 输入
// mq_id : 消息队列ID
// 输出
//消息的最大字节数
//ucosii 没有保存消息的字节大小，暂无法实现
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  OS_ENTER_CRITICAL();
  uint32_t size=hQueue->OSEventCnt;				
				
OS_EXIT_CRITICAL();	
  return (size);
}


uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  uint32_t count;
  INT8U rt;
  if (hQueue == NULL) {
     count = 0U;
  }
  else {	  
	OS_ENTER_CRITICAL();
	if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
	{
		OS_MBOX_DATA mbox_data;
		rt=OSMboxQuery(hQueue, &mbox_data);
		if(rt==OS_ERR_NONE)
		{
			if(mbox_data.OSMsg==NULL)
			{
				count=0;	
			}
			else
			{
				count=1;
			}
		}			
	}	
	else
	{
		OS_Q_DATA q_data;
		rt= OSQQuery(hQueue, &q_data);
		if(rt==OS_ERR_NONE)
		{
			count=q_data.OSNMsgs;

		}
		
	}	  
	OS_EXIT_CRITICAL();	
  }

  return ((uint32_t)count);
}

uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
   OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
	
  uint32_t space=0;
  INT8U rt;
if (hQueue == NULL) {
     space = 0U;
  }
  else {
	OS_ENTER_CRITICAL();	
	if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
	{
		OS_MBOX_DATA mbox_data;
		rt=OSMboxQuery(hQueue, &mbox_data);
		if(rt==OS_ERR_NONE)
		{
			if(mbox_data.OSMsg==NULL)
			{
				space=1;	
			}
			else
			{
				space=0;
			}
		}			
	}	
	else
	{
		OS_Q_DATA q_data;
		rt= OSQQuery(hQueue, &q_data);
		if(rt==OS_ERR_NONE)
		{
			space=q_data.OSQSize-q_data.OSNMsgs;

		}
		
	}
	OS_EXIT_CRITICAL();
  }

  return (space);
}

osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  osStatus_t stat;
  INT8U rt;
  //INT8U *OSQFlush (OS_EVENT *pevent);可以在ISR中调用
  if (hQueue == NULL) {
    stat = osErrorParameter;
  }
  else {
    stat = osOK;
	OS_ENTER_CRITICAL();	
	if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
	{
		hQueue->OSEventPtr=NULL;
	}	
	else
	{
		rt=OSQFlush(hQueue);
		if(rt!=OS_ERR_NONE)
		{
			stat=osError;
		}
		
	}
	hQueue->OSEventCnt=0;
	OS_EXIT_CRITICAL();   
  }

  return (stat);
}


osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id) {
  OS_EVENT * hQueue;
  hQueue = (OS_EVENT *)mq_id;
  osStatus_t stat;
  INT8U rt;

  
  if (!IS_IRQ()) {
 
	 //消息邮箱
    if(hQueue->OSEventType==OS_EVENT_TYPE_MBOX)
	{
		hQueue=OSMboxDel(hQueue,OS_DEL_ALWAYS,&rt);
		if(hQueue==NULL&&rt==OS_ERR_NONE)
		{
			stat = osOK;
		}
		else
		{
			stat = osError;
		}

	}
	else
	{
		hQueue=OSQDel(hQueue,OS_DEL_ALWAYS,&rt);
		if(hQueue==NULL&&rt==OS_ERR_NONE)
		{
			stat = osOK;
		}
		else
		{
			stat = osError;
		}
	}

  }
  else
  {
	 stat = osErrorISR; 
  }
  
  return stat;
}

/*
内存管理相关

ucosii的内存管理与CMSIS-RTOSV2的内存管理接口非常契合，能够轻易的实现 
//但是注意ucosii的内存池创建本质上也是静态创建
*/
osMemoryPoolId_t osMemoryPoolNew (uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr) {
  OS_MEM  *mp;
  INT8U rt;

  if (IS_IRQ()) {
    mp = NULL;
  }
  else if ((block_count == 0U) || (block_size == 0U)) {
    mp = NULL;
  }
  else {
    mp = NULL;
	if ((attr->mp_mem == NULL) && (attr->mp_size == 0U)) {
		//暂时不支持动态创建
    }
	else
	{
		mp=OSMemCreate(attr->mp_mem,block_count,block_size ,&rt);
		if(rt!=OS_ERR_NONE)
		{
			mp = NULL;
		}
		
	}	
  }

  return (mp);
}

const char *osMemoryPoolGetName (osMemoryPoolId_t mp_id) {
	//节省内存 不启用内存块名称
  (void)(mp_id);
  return (NULL);
}


void *osMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout) {
  OS_MEM  *mp;

  INT8U rt;
  void *block;
  (void)timeout;

  if (mp_id == NULL) {
    /* Invalid input parameters */
    block = NULL;
  }
  else {
    block = NULL;
    mp=(OS_MEM  *)mp_id; 
	block= OSMemGet(mp, &rt);
  }
  return (block);
}

osStatus_t osMemoryPoolFree (osMemoryPoolId_t mp_id, void *block) {
  OS_MEM  *mp;

  printf("start PoolFree\r\n");
  INT8U rt;
  osStatus_t stat;


  if ((mp_id == NULL) || (block == NULL)) {
    /* Invalid input parameters */
    stat = osErrorParameter;
  }
  else {
	mp=(OS_MEM  *)mp_id;   
	rt=OSMemPut(mp, (void *)block);
	if(rt==OS_ERR_NONE){
		stat = osOK;
    }
  }
  printf("over PoolFree\r\n");
  return (stat);
}

uint32_t osMemoryPoolGetCapacity (osMemoryPoolId_t mp_id) {
  OS_MEM  *mp;
  uint32_t  n;
  
  OS_MEM_DATA mem_data;
  if (mp_id == NULL) {
    /* Invalid input parameters */
    n = 0U;
  }
  else {
	 mp = (OS_MEM  *)mp_id;
	 OSMemQuery(mp, &mem_data);
	  n=mem_data.OSNBlks;
  }
  return (n);
}

uint32_t osMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id) {

  uint32_t  sz;
  OS_MEM  *mp;

  
  OS_MEM_DATA mem_data;
  if (mp_id == NULL) {
    /* Invalid input parameters */
    sz = 0U;
  }
  else {
	 mp = (OS_MEM  *)mp_id;
	 OSMemQuery(mp, &mem_data);
	 sz=mem_data.OSBlkSize;
  }

  /* Return memory block size in bytes */
  return (sz);
}

uint32_t osMemoryPoolGetCount (osMemoryPoolId_t mp_id) {
  OS_MEM  *mp;
  uint32_t  n;
  
  OS_MEM_DATA mem_data;
  if (mp_id == NULL) {
    /* Invalid input parameters */
    n = 0U;
  }
  else {
	 mp = (OS_MEM  *)mp_id;
	 OSMemQuery(mp, &mem_data);
	  n=mem_data.OSNUsed;
  }
  return (n);
}

uint32_t osMemoryPoolGetSpace (osMemoryPoolId_t mp_id) {
  OS_MEM  *mp;
  uint32_t  n;
  
  OS_MEM_DATA mem_data;
  if (mp_id == NULL) {
    /* Invalid input parameters */
    n = 0U;
  }
  else {
	 mp = (OS_MEM  *)mp_id;
	 OSMemQuery(mp, &mem_data);
	  n=mem_data.OSNFree;
  }
  return (n);
}
	

  
osStatus_t osMemoryPoolDelete (osMemoryPoolId_t mp_id) {
#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	 
  OS_MEM  *mp;
  INT8U rt;
  osStatus_t stat;

  if (mp_id == NULL) {
    /* Invalid input parameters */
    stat = osErrorParameter;
  }
  else if (IS_IRQ()) {
    stat = osErrorISR;
  }
  else {
	//静态内存分配没办法真正的删除内存块 只能清空内存块。
    mp = (OS_MEM  *)mp_id;
	OS_ENTER_CRITICAL();
    mp->OSMemAddr=NULL;
	mp->OSMemFreeList=NULL;
	mp->OSMemNFree=mp->OSMemNBlks;
	OS_EXIT_CRITICAL(); 
    stat = osOK;
  }

  return (stat);
}
