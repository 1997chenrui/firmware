#ifndef AD7699_H_
#define AD7699_H_
//#include "stm32f4xx_hal.h"
#include "os_implement.h"
#include "stm32f4xx_hal.h"
//回读
#define READ_BACK_CFG 							0x0000
#define NO_READ_BACK_CFG 						0x0001
//ad通道
#define CHANNEL_OFF 							0x0000
#define SEQ_OFF 								0x0000
#define UPDATE_CFG								0x0001
#define SACN_CHANNEL_TEMP 						0x0002
#define SACN_CHANNEL 							0x0003

//基准电压源
#define REF_VOLTAGE_NO_USE						0x0000
#define INTER_REF_VOLTAGE_TEMP  				0x0001    
#define EXIT_REF_VOLTAGE_TEMP  					0x0002 
#define EXIT_REF_VOLTAGE_INTER_BUF_TEMP 		0x0003 
#define EXIT_REF_VOLTAGE_NO_TEMP 			 	0x0006
#define INTER_REF_VOLTAGE_INTER_BUF_NO_TEMP  	0x0007

//带宽
#define QUARTER_BAND_WIDTH  					0x0000
#define FULL_BAND_WIDTH 						0x0001	
//通道
#define CHANNEL_IN0								0x0000
#define CHANNEL_IN1								0x0001 
#define CHANNEL_IN2								0x0002 
#define CHANNEL_IN3								0x0003 
#define CHANNEL_IN4								0x0004 
#define CHANNEL_IN5								0x0005 
#define CHANNEL_IN6								0x0006 
#define CHANNEL_IN7								0x0007

//输入通道配置
#define BIPOLAR_DIFF							0x0000
#define BIPOLAR									0x0002
#define TEMP_SENSOR								0x0003
#define SINGLE_DIFF								0x0004
#define SINGLE_COM								0x0006
#define SINGLE_GND								0x0007
//配置更新
#define KEEP_CFG								0x0000
#define COVER_CFG								0x0001


#define 	STANDARD      29000
typedef void (*NssFunc)(u8 v);
typedef struct AD7699CONFIG
{
	SPI_HandleTypeDef * adusedspi;
	u8 channel_cnt;	
	NssFunc nssfunc;
	u8 *mapping;
	
}AD7699CONFIG;

u16 AD7699_GetData(AD7699CONFIG* self, u8 channel, u16 average_cnt);
u16 AD7699_GetSingleData(AD7699CONFIG* self, u8 channel);
u16 AD7699_GetTemprature(AD7699CONFIG* self);

void AD7699_Init(AD7699CONFIG* self, SPI_HandleTypeDef * spi,u8 channel_cnt,NssFunc nssf,u8 *mapping);
u16 AD7699AllRead(AD7699CONFIG* self,u16* adData);
void AD7699AllRead_average(AD7699CONFIG* self,u16 *adData, u8 average_cnt);
#endif
