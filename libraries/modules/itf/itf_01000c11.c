/**
 * @file itf_01000c11.c
 * @author hds
 * @brief 电机移动在特定位置检测是否存在玻片
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_01000c11.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c11_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c11_t* self, bus_packet* packet, u32 state);

Itf_01000c11_t* Itf_01000c11_new(void){
    Itf_01000c11_t* self = (Itf_01000c11_t*)malloc(sizeof(Itf_01000c11_t));
    memset(&self->content, 0, sizeof(Itf_01000c11_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c11_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.y_speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.y_acce = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_01000c11_t* self, bus_packet* packet, u32 state){
	u8 data[2] = {0,};
	WriteU16(data, state);
    BUS_ITA_RTN((*packet), data, 2);
    free(self);
}
