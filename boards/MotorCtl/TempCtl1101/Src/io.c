#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

Io_readable_t* readable_io[READ_GROUP_CNT];

// static void io_hd_write(Io_cfg_t *self, Io_state_e state);


static u8 io_hd_read(Io_cfg_t *self);

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    }
};
void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}
