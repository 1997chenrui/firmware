/**
 * @file itf_00010071.c
 * @author wangz
 * @brief 电机移动监听事件接口，移动过程中监听事件，当事件触发停止监听
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010071.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010071_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010071_t* self, bus_packet* packet, u8 error_code, f32 trigger_distance);

Itf_00010071_t* Itf_00010071_new(void){
    Itf_00010071_t* self = (Itf_00010071_t*)malloc(sizeof(Itf_00010071_t));
    memset(&self->content, 0, sizeof(Itf_00010071_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010071_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.idx_rel.data = packet->data[offset++];
    self->content.acce = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.distance = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.distance_when_trigger = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_00010071_t* self, bus_packet* packet, u8 error_code, f32 trigger_distance){
    u8 data[5];
    u8 offset = 0;
    data[offset++] = error_code;
    WriteF32(&data[offset], trigger_distance);
    offset+=4;
    BUS_ITA_RTN((*packet), data, 5);
    free(self);
}
