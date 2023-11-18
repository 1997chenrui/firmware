#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_thread_t voltage_thread_os[VOLTAGE_CHANNEL_CNT];
//队列成员应该是命令码和send对象	
SeqQueue_Struct   voltage_queue[VOLTAGE_CHANNEL_CNT];

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
    {.max_voltage = 4096, .voltage_range = {0, 4096}},
};

VOLTAGE_CREATE_Q_BLOCK(0);

extern DAC_HandleTypeDef hdac;
#define CHANNEL DAC_CHANNEL_1

void dac_write(u32 value){
	HAL_DAC_Stop(&hdac, CHANNEL);
	HAL_DAC_SetValue(&hdac, CHANNEL, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, CHANNEL);
}

static void hd0_voltage_write(f32 voltage){
	dac_write((u32)voltage);
}


static void hd0_voltage_write_time(f32 voltage, u16 time){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
	dac_write(4095);
	OSTimeDly(50);
	dac_write((u32)voltage);
	OSTimeDly(time);
	dac_write(0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
}

void voltage_hd_init(void) {
	VOLTAGE_THRAD_CREATE(10, 0);
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[0].hd_voltage_write_time = hd0_voltage_write_time;
}



