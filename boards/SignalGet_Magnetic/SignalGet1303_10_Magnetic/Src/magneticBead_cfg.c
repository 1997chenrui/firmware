#include "magneticBead_cfg.h"
#include "magneticBeadCtrl.h"
#include "my_main.h"


extern TIM_HandleTypeDef htim1;//磁珠A 频率输入
extern TIM_HandleTypeDef htim3;//磁珠B 频率输入
extern TIM_HandleTypeDef htim4;//磁珠C 频率输入
extern TIM_HandleTypeDef htim2;//磁珠D 频率输入



extern TIM_HandleTypeDef htim5;//频率检测

MagneticBeadCtrl magneticBeadList[MAGNETICBEAD_NUM];

void electromagnetCtrl_0(bool v)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,(GPIO_PinState)v);
}

void electromagnetCtrl_1(bool v)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,(GPIO_PinState)v);
}

void electromagnetCtrl_2(bool v)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,(GPIO_PinState)v);
}

void electromagnetCtrl_3(bool v)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,(GPIO_PinState)v);
}

//磁珠A 频率输入 溢出处理
void TIM3_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
	magneticBeadList[0].overflowCount++;
}
//磁珠B 频率输入 溢出处理
void TIM1_UP_TIM10_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
	magneticBeadList[1].overflowCount++;
}

//磁珠C 频率输入 溢出处理
void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	magneticBeadList[2].overflowCount++;
}

//磁珠D 频率输入 溢出处理
void TIM4_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
	magneticBeadList[3].overflowCount++;
}

static uint32_t captureCount[MAGNETICBEAD_NUM];

void TIM5_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);
	
	captureCount[0]=__HAL_TIM_GET_COUNTER(&htim1);
	captureCount[1]=__HAL_TIM_GET_COUNTER(&htim3);
	captureCount[2]=__HAL_TIM_GET_COUNTER(&htim4);
	captureCount[3]=__HAL_TIM_GET_COUNTER(&htim2);
	
	__HAL_TIM_SET_COUNTER(&htim1,0);
	__HAL_TIM_SET_COUNTER(&htim2,0);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_SET_COUNTER(&htim4,0);
	
	
	//printf("%d,%d,%d,%d\r\n",captureCount[0],captureCount[1],captureCount[2],captureCount[3]);
	UpdateMagneticBeadCtrl();
	UpdateMagneticBeadCaptureCount(captureCount);
	UpdateMagneticBeadTaskTimeCtrl();
}

void MagneticBeadCtrlTaskInit(void)
{
	
	MagneticBeadTask_Static.magneticBeadCount=MAGNETICBEAD_NUM;
	MagneticBeadTask_Static.magneticBeadList=magneticBeadList;
	
	magneticBeadList[0].electromagnetCtrl=electromagnetCtrl_0;
	magneticBeadList[0].isStart=false;
	magneticBeadList[0].ctrlCycle=32;//控制周期时间
	magneticBeadList[0].ctrlPulse=6;//打开电磁阀的时间
	magneticBeadList[0].overflowNum=65535;//溢出值
	
	
	magneticBeadList[1].electromagnetCtrl=electromagnetCtrl_1;
	magneticBeadList[1].isStart=false;
	magneticBeadList[1].ctrlCycle=32;//控制周期时间
	magneticBeadList[1].ctrlPulse=6;//打开电磁阀的时间
	magneticBeadList[1].overflowNum=65535;//溢出值
	

	magneticBeadList[2].electromagnetCtrl=electromagnetCtrl_2;
	magneticBeadList[2].isStart=false;
	magneticBeadList[2].ctrlCycle=32;//控制周期时间
	magneticBeadList[2].ctrlPulse=6;//打开电磁阀的时间
	magneticBeadList[2].overflowNum=65535;//溢出值
	
	magneticBeadList[3].electromagnetCtrl=electromagnetCtrl_3;
	magneticBeadList[3].isStart=false;
	magneticBeadList[3].ctrlCycle=32;//控制周期时间
	magneticBeadList[3].ctrlPulse=6;//打开电磁阀的时间
	magneticBeadList[3].overflowNum=4294967295;//溢出值
	
	
//	magneticBeadList[0].isStart=true;
//	magneticBeadList[0].timeCount=6000;
//	magneticBeadList[1].isStart=true;
//	magneticBeadList[1].timeCount=6000;
//	
//	magneticBeadList[2].isStart=true;
//	magneticBeadList[2].timeCount=600000;
//	magneticBeadList[3].isStart=true;
//	magneticBeadList[3].timeCount=600000;
	
	HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
	
	
	
	
	MagneticBeadTask_Init();
}
