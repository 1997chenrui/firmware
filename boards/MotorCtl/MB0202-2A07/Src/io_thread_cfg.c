#include <stdlib.h>
#include "io_thread_cfg.h"
#include "tool.h"
#include "io_mod.h"

Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];

Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
	{//PE8 玻片数量检测传感器
        .io_type = IO_TYPE_IO,
        .port = GPIOE,
        .pin = GPIO_PIN_8,
        .valid_level = IO_LOW_VALID,
    },
	{//PA15 液位检测传感器1
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    },
	{//PA12 液位检测传感器2
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
    },
	{//PB12 玻片抓手Z电机零位
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
    },
	{//PB4 玻片抓手Y电机零位
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    }
	,
	{//PB14 玻片转手旋转电机零位
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_14,
        .valid_level = IO_LOW_VALID,
    }
	,
	{//PD15 玻片夹爪电机零位
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    }
};

Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = {
	{//PB6 电磁继电器
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
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

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	
	rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
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

void readable_io_init(void )
{
	readable_io[0] = readable_io_new(io_hd_read, READABLE_CHANGE_ENABLE_VALUE, readable_io_array_0, IO_READABLE_CNT_0);
}
