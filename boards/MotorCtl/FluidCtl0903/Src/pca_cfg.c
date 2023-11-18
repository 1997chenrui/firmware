#include "stm32f4xx.h"
#include "pca_cfg.h"
#include "Soft_iic.h"
#include "delayus.h"


// 正常设置数据
#define pca9685_SET_VALUE	((unsigned char)0x00)

// 关断模式，连接游标至B端，A端开路，但不改变游标寄存器的内容
#define pca9685_TURN_OFF		((unsigned char)0x20)
	
// 重置游标至中间值0x80
#define pca9685_RESET_MEDIAN	((unsigned char)0x40)
	


/********************************************************
 * 函数功能：计算AD5245寄存器值
 * 形    参：voltage：目标电压，单位：mV
 * 返 回 值：AD5245寄存器值
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
static unsigned char pca9685reg_value_calc(unsigned int voltage)
{
//	// 需求电压范围
	if(voltage > 22000)
	{
		voltage = 22000;
	}
	
	if(voltage <= 1940)
	{
		voltage = 1941;
	}
	unsigned int value = (unsigned int)(248320UL / (voltage - 1940UL));
	
	if((value >>8) != 0x00)
	{
		value = 0xFF;
	}
	return (unsigned char)(value);
}


/********************************************************
 * 函数功能：创建AD5245器件对象
 * 形    参：clk：时钟引脚
             sda：数据引脚
 * 返 回 值：总线句柄
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned int pca9685_create(unsigned int clk, unsigned int sda)
{
	return iic_object_create(clk, sda);
}

/********************************************************
 * 函数功能：向AD5245器件写数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
             voltage：输出目标电压值，单位：mV
 * 返 回 值：0=成功
             1=总线应答超时
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned int ad5245_write(unsigned int iic, unsigned char addr, unsigned char data)
{
	if(addr != 0)
	{
		addr = 1;
	}
	
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, 0x58 | (addr << 1)) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, AD5245_SET_VALUE) != 0) // 发送指令码
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
 * 函数功能：向AD5245器件写数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
             voltage：输出目标电压值，单位：mV
 * 返 回 值：0=成功
             1=总线应答超时
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned int ad5245_write_voltage(unsigned int iic, unsigned char addr, unsigned int voltage)
{
	if(addr != 0)
	{
		addr = 1;
	}
	
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, 0x58 | (addr << 1)) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, AD5245_SET_VALUE) != 0) // 发送指令码
	{
		return 1; // 总线应答超时
	}
	
	if(iic_write(iic, ad5245_reg_value_calc(voltage)) != 0) // 发送数据
	{
		return 1; // 总线应答超时
	}
	
	iic_stop(iic); // 关闭总线
	return 0;
}


/********************************************************
 * 函数功能：从AD5245器件读数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
 * 返 回 值：读出的数据
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned char ad5245_read(unsigned int iic, unsigned char addr)
{
	if(addr != 0)
	{
		addr = 1;
	}
	
	iic_start(iic); // 启动总线
	
	if(iic_write(iic, 0x58 | (addr << 1) | 0x01) != 0) // 发送从机地址
	{
		return 1; // 总线应答超时
	}
	
	unsigned char data = iic_read(iic, 0); // 读数据
	
	iic_stop(iic); // 关闭总线
	
	return data;
}

