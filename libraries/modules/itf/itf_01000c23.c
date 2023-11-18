/**
 * @file itf_01000c23.c
 * @author Chenr
 * @brief 电机移动在特定位置检测是否存在样本瓶
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_01000c23.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c23_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c23_t* self, bus_packet* packet, u8 error_code, f32 motor_move_volum, u32 stop_press, u32 final_v, u32 intial_press);

Itf_01000c23_t* Itf_01000c23_new(void){
    Itf_01000c23_t* self = (Itf_01000c23_t*)malloc(sizeof(Itf_01000c23_t));
    memset(&self->content, 0, sizeof(Itf_01000c23_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c23_t* self, bus_packet* packet){
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
	self->content.motor_id = packet->data[offset];
	
	offset++;
	self->content.absorb_expel_flag = packet->data[offset];
	
	offset++;
	self->content.temp_power = ReadF32(&packet->data[offset]);
	
	offset+=4;
	self->content.adjust_press_len = ReadU32(&packet->data[offset]);
	
	offset+=4;
	self->content.adjust_press = (f32 *)malloc(sizeof(f32)*self->content.adjust_press_len);
	memmove(self->content.adjust_press,&packet->data[offset],self->content.adjust_press_len*4);
	
	offset+=self->content.adjust_press_len*4;
	self->content.press_intial = ReadU32(&packet->data[offset]);
	
	offset+=4;
	self->content.standard_press = ReadU32(&packet->data[offset]);
	
	offset+=4;
	
//	printf("m1_begin_ml=%f,m2_dest_ml=%f,m3_speed=%f,m4_acce=%f,m5_done_valve_ctrl=%d,motor_id=%d,absorb_expel_flag=%d,temp_power=%f\r\n",
//	self->content.m1_begin_ml,self->content.m2_dest_ml,self->content.m3_speed,
//	self->content.m4_acce,self->content.m5_done_valve_ctrl,self->content.motor_id,self->content.absorb_expel_flag,self->content.temp_power);
//	printf("adjust_press_len = %d\r\n",self->content.adjust_press_len);	
//	for(int i=0; i<self->content.adjust_press_len; i++)
//	{
//		printf("%f,",self->content.adjust_press[i]);
//	}
//	putchar('\n');
//	printf("press_intial=%d, standard_press=%d\r\n",self->content.press_intial,self->content.standard_press);

}

static void ret_and_free(Itf_01000c23_t* self, bus_packet* packet, u8 error_code, f32 motor_move_volum ,u32 stop_press, u32 final_v, u32 intial_press){

	u8 *data = (u8 *)malloc(13);
	u8 offset = 0;
	data[offset++] = error_code;
	WriteF32(&data[offset], motor_move_volum);
	offset+=4;
	WriteU32(&data[offset], stop_press);
	offset+=4;
	WriteU32(&data[offset], final_v);
	offset+=4;
	WriteU32(&data[offset], intial_press);
	offset+=4;
    BUS_ITA_RTN((*packet), data, offset);//通过总线将数据发送给上位机
	free(self->content.adjust_press);
	free(data);
    free(self);
}
