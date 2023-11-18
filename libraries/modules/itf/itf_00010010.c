/**
 * @file itf_00010010.c
 * @author wangz
 * @brief 电机状态获取接口
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010010.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010010_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010010_t* self, bus_packet* packet, Itf_00010010_rtn_t* rtn_obj);

Itf_00010010_t* Itf_00010010_new(void){
    Itf_00010010_t* self = (Itf_00010010_t*)malloc(sizeof(Itf_00010010_t));
    memset(&self->content, 0, sizeof(Itf_00010010_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010010_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.id = packet->data[offset];
}

static void ret_and_free(Itf_00010010_t* self, bus_packet* packet, Itf_00010010_rtn_t* rtn_obj){
    u8 data[13];
    u8 offset = 0;
    data[offset++] = rtn_obj->is_zero_trigger;
    WriteF32(&data[offset], rtn_obj->current_cordinate);
    offset+=4;
    WriteF32(&data[offset], rtn_obj->lost_step);
    offset+=4;
    data[offset++] = rtn_obj->current_level;
    data[offset++] = rtn_obj->hold_level;
    WriteU16(&data[offset], rtn_obj->subdivision);
    offset+=2;
    BUS_ITA_RTN((*packet), data, 13);
    free(self);
}
