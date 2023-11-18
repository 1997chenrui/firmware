#include "stm32f4xx.h"
#include "pca9685.h"
#include "delayus.h"


#define IIC_OBJECT_COUNT	((unsigned char)10) // IIC总线最大支持对象个数



// IIC特性：
//       1、SDA线上的数据必须在SCL的高电平周期保持稳定
//       2、SDA线的高或低电平状态只有在SCL 线的时钟信号是低电平时才能改变
//       3、IIC总线的SDA和SCL两条信号线同时处于高电平时，规定为总线的空闲状态
//       4、起始条件：SCL线是高电平时 SDA线从高电平向低电平切换
//       5、停止条件：SCL线是高电平时 SDA线由低电平向高电平切换
//       6、数据位传输格式：MSB
//       7、每个字节后必须跟一个响应位


typedef struct _sIicDev
{
	unsigned int clk_pin; // IIC总线时钟引脚
	unsigned int sda_pin; // IIC总线数据引脚
}sIicDev_t;


static sIicDev_t sIicDev[IIC_OBJECT_COUNT];

/********************************************************
 * 函数功能：创建IIC总线对象
 * 形    参：clk_pin：IIC时钟引脚
             sda_pin：IIC数据引脚
 * 返 回 值：IIC总线对象，NULL=IIC总线对象已经达到最大个数或者GPIO初始化失败
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
IICHandle_t iic_object_create(unsigned int clk_pin, unsigned int sda_pin)
{
	static unsigned char iic_object_count = 0;
	
	if(iic_object_count >= IIC_OBJECT_COUNT)
	{
		return NULL; // IIC总线对象已经达到最大个数
	}
	
	sIicDev[iic_object_count].clk_pin = clk_pin;
	sIicDev[iic_object_count].sda_pin = sda_pin;
	
	// IIC总线的SDA和SCL两条信号线同时处于高电平时，规定为总线的空闲状态
	gpio_config(sIicDev[iic_object_count].clk_pin, eGPIO_OUT_PP_UP, 1); // SCL初始电平为高
	gpio_config(sIicDev[iic_object_count].sda_pin, eGPIO_OUT_PP_UP, 1); // SDA初始电平为高

	return ++iic_object_count;
}


/********************************************************
 * 函数功能：搜索IIC总线对象
 * 形    参：object：IIC总线对象句柄
 * 返 回 值：IIC总线对象，NULL=句柄为NULL或者未搜索到指定的对象
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
static sIicDev_t *iic_object_search(IICHandle_t handle)
{
	if(handle > IIC_OBJECT_COUNT || handle == NULL)
	{
		return NULL; // handle异常
	}
	
	return &sIicDev[handle - 1];
}


/********************************************************
 * 函数功能：IIC输出起始信号
 * 形    参：handle：IIC总线对象句柄
 * 返 回 值：0=成功，1=句柄为NULL
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_start(IICHandle_t handle)
{
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 1;
	}
	
	// SDA和SCL在高电平时，SDA拉低表示START
	// SCL拉低，表示可以传输数据
	gpio_write_pin(psIicDev->sda_pin, 1);
	gpio_write_pin(psIicDev->clk_pin, 1);
	delayUs(6); // 持续时间大于4.7us
	gpio_write_pin(psIicDev->sda_pin, 0); // 发送起始信号
	delayUs(6); // 持续时间大于4us
	gpio_write_pin(psIicDev->clk_pin, 0); // 准备发送或接收数据
	return 0;
}


/********************************************************
 * 函数功能：IIC输出停止信号
 * 形    参：handle：IIC总线对象句柄
 * 返 回 值：0=成功，1=句柄为NULL
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_stop(IICHandle_t handle)
{
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 1;
	}
	
	// SDA和SCL在低电平时，SDA拉高表示STOP
	// SCL拉高，表示传输数据结束
	gpio_write_pin(psIicDev->clk_pin, 0);
	gpio_write_pin(psIicDev->sda_pin, 0);
	gpio_write_pin(psIicDev->clk_pin, 1); // 结束数据传输
	delayUs(6); // 持续时间大于4us
	gpio_write_pin(psIicDev->sda_pin, 1); // 发送停止信号
	delayUs(6); // 持续时间大于4.7us
	return 0;
}


/********************************************************
 * 函数功能：IIC发送字节数据(内置等待应答的功能)
 * 形    参：handle：IIC总线对象句柄
             data：数据
 * 返 回 值：0=发送成功(收到应答)，1=发送失败(应答超时)
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int iic_write(IICHandle_t handle, unsigned char data)
{
	unsigned char timeout = 0;
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 2;
	}
	
	for(unsigned char i = 0; i < 8; i++)
	{
		gpio_write_pin(psIicDev->sda_pin, data & (0x80 >> i)); // 先发高位
		gpio_write_pin(psIicDev->clk_pin, 1);
		delayUs(6); // 持续时间大于4.7us
		gpio_write_pin(psIicDev->clk_pin, 0); // 拉低时钟开始数据传输
		delayUs(2);
	}
	
	// 等待从机应答
	// 注：此处SDA必须输出低电平，否则会产生stop信号，导致IIC总线结束
	gpio_config(psIicDev->sda_pin, eGPIO_IN_UP, 0);
	gpio_write_pin(psIicDev->clk_pin, 1); // 通知从机发送应答信号
	delayUs(2);
	while(gpio_read_pin(psIicDev->sda_pin) != 0)
	{
		if(timeout++ > 0xF0)
		{
			timeout = 0xFF; // 应答超时
			break;
		}
	}
	
	gpio_config(psIicDev->sda_pin, eGPIO_OUT_PP_UP, 0);
	gpio_write_pin(psIicDev->clk_pin, 0);
	return (timeout == 0xFF)? 1 : 0;
}


/********************************************************
 * 函数功能：IIC读字节数据
 * 形    参：handle：IIC总线对象句柄
             ack=0表示IIC总线需要给应答，否则不需要应答
 * 返 回 值：接收到的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned char iic_read(IICHandle_t handle, unsigned char ack)
{
	unsigned char data = 0;
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 2;
	}
	
	// 注：此处SDA必须输出低电平，否则会产生stop信号，导致IIC总线结束
	gpio_config(psIicDev->sda_pin, eGPIO_IN_UP, 0);
	for(unsigned char i = 0; i < 8; i++)
	{
		gpio_write_pin(psIicDev->clk_pin, 1);
		delayUs(4);
		if(gpio_read_pin(psIicDev->sda_pin) != 0)
		{
			data |= 0x80 >> i; // 先读高位
		}
		gpio_write_pin(psIicDev->clk_pin, 0);
		delayUs(2);
	}
	
	// 发送应答信号
	gpio_config(psIicDev->sda_pin, eGPIO_OUT_PP_UP, ack);
	gpio_write_pin(psIicDev->clk_pin, 1);
	delayUs(5); // 持续时间大于4us
	gpio_write_pin(psIicDev->clk_pin, 0);
	return data;
}


/*通道地址*/	
#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

