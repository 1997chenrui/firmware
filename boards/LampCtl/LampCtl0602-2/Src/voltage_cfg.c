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
		.max_voltage = 1024,
		.voltage_range = {0, 512}
	},
	{
		.max_voltage = 1024,
		.voltage_range = {0, 512}
	},
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
void hd2_voltage_write(f32 voltagef);
void hd3_voltage_write(f32 voltagef);

void voltage_hd_init(void) {

	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	vol_array[2].hd_voltage_write = hd2_voltage_write;
	vol_array[3].hd_voltage_write = hd3_voltage_write;
	
	hd0_voltage_write(80*2);//405nm 黄光
	hd1_voltage_write(200*2);//570nm
	hd2_voltage_write(55);//670nm
	hd3_voltage_write(55);//近红外

}


// 405nm
void hd0_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x01);
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

//570nm
void hd1_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x08);
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

//670nm
void hd2_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x04);
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

//近红外
void hd3_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x02);
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





