#include "stm32f4xx_hal.h"
#include "exti.h"
#include "gpio.h"

// GPIO外中断线类型
typedef enum _eExtiLineType
{
	eEXTI0,
	eEXTI1,
	eEXTI2,
	eEXTI3,
	eEXTI4,
	eEXTI5,
	eEXTI6,
	eEXTI7,
	eEXTI8,
	eEXTI9,
	eEXTI10,
	eEXTI11,
	eEXTI12,
	eEXTI13,
	eEXTI14,
	eEXTI15,
	eEXTI_COUNT, // 注：这不是中断线类型，不可删除，仅用做数量统计
}eExtiLineType_t;


// 外中断回调函数数据结构
#pragma pack(push, 1)
typedef struct _sExtiDev
{
	void *parg;
	void (*pCallBack)(void * const parg);
}sExtiDev_t;
#pragma pack(pop)


sExtiDev_t sExtiDev[eEXTI_COUNT] = {NULL};


void mcu_nvic_set(unsigned char channel, unsigned char prio)
{
//	unsigned int temp, temp1;
	
	// 设置分组
	unsigned char nvic_group = 4; // 0 - 4
//	temp1 = (~nvic_group) & 0x07; // 取后三位
//	temp1 = temp1 << 8;
//	temp = SCB->AIRCR;  // 读取先前的设置
//	temp &= 0x0000F8FF; // 清空先前分组
//	temp |= 0x05FA0000; // 写入钥匙
//	temp |= temp1;
//	SCB->AIRCR = temp; // 设置分组

	// 设置NVIC
	unsigned int temp;
	unsigned char sub_prio = 0; // 分组4的响应优先级固定为0
	
	// 注：优先级的设置必须要大于configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
	// 否则不能在中断服务函数中调用FreeRTOS相关的API函数
	if(prio <= 5)
	{
		prio = 6;
	}
	else if(prio > 15)
	{
		prio = 15;
	}
	
	temp = prio << (4 - nvic_group);
	temp |= sub_prio & (0x0F >> nvic_group);
	temp = temp & 0xF; // 取低四位
	NVIC->ISER[channel >> 5] |= 1 << (channel % 32); // 使能中断位(要清除的话,设置ICER对应位为1即可)
	NVIC->IP[channel] |= temp << 4; // 设置响应优先级和抢断优先级
}

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
unsigned int exti_enable(unsigned int pin, eExtiType_t trim)
{
	unsigned int pinx = pin & 0x0000FFFFUL;
	unsigned char exti_line = 31U - __CLZ(pinx);
	unsigned char exti_offset = (exti_line % 4) << 2;

	if((pin & 0x01FFF0000UL) == 0UL)
	{
		return 0; // 无法识别的GPIO端口
	}
	
	// 引脚初始化
	if(trim == eEXTI_FALLING_EDGE)
	{
		gpio_config(pin, eGPIO_IN_UP, 1); // 下降沿触发就将引脚上拉，默认输出高电平
	}
	else if(trim == eEXTI_RISING_EDGE)
	{
		gpio_config(pin, eGPIO_IN_DP, 0); // 上升沿触发就将引脚下拉，默认输出低电平
	}
	else
	{
		gpio_config(pin, eGPIO_IN_NP, 0); // 上升和下降沿都触发就将引脚设置无上下拉，默认输出低电平
	}
	
	RCC->APB2ENR |= 0x1UL << 14; // 使能SYSCFG时钟
	SYSCFG->EXTICR[exti_line >> 2] &= ~(0x000F << exti_offset); // 清除原来设置
	SYSCFG->EXTICR[exti_line >> 2] |= (15U - __CLZ(pin)) << exti_offset; // EXTI.BITx映射到GPIOx.BITx

	EXTI->IMR |= pinx; // 开启指定引脚上的中断(如果要禁止中断，则反操作即可)
	if(trim == eEXTI_FALLING_EDGE)
	{
		EXTI->FTSR |= pinx; // 下降沿触发
	}
	else if(trim == eEXTI_RISING_EDGE)
	{
		EXTI->RTSR |= pinx; // 上升沿触发
	}
	else if(trim == eEXTI_BOTH_EDGE)
	{
		EXTI->FTSR |= pinx; // 下降沿触发
		EXTI->RTSR |= pinx; // 上升沿触发
	}
	
	unsigned char IRQn = 0;
	switch(exti_line)
	{
		case eEXTI0: IRQn = EXTI0_IRQn; break;
		case eEXTI1: IRQn = EXTI1_IRQn; break;
		case eEXTI2: IRQn = EXTI2_IRQn; break;
		case eEXTI3: IRQn = EXTI3_IRQn; break;
		case eEXTI4: IRQn = EXTI4_IRQn; break;
		case eEXTI5:
		case eEXTI6:
		case eEXTI7:
		case eEXTI8:
		case eEXTI9: IRQn = EXTI9_5_IRQn; break;
		case eEXTI10:
		case eEXTI11:
		case eEXTI12:
		case eEXTI13:
		case eEXTI14:
		case eEXTI15: IRQn = EXTI15_10_IRQn; break;
		default : return 2;
	}
	mcu_nvic_set(IRQn, 6); // 抢占优先级6，响应优先级0，优先级分组4
	return 0;
}


