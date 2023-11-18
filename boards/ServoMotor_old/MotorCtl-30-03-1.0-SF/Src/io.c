#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"





#include "stdio.h"



Io_readable_t* readable_io[READ_GROUP_CNT];



Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_0,
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
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    }
	
	
};




static u8 io_hd_read(Io_cfg_t *self){
	
	bool rt;
	

		rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}



void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}
