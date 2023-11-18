#include "voltage_cfg.h"

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
    {.max_voltage = 255, .voltage_range = {0, 255}},
    {.max_voltage = 255, .voltage_range = {0, 255}},
    {.max_voltage = 255, .voltage_range = {0, 255}},
    {.max_voltage = 255, .voltage_range = {0, 255}},
};


u8 iic_id_array[VOLTAGE_CHANNEL_CNT];
static void hd0_voltage_write(f32 voltage);
static void hd1_voltage_write(f32 voltage);
static void hd2_voltage_write(f32 voltage);
static void hd3_voltage_write(f32 voltage);

void voltage_hd_init(void) {
	
    iic_id_array[0] = ad5245_create(PD12, PD11);//n800
    iic_id_array[1] = ad5245_create(PD14, PD13);//n660
    iic_id_array[2] = ad5245_create(PC8, PC7);//n405
    iic_id_array[3] = ad5245_create(PC6, PD15);//570
	
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	vol_array[2].hd_voltage_write = hd2_voltage_write;
	vol_array[3].hd_voltage_write = hd3_voltage_write;
	
	hd0_voltage_write(0);
	hd1_voltage_write(0);
	hd2_voltage_write(0);
	hd3_voltage_write(0);
	
}

static void hd0_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[0], 0, (u8)voltage);
}

static void hd1_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[1], 0, (u8)voltage);
}

static void hd2_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[2], 0, (u8)voltage);
}

static void hd3_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[3], 0, (u8)voltage);
}

