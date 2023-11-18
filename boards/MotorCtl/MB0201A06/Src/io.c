#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

extern TIM_HandleTypeDef htim1;

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_5,
        .valid_level = IO_LOW_VALID,
    }
};
Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {
        .io_type = IO_TYPE_PWM_N,
        .port = GPIOE,
        .pin = GPIO_PIN_8,
		.pwm_tim = &htim1,
		.pwm_channel = TIM_CHANNEL_1,
		.pwm_dutycycle = 500,
		.pwm_max_value = 1000,
    },
    {
        .io_type = IO_TYPE_PWM_N,
        .port = GPIOE,
        .pin = GPIO_PIN_10,
		.pwm_tim = &htim1,
		.pwm_channel = TIM_CHANNEL_2,
		.pwm_dutycycle = 500,
		.pwm_max_value = 1000,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_13,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_14,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_15,
        .valid_level = IO_HIGH_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_10,
        .valid_level = IO_HIGH_VALID,
    }
};
void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}