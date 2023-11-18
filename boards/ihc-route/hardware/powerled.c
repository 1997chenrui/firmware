#include "powerled.h"
#include "stm32f4xx_hal.h"

static PowerLed_t led;
extern DAC_HandleTypeDef hdac;

static void write(u8 idx, f32 voltage);
static void write_report(bus_packet* packet);
/** 全局函数 **/
void powerled_init(void){
    led.channel[0] = DAC_CHANNEL_1;
    led.channel[1] = DAC_CHANNEL_2;
    led.write = write;
    led.write_report = write_report;
}

PowerLed_t* powerled_get_as_ref(void){
    return &led;
}

/** 局部函数 **/
static u32 to_dialog(float voltage){
    return (u32)(voltage/3.3*4096);
}

static void dac_write(u32 channel, u32 value){
	HAL_DAC_Stop(&hdac, channel);
	HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac, channel);
}

/** PowerLed_t 成员函数 */
static void write(u8 idx, f32 voltage){
    dac_write(led.channel[0], to_dialog(voltage));
    dac_write(led.channel[1], to_dialog(voltage));
}

static void write_report(bus_packet* packet){
    u8 idx = packet->data[4];
    f32 voltage =  ReadF32(&packet->data[5]);
    dac_write(led.channel[0], to_dialog(voltage));
    dac_write(led.channel[1], to_dialog(voltage));
    u8 data[] = {0};
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,1,data);
}

/** 测试函数 **/
void powerled_write_3_3_test(){
    write(0, 3.3f);
    write(1, 3.3f);
}

void powerled_write_0_test(){
    write(0, 0.0f);
    write(1, 0.0f);
}
