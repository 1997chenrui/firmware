#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 

//Io_cfg_t readable_io_array[IO_READABLE_CNT] = {



//		
//};


Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    
	{//P1
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_9,
        .valid_level = IO_LOW_VALID,
    }						
};




static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
			HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);

    }

}


void writable_io_init(void )
{

	
    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);

	
	
}

