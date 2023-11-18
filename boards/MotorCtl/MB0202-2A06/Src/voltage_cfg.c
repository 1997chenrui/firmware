#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_thread_t voltage_thread_os[VOLTAGE_CHANNEL_CNT];
//队列成员应该是命令码和send对象	
SeqQueue_Struct   voltage_queue[VOLTAGE_CHANNEL_CNT];

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
    {.max_voltage = 4095, .voltage_range = {0, 4095}, .current_voltage = 500, .last_time = 600},
};

VOLTAGE_CREATE_Q_BLOCK(0);

extern DAC_HandleTypeDef hdac;
#define CHANNEL DAC_CHANNEL_1

void dac_write(u32 value){
	HAL_DAC_Stop(&hdac, CHANNEL);
	HAL_DAC_SetValue(&hdac, CHANNEL, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, CHANNEL);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

static void hd0_voltage_write(f32 voltage){
	dac_write((u32)voltage);
}

static void hd0_voltage_write_time(f32 voltage, u16 time){
	dac_write((u32)voltage);
	OSTimeDly(time);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void voltage_hd_init(void) {
	VOLTAGE_THRAD_CREATE(10, 0);
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[0].hd_voltage_write_time = hd0_voltage_write_time;
}



