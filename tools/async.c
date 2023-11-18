#include "async.h"
#include "data_struct.h"
#include "os_implement.h"

#define             ASYNC_PRIO                30u
#define             ASYNC_STK_SIZE            256
static  OS_STK      ASYNC_STK[ASYNC_STK_SIZE];


static SeqQueue_Struct async_q;//异步 队列
static bus_os_sem sem;//异步 线程 信号量

static bool pend(void)
{
    bus_os_pend_sem(&sem,0);
    return true;
}

static bool post(void)
{
    bus_os_post_sem(&sem);
    return true;
}

void Async_task(void * arg)
{

    printf("Async_task \r\n");
    while(1)
    {
        SeqQueue_Waite(&async_q);//等待  队列有新消息
        Async_Excute_t exc;
        SeqQueue_Delete(&async_q,&exc);//把一个队列消息 移到exc
        if(exc.handler != NULL)
        {
            ((void(*)(void *,...))(exc.handler))(exc.content[0], exc.content[1], exc.content[2], exc.content[3], exc.content[4], exc.content[5]);
			//运行本次异步队列消息的 处理函数
 		}
    }
}


void Async_Init(void)
{
    bus_os_create_sem(&sem,0);//生成 异步线程 的信号量
    SeqQueue_Init(&async_q,sizeof(Async_Excute_t),&pend,&post);//生成 异步线程 队列
	u8 err= OSTaskCreateExt(Async_task, 
					0,
					&ASYNC_STK[ASYNC_STK_SIZE - 1],
					ASYNC_PRIO,
					ASYNC_PRIO,
					&ASYNC_STK[0],
					ASYNC_STK_SIZE,
					0,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    if(err != 0)//开启异步 线程
    {
        printf("Async_Init err \r\n");
    }
}


bool Async_Excute(int argcnt, void *handler, ...)
{
    if(argcnt < ASYNC_MAX_ARG_CNT)
    {
        Async_Excute_t exc;
        exc.handler = handler;
        memmove(exc.content, &handler + 1, argcnt * 4); // 参数内容拷贝进去
        return SeqQueue_Enter(&async_q, &exc);
    }
    return false;
}


