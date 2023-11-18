#include <stdlib.h>
#include "io.h"
#include "tool.h"

#define IO_MAX 16

static Io_Group_t group;
static void write(u32 state);
static void write_report(bus_packet* packet);
static void timing_io_write(void* timing, u8 len);

void io_init(){
    group.io_array[0].port = GPIOE;
    group.io_array[0].pin = GPIO_PIN_2;
    group.io_array[0].state = 0;
    group.io_array[0].feature = 1;

    group.io_array[1].port = GPIOE;
    group.io_array[1].pin = GPIO_PIN_3;
    group.io_array[1].state = 0;
    group.io_array[1].feature= 1;
    
    group.io_array[2].port = GPIOE;
    group.io_array[2].pin = GPIO_PIN_4;
    group.io_array[2].state = 0;
    group.io_array[2].feature= 1;

    group.io_array[3].port = GPIOE;
    group.io_array[3].pin = GPIO_PIN_5;
    group.io_array[3].state = 0;
    group.io_array[3].feature= 1;

    group.io_array[4].port = GPIOE;
    group.io_array[4].pin = GPIO_PIN_6;
    group.io_array[4].state = 0;
    group.io_array[4].feature= 1;

    group.io_array[5].port = GPIOC;
    group.io_array[5].pin = GPIO_PIN_13;
    group.io_array[5].state = 0;
    group.io_array[5].feature= 1;

    group.io_array[6].port = GPIOA;
    group.io_array[6].pin = GPIO_PIN_1;
    group.io_array[6].state = 0;
    group.io_array[6].feature= 1;

    group.io_array[7].port = GPIOA;
    group.io_array[7].pin = GPIO_PIN_2;
    group.io_array[7].state = 0;
    group.io_array[7].feature= 1;

    group.io_array[8].port = GPIOA;
    group.io_array[8].pin = GPIO_PIN_3;
    group.io_array[8].state = 0;
    group.io_array[8].feature= 1;

    group.write = write;
    group.write_report = write_report;
    group.timing_io_write = timing_io_write;
}

Io_Group_t* io_get_as_ref(){
    return &group;
}


static void write(u32 state){
    int val;
    for(int i = 0; i < IO_MAX; i++){
        val = state & 0x03;
        if(val == 1){
            HAL_GPIO_WritePin(group.io_array[i].port, group.io_array[i].pin, group.io_array[i].state ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }else if(val == 2){
            HAL_GPIO_WritePin(group.io_array[i].port, group.io_array[i].pin, group.io_array[i].state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        state = state >> 2;
    }
}

static void timing_io_write(void* value, u8 len){
    Io_timing_t* timing = (Io_timing_t* )value;
    for(int i = 0;i<len;i++){
        write(timing[i].io_value);
        bus_os_sleep(timing[i].time_delay);
    }
}


static void write_report(bus_packet* packet){
    u32 state =  ReadU32(&packet->data[4]);
    write(state);
    u8 data[] = {0};
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,1,data);
}
