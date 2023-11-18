/**
 * @file itf_00060000.c
 * @author wangz
 * @brief 获取温度接口
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00060000.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00060000_t *, bus_packet *packet);
static void rtn_and_free(Itf_00060000_t *, bus_packet *packet, f32 temperature);

Itf_00060000_t *itf_00060000_new() {
    Itf_00060000_t *self = (Itf_00060000_t *)malloc(sizeof(Itf_00060000_t));
    memset(self, 0, sizeof(Itf_00060000_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00006000_t 成员函数 **/
static void packet_parse(Itf_00060000_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.channel_id = packet->data[offset++];
}

static void rtn_and_free(Itf_00060000_t* self, bus_packet *packet, f32 temperature) {
    u8 data[4];
    WriteF32(data, temperature);
    BUS_ITA_RTN((*packet), data, 4);
    free(self);
}
