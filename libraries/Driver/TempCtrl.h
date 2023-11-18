#ifndef TEMPCTRL_H_
#define TEMPCTRL_H_
#include <stdint.h>

#define	TEMPCTRL_INVALID						  -274.0f

typedef struct{
	float	current;			// 当前值	
	float	Target;				// 设定目标 Desired Value
	float	Offset;				// 目标偏差	
	float	Proportion;			// 比例常数 Proportional Const
	float	Integral;			// 积分常数 Integral Const
	float	Derivative;			// 微分常数 Derivative Const
	float	LastError;			// Error[-1]
	float	PrevError;			// Error[-2]
	float	SumError;			// Sums of Errors
	float 	pluse;				//占空比，方便根据温控制冷功率调节风扇
	float 	p_pluse;			//前一个制冷周期的占空比
	float	base_pwm;			//满功率时的装载值
	float	(*get_temp_value)(void);
	void	(*adjust_temp_pwm)(uint16_t pluse);
	void	(*set_fan_pwm)(uint16_t pluse);
} PID_Ctrl_Struct;

uint16_t HeatCtrl(PID_Ctrl_Struct * ths);
uint16_t CoolCtrl(PID_Ctrl_Struct * ths);
void TempCtrlPID_Init(PID_Ctrl_Struct * ths,void (*set_pwm)(uint16_t pluse),float (*get_temp)(void));

#endif

