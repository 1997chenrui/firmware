/**
 * @file itf_01000c10.c
 * @author wangz
 * @brief 电机移动在特定位置检测是否存在样本瓶
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_01000c10.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c10_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c10_t* self, bus_packet* packet, u32 state);

Itf_01000c10_t* Itf_01000c10_new(void){
    Itf_01000c10_t* self = (Itf_01000c10_t*)malloc(sizeof(Itf_01000c10_t));
    memset(&self->content, 0, sizeof(Itf_01000c10_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c10_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.x_speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.x_acce = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.y_speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.y_acce = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_01000c10_t* self, bus_packet* packet, u32 state){
	u8 data[4] = {0,};
	WriteU32(data, state);
    BUS_ITA_RTN((*packet), data, 4);
    free(self);
}
