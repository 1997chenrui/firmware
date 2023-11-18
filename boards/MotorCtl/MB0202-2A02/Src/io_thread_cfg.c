#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    {//阀 V0201 PE8
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_8,
        .valid_level = IO_HIGH_VALID,
    },
	{//阀 V0202 PE7
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_7,
        .valid_level = IO_HIGH_VALID,
    },			
    {//阀 V0203 PC5
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_5,
        .valid_level = IO_HIGH_VALID,
    },
	{//阀 V0204 PC4
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
    },
	{//阀 V0205 PC3
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },			
    {//阀 V0206 PC2
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
        .valid_level = IO_HIGH_VALID,
    }
    			
};



static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
		printf("port=%x,pin=%d,state %d\r\n",(int32_t)self->port,self->pin,state);
		HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
	
    }
}

void writable_io_init(void )
{

    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);
	
}




