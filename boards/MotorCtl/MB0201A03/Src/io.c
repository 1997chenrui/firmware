#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_7,
        .valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    }
};

