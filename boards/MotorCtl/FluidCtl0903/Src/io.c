#include <stdlib.h>
#include "io.h"
#include "tool.h"
#include "io_mod.h"


#include "pca9685.h"


#include "../../drivers/PCA9685.h"
#include "../../drivers/soft_iic.h"
#include "stdio.h"


static SoftIIc SoftIICInstance;

static void SoftIIC_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12);
}

static void SoftIIC_delayUs(volatile uint32_t us)
{
    uint32_t delay_i = 0,delay_j = 0;
	for(delay_j = 0; delay_j< us; delay_j++)
		for(delay_i = 0;delay_i<39;delay_i++);
}

static void SoftIIC_CFG_Init(void)
{
    SoftIICInstance.setSclOut=NULL;
    SoftIICInstance.setSdaOut=NULL;
    SoftIICInstance.setSdaOut=NULL;
    SoftIICInstance.writeScl=SoftIIC_writeScl_0;
    SoftIICInstance.writeSda=SoftIIC_writeSda_0;
    SoftIICInstance.readSda=SoftIIC_readSda_0;
    SoftIICInstance.delay=SoftIIC_delayUs;
    SoftIICInstance.ack_timeout=500;//应答超时时间 
	

}

PCA9685 PCA9685Instances;


static void PCA9685_startWr_0(void)
{
    Soft_iic_stop(&SoftIICInstance);
    Soft_iic_start(&SoftIICInstance);   
}

static void PCA9685_stopWr_0(void)
{
    Soft_iic_stop(&SoftIICInstance);
}

static bool PCA9685_writeByte_0(uint8_t data)
{
	
    if(Soft_iic_write(&SoftIICInstance,PCA9685Instances.address_write)==Soft_iic_timeout)
    {
        printf("IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }

    if(Soft_iic_write(&SoftIICInstance,data)==Soft_iic_timeout)
    {
        printf("IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	
//	printf("IIC WRITE DATA :%d\r\n",data);
	return true; 
}

void PCA9685InitDelay()
{
	SoftIIC_delayUs(1000);
}

void PCA9685_CFG_INIT(void)
{
    SoftIIC_CFG_Init();

    PCA9685Instances.address_write=0x80;
    PCA9685Instances.address_read=0x81;
    PCA9685Instances.startWR=PCA9685_startWr_0;
    PCA9685Instances.stopWR=PCA9685_stopWr_0;
    PCA9685Instances.writeByte=PCA9685_writeByte_0;
    PCA9685Instances.readByte=NULL;

    PCA9685Instances.stopWR();
	PCA9685Instances.startWR();
	
	PCA9685Instances.delay=PCA9685InitDelay;
	
	PCA9685Instances.freq=1000;
	Pca9685_Init(&PCA9685Instances);


}
Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
Io_readable_t* readable_io[READ_GROUP_CNT];


typedef enum {
    PCA_9685_CHANNEL0 = 0,
	PCA_9685_CHANNEL1,
	PCA_9685_CHANNEL2,
	PCA_9685_CHANNEL3,
	PCA_9685_CHANNEL4,
	PCA_9685_CHANNEL5,
	PCA_9685_CHANNEL6,
	PCA_9685_CHANNEL7,
	PCA_9685_CHANNEL8,
	PCA_9685_CHANNEL9,
	PCA_9685_CHANNEL10,
	PCA_9685_CHANNEL11,
	PCA_9685_CHANNEL12,
	PCA_9685_CHANNEL13,
	PCA_9685_CHANNEL14,
	PCA_9685_CHANNEL15,
}PCA9685_CHANNEL_CNT;


Io_cfg_t readable_io_array_0[IO_READABLE_CNT_0] = {
    {
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_2,
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
        .pin = GPIO_PIN_0,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_12,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_14,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
	{
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_5,
        .valid_level = IO_LOW_VALID,
    }
	
	
};


Io_cfg_t writable_io_array_0[IO_WRITABLE_CNT_0] = //��ͨIO
{
    {//V1 PA3
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_3,
        .valid_level = IO_LOW_VALID,
    },
	{//V2 PC4
        .io_type = IO_TYPE_IO,
        .port = GPIOA,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
    {//V3 PB10
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_4,
        .valid_level = IO_LOW_VALID,
    },
	{//V4 PD15
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_5,
        .valid_level = IO_LOW_VALID,
    },
    {//V5 PA4
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_10,
        .valid_level = IO_LOW_VALID,
    },
	{//V6 PC5
        .io_type = IO_TYPE_IO,
        .port = GPIOB,
        .pin = GPIO_PIN_11,
        .valid_level = IO_LOW_VALID,
    },
    {//V7 PB11
        .io_type = IO_TYPE_IO,
        .port = GPIOD,
        .pin = GPIO_PIN_15,
        .valid_level = IO_LOW_VALID,
    },
	{//V8 PC6
        .io_type = IO_TYPE_IO,
        .port = GPIOC,
        .pin = GPIO_PIN_6,
        .valid_level = IO_LOW_VALID,
    }
};



Io_cfg_t writable_io_array_1[IO_WRITABLE_CNT_1] = //PWM
{
    {//1
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL0,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//2
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL1,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//3
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL2,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//4
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL3,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//5
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL4,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//6
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL5,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//7
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL6,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//8
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL7,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//9
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL8,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//10
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL9,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//11
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL10,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//12
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL11,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//13
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL12,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//14
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL13,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
    {//15
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL14,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    },
	{//16
        .io_type = IO_TYPE_PCA_PWM,
		.pwm_channel = PCA_9685_CHANNEL15,
        .valid_level = IO_LOW_VALID,
		.pwm_dutycycle=50,
    }
};




static void io_hd_write(Io_cfg_t *self, Io_state_e state)
{
    if(self->io_type == IO_TYPE_IO)
	{
		printf("port=%x,pin=%d,state %d\r\n",(int32_t)self->port,self->pin,state);
		HAL_GPIO_WritePin((GPIO_TypeDef*) self->port, self->pin, (GPIO_PinState)state);
	
    }
	
	else if(self->io_type == IO_TYPE_PCA_PWM)
	{
		printf("io_hd_write_pwm %d\r\n",state);
		  if(state != 0)
		  {
			  Pca9685_Set_Pwm_L(&PCA9685Instances, self->pwm_channel, 0);
		  }
		  else
		  {
			  Pca9685_Set_Pwm_L(&PCA9685Instances, self->pwm_channel, self->pwm_dutycycle);
		  } 			
    }
}

static u8 io_hd_read(Io_cfg_t *self){
	bool rt;
	

		
		rt=  HAL_GPIO_ReadPin((GPIO_TypeDef*) self->port, self->pin);
	
    return rt;
}


void writable_io_init(void )
{

	PCA9685_CFG_INIT();
    writable_io[0] = writable_io_new(io_hd_write, writable_io_array_0, IO_WRITABLE_CNT_0);
	
	writable_io[1] = writable_io_new(io_hd_write, writable_io_array_1, IO_WRITABLE_CNT_1);  
	
	
	for(uint8_t i=0;i<16;i++)
		Pca9685_Set_Pwm_L(&PCA9685Instances,i,0);
	

	
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
