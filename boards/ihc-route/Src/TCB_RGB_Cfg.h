#ifndef TCS_RGB_CFG_H_
#define TCS_RGB_CFG_H_
#include "stm32f4xx_hal.h"
//IO方向设置  in 7 out 7
#define SDA_IN0()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7输入模式
#define SDA_OUT0() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7输出模式
//IO操作函数	 
#define IIC_SCL0    PBout(6) //SCL 6
#define IIC_SDA0    PBout(7) //SDA	7  
#define READ_SDA0   PBin(7)  //输入SDA 

#define RGB_CHANNEL_CNT			1

#endif

