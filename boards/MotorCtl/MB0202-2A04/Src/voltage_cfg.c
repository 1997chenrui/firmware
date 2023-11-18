#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_thread_t voltage_thread_os[VOLTAGE_QUEUE_CNT];
//队列成员应该是命令码和send对象	
SeqQueue_Struct   voltage_queue[VOLTAGE_QUEUE_CNT];

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
    {.max_voltage = 4095, .voltage_range = {0, 4095}, .current_voltage = 500, .last_time = 600}
	
//	{
//		.max_voltage = 255,
//		.voltage_range = {0, 255}
//	},
//	{
//		.max_voltage = 255,
//		.voltage_range = {0, 255}
//	}
};

VOLTAGE_CREATE_Q_BLOCK(0);

extern DAC_HandleTypeDef hdac;
#define CHANNEL DAC_CHANNEL_1
u8 iic_id_array[VOLTAGE_IIC_CHANNEL_CNT];

static void hd0_voltage_write(f32 voltage);
static void hd1_voltage_write(f32 voltage);
static void hd2_voltage_write(f32 voltage);

void dac_write(u32 value){
	HAL_DAC_Stop(&hdac, CHANNEL);
	HAL_DAC_SetValue(&hdac, CHANNEL, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, CHANNEL);
}

static void hd0_voltage_write(f32 voltage){
	dac_write((u32)voltage);
}

void hd0_voltage_write_time(f32 voltage, u16 time){
	printf("voltage_last = %f\r\n", voltage);
	dac_write((u32)(voltage));
	OSTimeDly(time);
	dac_write((u32)0);
}

void voltage_hd_init(void) {
	VOLTAGE_THRAD_CREATE(10, 0);
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[0].hd_voltage_write_time = hd0_voltage_write_time;
	
//	iic_id_array[0] = ad5245_create(PE4, PE5);
//	iic_id_array[1] = ad5245_create(PE2, PE3); 
//	
//	vol_array[1].hd_voltage_write = hd1_voltage_write;
//	vol_array[2].hd_voltage_write = hd2_voltage_write;
	
//	hd1_voltage_write(0);
//	hd2_voltage_write(0);
}



static void hd1_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[0], 0, (u8)voltage);
	printf("iic 0 write voltage=%d\r\n",(u8)voltage);
}

static void hd2_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[1], 0, (u8)voltage);
	printf("iic 1 write voltage=%d\r\n",(u8)voltage);
}
