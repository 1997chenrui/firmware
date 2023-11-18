/**
 * @file itf_00040000.c
 * @author wangz
 * @brief 获取信号
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00040000.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040000_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040000_t *, bus_packet *packet, u16 value);

Itf_00040000_t *itf_00040000_new() {
    Itf_00040000_t *self = (Itf_00040000_t *)malloc(sizeof(Itf_00040000_t));
    memset(self, 0, sizeof(Itf_00040000_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00006000_t 成员函数 **/
static void packet_parse(Itf_00040000_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.sensor_id = packet->data[offset++];
}

static void rtn_and_free(Itf_00040000_t* self, bus_packet *packet, u16 value) {
    u8 data[2];
    WriteU16(data, value);
    BUS_ITA_RTN((*packet), data, 2);
    free(self);
}
