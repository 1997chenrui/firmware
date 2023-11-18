#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
	{
		.max_voltage = 255,
		.voltage_range = {0, 255}
	},
	{
		.max_voltage = 255,
		.voltage_range = {0, 255}
	}
};

u8 iic_id_array[VOLTAGE_IIC_CHANNEL_CNT];

static void hd0_voltage_write(f32 voltage);
static void hd1_voltage_write(f32 voltage);

void voltage_hd_init(void) {
	
	iic_id_array[0] = ad5245_create(PA2, PA3);
	iic_id_array[1] = iic_id_array[0]; 
	
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	
	hd0_voltage_write(0);
	hd1_voltage_write(0);
}



static void hd0_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[0], 0, (u8)voltage);
	printf("iic 0 write voltage=%d\r\n",(u8)voltage);
}

static void hd1_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[1], 0, (u8)voltage);
	printf("iic 1 write voltage=%d\r\n",(u8)voltage);
}


