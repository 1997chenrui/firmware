#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
    {//PB4 垃圾盒检测
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
	{//PD4 垃圾箱检测
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
	{//PD3 样本2在位检测
		.io_type = IO_TYPE_IO,
		.port = GPIOD,
		.pin = GPIO_PIN_3,
		.valid_level = IO_LOW_VALID,
    },
	{//PD2 样本1在位检测
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    },
	{//PD1 膜杯2在位检测
		.io_type = IO_TYPE_IO,
		.port = GPIOD,
		.pin = GPIO_PIN_1,
		.valid_level = IO_LOW_VALID,
    },
	{//PD0 膜杯1在位检测
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_0,
        .valid_level = IO_LOW_VALID,
    },
	{//PD15 吸附拓印玻片1
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    },
	{//PB6 透明翻门传感器
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    }
		
};

