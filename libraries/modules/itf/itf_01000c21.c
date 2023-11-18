/**
 * @file itf_01000c21.c
 * @author chenr
 * @brief 电机移动在特定位置检测是否存在样本瓶
 * @version 0.1
 * @date 2023-04-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_01000c21.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c21_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c21_t* self, bus_packet* packet, u8 error_code, int differ_max, f32 final_v);

Itf_01000c21_t* Itf_01000c21_new(void){
    Itf_01000c21_t* self = (Itf_01000c21_t*)malloc(sizeof(Itf_01000c21_t));
    memset(&self->content, 0, sizeof(Itf_01000c21_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c21_t* self, bus_packet* packet){
    u8 offset = 4;
	self->content.m0_absorb_expel_flag = packet->data[offset];
	offset++;
    self->content.m1_begin_ml = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.m2_dest_ml = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.m3_speed = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m4_acce = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m5_done_valve_ctrl = ReadU32(&packet->data[offset]);
	offset+=4;
	self->content.m6_standard_integral = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m7_standard_press = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m8_power = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m9_channel_id = packet->data[offset];
	offset++;
	self->content.m10_k = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m11_b = ReadF32(&packet->data[offset]);
	offset++;
	self->content.stop_flag_press = packet->data[offset];
}

static void ret_and_free(Itf_01000c21_t* self, bus_packet* packet, u8 error_code, int differ_max, f32 final_v){
	u8 data[9];
	data[0] = error_code;
	WriteU32(&data[1], differ_max);
	WriteF32(&data[5], final_v);
    BUS_ITA_RTN((*packet), data, 9);//通过总线将数据发送给上位机
    free(self);
}
