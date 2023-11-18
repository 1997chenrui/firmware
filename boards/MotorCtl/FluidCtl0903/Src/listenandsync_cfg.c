#include "listenandsync_cfg.h"
extern TIM_HandleTypeDef htim2;
SyncGpio listenGpio_0_sync[2]=
{
	{.port=GPIOB,.pin=GPIO_PIN_6},
	{.port=GPIOB,.pin=GPIO_PIN_7}
};
ListenGpio listenGpio[4]=
{
	{
	 .port=GPIOB,
	 .pin=GPIO_PIN_6,//需要监听的引脚
	 .listenMode=Normal,//监听模式
	 .fd=1,//分频
	 .syncListLen=(sizeof(listenGpio_0_sync)/sizeof(SyncGpio)),//需要同步的引脚数量
	 .syncList=listenGpio_0_sync,//需要同步的引脚
	},
	{
	 .port=GPIOB,
	 .pin=GPIO_PIN_7,//需要监听的引脚
	 .listenMode=Normal,//监听模式
	 .fd=1,//分频
	 .syncListLen=(sizeof(listenGpio_0_sync)/sizeof(SyncGpio)),//需要同步的引脚数量
	 .syncList=listenGpio_0_sync,//需要同步的引脚
	},
	{
	 .port=GPIOB,
	 .pin=GPIO_PIN_15,//需要监听的引脚
	 .listenMode=Normal,//监听模式
	 .fd=1,//分频
	 .syncListLen=(sizeof(listenGpio_0_sync)/sizeof(SyncGpio)),//需要同步的引脚数量
	 .syncList=listenGpio_0_sync,//需要同步的引脚
	},
	{
	 .port=GPIOB,
	 .pin=GPIO_PIN_3,//需要监听的引脚
	 .listenMode=Normal,//监听模式
	 .fd=1,//分频
	 .syncListLen=(sizeof(listenGpio_0_sync)/sizeof(SyncGpio)),//需要同步的引脚数量
	 .syncList=listenGpio_0_sync,//需要同步的引脚
	}
};


void ListenAndSync_Init_()
{
	ListenAndSync_Init(&htim2,listenGpio,LISTGPIOCNT);
	listenAndSync_config.Start();
	
}
