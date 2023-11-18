#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"


Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    {//阀 V0401 PE7
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_7,
        .valid_level = IO_HIGH_VALID,
    },
	{//泵 P0401 PA3
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },			
    {//阀 V0402 PA2
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .valid_level = IO_HIGH_VALID,
    }
    			
};

