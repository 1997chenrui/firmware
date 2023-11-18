#include "sensors.h"
#include "stm32f4xx_hal.h"
#include "tool.h"

Temperature_sensor_t temperature;
static u8 temperature_dq_read(void);
static void temperature_dq_write(u8 val);
static void temperature_buffer_report(bus_packet* packet);
static f32 temperature_read(void);


void temperature_sensor_init(void){
    DS18B20_Init(&temperature.sensor, temperature_dq_write, temperature_dq_read);
    temperature.read = temperature_read;
    temperature.buffer_report = temperature_buffer_report;
}

Temperature_sensor_t* temperaturesensor_get_as_ref(void){
    return &temperature;
}

/** 局部函数 **/
static u8 temperature_dq_read(void){
    return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
}

static void temperature_dq_write(u8 val){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, (GPIO_PinState)val);
}

/** Temperature_sensor_t 成员函数 **/
static f32 temperature_read(void){
    return GetTemp_DS18B20_median_filter(&temperature.sensor);
}

static void temperature_buffer_report(bus_packet* packet){
    u8 data[5];
    data[0] = 0;
    f32 current = GetTemp_Buffer(&temperature.sensor);
    printf("current temp = %4f\r\n", current);
    WriteF32(&data[1], current);
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,5,data);
}



