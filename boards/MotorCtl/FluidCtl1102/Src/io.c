#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_4,
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
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_9,
        .valid_level = IO_LOW_VALID,
    }
};
