#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

static void io_hd_write(Io_cfg_t *self, Io_state_e state);


static u8 io_hd_read(Io_cfg_t *self);


static u8 io_hd_read(Io_cfg_t *self);

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
	{//垃圾箱传感器4
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_14,
        .valid_level = IO_LOW_VALID,
    },
	{//预留传感器1
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
	{//预留传感器2
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
    },
	{//预留传感器3
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_13,
        .valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = //普通IO
{
    {//fan1 PE3 风扇1
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_3,
        .valid_level = IO_HIGH_VALID,
    },
	{//fan2 PE2 风扇2
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_2,
        .valid_level = IO_HIGH_VALID,
    }
};

void writable_io_init(void )
{
    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);

}

void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}

static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
		if(self->valid_level==IO_HIGH_VALID)
			HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
		else
			HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)!state);
    }
}



static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	if(self->valid_level==IO_LOW_VALID)
	{
		rt= ! HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	}
	else
	{
		
		rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	}
    return rt;
}



