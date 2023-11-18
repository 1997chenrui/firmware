#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
	{//PE15 透明翻门传感器状态
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_1,
        .valid_level = IO_LOW_VALID,
    }
};
Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
    {//PB3 风扇1控制
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },
	{//PB4 灯带控制
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_4,
        .valid_level = IO_HIGH_VALID,
    }						
};



static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
		
		HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
    }

}

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}


void writable_io_init(void )
{

    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);
	
}


void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}
