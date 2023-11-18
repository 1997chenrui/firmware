#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"
extern TIM_HandleTypeDef htim2;

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {

	{//PD0 浮子开关1
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_0,
        .valid_level = IO_LOW_VALID,
    },
	{//PD1 浮子开关2
	.io_type = IO_TYPE_IO,
	.port = GPIOD,
	.pin = GPIO_PIN_1,
	.valid_level = IO_LOW_VALID,
    },
	{//PD2 浮子开关3
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
	{//PC0 染色抽屉在位检测
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_0,
        .valid_level = IO_LOW_VALID,
    }
		
};


Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {//泵 P0601 PE1
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_1,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//泵 P0602 PE0
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOE,
        .pin = GPIO_PIN_0,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },	
    {//泵 P0603 PB7
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOB,
        .pin = GPIO_PIN_7,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
    {//泵 P0604 PD7
        .io_type = IO_TYPE_PWN_SOFT,
        .port = GPIOD,
        .pin = GPIO_PIN_7,
        .pwm_tim = &htim2,
		.pwm_dutycycle = 00,
		.pwm_max_value = 1000,
		.polarity=1,
    },
	
    {//泵 P0605 PD6
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_6,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0601 PC3
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0602 PC2
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_2,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0603 PD5
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0604 PD4
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
    },  
    {//阀 V0605 PD3
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },
    {//电磁锁 S0605 PC1
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_1,
        .valid_level = IO_HIGH_VALID,
    }
							
};

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
	for(u8 i=0;i<IO_WRITABLE_CNT;i++)
	{
		if(writable_io_array[i].io_type == IO_TYPE_PWN_SOFT)
		{

			if(count<writable_io_array[i].pwm_dutycycle && writable_io_array[i].pwm_dutycycle!=0)
			{
				writable_io_array[i].polarity==1?
					HAL_GPIO_WritePin(writable_io_array[i].port,writable_io_array[i].pin,GPIO_PIN_SET):
					HAL_GPIO_WritePin(writable_io_array[i].port,writable_io_array[i].pin,GPIO_PIN_RESET);	
			}
			else
			{
				writable_io_array[i].polarity==1?
					HAL_GPIO_WritePin(writable_io_array[i].port,writable_io_array[i].pin,GPIO_PIN_RESET):
					HAL_GPIO_WritePin(writable_io_array[i].port,writable_io_array[i].pin,GPIO_PIN_SET);				
			}
		}
	}
	
	count=(count+100)%1000;
}
}

