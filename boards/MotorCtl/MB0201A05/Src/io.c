#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"


extern TIM_HandleTypeDef htim4;

Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_10,
        .valid_level = IO_HIGH_VALID,
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
    }
};

