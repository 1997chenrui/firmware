/**
 * @file itf_000100a2.c
 * @author wangz
 * @brief 旋转阀关闭接口
 * @version 0.1
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_000100a2.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_000100a2_t* self, bus_packet* packet);
static void ret_and_free(Itf_000100a2_t* self, bus_packet* packet, u8 error_code);

Itf_000100a2_t* Itf_000100a2_new(void){
    Itf_000100a2_t* self = (Itf_000100a2_t*)malloc(sizeof(Itf_000100a2_t));
    memset(&self->content, 0, sizeof(Itf_000100a2_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_000100a2_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.id = packet->data[offset];
}

static void ret_and_free(Itf_000100a2_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
