#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"
extern TIM_HandleTypeDef htim5;

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];
void io_hd_init(void);

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {

	{//PD0 浮子开关1
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    },
	{//PD1 浮子开关2
	.io_type = IO_TYPE_IO,
	.port = GPIOB,
	.pin = GPIO_PIN_6,
	.valid_level = IO_HIGH_VALID,
    },
	{//PD2 浮子开关3
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_7,
        .valid_level = IO_HIGH_VALID,
    },
};


Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    {//泵 P0301 PA0
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOA,
        .pin = GPIO_PIN_0,
        .pwm_tim = &htim5,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//泵 P0302 PA1
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOA,
        .pin = GPIO_PIN_1,
        .pwm_tim = &htim5,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },	
    {//泵 P0303 PA2
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .pwm_tim = &htim5,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//阀 V0301 PA3
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0302 PA4
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
    },  
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
	
	HAL_TIM_Base_Start_IT(&htim5);    
	printf("in\r\n");
	//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
}

void TIM5_IRQHandler(void)////时钟5 中断回调函数
{
	static u16 count=0;
	//printf("%x\r\n",TIM2->SR);
	__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);
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

