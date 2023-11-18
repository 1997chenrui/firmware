#ifndef OS_IMPLEMENT_PC_H_
#define OS_IMPLEMENT_PC_H_

//属于bus总线的所有的c文件都需要包含这个头 这个里面定义了数据类型和操作系统的接口
// _pc.h 的含义就是pc上实现的东西  其他平台有其他后缀
//每个平台的实现是不同的 但是在这里统一接口

typedef unsigned char		u8;
typedef signed   char		s8;
typedef unsigned short		u16;
typedef signed   short		s16;
typedef unsigned int		u32;
typedef signed   int		s32;
typedef float				f32;
typedef void(*bus_os_timer_handler)(void);


//------------------------ UCOS2 平台-------------------------------------------------
#include "ucos_ii.h"
#pragma anon_unions
// UCOS 的保护方法

extern void cpuPend(void);
extern void cpuPost(void);

#define    CPU_DEFE()		//CPU_SR_ALLOC()		
#define    CPU_DEF()        //CPU_SR_ALLOC()
#define    CPU_PEND()      {  cpuPend()               //CPU_CRITICAL_ENTER()   这样保护的时候就不会关中断
#define    CPU_POST()         cpuPost(); }                 //CPU_CRITICAL_EXIT()


#ifndef  CPU_SR_ALLOC 
    #define  CPU_SR_ALLOC()             unsigned int  cpu_sr = (unsigned int)0
#endif 


typedef    struct 
{
	OS_EVENT * sem;
    u32        life_flag;
}bus_os_sem;

typedef struct 
{
	int  nill;
    
}bus_os_thread;


u32 bus_create_thread(bus_os_thread *tcb, void(*handler)(void *), u32 *stack, u32 stack_size, u32 prio, void *args);
u32 bus_os_create_sem(bus_os_sem *sem, u32 init_value);
u32 bus_os_del_sem(bus_os_sem *sem);
u32 bus_os_pend_sem(bus_os_sem *sem,u32 ticks);
u32 bus_os_post_sem(bus_os_sem *sem);
u32 bus_os_set_sem(bus_os_sem *sem,int cnt);
u32 bus_os_time_get(void);
u32 bus_os_sleep(u32 ticks);


#endif

