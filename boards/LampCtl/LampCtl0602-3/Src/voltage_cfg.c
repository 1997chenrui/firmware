#include "stm32f4xx.h"
#include "voltage.h"
#include "macro.h"
#include "soft_iic.h"
#include "my_main.h"
#include "voltage_cfg.h"


SoftIIc voltageIIC=
{
	.IO=
	{
		GPIOE,GPIO_PIN_13,GPIOE,GPIO_PIN_14
	},
	.speed=LOW,
	.mode=OD,
	.ack_timeout=250
};

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {

	{
		.max_voltage = 500,
		.voltage_range = {0, 255}
	},
	{
		.max_voltage = 500,
		.voltage_range = {0, 255}
	},
	{
		.max_voltage = 500,
		.voltage_range = {0, 255}
	},
	{
		.max_voltage = 500,
		.voltage_range = {0, 255}
	}
};

void hd0_voltage_write(f32 voltagef);
void hd1_voltage_write(f32 voltagef);
void hd2_voltage_write(f32 voltagef);
void hd3_voltage_write(f32 voltagef);


void voltage_hd_init(void) {

	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	vol_array[2].hd_voltage_write = hd0_voltage_write;
	vol_array[3].hd_voltage_write = hd1_voltage_write;
	
//	hd0_voltage_write(100);//570 黄光
//	hd1_voltage_write(30);
//	hd2_voltage_write(30);
//	hd3_voltage_write(150);//405紫光

}

void hd0_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	

	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC,0x80);
		
	    Soft_iic_write(&voltageIIC,0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC,voltage>>8);
		
		Soft_iic_write(&voltageIIC,(voltage)&0x00ff);
	}

	Soft_iic_stop(&voltageIIC);
}

void hd1_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);

	

	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC,0x80);
		
	    Soft_iic_write(&voltageIIC,0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC,voltage>>8);
		
		Soft_iic_write(&voltageIIC,(voltage)&0x00ff);
	}

	Soft_iic_stop(&voltageIIC);
}

void hd2_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;

	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC,0x80);
		
	    Soft_iic_write(&voltageIIC,0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC,voltage>>8);
		
		Soft_iic_write(&voltageIIC,(voltage)&0x00ff);
	}

	Soft_iic_stop(&voltageIIC);
}

void hd3_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;

	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0x18);
	
	if(voltage==0)
	{
		Soft_iic_write(&voltageIIC,0x80);
		
	    Soft_iic_write(&voltageIIC,0x00);
	}
	else
	{
		voltage=(voltage<<4)&0x3ff0;
		Soft_iic_write(&voltageIIC,voltage>>8);
		
		Soft_iic_write(&voltageIIC,(voltage)&0x00ff);
	}

	Soft_iic_stop(&voltageIIC);
}




