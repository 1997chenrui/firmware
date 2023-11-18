#ifndef __MCU_LIB_H
#define __MCU_LIB_H

#ifdef __cplus_plus
extern "C" {
#endif

#include <stdio.h>
#include "stm32f4xx.h"


// 建议：在底层驱动中，中断优先级的配置为：外中断 > 总线UART > CAN > 专用TIM > 其它UART > 其它TIM

#define STM32F103CBTx	((unsigned char)0)
#define STM32F407VETx	((unsigned char)1)
#define STM32F407ZETx	((unsigned char)2)


/********************************************************
 * 函数功能：系统运行入口函数（由编译器调用）
 * 形    参：无
 * 返 回 值：1=系统崩溃，正常情况下该函数会直接将CPU交给操作系统内核来控制
 * 开 发 者：王志超
 * 维护日期：2020年5月12日
 * 修订日志：开发
 ********************************************************/
int main(void);


/********************************************************
 * 函数功能：设置单片机型号（必须在用户功能初始化之前调用）
 * 形    参：无
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月12日
 * 修订日志：开发
 ********************************************************/
void mcu_type_set(unsigned char type);


/********************************************************
 * 函数功能：获取单片机型号
 * 形    参：无
 * 返 回 值：单片机型号
 * 开 发 者：王志超
 * 维护日期：2020年5月12日
 * 修订日志：开发
 ********************************************************/
unsigned char mcu_type_get(void);


/********************************************************
 * 函数功能：中断优先级设置(组4)，注意优先级不能超过设
            定的组的范围否则会有意想不到的错误。组划分如下：
			    组0:0位抢占优先级,4位响应优先级
			    组1:1位抢占优先级,3位响应优先级
			    组2:2位抢占优先级,2位响应优先级
			    组3:3位抢占优先级,1位响应优先级
			    组4:4位抢占优先级,0位响应优先级
			抢占优先级和响应优先级的数值越小，优先级越高，处理器优
			先比较顺序：先看抢占优先级然后再看响应优先级。
 * 形    参：channel：中断通道
			 prio：抢占优先级（分组4的响应优先级固定为0）
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
void mcu_nvic_set(unsigned char channel, unsigned char prio);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        操作系统接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//


// 系统任务优先级
typedef enum _eOSTaskPriority
{
	eOS_INVALID = 0, // 无效优先级
	eOS_PRIO_LL = 1, // 最低优先级
	eOS_PRIO_LM = 2, // 中低优先级
	eOS_PRIO_MM = 3, // 中等优先级
	eOS_PRIO_MH = 4, // 中高优先级
	eOS_PRIO_HH = 5, // 最高优先级
	eOS_PRIO_COUNT, // 注：这不是优先级，不可删除，仅用做数量统计
}eOSTaskPriority_t;


// 信号量类型
typedef void * os_sem;

// 列队类型
typedef void * os_queue;


#undef NULL
#define NULL 0


// 打开/关闭总中断
#define ENABLE_INTERRUPT()	__enable_irq() // {__ASM  volatile ("cpsie i");}
#define DISABLE_INTERRUPT()	__disable_irq()	// {__ASM  volatile ("cpsid i");}

#define SYSTEM_RESET() NVIC_SystemReset()

/********************************************************
 * 函数功能：开启任务调度
 * 形    参：无
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void os_start(void);


/********************************************************
 * 函数功能：创建任务
 * 形    参：pfunc：任务函数指针
             parg：任务函数形参
			 stk_size：任务堆栈大小
			 prio：任务优先级
 * 返 回 值：0=成功
             1=失败
             2=函数指针为NULL
             3=优先级设置错误
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_task_create(void (*pfunc)(void *), void * const parg, unsigned int stk_size, eOSTaskPriority_t prio);


/********************************************************
 * 函数功能：删除任务（只能删除任务自己，无法删除其它任务）
 * 形    参：无
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void os_task_delete(void);


/********************************************************
 * 函数功能：创建信号量
 * 形    参：无
 * 返 回 值：信号量指针
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
os_sem os_sem_create(void);


/****************************************************************************
 * 函数功能：删除信号量
 * 形    参：信号量指针
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ****************************************************************************/
void os_sem_delete(const os_sem sem);


/********************************************************
 * 函数功能：等待信号量
 * 形    参：sem：信号量指针
             ticks：等待时间（0表示永久等待）
 * 返 回 值：0=成功
             1=失败
             2=信号量指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_sem_pend(const os_sem sem, unsigned int ticks);


/********************************************************
 * 函数功能：释放信号量
 * 形    参：sem：信号量指针
 * 返 回 值：0=成功
             1=失败
             2=信号量指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_sem_post(const os_sem sem);


/********************************************************
 * 函数功能：创建互斥信号量
 * 形    参：无
 * 返 回 值：信号量指针
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
os_sem os_mutex_sem_create(void);


/****************************************************************************
 * 函数功能：删除互斥信号量
 * 形    参：信号量指针
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ****************************************************************************/
#define os_mutex_sem_delete(sem)	os_sem_delete(sem)


/********************************************************
 * 函数功能：等待互斥信号量
 * 形    参：sem：信号量指针
             ticks：等待时间（0表示永久等待）
 * 返 回 值：0=成功
             1=失败
             2=信号量指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_mutex_sem_pend(const os_sem sem, unsigned int ticks);


/********************************************************
 * 函数功能：释放互斥信号量
 * 形    参：sem：信号量指针
 * 返 回 值：0=成功
             1=失败
             2=信号量指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_mutex_sem_post(const os_sem sem);


/********************************************************
 * 函数功能：创建数据列队
 * 形    参：item_count：列队项的个数
             item_size：列队项的大小
 * 返 回 值：列队句柄
             NULL=创建失败
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
os_queue os_queue_create(unsigned int item_count, unsigned int item_size);


/****************************************************************************
 * 函数功能：删除数据列队
 * 形    参：数据列队指针
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ****************************************************************************/
void os_queue_delete(const os_queue queue);


/********************************************************
 * 函数功能：等待数据列队
 * 形    参：queue：列队句柄
             msg：列队项中的数据内容
             ticks：等待时间（0表示永久等待）
 * 返 回 值：0=成功
             1=失败
             2=列队句柄为NULL
             3=数据指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_queue_pend(const os_queue queue, void * const msg, unsigned int ticks);


/********************************************************
 * 函数功能：发送数据列队
 * 形    参：queue：列队句柄
             msg：列队项中的数据内容
             ticks：等待时间（0表示永久等待）
 * 返 回 值：0=成功
             1=失败
             2=列队句柄为NULL
             3=数据指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_queue_post(const os_queue queue, const void *msg, unsigned int ticks);


/********************************************************
 * 函数功能：进入临界保护
 * 形    参：无
 * 返 回 值：中断寄存器basepri的备份值
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_critical_enter(void);


/********************************************************
 * 函数功能：退出临界保护
 * 形    参：lev：中断寄存器basepri的备份值
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void os_critical_exit(unsigned int lev);


/********************************************************
 * 函数功能：获取当前系统滴答计数器的值
 * 形    参：无
 * 返 回 值：当前系统滴答计数器的值
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int os_ticks_get(void);


/********************************************************
 * 函数功能：系统延时(毫秒级)
 * 形    参：ticks：延时时间
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void os_delay(unsigned int ticks);


/********************************************************
 * 函数功能：动态申请内存
 * 形    参：size：申请内存的大小
 * 返 回 值：内存地址
             NULL=申请失败
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void *os_malloc(unsigned int size);
 
 
 /********************************************************
 * 函数功能：动态释放内存
 * 形    参：p：内存地址
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
void os_free(void *p);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        网络收发接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



#define NET_RECV_PACKET_COUNT	((unsigned char)50) // 网络接收数据包最多缓存个数


/********************************************************
 * 函数功能：网络通信初始化
 * 形    参：localId：本地ID（此模块中未使用，可为任意值）
 * 返 回 值：0=成功
             1=数据处理线程创建失败
             2=数据列队创建失败
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int net_init(unsigned char localId);


/********************************************************
 * 函数功能：网络发送数据
 * 形    参：pbuf：数据指针
             length：数据长度
             extId：扩展ID，只针对CAN端口有效，此处是为了保持接口统一
 * 返 回 值：0=成功
             1=数据发送失败
             2=数据指针为NULL
             3=数据长度错误
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int net_write(const unsigned char *pbuf, unsigned int length, unsigned int extId);


/********************************************************
 * 函数功能：注册网络数据处理回调函数
 * 形    参：pfunc：函数指针
 * 返 回 值：0=成功
             1=函数指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月4日
 * 修订日志：开发
 ********************************************************/
unsigned int net_recv_callback_add(unsigned int (*pfunc)(const unsigned char *p, unsigned int length));


/********************************************************
 * 函数功能：网络重连
 * 形    参：无
 * 返 回 值：0=成功
             1=失败
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_reconnect(void);


/********************************************************
 * 函数功能：获取网络连接状态
 * 形    参：无
 * 返 回 值：0=连接正常
             1=连接异常
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_connect_status_get(void);


/********************************************************
 * 函数功能：设置本地的网络IP、端口号
 * 形    参：local_ip：本地IP（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             local_port：本地端口号
 * 返 回 值：0=成功
             1=IP为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_local_ip_msg_set(const unsigned char *ip, unsigned short port);


/********************************************************
 * 函数功能：获取本地的网络IP、端口号(其中某个参数为NULL时就表示不获取该参数)
 * 形    参：ip：IP（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             port：端口号
 * 返 回 值：0=成功
             1=所有参数都为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_local_ip_msg_get(unsigned char * const ip, unsigned short * const port);


/********************************************************
 * 函数功能：设置远程端的网络IP、端口号
 * 形    参：ip：远程IP（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             port：远程端口号
 * 返 回 值：0=成功
             1=IP为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_remote_ip_msg_set(const unsigned char *ip, unsigned short port);


/********************************************************
 * 函数功能：获取远程端的网络IP、端口号(其中某个参数为NULL时就表示不获取该参数)
 * 形    参：ip：IP（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             port：端口号
 * 返 回 值：0=成功
             1=所有参数都为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_remote_ip_msg_get(unsigned char * const ip, unsigned short * const port);


/********************************************************
 * 函数功能：设置本地的网络IP、端口号
 * 形    参：gateway：网关（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             mask：子网掩码（例：ip[0]=255,ip[1]=255,ip[2]=255,ip[3]=0）
 * 返 回 值：0=成功
             1=网关为NULL
             1=子网掩码为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_local_other_msg_set(const unsigned char *gateway, const unsigned char *mask);


/********************************************************
 * 函数功能：获取本地的网关、子网掩码(其中某个参数为NULL时就表示不获取该参数)
 * 形    参：gateway：网关（例：ip[0]=192,ip[1]=168,ip[2]=1,ip[3]=1）
             mask：子网掩码（例：ip[0]=255,ip[1]=255,ip[2]=255,ip[3]=0）
 * 返 回 值：0=成功
             1=所有参数都为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int net_local_other_msg_get(unsigned char * const gateway, unsigned char * const mask);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                         定时器接口                                                         //
//===========================================================================================================================//
//===========================================================================================================================//



// 定时器类型
typedef enum _eTimType
{
	eTIM6,
	eTIM7,
	eTIM_COUNT, // 注：这不是定时器类型，不可删除，仅用做数量统计
}eTimType_t;

#define TIM_TASK_COUNT	((unsigned char)10) // 每个定时器支持的任务个数


/********************************************************
 * 函数功能：定时器初始化(初始化完成后定时器自动启动，重复初始化会将该定时器下的所有任务清空)
 * 形    参：tim：指定定时器(定时器时钟频率1MHz，递增计数模式)
             interval：定时器中断时间间隔，单位：微秒
 * 返 回 值：0=成功
             1=定时器类型错误
             2=定时周期错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_init(eTimType_t tim, unsigned short interval);


/********************************************************
 * 函数功能：定时器停止计数
 * 形    参：tim：指定定时器
 * 返 回 值：0=成功
             1=定时器类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_stop(eTimType_t tim);


/********************************************************
 * 函数功能：定时器重新开始计数
 * 形    参：tim：指定定时器
 * 返 回 值：0=成功
             1=定时器类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_restart(eTimType_t tim);


/********************************************************
 * 函数功能：定时器任务追加(任务立即进入运行状态)
 * 形    参：pfunc：任务函数指针
             parg：任务函数形参
             interval：任务执行时间间隔，单位：微秒
             tim：指定定时器
 * 返 回 值：0=成功
             1=任务链表满了，无法再追加任务
             2=任务函数指针为NULL
             3=定时器类型错误
             4=任务执行时间间隔错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_task_append(void (*pfunc)(void *), const void *parg, unsigned int interval, eTimType_t tim);


/********************************************************
 * 函数功能：定时器任务删除
 * 形    参：pfunc：任务函数指针
             tim：指定定时器
 * 返 回 值：0=成功
             1=未找到与之匹配的任务
             2=任务函数指针为NULL
             3=定时器类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_task_delete(void (*pfunc)(void *), eTimType_t tim);


/********************************************************
 * 函数功能：定时器任务停止运行
 * 形    参：pfunc：任务函数指针
             tim：指定定时器
 * 返 回 值：0=成功
             1=未找到与之匹配的任务
             2=任务函数指针为NULL
             3=定时器类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_task_stop(void (*pfunc)(void *), eTimType_t tim);


/********************************************************
 * 函数功能：定时器任务启动运行
 * 形    参：pfunc：任务函数指针
             tim：指定定时器
 * 返 回 值：0=成功
             1=未找到与之匹配的任务
             2=任务函数指针为NULL
             3=定时器类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
unsigned int tim_task_start(void (*pfunc)(void *), eTimType_t tim);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        精确延时接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



/********************************************************
 * 函数功能：使用滴答定时器做的微秒级延时函数（强阻塞）
 * 形    参：nus：延时时间，单位：微秒
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
void delay_us(unsigned short nus);


/********************************************************
 * 函数功能：使用滴答定时器做的毫秒级延时函数（强阻塞）
 * 形    参：nms：延时时间，单位：毫秒
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月5日
 * 修订日志：开发
 ********************************************************/
void delay_ms(unsigned short nms);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        GPIO操作接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



// 单片机引脚定义
#define PA		((unsigned int)0x0001FFFF)
#define PA0		((unsigned int)0x00010001)
#define PA1		((unsigned int)0x00010002)
#define PA2		((unsigned int)0x00010004)
#define PA3		((unsigned int)0x00010008)
#define PA4		((unsigned int)0x00010010)
#define PA5		((unsigned int)0x00010020)
#define PA6		((unsigned int)0x00010040)
#define PA7		((unsigned int)0x00010080)
#define PA8		((unsigned int)0x00010100)
#define PA9		((unsigned int)0x00010200)
#define PA10	((unsigned int)0x00010400)
#define PA11	((unsigned int)0x00010800)
#define PA12	((unsigned int)0x00011000)
#define PA13	((unsigned int)0x00012000)
#define PA14	((unsigned int)0x00014000)
#define PA15	((unsigned int)0x00018000)

#define PB		((unsigned int)0x0002FFFF)
#define PB0		((unsigned int)0x00020001)
#define PB1		((unsigned int)0x00020002)
#define PB2		((unsigned int)0x00020004)
#define PB3		((unsigned int)0x00020008)
#define PB4		((unsigned int)0x00020010)
#define PB5		((unsigned int)0x00020020)
#define PB6		((unsigned int)0x00020040)
#define PB7		((unsigned int)0x00020080)
#define PB8		((unsigned int)0x00020100)
#define PB9		((unsigned int)0x00020200)
#define PB10	((unsigned int)0x00020400)
#define PB11	((unsigned int)0x00020800)
#define PB12	((unsigned int)0x00021000)
#define PB13	((unsigned int)0x00022000)
#define PB14	((unsigned int)0x00024000)
#define PB15	((unsigned int)0x00028000)

#define PC		((unsigned int)0x0004FFFF)
#define PC0		((unsigned int)0x00040001)
#define PC1		((unsigned int)0x00040002)
#define PC2		((unsigned int)0x00040004)
#define PC3		((unsigned int)0x00040008)
#define PC4		((unsigned int)0x00040010)
#define PC5		((unsigned int)0x00040020)
#define PC6		((unsigned int)0x00040040)
#define PC7		((unsigned int)0x00040080)
#define PC8		((unsigned int)0x00040100)
#define PC9		((unsigned int)0x00040200)
#define PC10	((unsigned int)0x00040400)
#define PC11	((unsigned int)0x00040800)
#define PC12	((unsigned int)0x00041000)
#define PC13	((unsigned int)0x00042000)
#define PC14	((unsigned int)0x00044000)
#define PC15	((unsigned int)0x00048000)

#define PD		((unsigned int)0x0008FFFF)
#define PD0		((unsigned int)0x00080001)
#define PD1		((unsigned int)0x00080002)
#define PD2		((unsigned int)0x00080004)
#define PD3		((unsigned int)0x00080008)
#define PD4		((unsigned int)0x00080010)
#define PD5		((unsigned int)0x00080020)
#define PD6		((unsigned int)0x00080040)
#define PD7		((unsigned int)0x00080080)
#define PD8		((unsigned int)0x00080100)
#define PD9		((unsigned int)0x00080200)
#define PD10	((unsigned int)0x00080400)
#define PD11	((unsigned int)0x00080800)
#define PD12	((unsigned int)0x00081000)
#define PD13	((unsigned int)0x00082000)
#define PD14	((unsigned int)0x00084000)
#define PD15	((unsigned int)0x00088000)

#define PE		((unsigned int)0x0010FFFF)
#define PE0		((unsigned int)0x00100001)
#define PE1		((unsigned int)0x00100002)
#define PE2		((unsigned int)0x00100004)
#define PE3		((unsigned int)0x00100008)
#define PE4		((unsigned int)0x00100010)
#define PE5		((unsigned int)0x00100020)
#define PE6		((unsigned int)0x00100040)
#define PE7		((unsigned int)0x00100080)
#define PE8		((unsigned int)0x00100100)
#define PE9		((unsigned int)0x00100200)
#define PE10	((unsigned int)0x00100400)
#define PE11	((unsigned int)0x00100800)
#define PE12	((unsigned int)0x00101000)
#define PE13	((unsigned int)0x00102000)
#define PE14	((unsigned int)0x00104000)
#define PE15	((unsigned int)0x00108000)

#define PF		((unsigned int)0x0020FFFF)
#define PF0		((unsigned int)0x00200001)
#define PF1		((unsigned int)0x00200002)
#define PF2		((unsigned int)0x00200004)
#define PF3		((unsigned int)0x00200008)
#define PF4		((unsigned int)0x00200010)
#define PF5		((unsigned int)0x00200020)
#define PF6		((unsigned int)0x00200040)
#define PF7		((unsigned int)0x00200080)
#define PF8		((unsigned int)0x00200100)
#define PF9		((unsigned int)0x00200200)
#define PF10	((unsigned int)0x00200400)
#define PF11	((unsigned int)0x00200800)
#define PF12	((unsigned int)0x00201000)
#define PF13	((unsigned int)0x00202000)
#define PF14	((unsigned int)0x00204000)
#define PF15	((unsigned int)0x00208000)

#define PG		((unsigned int)0x0040FFFF)
#define PG0		((unsigned int)0x00400001)
#define PG1		((unsigned int)0x00400002)
#define PG2		((unsigned int)0x00400004)
#define PG3		((unsigned int)0x00400008)
#define PG4		((unsigned int)0x00400010)
#define PG5		((unsigned int)0x00400020)
#define PG6		((unsigned int)0x00400040)
#define PG7		((unsigned int)0x00400080)
#define PG8		((unsigned int)0x00400100)
#define PG9		((unsigned int)0x00400200)
#define PG10	((unsigned int)0x00400400)
#define PG11	((unsigned int)0x00400800)
#define PG12	((unsigned int)0x00401000)
#define PG13	((unsigned int)0x00402000)
#define PG14	((unsigned int)0x00404000)
#define PG15	((unsigned int)0x00408000)

#define PH		((unsigned int)0x0080FFFF)
#define PH0		((unsigned int)0x00800001)
#define PH1		((unsigned int)0x00800002)
#define PH2		((unsigned int)0x00800004)
#define PH3		((unsigned int)0x00800008)
#define PH4		((unsigned int)0x00800010)
#define PH5		((unsigned int)0x00800020)
#define PH6		((unsigned int)0x00800040)
#define PH7		((unsigned int)0x00800080)
#define PH8		((unsigned int)0x00800100)
#define PH9		((unsigned int)0x00800200)
#define PH10	((unsigned int)0x00800400)
#define PH11	((unsigned int)0x00800800)
#define PH12	((unsigned int)0x00801000)
#define PH13	((unsigned int)0x00802000)
#define PH14	((unsigned int)0x00804000)
#define PH15	((unsigned int)0x00808000)

#define PI		((unsigned int)0x0100FFFF)
#define PI0		((unsigned int)0x01000001)
#define PI1		((unsigned int)0x01000002)
#define PI2		((unsigned int)0x01000004)
#define PI3		((unsigned int)0x01000008)
#define PI4		((unsigned int)0x01000010)
#define PI5		((unsigned int)0x01000020)
#define PI6		((unsigned int)0x01000040)
#define PI7		((unsigned int)0x01000080)
#define PI8		((unsigned int)0x01000100)
#define PI9		((unsigned int)0x01000200)
#define PI10	((unsigned int)0x01000400)
#define PI11	((unsigned int)0x01000800)
#define PI12	((unsigned int)0x01001000)
#define PI13	((unsigned int)0x01002000)
#define PI14	((unsigned int)0x01004000)
#define PI15	((unsigned int)0x01008000)


// GPIO输入输出类型
typedef enum _eGpioModeType
{
	// 系统上电默认为输入模式
	eGPIO_AN, // 模拟输入
	eGPIO_AF_UP, // 复用功能、上拉
	eGPIO_AF_DP, // 复用功能、下拉
	eGPIO_AF_NP, // 复用功能、无上下拉
	eGPIO_IN_UP, // 普通上拉输入
	eGPIO_IN_DP, // 普通下拉输入
	eGPIO_IN_NP, // 普通输入无上下拉
	eGPIO_OUT_PP_UP, // 上拉、推挽、输出
	eGPIO_OUT_PP_DP, // 下拉、推挽、输出
	eGPIO_OUT_PP_NP, // 无上下拉、推挽、输出
	eGPIO_OUT_OD_UP, // 上拉、开漏、输出
	eGPIO_OUT_OD_DP, // 下拉、开漏、输出
	eGPIO_OUT_OD_NP, // 无上下拉、开漏、输出
}eGpioModeType_t;


// GPIO引脚复用类型
typedef enum _eGpioAfType
{
	eGPIO_AF_RTC_50Hz = 0, // RTC_50Hz Alternate Function mapping
	eGPIO_AF_MCO = 0, // MCO (MCO1 and MCO2) Alternate Function mapping
	eGPIO_AF_TAMPER = 0, // TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping
	eGPIO_AF_SWJ = 0, // SWJ (SWD and JTAG) Alternate Function mapping
	eGPIO_AF_TRACE = 0, // TRACE Alternate Function mapping
	eGPIO_AF_TIM1 = 1, // TIM1 Alternate Function mapping
	eGPIO_AF_TIM2 = 1, // TIM2 Alternate Function mapping
	eGPIO_AF_TIM3 = 2, // TIM3 Alternate Function mapping
	eGPIO_AF_TIM4 = 2, // TIM4 Alternate Function mapping
	eGPIO_AF_TIM5 = 2, // TIM5 Alternate Function mapping
	eGPIO_AF_TIM8 = 3, // TIM8 Alternate Function mapping
	eGPIO_AF_TIM9 = 3, // TIM9 Alternate Function mapping
	eGPIO_AF_TIM10 = 3, // TIM10 Alternate Function mapping
	eGPIO_AF_TIM11 = 3, // TIM11 Alternate Function mapping
	eGPIO_AF_I2C1 = 4, // I2C1 Alternate Function mapping
	eGPIO_AF_I2C2 = 4, // I2C2 Alternate Function mapping
	eGPIO_AF_I2C3 = 4, // I2C3 Alternate Function mapping
	eGPIO_AF_SPI1 = 5, // SPI1/I2S1 Alternate Function mapping
	eGPIO_AF_SPI2 = 5, // SPI2/I2S2 Alternate Function mapping
	eGPIO_AF5_SPI3 = 5, // SPI3/I2S3 Alternate Function mapping (Only for STM32F411xE Devices)
	eGPIO_AF_SPI4 = 5, // SPI4/I2S4 Alternate Function mapping
	eGPIO_AF_SPI5 = 5, // SPI5 Alternate Function mapping
	eGPIO_AF_SPI6 = 5, // SPI6 Alternate Function mapping
	eGPIO_AF_SPI3 = 6, // SPI3/I2S3 Alternate Function mapping
	eGPIO_AF6_SPI2 = 6, // SPI2 Alternate Function mapping (Only for STM32F411xE Devices)
	eGPIO_AF6_SPI4 = 6, // SPI4 Alternate Function mapping (Only for STM32F411xE Devices)
	eGPIO_AF6_SPI5 = 6, // SPI5 Alternate Function mapping (Only for STM32F411xE Devices)
	eGPIO_AF_SAI1 = 6, // SAI1 Alternate Function mapping
	eGPIO_AF_USART1 = 7, // USART1 Alternate Function mapping
	eGPIO_AF_USART2 = 7, // USART2 Alternate Function mapping
	eGPIO_AF_USART3 = 7, // USART3 Alternate Function mapping
	eGPIO_AF7_SPI3 = 7, // SPI3/I2S3ext Alternate Function mapping
	eGPIO_AF_I2S3EXT = 7, // AF 7 selection Legacy 
	eGPIO_AF_UART4 = 8, // UART4 Alternate Function mapping
	eGPIO_AF_UART5 = 8, // UART5 Alternate Function mapping
	eGPIO_AF_USART6 = 8, // USART6 Alternate Function mapping
	eGPIO_AF_UART7 = 8, // UART7 Alternate Function mapping
	eGPIO_AF_UART8 = 8, // UART8 Alternate Function mapping
	eGPIO_AF_CAN1 = 9, // CAN1 Alternate Function mapping
	eGPIO_AF_CAN2 = 9, // CAN2 Alternate Function mapping
	eGPIO_AF_TIM12 = 9, // TIM12 Alternate Function mapping
	eGPIO_AF_TIM13 = 9, // TIM13 Alternate Function mapping
	eGPIO_AF_TIM14 = 9, // TIM14 Alternate Function mapping
	eGPIO_AF9_I2C2 = 9, // I2C2 Alternate Function mapping (Only for STM32F401xx/STM32F411xE Devices)
	eGPIO_AF9_I2C3 = 9, // I2C3 Alternate Function mapping (Only for STM32F401xx/STM32F411xE Devices)
	eGPIO_AF_OTG_FS = 10, // OTG_FS Alternate Function mapping
	eGPIO_AF_OTG_HS = 10, // OTG_HS Alternate Function mapping
	eGPIO_AF_ETH = 11, // ETHERNET Alternate Function mapping
	eGPIO_AF_SDIO = 12, // SDIO Alternate Function mapping
	eGPIO_AF_OTG_HS_FS = 12, // OTG HS configured in FS, Alternate Function mapping
	eGPIO_AF_FMC = 12, // FMC Alternate Function mapping
	eGPIO_AF_FSMC = 12, // FSMC Alternate Function mapping
	eGPIO_AF_DCMI = 13, // DCMI Alternate Function mapping
	eGPIO_AF_LTDC = 14, // LCD-TFT Alternate Function mapping
	eGPIO_AF_EVENTOUT = 15, // EVENTOUT Alternate Function mapping
	eGPIO_AF_NONE, // 无效的AF值
}eGpioAfType_t;


/********************************************************
 * 函数功能：引脚复用配置
 * 形    参：pin：指定GPIO引脚
             af：复用类型
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_af_config(unsigned int pin, eGpioAfType_t af);


/********************************************************
 * 函数功能：通用引脚配置(不同端口的引脚不能进行或运算)
 * 形    参：pin：指定GPIO引脚
             mode：引脚模式
             level：初始电平状态(0 or 1)
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_config(unsigned int pin, eGpioModeType_t mode, unsigned char level);


/********************************************************
 * 函数功能：输出引脚电平设置
 * 形    参：pin：指定GPIO引脚
             level：初始电平状态(0 or 1)
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_write_pin(unsigned int pin, unsigned char level);


/********************************************************
 * 函数功能：输出端口电平设置
 * 形    参：pin：指定GPIO端口
             level：初始电平状态(0 or 1)
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_write_port(unsigned int pin, unsigned int level);


/********************************************************
 * 函数功能：输入引脚电平状态读取
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：指定引脚电平状态（0 or 1）
             0xFF=异常
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned char gpio_read_pin(unsigned int pin);


/********************************************************
 * 函数功能：输入端口电平状态读取
 * 形    参：pin：指定GPIO端口
 * 返 回 值：指定端口数据，0xFFFFFFFF=异常
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_read_port(unsigned int pin);


/********************************************************
 * 函数功能：输出引脚电平翻转
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_write_reversal(unsigned int pin);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        外部中断接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



// GPIO外中断触发类型
typedef enum _eExtiType
{
	eEXTI_BOTH_EDGE, // 任意电平触发
	eEXTI_RISING_EDGE,  // 上升沿触发
	eEXTI_FALLING_EDGE, // 下降沿触发
}eExtiType_t;


/********************************************************
 * 函数功能：外部中断配置函数，每次只能配置一个引脚(配置完立即使能中断，配置之后需重新注册回调函数)
 * 形    参：pin：指定GPIO引脚
             trim：中断触发模式
             pCallBack：回调函数指针
             parg：回调函数的参数
 * 返 回 值：0=成功
             1=回调函数指针为NULL
             2=GPIO引脚错误
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int exti_enable(unsigned int pin, eExtiType_t trim, void (*pCallBack)(void * const parg), void const *parg);


/********************************************************
 * 函数功能：外部中断禁能配置，每次只配置一个引脚
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
void exti_disable(unsigned int pin);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        CAN 总线接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



typedef enum _eCanType
{
	eCAN1,
	eCAN2,
	eCAN_COUNT, // 注：这不是CAN端口类型，不可删除，仅用做数量统计
}eCanType_t;


typedef enum _eCanBuadRateType
{
	eCAN_BR_20K, // CAN建议最长通信距离3300米
	eCAN_BR_50K, // CAN建议最长通信距离1300米
	eCAN_BR_100K, // CAN建议最长通信距离620米
	eCAN_BR_125K, // CAN建议最长通信距离530米
	eCAN_BR_250K, // CAN建议最长通信距离270米
	eCAN_BR_500K, // CAN建议最长通信距离130米
	eCAN_BR_800K, // CAN建议最长通信距离70米
	eCAN_BR_1000K, // CAN建议最长通信距离40米
}eCanBuadRateType_t;


/********************************************************
 * 函数功能：CAN初始化
 * 形    参：can：CAN端口
             pin：(TX Pin | RX Pin)
             baudrate：CAN波特率
 * 返 回 值：0=成功
             1=无法识别的CAN端口
             2=CAN进入硬件初始化模式失败
             3=波特率错误
             4=CAN退出硬件初始化模式失败
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int can_init(eCanType_t can, unsigned int pin, eCanBuadRateType_t baudrate);


/********************************************************
 * 函数功能：CAN过滤器ID设置，与此ID相同的数据帧才会被接收
 * 形    参：can：CAN端口
             filter_id：过滤ID
 * 返 回 值：0=成功
             1=失败，28组过滤器使用完了
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int can_filter_id_set(eCanType_t can, unsigned char filter_id);


/********************************************************
 * 函数功能：CAN1端口发送数据帧
 * 形    参：pbuf：数据指针
             length：数据字节数
             extId：扩展ID
 * 返 回 值：0=成功
             1=发送失败（物理连接或初始化问题导致）
             2=无空邮箱，发送失败
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int can1_write(const unsigned char *pbuf, unsigned int length, unsigned int extId);


/********************************************************
 * 函数功能：CAN2端口发送数据帧
 * 形    参：pbuf：数据指针
             length：数据字节数
             extId：扩展ID
 * 返 回 值：0=成功
             1=发送失败（物理连接或初始化问题导致）
             2=无空邮箱，发送失败
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int can2_write(const unsigned char *pbuf, unsigned int length, unsigned int extId);


/********************************************************
 * 函数功能：注册CAN中断接收回调函数
 * 形    参：can：指定CAN端口
             pfunc：回调函数指针
 * 返 回 值：0=成功
             1=CAN端口错误
             2=函数指针为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int can_isr_callback_add(eCanType_t can, unsigned int (*pfunc)(const unsigned char *pbuf, unsigned char length, unsigned int extId));



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        IIC 总线接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



#define IIC_OBJECT_COUNT	((unsigned char)10) // IIC总线最大支持对象个数

typedef unsigned int IICHandle_t; // IIC总线对象句柄


/********************************************************
 * 函数功能：创建IIC总线对象
 * 形    参：clk_pin：IIC时钟引脚
             sda_pin：IIC数据引脚
 * 返 回 值：IIC总线对象，NULL=IIC总线对象已经达到最大个数或者GPIO初始化失败
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
IICHandle_t iic_object_create(unsigned int clk_pin, unsigned int sda_pin);


/********************************************************
 * 函数功能：IIC输出起始信号
 * 形    参：handle：IIC总线对象句柄
 * 返 回 值：0=成功，1=句柄为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_start(IICHandle_t handle);


/********************************************************
 * 函数功能：IIC输出停止信号
 * 形    参：handle：IIC总线对象句柄
 * 返 回 值：0=成功，1=句柄为NULL
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_stop(IICHandle_t handle);


/********************************************************
 * 函数功能：IIC发送字节数据(内置等待应答的功能)
 * 形    参：handle：IIC总线对象句柄
             data：数据
 * 返 回 值：0=发送成功(收到应答)，1=发送失败(应答超时)
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_write(IICHandle_t handle, unsigned char data);


/********************************************************
 * 函数功能：IIC读字节数据
 * 形    参：handle：IIC总线对象句柄
             ack=0表示IIC总线需要给应答，否则不需要应答
 * 返 回 值：接收到的数据
 * 开 发 者：王志超
 * 维护日期：2020年5月7日
 * 修订日志：开发
 ********************************************************/
unsigned char iic_read(IICHandle_t handle, unsigned char ack);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        PWM 操作接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



// PWM输出通道类型
typedef enum _ePwmChannelType
{
	eTIM1_CH1,
	eTIM1_CH2,
	eTIM1_CH3,
	eTIM1_CH4,
	eTIM2_CH1,
	eTIM2_CH2,
	eTIM2_CH3,
	eTIM2_CH4,
	eTIM3_CH1,
	eTIM3_CH2,
	eTIM3_CH3,
	eTIM3_CH4,
	eTIM4_CH1,
	eTIM4_CH2,
	eTIM4_CH3,
	eTIM4_CH4,
	eTIM5_CH1,
	eTIM5_CH2,
	eTIM5_CH3,
	eTIM5_CH4,
	eTIM8_CH1,
	eTIM8_CH2,
	eTIM8_CH3,
	eTIM8_CH4,
	eTIM9_CH1,
	eTIM9_CH2,
	eTIM10_CH1,
	eTIM11_CH1,
	eTIM12_CH1,
	eTIM12_CH2,
	eTIM13_CH1,
	eTIM14_CH1,
	ePWM_TIM_COUNT, // 注：这定时器PWM通道类型，不可删除，仅用做数量统计
}ePwmChannelType_t;


/********************************************************
 * 函数功能：PWM输出配置(默认占空比为50%)
 * 形    参：channel：PWM输出通道
             frequency：PWM输出频率(最大输出频率：500KHz)
             pin：PWM输出引脚
 * 返 回 值：0=成功
             1=PWM输出通道错误
             2=PWM输出频率错误
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned int pwm_config(ePwmChannelType_t channel, unsigned int frequency, unsigned int pin);


/********************************************************
 * 函数功能：PWM输出使能
 * 形    参：channel：PWM输出通道
 * 返 回 值：0=成功，1=PWM输出通道错误
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned int pwm_start(ePwmChannelType_t channel);


/********************************************************
 * 函数功能：PWM输出禁能
 * 形    参：channel：PWM输出通道
 * 返 回 值：0=成功，1=PWM输出通道错误
 * 开 发 者：王志超
 * 维护日期：2020年5月18日
 * 修订日志：开发
 ********************************************************/
unsigned int pwm_stop(ePwmChannelType_t channel);


/********************************************************
 * 函数功能：PWM输出占空比调节
 * 形    参：channel：PWM输出通道
             duty_cycle：占空比（范围：0-100）
 * 返 回 值：0=成功
             1=PWM输出通道错误
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned int pwm_duty_cycle_set(ePwmChannelType_t channel, unsigned char duty_cycle);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        ADC 操作接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



// ADC通道类型
typedef enum _eAdcChannelType
{
	eADC1_CH0,
	eADC1_CH1,
	eADC1_CH2,
	eADC1_CH3,
	eADC1_CH4,
	eADC1_CH5,
	eADC1_CH6,
	eADC1_CH7,
	eADC1_CH8,
	eADC1_CH9,
	eADC1_CH10,
	eADC1_CH11,
	eADC1_CH12,
	eADC1_CH13,
	eADC1_CH14,
	eADC1_CH15,
	
	eADC2_CH0,
	eADC2_CH1,
	eADC2_CH2,
	eADC2_CH3,
	eADC2_CH4,
	eADC2_CH5,
	eADC2_CH6,
	eADC2_CH7,
	eADC2_CH8,
	eADC2_CH9,
	eADC2_CH10,
	eADC2_CH11,
	eADC2_CH12,
	eADC2_CH13,
	eADC2_CH14,
	eADC2_CH15,
	
	eADC3_CH0,
	eADC3_CH1,
	eADC3_CH2,
	eADC3_CH3,
	eADC3_CH10,
	eADC3_CH11,
	eADC3_CH12,
	eADC3_CH13,
}eAdcChannelType_t;


/********************************************************
 * 函数功能：ADC初始化
 * 形    参：channel：指定ADC通道
 * 返 回 值：0:成功
             1:指定的ADC类型错误
             2:获取GPIO句柄失败
 * 开 发 者：王志超
 * 维护日期：2020年5月15日
 * 修订日志：开发
 ********************************************************/
unsigned char adc_init(eAdcChannelType_t channel);


/********************************************************
 * 函数功能：开启AD转换
 * 形    参：channel：指定ADC通道
 * 返 回 值：0:成功 1:ADC类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月15日
 * 修订日志：开发
 ********************************************************/
unsigned char adc_start(eAdcChannelType_t channel);


/********************************************************
 * 函数功能：关闭AD转换器
 * 形    参：channel：指定ADC通道
 * 返 回 值：0:成功 1:ADC类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月15日
 * 修订日志：开发
 ********************************************************/
unsigned char adc_stop(eAdcChannelType_t channel);


/********************************************************
 * 函数功能：获取指定通道的转换值，取times次,然后平均
 * 形    参：channel：指定ADC通道
			times:读取AD的次数
			interval:两个相邻AD值之间的延时时间，单位：us
 * 返 回 值：通道channel的times次转换结果的平均值，0xFFFFFFFF=转换失败
 * 开 发 者：王志超
 * 维护日期：2020年5月15日
 * 修订日志：开发
 ********************************************************/
unsigned int adc_average_get(eAdcChannelType_t channel, unsigned short times, unsigned int interval);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        SPI 操作接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



typedef enum _eSpiType
{
	eSPI1,
	eSPI2,
	eSPI3,
	eSPI4,
	eSPI5,
	eSPI6,
	eSPI_COUNT, // 注：这不是SPI端口类型，不可删除，仅用做数量统计
}eSpiType_t;

// SPI总线最大设备个数
#define SPI_DEVICE_COUNT	((unsigned char)10)
#define SPI_DEVICE_ENABLE	((unsigned char)0)
#define SPI_DEVICE_DISABLE	((unsigned char)1)
	

/********************************************************
 * 函数功能：SPI总线初始化
 * 形    参：spi：SPI类型
			 sck：SPI时钟引脚
             miso：SPI数据输出引脚
             mosi：SPI数据输入引脚
 * 返 回 值：0=成功
            1=SPI类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned int spi_init(eSpiType_t spi, unsigned int sck, unsigned int miso, unsigned int mosi);


/********************************************************
 * 函数功能：SPI总线读写一个字节
 * 形    参：cs_pin：SPI器件片选引脚
			 tx_data：要写入的字节
 * 返 回 值：读取到的字节
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned char spi_rw(unsigned int cs_pin, unsigned char tx_data);
	



/********************************************************
 * 函数功能：SPI器件挂载
 * 形    参：spi：SPI类型
             cs_pin：SPI器件片选引脚
 * 返 回 值：0=成功
            1=失败
            2=SPI设备达到上限，无法再挂载其它设备了
 * 开 发 者：王志超
 * 维护日期：2020年5月22日
 * 修订日志：开发
 ********************************************************/
unsigned int spi_device_append(eSpiType_t spi, unsigned int cs_pin);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                        串口操作接口                                                        //
//===========================================================================================================================//
//===========================================================================================================================//



typedef enum _eUartType
{
	eUART1,
	eUART2,
	eUART3,
	eUART4,
	eUART5,
	eUART6,
	eUART_COUNT, // 注：这不是串口类型，不可删除，仅用做数量统计
}eUartType_t;


/********************************************************
 * 函数功能：串口初始化
 * 形    参：uart：串口类型
			 tx_pin：TX引脚
             rx_pin：RX引脚
             baudrate：串口波特率
             pfunc：串口接收回调函数指针
             parg：串口接收回调函数参数
 * 返 回 值：0=成功
             1=串口类型错误
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int uart_init(eUartType_t uart, unsigned int tx_pin, unsigned int rx_pin, unsigned int baudrate, void (*pfunc)(void *parg, unsigned char rxdata), void *parg);


/********************************************************
 * 函数功能：串口发送数据
 * 形    参：UARTx：串口句柄
			 data：数据
 * 返 回 值：0=成功
             1=串口类型错误
             2=数据异常
             3=缓存区溢出
             4=发送超时
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int uart_write(eUartType_t uart, const void *pbuf, unsigned int length);



//===========================================================================================================================//
//===========================================================================================================================//
//                                                     内部Flash操作接口                                                     //
//===========================================================================================================================//
//===========================================================================================================================//



/********************************************************
 * 函数功能：将数据写入内部 Flash（自动擦除）
 * 形    参：addr：起始地址(必须是4的倍数)
             pbuf：数据指针
             length：数组长度
 * 返 回 值：0=成功
             1=地址错误
             2=数据异常
             3=Flash擦除失败
             4=写入失败
 * 开 发 者：王志超
 * 维护日期：2020年5月23日
 * 修订日志：开发
 ********************************************************/
unsigned int inflash_write(unsigned int addr, const void *pbuf, unsigned int length);


/********************************************************
 * 函数功能：读取内部 Flash 中的数据
 * 形    参：addr：起始地址(必须是4的倍数)
             pbuf：数据指针
             length：数组长度
 * 返 回 值：0=成功
             1=地址错误
             2=数据异常
 * 开 发 者：王志超
 * 维护日期：2020年5月23日
 * 修订日志：开发
 ********************************************************/
unsigned char inflash_read(unsigned int addr, void * const pbuf, unsigned int length);


/********************************************************
 * 函数功能：读取内部 Flash 大小
 * 形    参：无
 * 返 回 值：内部 Flash 大小，单位：KB
 * 开 发 者：王志超
 * 维护日期：2020年5月23日
 * 修订日志：开发
 ********************************************************/
unsigned int inflash_read_size(void);



#ifdef __cplus_plus
}
#endif

#endif