/*PAC9685模式地址*/
#define PCA9685_ADDR_WRITE 0x80
#define PCA9685_ADDR_READ 0x81
#define PCA_MODE 0x00
#define PCA_PRE 0xFE

#define IIC_CHANNEL_CNT 10
#define PWM_CHANNEL 16

u8 iic_id_array[IIC_CHANNEL_CNT];




/********************************************************
 * 函数功能：创建PCA9685器件对象
 * 形    参：clk：时钟引脚
             sda：数据引脚
 * 返 回 值：总线句柄
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int pca9685_create(unsigned int clk, unsigned int sda)
{
	return iic_object_create(clk, sda);
}

/********************************************************
 * 函数功能：向PCA9685器件写数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
             voltage：输出目标电压值，单位：mV
 * 返 回 值：0=成功
             1=总线应答超时
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int pca9685_write(unsigned int iic, unsigned int addr, unsigned char data)
{
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, PCA9685_ADDR_WRITE) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, addr) != 0) // 发送指令码
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, data)) // 发送数据
	{
		return 1; // 总线应答超时
	}
	
	iic_stop(iic); // 关闭总线
	return 0;
}




/********************************************************
 * 函数功能：从PCA9685器件读数据
 * 形    参：iic：总线句柄
 * 返 回 值：读出的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned char pca9685_read(unsigned int iic, unsigned int addr)
{
	
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, PCA9685_ADDR_WRITE) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, addr) != 0) // 发送指令码
	{
		return 1; // 总线应答超时
	}
	
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, PCA9685_ADDR_READ) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	unsigned char data = iic_read(iic, 0); // 读数据
	
	iic_stop(iic); // 关闭总线
	
	return data;
}

void pca9685_init()
{
	
}

/********************************************************
 * 函数功能：设置PCA9685输出PWM频率
 * 形    参：iic：总线句柄
 * 返 回 值：读出的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
static void pca_setfreq(unsigned int iic, float freq)
{
	u8 prescale,oldmode,newmode;
	double prescaleval;
	/*根据频率计算预分频*/
	freq *= 0.915; 
	prescaleval = 8000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	prescale =(f32)(prescaleval + 0.5f);


	oldmode = pca9685_read(iic, PCA_MODE);

	newmode = (oldmode&0x7F) | 0x10; // sleep

	pca9685_write(iic,PCA_MODE, newmode); // 设置时钟前必须先进入sleep模式
	
	pca9685_write(iic,PCA_PRE, prescale);//设置分频
	
	pca9685_write(iic,PCA_MODE, oldmode);//写入原先模式
	
	delayMs(2);

	pca9685_write(iic,PCA_MODE, oldmode | 0xa1);//写入原先模式
		

}


