#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"


Io_cfg_t readable_io_array[IO_READABLE_CNT] = {

	{
		.io_type = IO_TYPE_IO,
		.port = GPIOB,
		.pin = GPIO_PIN_6,
		.valid_level = IO_LOW_VALID,
    },
	{
		.io_type = IO_TYPE_IO,
		.port = GPIOB,
		.pin = GPIO_PIN_7,
		.valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_0,
        .valid_level = IO_LOW_VALID,
    },
	{
	.io_type = IO_TYPE_IO,
	.port = GPIOA,
	.pin = GPIO_PIN_1,
	.valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    }
		
};


//Io_cfg_t writable_io_array[IO_WRITABLE_CNT] = {
//    
//							
//};

void io_hd_init(void)
{
	
	
}



