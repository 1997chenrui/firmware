/**
 * @file itf_01000c33.c
 * @author huadaoshun
 * @brief 电机转动，边转动边采集信号 放大倍数版本
 * @version 0.1
 * @date 2022-11-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_01000c33.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(itf_01000c33_t* self, bus_packet* packet);
static void ret_and_free(itf_01000c33_t* self, bus_packet *packet, u8 error_code);


itf_01000c33_t* itf_01000c33_new(void){
    itf_01000c33_t* self = (itf_01000c33_t*)malloc(sizeof(itf_01000c33_t));
    memset(&self->content, 0, sizeof(itf_01000c33_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(itf_01000c33_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.m1_option = packet->data[offset++];
    offset++;

}

static void ret_and_free(itf_01000c33_t* self, bus_packet *packet, u8 error_code){
	
   
	BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}



