/**
 * @file itf_00010031.c
 * @author wangz
 * @brief 电机移动接口, 根据event_id发送通知
 * eg:通过两个电路板之间的同步线，拉高或拉低电平通知对方次电机移动完成
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010031.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010031_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010031_t* self, bus_packet* packet, u8 error_code);

Itf_00010031_t* Itf_00010031_new(void){
    Itf_00010031_t* self = (Itf_00010031_t*)malloc(sizeof(Itf_00010031_t));
    memset(&self->content, 0, sizeof(Itf_00010031_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010031_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.idx_rel.data = packet->data[offset++];
    self->content.acce = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.distance = ReadF32(&packet->data[offset]);
    offset+=4;
}

static void ret_and_free(Itf_00010031_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
