/**
 * @file itf_000100a1.c
 * @author wangz
 * @brief 旋转阀开启接口
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_000100a1.h"
#include "bus.h"
#include <stdlib.h>
#include <string.h>


static void packet_parse(Itf_000100a1_t* self, bus_packet* packet);
static void ret_and_free(Itf_000100a1_t* self, bus_packet* packet, u8 error_code);

Itf_000100a1_t* Itf_000100a1_new(void){
    Itf_000100a1_t* self = (Itf_000100a1_t*)malloc(sizeof(Itf_000100a1_t));
    memset(&self->content, 0, sizeof(Itf_000100a1_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_000100a1_t* self, bus_packet* packet){
    Itf_000100a1_content_t* content = (Itf_000100a1_content_t*)malloc(sizeof(Itf_000100a1_content_t));
    u8 offset = 4;
    self->content.id = packet->data[offset++];
    self->content.channel = packet->data[offset++];
}

static void ret_and_free(Itf_000100a1_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}

