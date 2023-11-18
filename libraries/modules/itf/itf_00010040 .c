/**
 * @file itf_00010040.c
 * @author wangz
 * @brief 电机回零接口
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010040.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010040_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010040_t* self, bus_packet* packet, u8 error_code);

Itf_00010040_t* Itf_00010040_new(void){
    Itf_00010040_t* self = (Itf_00010040_t*)malloc(sizeof(Itf_00010040_t));
    memset(&self->content, 0, sizeof(Itf_00010040_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010040_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.idx = packet->data[offset++];
    self->content.acce = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.speed = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_00010040_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
