#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "bus.h"
#include "tool.h"
#include "color.h"

static u32 reg_led[REG_LED_CNT];
static u32 slide_led[SLIDE_LED_CNT];
static ColorMod_t color;
//static WS2812B_CREATE m_ws2812b_create;
//static WS2812B_WRITE m_ws2812b_write;

static void reg_chip_init(ColorChip_t* self);
static void slide_chip_init(ColorChip_t* self);
static void write(u8 channel, u32 color);
static void write_report(bus_packet* packet);
static u32 pack(u8 r, u8 g, u8 b);
static void write_chip(ColorChip_t* self);

/***外部函数***/
void colormod_init(){
    reg_chip_init(&color.reg_chip);
    slide_chip_init(&color.slide_chip);
    color.write = write;
    color.write_report =  write_report;
    color.pack = pack;
}

ColorMod_t* colormod_get_as_ref(void){
    return &color;
}

/** ColorChip_t成员函数 **/
static void ws_reg_pin_write(u8 value){
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, (GPIO_PinState)value);
}

static void reg_chip_init(ColorChip_t* self){
    ws2812b_init(&self->ws, ws_reg_pin_write);
    self->led_array = reg_led;
    self->led_len = 3;
    reg_led[0] = 0x0;
    reg_led[1] = 0x0;
    reg_led[2] = 0x0;
    write_chip(self);
}

static void ws_slide_pin_write(u8 value){
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, (GPIO_PinState)value);
}

static void slide_chip_init(ColorChip_t* self){
	ws2812b_init(&self->ws, ws_slide_pin_write);
    self->led_array = slide_led;
    self->led_len = 5;
    slide_led[0] = 0x0;
    slide_led[1] = 0x0;
    slide_led[2] = 0x0;
    slide_led[3] = 0x0;
    slide_led[4] = 0x0;
    write_chip(self);
}

static void write_chip(ColorChip_t* self){
    self->ws.write(&self->ws, self->led_array, self->led_len);
}

static void chip_color_set(ColorChip_t* self, u8 idx, u32 value){
    self->led_array[idx] = value;
}

/** ColorMod成员函数 **/
static u32 pack(u8 r, u8 g, u8 b){
    Color_u color;
    color.rgb.r = r;
    color.rgb.g = g;
    color.rgb.b = b;
    return color.value;
}

static void write(u8 channel, u32 value){
    ASSERT(channel >= REG_LED_CNT + SLIDE_LED_CNT, "channel >= REG_LED_CNT + SLIDE_LED_CNT");
    if(channel <= REG_LED_CNT){
        chip_color_set(&color.reg_chip, channel, value);
        write_chip(&color.reg_chip);
    }else{
        chip_color_set(&color.slide_chip, channel - REG_LED_CNT, value);
        write_chip(&color.slide_chip);
    }
}

static void write_report(bus_packet* packet){
    u8 channel = packet->data[4];
    u32 value =  ReadU32(&packet->data[5]);
    Color_u color;
    color.rgb.r = value & 0xff;
    color.rgb.g = (value >> 8) & 0xff;
    color.rgb.b = (value >> 16) & 0xff;
    write(channel, color.value);
    u8 data[] = {0};
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,1,data);
}

/** 测试函数 **/
void colormod_write_color_test(u8 channel, u8 r, u8 g, u8 b){
    write(channel, pack(r,g,b));
}
