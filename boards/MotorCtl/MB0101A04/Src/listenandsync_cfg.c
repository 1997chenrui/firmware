#include "listenandsync_cfg.h"
extern TIM_HandleTypeDef htim2;
SyncGpio listenGpio_0_sync[2]=
{
	{.port=GPIOC,.pin=GPIO_PIN_0},
	{.port=GPIOC,.pin=GPIO_PIN_1}
};
ListenGpio listenGpio[1]=
{
	{
	 .port=GPIOA,
	 .pin=GPIO_PIN_1,//需要监听的引脚
	 .listenMode=LOW,//监听模式
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