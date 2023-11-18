/**
 * @file itf_01000c12.c
 * @author hds
 * @brief 电机移动在特定位置检测是否存在玻片
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_01000c12.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c12_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c12_t* self, bus_packet* packet, u16 *slide_position, u16 slide_cnt);

Itf_01000c12_t* Itf_01000c12_new(void){
    Itf_01000c12_t* self = (Itf_01000c12_t*)malloc(sizeof(Itf_01000c12_t));
    memset(&self->content, 0, sizeof(Itf_01000c12_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c12_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.m1_speed = ReadF32(&packet->data[offset]);
    offset+=4;
	self->content.m2_acce = ReadF32(&packet->data[offset]);
	offset+=4;
	self->content.m3_target_distance = ReadF32(&packet->data[offset]);
}

static void ret_and_free(Itf_01000c12_t* self, bus_packet* packet, u16 *slide_position, u16 slide_cnt){
	u8 *data=(u8 *)malloc(sizeof(u16)*slide_cnt+4);
    WriteU32(data, slide_cnt);//将长度赋值到data指向的前四个字节的空间中去
	memmove(data+4,slide_position,2*slide_cnt);//将value指向的数据的2*length个字节传到data指向的前四个字节后的空间中去
    BUS_ITA_RTN((*packet), data, 2*slide_cnt+4);//通过总线将数据发送给上位机
	free(data);
    free(self);//释放空间
}
