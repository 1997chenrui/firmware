#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_3,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    }
};
 

Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
 
	{
        .io_type = IO_TYPE_PWM,
        .port = GPIOD,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim4,
        .pwm_channel = TIM_CHANNEL_1,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOD,
        .pin = GPIO_PIN_13,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim4,
        .pwm_channel = TIM_CHANNEL_2,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOD,
        .pin = GPIO_PIN_14,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim4,
        .pwm_channel = TIM_CHANNEL_3,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOD,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim4,
        .pwm_channel = TIM_CHANNEL_4,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOC,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim3,
        .pwm_channel = TIM_CHANNEL_1,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOC,
        .pin = GPIO_PIN_7,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim3,
        .pwm_channel = TIM_CHANNEL_2,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
        {
        .io_type = IO_TYPE_PWM,
        .port = GPIOC,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim3,
        .pwm_channel = TIM_CHANNEL_3,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOC,
        .pin = GPIO_PIN_9,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim3,
        .pwm_channel = TIM_CHANNEL_4,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_PWM,
        .port = GPIOA,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim1,
        .pwm_channel = TIM_CHANNEL_1,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_11,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_12,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_15,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
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
		 __HAL_TIM_SetCompare((TIM_HandleTypeDef*)self->pwm_tim, self->pwm_channel, self->pwm_dutycycle); 
    }
}

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}


void writable_io_init(void )
{
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);  
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);  
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);  
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);  
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);  
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);  
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);  
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);  
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);  
	
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);  
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);  
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);  
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);  
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);  
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);  
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);  
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);  
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,0); 
	

    writable_io[0] = writable_io_new(io_hd_write, writable_io_array, IO_WRITABLE_CNT);	
}



void readable_io_init(void )
{

	
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array, IO_READABLE_CNT);
	
}

