#include "stm32f4xx.h"
#include "gpio.h"




/********************************************************
 * 函数功能：获取对应GPIO句柄
 * 形    参：gpio：指定GPIO端口
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
static GPIO_TypeDef *gpio_handle_get(unsigned int pin)
{
	switch(pin & 0xFFFF0000UL)
	{
		case 0x00010000UL: return GPIOA;
		case 0x00020000UL: return GPIOB;
		case 0x00040000UL: return GPIOC;
		case 0x00080000UL: return GPIOD;
		case 0x00100000UL: return GPIOE;
		case 0x00200000UL: return GPIOF;
		case 0x00400000UL: return GPIOG;
		case 0x00800000UL: return GPIOH;
		case 0x01000000UL: return GPIOI;
		default: break;
	}
	
	return NULL;
}


/********************************************************
 * 函数功能：引脚复用配置
 * 形    参：pin：指定GPIO引脚
             af：复用类型
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_af_config(unsigned int pin, eGpioAfType_t af)
{
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin);
	if(GPIOx != NULL)
	{
		for(unsigned int k = __CLZ(__RBIT(pin)); k < 32U - __CLZ(pin & 0x0000FFFFUL); k++)
		{
			if((pin & (0x1UL << k)) != 0UL)
			{
				GPIOx->AFR[k >> 3] &= ~(0x0FUL << ((k & 0x07UL) << 2));
				GPIOx->AFR[k >> 3] = GPIOx->AFR[k >> 3] | ((unsigned int)af << ((k & 0x07UL) << 2));
			}
			else
			{
				// 未使能的引脚不做处理
			}
		}
	}
	else
	{
		return 1; // 未找到指定端口不做处理
	}
	
	return 0;
}


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
unsigned int gpio_config(unsigned int pin, eGpioModeType_t mode, unsigned char level)
{
	unsigned int ospeed = 0x03; // 0=2MHz; 1=25MHz; 2=50MHz; 3=100MHz
	
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin);
	if(GPIOx != NULL)
	{
		RCC->AHB1ENR |= (pin >> 16) & 0x0000FFFFUL; // 使能GPIO时钟
		for(unsigned int k = __CLZ(__RBIT(pin)); k < 32U - __CLZ(pin & 0x0000FFFFUL); k++)
		{
			if((pin & (0x1UL << k)) != 0)
			{
				unsigned int pin_pupd = 0; // 默认无上下拉
				unsigned int pin_mode = 0; // 默认普通输入
				if(mode == eGPIO_IN_UP || mode == eGPIO_IN_DP || mode == eGPIO_IN_NP)
				{
					pin_mode = 0x00; // 普通输入(系统复位默认状态)
				}
				else if(mode == eGPIO_AN)
				{
					pin_mode = 0x03; // 模拟输入
				}
				else if(mode == eGPIO_AF_UP || mode == eGPIO_AF_DP || mode == eGPIO_AF_NP)
				{
					pin_mode = 0x02; // 复用功能
				}
				else
				{
					pin_mode = 0x01; // 普通输出
				}
				
				GPIOx->MODER &= ~(0x3UL << (k << 1)); // 先清除原来的设置
				GPIOx->MODER |= pin_mode << (k << 1); // 设置新的模式
				
				if((pin_mode == 0x01) || (pin_mode == 0x02)) // 如果是输出模式/复用功能模式
				{
					unsigned int otype = 0; // 默认推挽输出
					if(mode == eGPIO_OUT_OD_UP || mode == eGPIO_OUT_OD_DP || mode == eGPIO_OUT_OD_NP)
					{
						otype = 1; // 开漏输出
					}
					
					// Speed mode configuration
					GPIOx->OSPEEDR &= ~(0x3UL << (k << 1)); // 清除原来的设置
					GPIOx->OSPEEDR |= (ospeed << (k << 1));	// 设置新的速度值
					
					// Output mode configuration
					GPIOx->OTYPER &= ~(0x1U << k); // 清除原来的设置
					GPIOx->OTYPER |= otype << k; // 设置新的输出模式
				}
				
				if(mode == eGPIO_OUT_OD_UP || mode == eGPIO_OUT_PP_UP || mode == eGPIO_IN_UP || mode == eGPIO_AF_UP)
				{
					pin_pupd = 1; // 上拉
				}
				else if(mode == eGPIO_OUT_OD_DP || mode == eGPIO_OUT_PP_DP || mode == eGPIO_IN_DP || mode == eGPIO_AF_DP)
				{
					pin_pupd = 2; // 下拉
				}
				else if(mode == eGPIO_OUT_OD_NP || mode == eGPIO_OUT_PP_NP || mode == eGPIO_IN_NP || mode == eGPIO_AF_NP)
				{
					pin_pupd = 0; // 无上下拉
				}
				else
				{
					pin_pupd = 3; // 保留
				}
				
				// Pull-up Pull down resistor configuration
				GPIOx->PUPDR &= ~(0x3UL << (k << 1)); // 先清除原来的设置
				GPIOx->PUPDR |= pin_pupd << (k << 1); // 设置新的上下拉
				
				// 设置引脚初始电平
				if(pin_mode == 0x00 || pin_mode == 0x01)
				{
					// 只有在输入或输出的时候才设置引脚的初始化电平
					gpio_set_pin(pin, level);
				}
			}
			else
			{
				// 未使能的引脚不做处理
			}
		}
	}
	else
	{
		return 1;// 未找到指定端口不做处理
	}
	
	return 0;
}


/********************************************************
 * 函数功能：输出引脚电平设置
 * 形    参：pin：指定GPIO引脚
             level：初始电平状态(0 or 1)
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_set_pin(unsigned int pin, unsigned char level)
{
	union
	{
		unsigned int idata;
		unsigned short sdata[2];
	}data;
	
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin);
	if(GPIOx != NULL)
	{
		data.idata = GPIOx->BSRR;
		if(level != 0)
		{
			data.sdata[0] = (unsigned short)(pin & 0x0000FFFFUL); // 输出高电平
		}
		else
		{
			data.sdata[1] = (unsigned short)(pin & 0x0000FFFFUL); // 输出低电平
		}
		
		GPIOx->BSRR = data.idata;
	}
	else
	{
		return 1; // 未找到指定端口不做处理
	}
	
	return 0;
}


/********************************************************
 * 函数功能：输出端口电平设置
 * 形    参：pin：指定GPIO端口
             level：初始电平状态(0 or 1)
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_set_port(unsigned int pin, unsigned int level)
{
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin);
	if(GPIOx != NULL)
	{
		GPIOx->ODR = level;
	}
	else
	{
		return 1; // 未找到指定端口不做处理
	}
	
	return 0;
}


/********************************************************
 * 函数功能：输入引脚电平状态读取
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：指定引脚电平状态（0 or 1）
             0xFF=异常
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned char gpio_read_pin(unsigned int pin)
{
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin & 0xFFFF0000UL);
	if(GPIOx != NULL)
	{
		if((GPIOx->IDR & (pin & 0x0000FFFFUL)) != 0)
		{
			return 1U;
		}
		else
		{
			return 0U;
		}
	}
	return 0xFF;
}


/********************************************************
 * 函数功能：输入端口电平状态读取
 * 形    参：pin：指定GPIO端口
 * 返 回 值：指定端口数据，0xFFFFFFFF=异常
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_read_port(unsigned int pin)
{
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin & 0xFFFF0000UL);
	if(GPIOx != NULL)
	{
		return GPIOx->IDR;
	}
	else
	{
		return 0xFFFFFFFFUL;
	}
}


/********************************************************
 * 函数功能：输出引脚电平翻转
 * 形    参：pin：指定GPIO引脚
 * 返 回 值：0=成功，1=找不到指定端口
 * 开 发 者：王志超
 * 维护日期：2020年5月6日
 * 修订日志：开发
 ********************************************************/
unsigned int gpio_out_reversal(unsigned int pin)
{
	GPIO_TypeDef *GPIOx = gpio_handle_get(pin & 0xFFFF0000UL);
	if(GPIOx != NULL)
	{
		GPIOx->ODR ^= pin & 0x0000FFFFUL;
	}
	else
	{
		return 1;
	}
	
	return 0;
}






















