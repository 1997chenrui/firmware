#include "ad5282_cfg.h"
#include "stm32f4xx_hal.h"
#include "soft_iic.h"
#include "stdio.h"


static SoftIIc SoftIICInstance[2];

static void SoftIIC_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5);
}

static void SoftIIC_writeScl_1(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_1(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_1(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);
}

static void SoftIIC_delayUs(volatile uint32_t us)
{
    uint32_t delay_i = 0,delay_j = 0;
	for(delay_j = 0; delay_j< us; delay_j++)
		for(delay_i = 0;delay_i<39;delay_i++);
}

static void SoftIIC_CFG_Init(void)
{
    SoftIICInstance[0].setSclOut=NULL;
    SoftIICInstance[0].setSdaOut=NULL;
    SoftIICInstance[0].setSdaOut=NULL;
    SoftIICInstance[0].writeScl=SoftIIC_writeScl_0;
    SoftIICInstance[0].writeSda=SoftIIC_writeSda_0;
    SoftIICInstance[0].readSda=SoftIIC_readSda_0;
    SoftIICInstance[0].delay=SoftIIC_delayUs;
    SoftIICInstance[0].ack_timeout=500;//应答超时时间 
	
	SoftIICInstance[1].setSclOut=NULL;
    SoftIICInstance[1].setSdaOut=NULL;
    SoftIICInstance[1].setSdaOut=NULL;
    SoftIICInstance[1].writeScl=SoftIIC_writeScl_1;
    SoftIICInstance[1].writeSda=SoftIIC_writeSda_1;
    SoftIICInstance[1].readSda=SoftIIC_readSda_1;
    SoftIICInstance[1].delay=SoftIIC_delayUs;
    SoftIICInstance[1].ack_timeout=500;//应答超时时间 
}




AD5282 AD5282Instances[2];

static void AD5282_startWr_0(void)
{
    Soft_iic_stop(&SoftIICInstance[0]);
    Soft_iic_start(&SoftIICInstance[0]);   
}

static void AD5282_stopWr_0(void)
{
    Soft_iic_stop(&SoftIICInstance[0]);
}

static bool AD5282_writeByte_0(uint8_t data)
{
	
	AD5282_startWr_0();
    if(Soft_iic_write(&SoftIICInstance[0],AD5282Instances[0].address_write)==Soft_iic_timeout)
    {
        printf("IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }

    if(Soft_iic_write(&SoftIICInstance[0],data)==Soft_iic_timeout)
    {
        printf("IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	
	printf("IIC WRITE DATA :%d\r\n",data);
	return true; 
}

static uint8_t AD5282_readByte_0(bool acken)
{
	AD5282_startWr_0();
    // |0x01
    return Soft_iic_read(&SoftIICInstance[0],acken);
}

static void AD5282_startWr_1(void)
{
    Soft_iic_stop(&SoftIICInstance[1]);
    Soft_iic_start(&SoftIICInstance[1]);   
}

static void AD5282_stopWr_1(void)
{
    Soft_iic_stop(&SoftIICInstance[1]);
}

static bool AD5282_writeByte_1(uint8_t data)
{
	
	AD5282_startWr_1();
    if(Soft_iic_write(&SoftIICInstance[1],AD5282Instances[1].address_write)==Soft_iic_timeout)
    {
        printf("IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }

    if(Soft_iic_write(&SoftIICInstance[1],data)==Soft_iic_timeout)
    {
        printf("IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	
	printf("IIC WRITE DATA :%d\r\n",data);
	return true; 
}

static uint8_t AD5282_readByte_1(bool acken)
{
	AD5282_startWr_1();
    // |0x01
    return Soft_iic_read(&SoftIICInstance[1],acken);
}



void AD5282_CFG_INIT(void)
{
    SoftIIC_CFG_Init();

    AD5282Instances[0].address_write=0x58;
    AD5282Instances[0].address_read=0x59;
    AD5282Instances[0].startWR=AD5282_startWr_0;
    AD5282Instances[0].stopWR=AD5282_stopWr_0;
    AD5282Instances[0].writeByte=AD5282_writeByte_0;
    AD5282Instances[0].readByte=AD5282_readByte_0;

    AD5282Instances[0].stopWR();
	AD5282Instances[0].startWR();
    AD5282_Write(&AD5282Instances[0],1,0);
	AD5282_Write(&AD5282Instances[0],0,0);
	
	
	AD5282Instances[1].address_write=0x58;
    AD5282Instances[1].address_read=0x59;
    AD5282Instances[1].startWR=AD5282_startWr_1;
    AD5282Instances[1].stopWR=AD5282_stopWr_1;
    AD5282Instances[1].writeByte=AD5282_writeByte_1;
    AD5282Instances[1].readByte=AD5282_readByte_1;

    AD5282Instances[1].stopWR();
	AD5282Instances[1].startWR();
    AD5282_Write(&AD5282Instances[1],1,0);
	AD5282_Write(&AD5282Instances[1],0,0);
}

