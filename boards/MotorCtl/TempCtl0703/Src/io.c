#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

static void io_hd_write(Io_cfg_t *self, Io_state_e state);
static void io_write_test_0(u8 open);
static void io_write_test_1(u8 open);

static u8 io_hd_read(Io_cfg_t *self);
static void io_read_test_0(void);

static u8 io_hd_read(Io_cfg_t *self);

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
    {// Reagant hall sensor (middle)
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_1,
        .valid_level = IO_LOW_VALID,
    },
	{// Emmergency hall sensor (right)
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_2,
        .valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = //普通IO
{
    {//fan  PC10
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    },
	{//solenvid valve PC11
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
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
        HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
    }
}

static void io_write_test_0(u8 open){
    u32 state = 0;
    for(int i = 0; i < IO_WRITABLE_CNT_0; i++){
        state |= (open ? 2 : 1) << (i<<1);
    }
    writable_io[0]->group_write(writable_io[0], state);
}

static u8 io_hd_read(Io_cfg_t *self){
    return HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
}

static void io_read_test_0(void){
	u32 state = readable_io[0]->group_read(readable_io[0]);
	for(int i = 0; i < IO_WRITABLE_CNT_0; i++){
		printf("%d,", (state >> i) & 0x01);
	}
	printf("\r\n");
}

