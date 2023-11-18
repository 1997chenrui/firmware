#ifndef AD7699_H_
#define AD7699_H_
#include "stm32f4xx_hal.h"
#include "os_implement.h"

//#define GET_AD_BY_MID_DATA  //是否采用中值读取AD值

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
//#define ADC_CFG(RB,SEQ,REF,BW,INx,INCC,CFG) RB|SEQ|REF|BW|INx|INCC<<|CFG<<15


#define SetNSS(val) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (GPIO_PinState)val)

u16 get_ad_data(u8 channel, u16* buff, u8 buffCnt);
u16 get_ad_sigle_data(u8 channel);
void getAdcTemperature(void);

#ifdef GET_AD_BY_MID_DATA 
u16 get_ad_by_mid_data(u8 , u16* , u8 );  //采用中值法采值
#endif

#endif
