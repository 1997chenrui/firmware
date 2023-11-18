#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

extern TIM_HandleTypeDef htim2;

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];
void io_hd_init(void);

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
	{//PD8 对射传感器 S0510
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
    }, 
	{//PD9 垃圾盒检测 S0501
		.io_type = IO_TYPE_IO,
		.port = GPIOD,
		.pin = GPIO_PIN_9,
		.valid_level = IO_LOW_VALID,
    },
	{//PD10 垃圾箱检测 S0502
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    },
	{//PD12 样本1抽屉检测 S0503
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
    },
	{//PD11 样本2抽屉检测 S0504
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
	{//PD14 膜杯1抽屉检测 S0505
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_14,
        .valid_level = IO_LOW_VALID,
    },
	{//PD13 膜杯2抽屉检测 S0506
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_13,
        .valid_level = IO_LOW_VALID,
    },
	{//PD6 染色池抽屉检测 S0507
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    },
	{//PE5 浮子开盖
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    }	
};

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    {//PE15 阀 V0501
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_15,
        .valid_level = IO_HIGH_VALID,
    },  
    {//PE14 阀 V0502
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_14,
        .valid_level = IO_HIGH_VALID,
    },  
    {//PE13 阀 V0503
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_13,
        .valid_level = IO_HIGH_VALID,
    },  
    {//PE12 阀 V0504
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_12,
        .valid_level = IO_HIGH_VALID,
    },  
    {//PE11 阀 P0505
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_11,
        .valid_level = IO_HIGH_VALID,
    },
    {//泵 P0501 PE10
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_10,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//泵 P0502 PE9
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_9,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },	
    {//泵 P0503 PE8
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_8,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//泵 P0504 PE6
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_6,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    }
							
};


static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
     if(self->io_type == IO_TYPE_IO)
	{
		printf("port=%x,pin=%d,state %d\r\n",(int32_t)self->port,self->pin,state);
		HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
	
    }
	else if(self->io_type == IO_TYPE_PWM)
	{
        TIM_HandleTypeDef* tim = self->pwm_tim;
		if(self->pwm_channel == TIM_CHANNEL_1){
			tim->Instance->CCR1=self->pwm_dutycycle;
		}else if(self->pwm_channel == TIM_CHANNEL_2){
			tim->Instance->CCR2=self->pwm_dutycycle;
		}else if(self->pwm_channel == TIM_CHANNEL_3){
			tim->Instance->CCR3=self->pwm_dutycycle;
		}else{
			tim->Instance->CCR4=self->pwm_dutycycle;
		}
		if(self->pwm_dutycycle == 0){
			if(self->io_type == IO_TYPE_PWM_N) {
				HAL_TIMEx_OCN_Stop(tim, self->pwm_channel);
			}else{
				HAL_TIM_PWM_Stop(tim, self->pwm_channel);
			}
        }else{
			if(self->io_type == IO_TYPE_PWM_N) {
				HAL_TIMEx_OCN_Start(tim, self->pwm_channel);
			} else {
				HAL_TIM_PWM_Start(tim, self->pwm_channel);
			}
		}     
    }
}

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}

void writable_io_init(void )
{
	io_hd_init();
    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);
	
}

void writable_io_Test()
{
	for(uint8_t i=0;i<8;i++)
	{
		io_hd_write(&writable_io_array_0[i],IO_ENABLE);
		OSTimeDly(1000);
	}
	for(uint8_t i=0;i<8;i++)
	{
		io_hd_write(&writable_io_array_0[i],IO_DISABLE);
		OSTimeDly(1000);
	}	
}

void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}

void io_hd_init(void)
{
	
	HAL_TIM_Base_Start_IT(&htim2);    
	printf("in\r\n");
	//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
}

void TIM2_IRQHandler(void)////时钟2 中断回调函数
{
	static u16 count=0;
	//printf("%x\r\n",TIM2->SR);
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	//if(TIM2->SR&0x0001)
	{
	for(u8 i=0;i<IO_WRITABLE_CNT_0;i++)
	{
		if(writable_io_array_0[i].io_type == IO_TYPE_PWN_SOFT)
		{

			if(count<writable_io_array_0[i].pwm_dutycycle && writable_io_array_0[i].pwm_dutycycle!=0)
			{
				writable_io_array_0[i].polarity==1?
					HAL_GPIO_WritePin(writable_io_array_0[i].port,writable_io_array_0[i].pin,GPIO_PIN_SET):
					HAL_GPIO_WritePin(writable_io_array_0[i].port,writable_io_array_0[i].pin,GPIO_PIN_RESET);	
			}
			else
			{
				writable_io_array_0[i].polarity==1?
					HAL_GPIO_WritePin(writable_io_array_0[i].port,writable_io_array_0[i].pin,GPIO_PIN_RESET):
					HAL_GPIO_WritePin(writable_io_array_0[i].port,writable_io_array_0[i].pin,GPIO_PIN_SET);				
			}
		}
	}
	
	count=(count+100)%1000;
}
}

