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
		GPIOE,GPIO_PIN_6,GPIOE,GPIO_PIN_5
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
void hd4_voltage_write(f32 voltagef);
void hd5_voltage_write(f32 voltagef);
void hd6_voltage_write(f32 voltagef);
void hd7_voltage_write(f32 voltagef);


void voltage_hd_init(void) {

	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	vol_array[2].hd_voltage_write = hd2_voltage_write;
	vol_array[3].hd_voltage_write = hd3_voltage_write;
	vol_array[4].hd_voltage_write = hd4_voltage_write;
	vol_array[5].hd_voltage_write = hd5_voltage_write;
	vol_array[6].hd_voltage_write = hd6_voltage_write;
	vol_array[7].hd_voltage_write = hd7_voltage_write;
	
	hd0_voltage_write(145);//570nm 黄光
	hd1_voltage_write(115);//405nm 紫光
	hd2_voltage_write(65);//805nm 红外光
	hd3_voltage_write(20);//670nm 红光
	hd4_voltage_write(130);//570nm 黄光
	hd5_voltage_write(180);//405nm 紫光
	hd6_voltage_write(55);//805nm 红外光
	hd7_voltage_write(22);//670nm 红光
	printf("inital lamp set successfully!!!\r\n");

}


// 570nm
void hd0_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x01);//通道0
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

//405nm
void hd1_voltage_write(f32 voltagef)
{	
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x08);//通道3
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

//805nm
void hd2_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x04);//通道2
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

//680nm
void hd3_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x02);//通道1
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

// 570nm
void hd4_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x10);//通道4
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

//405nm
void hd5_voltage_write(f32 voltagef)
{	
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x80);//通道7
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

//805nm
void hd6_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x40);//通道6
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

//680nm
void hd7_voltage_write(f32 voltagef)
{
	u16 voltage=(u16)voltagef;
	Soft_iic_stop(&voltageIIC);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	OSTimeDly(100);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	OSTimeDly(100);	
	
	Soft_iic_start(&voltageIIC);
	Soft_iic_write(&voltageIIC,0xe0);
	Soft_iic_write(&voltageIIC,0x20);//通道5
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





