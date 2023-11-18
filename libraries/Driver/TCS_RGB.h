#ifndef TCS_RGB_H_

#include "os_implement.h"
//IIC
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

#define SLAVE_ADDR_READ			83
#define SLAVE_ADDR_WRITE		82
//Register Address
#define RGB_ENABLE_REG			0x00
#define RGB_TIMING_REG			0x01
#define RGB_WAITTIME_REG		0x03
#define RGB_INT_THRD_LOW_L_REG  0x04
#define RGB_INT_THRD_LOW_H_REG  0x05
#define RGB_INT_THRD_HIGH_L_REG 0x06
#define RGB_INT_THRD_HIGH_H_REG 0x07
#define RGB_PERSISTENCE_REG		0x0c
#define RGB_CFG_REG				0x0d
#define RGB_CTRL_REG			0x0f
#define RGB_ID_REG				0x12
#define RGB_STATUS_REG			0x13
#define RGB_CLEARDATA_L_REG		0x14
#define RGB_CLEARDATA_H_REG		0x15
#define RGB_RDATA_L_REG			0x16
#define RGB_RDATA_H_REG			0x17
#define RGB_GDATA_L_REG			0x18
#define RGB_GDATA_H_REG			0x19
#define RGB_BDATA_L_REG			0x1a
#define RGB_BDATA_H_REG			0x1b

typedef enum{
	AIDISEN = 0,
	AIEN
}AIEN_ENUM;
typedef enum{
	WDISEN = 0,
	WEN
}WEN_ENUM;
typedef enum{
	ADISEN = 0,
	AEN
}AEN_ENUM;
typedef enum{
	POFF = 0,
	PON
}PON_ENUM;
typedef enum{
	WLONG_DISEN=0,
	WLONG_EN
}WLONG_ENUM;
typedef enum{
	AGAIN_1GAIN=0,
	AGAIN_4GAIN,
	AGAIN_16GAIN,
	AGAIN_60GAIN
}AGAIN_ENUM;
//#define CMD_SPECIAL_FUNC   		0x03

void RGBAddress_Get(u8 idx);
u8 RGBID_Read(u8 idx);
void RGBEnable_Set(u8 idx,u8 aidisen,u8 wen,u8 aen,u8 pon);
//Count = (256 ? ATIME) × 1024 
void IntegrationTime_Set(u8 idx,u8 data);
//Count = (256 ? ATIME) × 1024 
void WaitTime_Set(u8 idx,u8 data);
void WLONG_Set(u8 idx);
void AGAIN_Set(u8 idx,u8 agin);
u8 AINT_Status_Get(u8 idx);
u8 AVALID_Status_Get(u8 idx);

void RGBDATA_Get(u8 idx,u16*cdata,u16* rdata,u16*gdata,u16* bdata);
#endif
