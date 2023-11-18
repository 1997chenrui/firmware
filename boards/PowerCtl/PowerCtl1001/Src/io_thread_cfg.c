#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {//PB10 风扇1状态
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    },
	{//PE15 染色区空气过滤风扇状态
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    }
};
Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {//PE14 风扇1控制
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_14,
        .valid_level = IO_HIGH_VALID,
    },
    {//PE13 染色区空气过滤风扇控制
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_13,
        .valid_level = IO_HIGH_VALID,
    }
							
};
