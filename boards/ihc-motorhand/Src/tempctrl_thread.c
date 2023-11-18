#include <stdlib.h>
#include <stdio.h>
#include "tempctrl_thread.h"
#include "stm32f4xx_hal.h"
#include "heat_pwm_ctrl.h"


#include "ad5245.h"
#include "gpio.h"

//TIM4:干燥  TIM6：双向温控
//双向温控放到0通道			试剂1通道    
#define DOUBLE_POWER_CHANNEL 0
volatile u8 double_power_state = 0;

#define  TEMP_CTRL_HOST_PRO 		              20u
#define  TEMP_CTRL_STK_SIZE                       512
extern TIM_HandleTypeDef htim4;

static struct
{
    bus_os_thread               tcb;
    bus_os_sem                  sem;
    u32                         stack[TEMP_CTRL_STK_SIZE]; 
}TempCtrl_Cfg;

TempCtrl_Struct temp_ctrl_mod[TempCtrl_Channel_Cnt] = {
														{
															.is_start = 0,
															.PID_Parm.current = TEMPCTRL_INVALID,
															.PID_Parm.Target = 26,
															.PID_Parm.Offset = 0,
															.PID_Parm.Proportion = 1.0,
															.PID_Parm.Integral = 0,
															.PID_Parm.Derivative = 0,
															.PID_Parm.LastError=0.0,
															.PID_Parm.PrevError=0.0,
															.PID_Parm.SumError=0.0
														}
													};

static void TempCtrl_Thread(void* pdata);

#define GET_DQ_STATE(tId,Port,Pin)			static u8 get_dq##tId##_state()				\
											{											\
												 return HAL_GPIO_ReadPin(Port,Pin);		\
											}											\
											static void set_dq##tId##_state(u8 val)		\
											{											\
												 HAL_GPIO_WritePin(Port,Pin, (GPIO_PinState)val);		\
											}

#define GET_TEMP_VAL(tId,ChannelId)			static float get_temp##tId##_val(void)							\
											{																\
												return GetTemp_DS18B20(&temp_ctrl_mod[ChannelId].sensor##tId);	\
											}
GET_DQ_STATE(0,GPIOA,GPIO_PIN_0);
GET_DQ_STATE(1,GPIOA,GPIO_PIN_1);
GET_DQ_STATE(2,GPIOA,GPIO_PIN_2);

GET_TEMP_VAL(0,0);
GET_TEMP_VAL(1,0);
GET_TEMP_VAL(2,0);

static float get_temp_value(void)
{
	float t1 = get_temp0_val();
	float t2 = get_temp1_val();
	float t3 = get_temp2_val();
	return (t1+t2+t3)/3.0f;
}

unsigned int hiic =0;
void TempCtrl_Init(void)
{
	DS18B20_Init(&temp_ctrl_mod[0].sensor0, set_dq0_state, get_dq0_state);
	DS18B20_Init(&temp_ctrl_mod[0].sensor1, set_dq1_state, get_dq1_state);
	DS18B20_Init(&temp_ctrl_mod[0].sensor2, set_dq2_state, get_dq2_state);
	TempCtrlPID_Init(&temp_ctrl_mod[0].PID_Parm, NULL, get_temp_value);

	gpio_config(PE3, eGPIO_OUT_PP_UP, 0);
	hiic = ad5245_create(PE0, PE1);
	TempCtrl_Close(0);
	bus_create_thread(&TempCtrl_Cfg.tcb,TempCtrl_Thread,TempCtrl_Cfg.stack,TEMP_CTRL_STK_SIZE,TEMP_CTRL_HOST_PRO,NULL);
}

// maxvol:输出最大电压，单位V
static unsigned char ad5245_value_calc(float maxvol, float adjust)
{
	unsigned char value = 0;
	float target = maxvol * adjust / 2000.0f;
	if(target < 2.0f)
	{
		value = 255;
	}
	else
	{
		float data = (255.0f * 0.97f) / (target - 1.94f);
		if(data < 12.0)
		{
			value = 12;
		}
		else if(data > 255.0)
		{
			value = 255;
		}
		else
		{
			value = (unsigned char)data;
		}
	}
	return value;
}

static void TempCtrl_Thread(void* pdata)
{
	unsigned int adjust = 0;
	
	while (1)
	{
		if(temp_ctrl_mod[0].is_start)
		{
			adjust =  CoolCtrl(&temp_ctrl_mod[0].PID_Parm);
			ad5245_write(hiic, 0, ad5245_value_calc(17.0f, adjust));
//			printf("adjust=%d, target=%f\r\n", adjust, temp_ctrl_mod[0].PID_Parm.Target);
			if(adjust == 0)
			{
				gpio_write_pin(PE3, 0); // 关闭开关
			}
			else
			{
				gpio_write_pin(PE3, 1); // 打开开关
			}
		}
		else
		{
			gpio_write_pin(PE3, 0); // 关闭开关
		}
        
		bus_os_sleep(500);
	}
}

float TempCtrl_GetTemp(int channelId, bool buffer)
{
	float t = -247.0f;
	switch(channelId)
	{
		case 0:
			t = get_temp0_val();
			break;
		case 1:
			t = get_temp1_val();
			break;
		case 2:
			t = get_temp2_val();
			break;
		default:break;
	}
	return t;
}

void TempCtrl_Close(int channelId)
{
	gpio_write_pin(PE3, 0); // 关闭开关
	temp_ctrl_mod[0].is_start = 0;
}

void TempCtrl_Open(int channelId)
{
	gpio_write_pin(PE3, 1); // 打开开关
	temp_ctrl_mod[0].is_start = 1;
}



