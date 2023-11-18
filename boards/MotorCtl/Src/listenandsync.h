#ifndef _LISTENANDSYNC_H_
#define _LISTENANDSYNC_H_

#include "stm32f4xx_hal.h"

#include "macro.h"

typedef enum ListenAndSyncState
{
	OFF,
	ON	
}ListenAndSyncState;

typedef enum ListenIsCapture
{
	NO,
	OK
	
}ListenIsCapture;
typedef enum ListenMode{//捕获的方式
	Normal,//普通模式
	LOW,//捕获低电平
	HIGH,//捕获高电平
	FALLING,//捕获下降沿
	RISING//捕获上升沿
} ListenMode;
typedef struct SyncGpio{ //需要同步的引脚
	void* port;
	u16 pin;		
}SyncGpio;

typedef struct ListenGpio{//需要监听的引脚
	void* port;
	u16 pin;
	ListenMode listenMode;//该引脚的捕获方式
	SyncGpio* syncList;//需要和该引脚同步的引脚列表
	u16 syncListLen;
	u16 fd;//该引脚的分频
	u16 width;//时间宽度 （低电平和高电平方式模式时 为持续的时间；上升沿和下降沿模式时，为脉冲宽度）
	ListenAndSyncState state;//该引脚的监听状态 用来开启或关闭监听事件 on开启 off关闭
	s16 listenCount;//监听翻转次数 -1为永久开启
	ListenIsCapture isCaptuer;//是否已经捕获
	
	void (*Start)(struct ListenGpio* _this); //开启该引脚监听
	void (*Stop) (struct ListenGpio* _this); //关闭该引脚监听
	
}ListenGpio;


typedef struct ListenAndSync_cfg_t{
	ListenAndSyncState state;//总监听状态
	TIM_HandleTypeDef *htim;//使用的定时器
	ListenGpio* listenList;//监听引脚列表
	u16 listenListLen;//引脚列表长度
	u16 betweenTime;//每隔多长时间监听一次
	 
	void (*SetBetweenTime)(u16);//设置 每隔多长时间监听
	void (*Start)(); //开启总监听
	void (*Stop)();  //关闭总监听
}ListenAndSync_cfg_t;

void ListenAndSync_Init(TIM_HandleTypeDef *htim,ListenGpio* listenList,u16 len);
extern ListenAndSync_cfg_t listenAndSync_config;
#endif



