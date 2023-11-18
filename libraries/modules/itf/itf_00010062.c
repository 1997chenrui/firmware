/**
 * @file itf_00010062.c
 * @author huadaoshun
 * @brief 电机使能接口
 * @version 0.1
 * @date 2022-11-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00010062.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010062_t* self, bus_packet* packet);
static void ret_and_free(Itf_00010062_t* self, bus_packet* packet, u8 error_code);

Itf_00010062_t* Itf_00010062_new(void){
    Itf_00010062_t* self = (Itf_00010062_t*)malloc(sizeof(Itf_00010062_t));
    memset(&self->content, 0, sizeof(Itf_00010062_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010062_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.idx = packet->data[offset++];
    self->content.state = packet->data[offset++];

}

static void ret_and_free(Itf_00010062_t* self, bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
