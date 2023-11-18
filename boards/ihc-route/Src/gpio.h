#ifndef __GPIO_H
#define __GPIO_H


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



#endif
