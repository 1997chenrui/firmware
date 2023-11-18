/**
 * @file itf_01000c50.c
 * @author ChenR
 * @brief 电机复位接口
 * @version 0.1
 * @date 2023-3-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_01000c50.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c50_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c50_t* self, bus_packet* packet, u8 error_code);

Itf_01000c50_t* Itf_01000c50_new(void){
    Itf_01000c50_t* self = (Itf_01000c50_t*)malloc(sizeof(Itf_01000c50_t));
    memset(&self->content, 0, sizeof(Itf_01000c50_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c50_t* self, bus_packet* packet){
    u8 offset = 4;
	self->content.motor_id = packet->data[offset];
	offset ++;
    self->content.m1_k = ReadF32(&packet->data[offset]);
	offset +=4;
    self->content.m2_b = ReadF32(&packet->data[offset]);
}

static void ret_and_free(Itf_01000c50_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
