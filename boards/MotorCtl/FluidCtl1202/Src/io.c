#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

extern TIM_HandleTypeDef htim4;



static void io_hd_write(Io_cfg_t *self, Io_state_e state);

static u8 io_hd_read(Io_cfg_t *self);

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_14,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_13,
        .valid_level = IO_HIGH_VALID,
    }
};

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
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
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
        .pwm_tim = &htim4,
        .pwm_channel = TIM_CHANNEL_1,
        .pwm_dutycycle = 800,
        .pwm_max_value = 1000
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    }
};



void writable_io_init(void )
{
    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);

}

void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}

static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
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

