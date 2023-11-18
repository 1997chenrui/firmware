#ifndef __PCA9685_H
#define __PCA9685_H


#include "gpio.h"


typedef unsigned int IICHandle_t; // IIC总线对象句柄

/********************************************************
 * 函数功能：创建PCA9685器件对象
 * 形    参：clk：时钟引脚
             sda：数据引脚
 * 返 回 值：对象句柄
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int pca9685_create(unsigned int clk, unsigned int sda);


/********************************************************
 * 函数功能：向PCA9685器件写数据
 * 形    参：iic：总线句柄
             data：写入的数据
 * 返 回 值：0=成功
             1=总线应答超时
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned int pca9685_write(unsigned int iic, unsigned int addr, unsigned char data);

/********************************************************
 * 函数功能：向PCA9685器件写数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
             voltage：输出目标电压值，单位：mV
 * 返 回 值：0=成功
             1=总线应答超时
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
//unsigned int pca9685_write_voltage(unsigned int iic, unsigned char addr, unsigned int voltage);


/********************************************************
 * 函数功能：从PCA9685器件读数据
 * 形    参：iic：总线句柄
 * 返 回 值：读出的数据
 * 开 发 者：Chenr
 * 维护日期：2023年3月30日
 * 修订日志：开发
 ********************************************************/
unsigned char pca9685_read(unsigned int iic, unsigned int addr);

void PCA9685_Init(float freq);

void pca_set_pwm_ex(unsigned char channel, unsigned int on, unsigned int off);

void pca_set_pwm_singal(unsigned char channel, unsigned int on, unsigned int off);

void pca_set_pwm_group(unsigned int channelgroup, unsigned int on, unsigned int off);
	
#endif
