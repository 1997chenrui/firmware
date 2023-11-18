#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_1,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_9,
        .valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {//风扇
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
    }
};
