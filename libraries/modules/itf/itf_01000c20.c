/**
 * @file itf_01000c20.c
 * @author Chenr
 * @brief 电机移动在特定位置检测是否存在样本瓶
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_01000c20.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c20_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c20_t* self, bus_packet* packet, u8 error_code, u32 final_v);

Itf_01000c20_t* Itf_01000c20_new(void){
    Itf_01000c20_t* self = (Itf_01000c20_t*)malloc(sizeof(Itf_01000c20_t));
    memset(&self->content, 0, sizeof(Itf_01000c20_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c20_t* self, bus_packet* packet){
    u8 offset = 4;
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
	self->content.adjust_press_len = ReadU32(&packet->data[offset]);
	
	offset+=4;
	self->content.press_adjust = (f32 *)malloc(sizeof(f32)*self->content.adjust_press_len);
	memmove(self->content.press_adjust,&packet->data[offset],self->content.adjust_press_len*4);
	
	offset+=self->content.adjust_press_len*4;
	self->content.press_ad = ReadU32(&packet->data[offset]);
	
	offset+=4;
	self->content.standard_press = ReadU32(&packet->data[offset]);
	
	offset+=4;
	
//	printf("adjust_press_len = %d\r\n",self->content.adjust_press_len);	
//	for(int i=0; i<self->content.adjust_press_len; i++)
//	{
//		printf("%f,",self->content.press_adjust[i]);
//	}
//	putchar('\n');
}

static void ret_and_free(Itf_01000c20_t* self, bus_packet* packet, u8 error_code, u32 final_v){

	u8 data[5];
	data[0] = error_code;
	WriteU32(&data[1], final_v);
    BUS_ITA_RTN((*packet), data, 5);//通过总线将数据发送给上位机
	free(self->content.press_adjust);
    free(self);
}
