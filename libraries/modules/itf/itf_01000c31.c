/**
 * @file itf_01000c31.c
 * @author huadaoshun
 * @brief 电机转动，边转动边采集信号 放大倍数版本
 * @version 0.1
 * @date 2022-11-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_01000c31.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c31_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000c31_t* self, bus_packet* packet, u16* value_array, u16 array_len);
static void Onlyfree(Itf_01000c31_t* self);
Itf_01000c31_t* Itf_01000c31_new(void){
    Itf_01000c31_t* self = (Itf_01000c31_t*)malloc(sizeof(Itf_01000c31_t));
    memset(&self->content, 0, sizeof(Itf_01000c31_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000c31_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.lamp_id = ReadU16(&packet->data[offset]);
    offset+=2;
}

static void ret_and_free(Itf_01000c31_t* self, bus_packet* packet, u16* value_array, u16 array_len){
	
    BUS_ITA_RTN((*packet), (u8*) value_array, array_len * sizeof(u16));
    free(self);
}


