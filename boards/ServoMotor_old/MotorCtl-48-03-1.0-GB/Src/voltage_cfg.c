#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_thread_t voltage_thread_os[VOLTAGE_QUEUE_CNT];
//队列成员应该是命令码和send对象	
SeqQueue_Struct   voltage_queue[VOLTAGE_QUEUE_CNT];

Voltage_t vol_array[VOLTAGE_CHANNEL_CNT] = {
    {.max_voltage = 4095, .voltage_range = {0, 4095}, .current_voltage = 500, .last_time = 600}
};

VOLTAGE_CREATE_Q_BLOCK(0);

extern DAC_HandleTypeDef hdac;
#define CHANNEL DAC_CHANNEL_1



static void hd0_voltage_write(f32 voltage);

void dac_write(u32 value){
	HAL_DAC_Stop(&hdac, CHANNEL);
	HAL_DAC_SetValue(&hdac, CHANNEL, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, CHANNEL);
}

static void hd0_voltage_write(f32 voltage){
	dac_write((u32)voltage);
}

//static void hd0_voltage_write_time(f32 voltage, u16 time){
//static void hd0_voltage_write_time(f32 voltage, u16 time){
void hd0_voltage_write_time(f32 voltage, u16 time){
	if(time>4095){time=4095;}
	dac_write((u32)(voltage));
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	if(time>5000){time=5000;}
	OSTimeDly(time);
	dac_write((u32)0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}

void voltage_hd_init(void) {
	VOLTAGE_THRAD_CREATE(10, 0);
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[0].hd_voltage_write_time = hd0_voltage_write_time;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	
	
	//hd0_voltage_write_time(3000,2000);
}





