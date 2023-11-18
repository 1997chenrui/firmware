#include "stm32f4xx.h"
#include "voltage_cfg.h"

Voltage_thread_t voltage_thread_os[VOLTAGE_QUEUE_CNT];
//队列成员应该是命令码和send对象	
SeqQueue_Struct   voltage_queue[VOLTAGE_QUEUE_CNT];

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

//VOLTAGE_CREATE_Q_BLOCK(0);


u8 iic_id_array[VOLTAGE_IIC_CHANNEL_CNT];


static void hd0_voltage_write(f32 voltage);
static void hd1_voltage_write(f32 voltage);



void voltage_hd_init(void) {
	
	iic_id_array[0] = ad5245_create(PE1, PE0);
	iic_id_array[1] = iic_id_array[0];
	
	vol_array[0].hd_voltage_write = hd0_voltage_write;
	vol_array[1].hd_voltage_write = hd1_voltage_write;
	
	hd0_voltage_write(0);
	hd1_voltage_write(0);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);//关闭650光源
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);//关闭405光源
	
}

static void hd0_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[0], 0, (u8)voltage);
	printf("iic 0 write voltage=%d\r\n",(u8)voltage);
	GPIO_PinState state = voltage < 0.1 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,state);//操作405光源（state = 0, 开启，state = 1, 关闭）

}

static void hd1_voltage_write(f32 voltage){
	ad5245_write(iic_id_array[1], 1, (u8)voltage);
	printf("iic 1 write voltage=%d\r\n",(u8)voltage);
	GPIO_PinState state = voltage < 0.1 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,state);//操作650光源（state = 0, 开启，state = 1, 关闭）
}


