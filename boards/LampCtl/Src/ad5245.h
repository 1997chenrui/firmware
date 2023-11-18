#ifndef __AD5245_H
#define __AD5245_H


#include "gpio.h"


/********************************************************
 * 函数功能：创建AD5245器件对象
 * 形    参：clk：时钟引脚
             sda：数据引脚
 * 返 回 值：对象句柄
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned int ad5245_create(unsigned int clk, unsigned int sda);

unsigned int ad5245_write_test(unsigned int iic, unsigned char addr, unsigned int voltage);
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
unsigned int ad5245_write(unsigned int iic, unsigned char addr, unsigned int voltage);


/********************************************************
 * 函数功能：从AD5245器件读数据
 * 形    参：iic：总线句柄
             addr：器件地址(0 or 1)
 * 返 回 值：读出的数据
 * 开 发 者：王志超
 * 维护日期：2020年5月27日
 * 修订日志：开发
 ********************************************************/
unsigned char ad5245_read(unsigned int iic, unsigned char addr);




#endif
