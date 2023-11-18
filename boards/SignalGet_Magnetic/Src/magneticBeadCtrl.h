#ifndef _MAGNETICBEADCTRL_H_
#define _MAGNETICBEADCTRL_H_
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>



typedef struct magneticBeadCtrl
{
	void (*electromagnetCtrl)(bool );//磁珠控制函数 需外部实现
	
	bool isStart;//是否启动
	
	uint16_t ctrlCycle;// 控制周期
	uint16_t ctrlPulse;// 控制占空比
	uint16_t ctrlCount;// 控制计数值
	 
	uint8_t overflowCount;//溢出计数
	uint32_t overflowNum;//溢出值
	
	uint8_t captureCount;//捕获计数
	uint32_t captureValue[10]; //捕获值
	uint16_t captureValueCount;//数据上报计数
	
	
	uint32_t timeCount;//任务计时

}MagneticBeadCtrl;


typedef struct magneticBeadTask
{
	uint8_t magneticBeadCount;
	MagneticBeadCtrl *magneticBeadList;
	

}MagneticBeadTask;

extern MagneticBeadTask MagneticBeadTask_Static;

void MagneticBeadTask_Init(void);



bool StartMBCtrl(MagneticBeadCtrl *mb,uint16_t time);

void UpdateMagneticBeadCaptureCount(uint32_t *count);

void UpdateMagneticBeadCtrl(void);
void UpdateMagneticBeadTaskTimeCtrl(void);
#endif