/********************************************************
 * 函数功能：外部中断禁能配置，每次只配置一个引脚
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
void exti_disable(unsigned int pin)
{
	unsigned char exti_line = 31U - __CLZ(pin & 0x0000FFFFUL);
	unsigned char exti_offset = (exti_line % 4) << 2;
	
	SYSCFG->EXTICR[exti_line >> 2] &= ~(0x000F << exti_offset); // 清除原来设置
	SYSCFG->EXTICR[exti_line >> 2] |= (15U - __CLZ(pin)) << exti_offset; // EXTI.BITx映射到GPIOx.BITx
	EXTI->IMR |= ~(pin & 0x0000FFFFUL); // 关闭指定引脚上的中断(如果要使能中断，则反操作即可)
}


///********************************************************
// * 函数功能：外部中断回调函数(中断专用)
// * 形    参：line：指定的外中断线
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//static void exti_isr_callback(eExtiLineType_t line)
//{
//	EXTI->PR = 0x1UL << line; // 清除中断标志位
//	if(sExtiDev[line].pCallBack != NULL)
//	{
//		// 执行中断回调函数
//		sExtiDev[line].pCallBack(sExtiDev[line].parg);
//	}
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI0_IRQHandler(void)
//{
//	exti_isr_callback(eEXTI0);
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI1_IRQHandler(void)
//{
//	exti_isr_callback(eEXTI1);
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI2_IRQHandler(void)
//{
//	exti_isr_callback(eEXTI2);
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI3_IRQHandler(void)
//{
//	exti_isr_callback(eEXTI3);
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI4_IRQHandler(void)
//{
//	exti_isr_callback(eEXTI4);
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI9_5_IRQHandler(void)
//{
//	unsigned int pr = EXTI->PR;
//	if(pr & (0x1UL << eEXTI5))
//	{
//		exti_isr_callback(eEXTI5);
//	}
//	else if(pr & (0x1UL << eEXTI6))
//	{
//		exti_isr_callback(eEXTI6);
//	}
//	else if(pr & (0x1UL << eEXTI7))
//	{
//		exti_isr_callback(eEXTI7);
//	}
//	else if(pr & (0x1UL << eEXTI8))
//	{
//		exti_isr_callback(eEXTI8);
//	}
//	else if(pr & (0x1UL << eEXTI9))
//	{
//		exti_isr_callback(eEXTI9);
//	}
//	else
//	{
//		// 未知中断线，不可能会发生
//	}
//}


///********************************************************
// * 函数功能：外部中断服务函数
// * 形    参：无
// * 返 回 值：无
// * 开 发 者：王志超
// * 维护日期：2020年5月6日
// * 修订日志：开发
// ********************************************************/
//void EXTI15_10_IRQHandler(void)
//{
//	unsigned int pr = EXTI->PR;
//	if(pr & (0x1UL << eEXTI10))
//	{
//		exti_isr_callback(eEXTI10);
//	}
//	else if(pr & (0x1UL << eEXTI11))
//	{
//		exti_isr_callback(eEXTI11);
//	}
//	else if(pr & (0x1UL << eEXTI12))
//	{
//		exti_isr_callback(eEXTI12);
//	}
//	else if(pr & (0x1UL << eEXTI13))
//	{
//		exti_isr_callback(eEXTI13);
//	}
//	else if(pr & (0x1UL << eEXTI14))
//	{
//		exti_isr_callback(eEXTI14);
//	}
//	else if(pr & (0x1UL << eEXTI15))
//	{
//		exti_isr_callback(eEXTI15);
//	}
//	else
//	{
//		// 未知中断线，不可能会发生
//	}
//}








