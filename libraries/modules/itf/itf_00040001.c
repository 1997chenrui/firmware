/**
 * @file itf_00040001.c
 * @author chenr
 * @brief 获取信号
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_00040001.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040001_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040001_t *, bus_packet *packet, u32 value);

Itf_00040001_t *itf_00040001_new() {
    Itf_00040001_t *self = (Itf_00040001_t *)malloc(sizeof(Itf_00040001_t));
    memset(self, 0, sizeof(Itf_00040001_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_000400001_t 成员函数 **/
static void packet_parse(Itf_00040001_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.sensor_id = packet->data[offset++];
}

static void rtn_and_free(Itf_00040001_t* self, bus_packet *packet, u32 value) {
    u8 data[4];
    WriteU32(data, value);
    BUS_ITA_RTN((*packet), data, 4);
    free(self);
}
