#ifndef SP_LIGHT_H_
#define SP_LIGHT_H_
#include "stdint.h"
#include "bus.h"
//typedef uint16_t(*LFunc)(uint8_t channel, uint16_t* buff, uint16_t buffCnt);

//#define AVER_CNT        64
//#define TOOCH_CNT       80

//#define LIGHT_STOP_STATE    0
//#define LIGHT_START_STATE   1


#define LIGHT_BUFF_SIZE  		((unsigned short)2000)
#define LIGHT_CHANNEL_SIZE  	12
//每s采值4个数据
#define LIGHT_CNT_S				4

typedef struct {
	u16 	channelIdx;
	u16     mix2FirstAdTime;
	u16		data[LIGHT_CHANNEL_SIZE];
}Light_Ad_t;

typedef struct
{
	unsigned short rindex;
	unsigned short windex;
	Light_Ad_t Ads[LIGHT_BUFF_SIZE];

	u8	 		preChannelIndex ;
	u8	 		currentChannelIndex ;
	u8			state;
	int 		near_cnt;
	unsigned char CircleCount; // 光盘圈数计数器
	unsigned char ChannelStartFlag; // 每种波长开始采集的标志位，1=已经开始，0=未开始
}Light_t;



void Light_Init(void );
void Light_Start(void );

void Light_ClearData(void);					//清除队列里面已有数据
void Light_ExitClose(void);
void Light_ExitOpen(void);

// 返回剩余数据个数
unsigned short Ligrh_RemianDataCountGet(void);

// 返回读取到的数据个数
unsigned char Light_GetData(Light_Ad_t * const data, unsigned int readCnt, unsigned int channel);


#endif