/********************************************************
 * 函数功能：设置PCA9685输出PWM频率
 * 形    参：iic：总线句柄
 * 返 回 值：读出的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
static void pca_setpwm(unsigned int iic, unsigned char channel, unsigned int on, unsigned int off)
{
	pca9685_write(iic,LED0_ON_L+4*channel, on); // 
	
	pca9685_write(iic,LED0_ON_H+4*channel, on>>8);//
	
	pca9685_write(iic,LED0_OFF_L+4*channel, off);//
	
	pca9685_write(iic,LED0_OFF_H+4*channel, off>>8);//
	
}
/**
 *@num:舵机PWM输出引脚0~15
 *@on:PWM上升计数值0~4096
 *@off:PWM下降计数值0~4096
 *一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时跳变为低电平，直到计满4096重新开始。所以当on不等于0时可作延时,当on等于0时，off/4096的值就是PWM的占空比。
**/


/********************************************************
 * 函数功能：初始化PCA9685，设置PWM频率
 * 形    参：iic：总线句柄
 * 返 回 值：读出的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
void PCA9685_Init(float freq)
{
	iic_id_array[0] = pca9685_create(PE4, PE5);
	
	pca9685_write(iic_id_array[0],PCA_MODE, 0x00);//写入原先模式
	
	pca_setfreq(iic_id_array[0], freq);//设置PWM频率
	
	delayMs(500);
}

void pca_set_freq_ex(float freq)//设置PCA输出PWM的频率
{
	pca_setfreq(iic_id_array[0], freq);
}

void pca_set_pwm_singal(unsigned char channel, unsigned int on, unsigned int off)//设置单通道的PWM占空比
{
	pca9685_write(iic_id_array[0],LED0_ON_L+4*channel, on); // 
	
	pca9685_write(iic_id_array[0],LED0_ON_H+4*channel, on>>8);//
	
	pca9685_write(iic_id_array[0],LED0_OFF_L+4*channel, off);//
	
	pca9685_write(iic_id_array[0],LED0_OFF_H+4*channel, off>>8);//
	
}
void pca_set_pwm_group(unsigned int channel, unsigned int on, unsigned int off)//设置组通道的PWM占空比
{
	for(int i=0 ; i<PWM_CHANNEL ; i++)
	{
		u8 flag = channel&0x11;
		switch(flag)
		{
			case 0 :
			{
				continue;
			}
			case 1 ://开
			{
				pca_set_pwm_singal(i, on, off);
			}
			case 2 ://关
			{
				pca_set_pwm_singal(i, 0, 4096);
			}
		}
	}
}







