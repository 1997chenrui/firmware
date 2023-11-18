#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {//PA2 预留传感器S0701
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
	{//PE2 预留传感器S0702
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
	{//PE3 预留传感器S0703
	.io_type = IO_TYPE_IO,
	.port = GPIOE,
	.pin = GPIO_PIN_3,
	.valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
	{//PE5 阀1
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    },
	{//PE4 阀2
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
    },
};


