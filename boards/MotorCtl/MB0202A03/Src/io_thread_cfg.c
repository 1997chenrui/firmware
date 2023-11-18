#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"



Io_cfg_t readable_io_array[IO_READABLE_CNT] = {
	{//PC1
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_1,
        .valid_level = IO_LOW_VALID,
    }
};