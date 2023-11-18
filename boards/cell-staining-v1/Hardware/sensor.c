#include "sensor.h"
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

static u16 read(u8 index);
static void read_report(bus_packet* packet);
static Sensor_t sensor;
static u16 adc_buffer_array[SENSOR_CNT];

void sensor_init(){
    sensor.read = read;
    sensor.read_report = read_report;
	HAL_ADC_Start_DMA(&hadc1, (u32*)(&adc_buffer_array[0]), 4);
}

Sensor_t* sensor_get_as_ref(){
    return &sensor;
}

static u16 read(u8 index){
    ASSERT(index >= SENSOR_CNT, "sensor get error\r\n");
    return adc_buffer_array[index];
}

void read_report(bus_packet* packet){
    u8 idx = packet->data[4];
    u8 data[2];
    WriteU16(data, read(idx));   
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,2,data);
}
