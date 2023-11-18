#include "TempCtrl.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef STM32F407xx 
#include "stm32f4xx_hal.h"
#endif
//extern TIM_HandleTypeDef htim4;
//此宏决定，在允许全功率工作时器件的功率
#define COOL_PWM 2000.0f
#define HEAT_PWM 2000.0f

static float PIDAdjust(PID_Ctrl_Struct *pPID) ;

void TempCtrlPID_Init(PID_Ctrl_Struct * ths,
						void (*set_pwm)(uint16_t pluse),
						float (*get_temp)(void))
{
	ths->adjust_temp_pwm = set_pwm;
	ths->get_temp_value = get_temp;
	ths->set_fan_pwm  = set_pwm;
}

// 返回调节后的PWM占空比
uint16_t HeatCtrl(PID_Ctrl_Struct *ths)
{
	float bufTemp,PWM;
	uint16_t pluse = 0;
	if(ths == NULL)
	{
		return 0;
	}
	
	bufTemp = ths->get_temp_value();
//	printf("bufTemp = %f, temp cur=%f, temp tar=%f\r\n", bufTemp, ths->current, ths->Target);

	if(((-20.0f)<bufTemp && bufTemp<200.0f))
	{
		ths->current= bufTemp+ths->Offset;
		PWM=PIDAdjust(ths);
		if(PWM<0.0f)PWM=0;
		if(PWM>1.0f)PWM=1;
		pluse = (uint16_t)((HEAT_PWM)*((float)PWM));
		printf("pluse = %d\r\n",pluse);
		ths->adjust_temp_pwm(pluse);
	}
	else
	{
		ths->current = TEMPCTRL_INVALID;
		ths->adjust_temp_pwm(0);//关闭
	}
	
	return pluse;
}

// 返回调节后的PWM占空比
uint16_t CoolCtrl(PID_Ctrl_Struct * ths)
{
	float bufTemp,PWM;
	uint16_t pluse = 0;
	bufTemp = ths->get_temp_value();
	if(((-20.0f)<bufTemp&& bufTemp<200.0f))
	{
		ths->current = bufTemp+ths->Offset;
		PWM=PIDAdjust(ths);
		if(PWM>0.0f)PWM=0;
		if(PWM<0.0f)PWM=-PWM;
		if(PWM>1.0f)PWM=1.0f;
		pluse = (uint16_t)((COOL_PWM)*((float)PWM));
		printf("pluse =%d\r\n",pluse);
		ths->adjust_temp_pwm(pluse);
		ths->pluse = PWM;
		printf("Cool current= %5f2target= %5f2,PWM = %f, SumError = %f\r\n",ths->current,ths->Target,	PWM,ths->SumError);
	}
	else
	{
		printf("TEMPCTRL_INVALID %5f\r\n",bufTemp);
		ths->current = TEMPCTRL_INVALID;
		ths->adjust_temp_pwm(0);//关闭
//		printf("[error]:Channel %d temperature sensor error!\r\n",channel);
	}
	
	return pluse;
}

static float PIDAdjust(PID_Ctrl_Struct *pPID) 
{
	float iError = 0;
	float dError = 0;	
	float iPWM = 0;
	iError = pPID->Target - pPID->current;	//偏差
	pPID->SumError += iError;				//积分
	dError = iError -  pPID->LastError;		//微分
	pPID->LastError = iError;				//记录这次偏差以供下次计算用  
	
	iPWM = pPID->Proportion * iError
			+pPID->Integral * pPID->SumError
			+pPID->Derivative *	dError;
	//printf("iPWM=%f,SumError=%f\r\n",iPWM,pPID->SumError);
	return iPWM;
}

