#include "stm32f4xx.h"
#include "voltage.h"
#include "macro.h"
#include "../../drivers/soft_iic.h"
#include "my_main.h"
#include "voltage_cfg.h"


SoftIIc voltageIIC[2];

static void SoftIIC_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4);
}

static void SoftIIC_writeScl_1(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_1(bool v)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_1(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2);
}


static void SoftIIC_delayUs(volatile uint32_t us)
{
    uint32_t delay_i = 0,delay_j = 0;
	for(delay_j = 0; delay_j< us; delay_j++)
		for(delay_i = 0;delay_i<39;delay_i++);
}

static void SoftIIC_CFG_Init(void)
{
    voltageIIC[0].setSclOut=NULL;
    voltageIIC[0].setSdaOut=NULL;
    voltageIIC[0].setSdaIn=NULL;
	
    voltageIIC[0].writeScl=SoftIIC_writeScl_0;
    voltageIIC[0].writeSda=SoftIIC_writeSda_0;
    voltageIIC[0].readSda=SoftIIC_readSda_0;
    voltageIIC[0].delay=SoftIIC_delayUs;
    voltageIIC[0].ack_timeout=500;//应答超时时间 
	
    voltageIIC[1].setSclOut=NULL;
    voltageIIC[1].setSdaOut=NULL;
    voltageIIC[1].setSdaIn=NULL;
	
    voltageIIC[1].writeScl=SoftIIC_writeScl_1;
    voltageIIC[1].writeSda=SoftIIC_writeSda_1;
    voltageIIC[1].readSda=SoftIIC_readSda_1;
    voltageIIC[1].delay=SoftIIC_delayUs;
    voltageIIC[1].ack_timeout=500;//应答超时时间 
}


Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {

	{
		.max_voltage = 1024,
		.voltage_range = {0, 512}
	},
	{
		.max_voltage = 1024,
		.voltage_range = {0, 512}
	}
};

void hd0_voltage_write(f32 voltagef);
void hd1_voltage_write(f32 voltagef);


void voltage_hd_init(void) {

	
	SoftIIC_CFG_Init();
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;

	
	hd0_voltage_write(1024/3);//485nm
	hd1_voltage_write(1024/3);//575nm


}

void hd0_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;

	Soft_iic_start(&voltageIIC[0]);
	Soft_iic_write(&voltageIIC[0],0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC[0],0x80);
		
	    Soft_iic_write(&voltageIIC[0],0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC[0],voltage>>8);
		
		Soft_iic_write(&voltageIIC[0],(voltage)&0x00ff);
	}
	Soft_iic_stop(&voltageIIC[0]);
}


//570nm
void hd1_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;

	

	
	Soft_iic_start(&voltageIIC[1]);
	Soft_iic_write(&voltageIIC[1],0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC[1],0x80);
		
	    Soft_iic_write(&voltageIIC[1],0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC[1],voltage>>8);
		
		Soft_iic_write(&voltageIIC[1],(voltage)&0x00ff);
	}

	Soft_iic_stop(&voltageIIC[1]);
}







