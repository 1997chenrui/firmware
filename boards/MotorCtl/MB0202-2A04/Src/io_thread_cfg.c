#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT];

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    {//电磁锁 S0401 PC3
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_3,
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


void writable_io_Test()
{
	for(uint8_t i=0;i<8;i++)
	{
		io_hd_write(&writable_io_array_0[i],IO_ENABLE);
		OSTimeDly(1000);
	}
	for(uint8_t i=0;i<8;i++)
	{
		io_hd_write(&writable_io_array_0[i],IO_DISABLE);
		OSTimeDly(1000);
	}	
}