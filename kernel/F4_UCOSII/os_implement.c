#include "os_implement.h"
#include <string.h>
#include <stdio.h>
#include "ucos_ii.h"
#include <stdarg.h>


#define PRINT_ERR(err) if(err!=0) while(1)printf("uc err %d   ,F %s  ,L %d\r\n",err,__FILE__,__LINE__)


u32 bus_create_thread(bus_os_thread *tcb,void(*handler)(void *), u32 *stack, u32 stack_size, u32 prio,void *arg)
{
    INT8U err;
    printf("thread prio = %d\r\n", prio);
    err = OSTaskCreateExt(handler, //指向任务代码的指针
	arg,//指向一个类型可以选择的数据区（很多时候为0）
                    &stack[stack_size - 1],//堆栈向高地址增长（指向任务堆栈空间的最高地址）
                    prio,//优先级
                    prio,//id任务的标识，可以设置同优先级
                    &stack[0],//指向堆栈底端的指针
                    stack_size,//任务栈的大小
	0,//（定义数据结构的指针，很多时候为0）
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));//允许堆栈检查
    if(err != 0)
	{
		PRINT_ERR(err);
        return 1;
	}
    else
        return 0;
}

u32 bus_os_create_sem(bus_os_sem *sem, u32 init_value) 
{
    sem->sem =  OSSemCreate(init_value);
    if(sem->sem == NULL) 
    {
        return 1;
    }
    else
    {
        sem->life_flag =  ~((u32)sem->sem);
        return 0;
    }

}


u32 bus_os_set_sem(bus_os_sem *sem,int cnt)
{
    INT8U err;
    OSSemSet(sem->sem,cnt,&err);
    if(err != 0 )
    {
        PRINT_ERR(err);
    }
    return  0 ;
}

// 删除信号量
u32 bus_os_del_sem(bus_os_sem *sem)
{

    if(sem->life_flag == 0 )
    {
        //PRINT_ERR(888);
        printf("warning: the sem is already deleted\r\n");
        return 0 ;
    }
    else
    {
        sem->life_flag = 0;
    }
    INT8U err;
    OSSemDel(sem->sem,OS_DEL_ALWAYS,&err);
//    printf("del %p\r\n",sem);
    if(err)
    {
        PRINT_ERR(err);
    }
    
	return 0;
}

// 等待信号量
u32 bus_os_pend_sem(bus_os_sem *sem,u32 ticks)
{
    if(sem->life_flag !=  ~((u32)sem->sem))
    {
        PRINT_ERR(888);
    }
    INT8U err;
    OSSemPend(sem->sem,ticks,&err);//等待信号量
    
    if(err == OS_ERR_TIMEOUT)
        return 1;
    else if (err != 0)
    {
        PRINT_ERR(err);
        return 2;
    }
    else
        return 0;
}

// 释放信号量
u32 bus_os_post_sem(bus_os_sem *sem)
{
    if(sem->life_flag !=  ~((u32)sem->sem))
    {
        PRINT_ERR(888);
    }
    INT8U err;
    err = OSSemPost(sem->sem);
    PRINT_ERR(err);
    if(err != 0)
        return 1;    
	return 0;
}


u32 bus_os_time_get(void)
{
    return OSTimeGet();
 
}

u32 bus_os_sleep(u32 ticks)
{
    OSTimeDly(ticks);
    return 0;
}


static bus_os_sem mutex;

static u8 mutex_ok = 0;
static int mutex_cnt = 0;

void cpuPend(void)
{
    if(!mutex_ok )
    {
        bus_os_create_sem(&mutex,1);
        mutex_ok = 1;
    }
    if(mutex_cnt == 0 )
    {
        bus_os_pend_sem(&mutex,0);
    }

    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    mutex_cnt ++;
    OS_EXIT_CRITICAL();

}


void cpuPost(void)
{
    if(mutex_cnt == 1)
    {
        bus_os_post_sem(&mutex);
    }
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    mutex_cnt --;
    OS_EXIT_CRITICAL();

}







