/**
 * @file itf_00010050.c
 * @author wangz
 * @brief 电机旋转接口
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010050.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010050_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010050_t* self, bus_packet* packet, u8 error_code);

Itf_00010050_t* Itf_00010050_new(void){
    Itf_00010050_t* self = (Itf_00010050_t*)malloc(sizeof(Itf_00010050_t));
    memset(&self->content, 0, sizeof(Itf_00010050_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010050_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.idx_dir.data = packet->data[offset++];
    self->content.acce = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.speed = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_00010050_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
