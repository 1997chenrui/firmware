#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"


//Io_cfg_t readable_io_array[IO_READABLE_CNT] = {



//		
//};


Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
    
	{//P1
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_2,
        .valid_level = IO_HIGH_VALID,
    },
	{//V1
        .io_type = IO_TYPE_IO,
        .port = GPID,
        .pin = GPIO_PIN_0,
        .valid_level = IO_HIGH_VALID,
    },
	{//V2
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_1,
        .valid_level = IO_HIGH_VALID,
    }							
};

void io_hd_init(void)
{
	
	
}



