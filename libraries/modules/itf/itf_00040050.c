/**
 * @file itf_00040050.c
 * @author Chenr
 * @brief 获取信号
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_00040050.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040050_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040050_t *, bus_packet *packet, u16 value);

Itf_00040050_t *itf_00040050_new() {
    Itf_00040050_t *self = (Itf_00040050_t *)malloc(sizeof(Itf_00040050_t));
    memset(self, 0, sizeof(Itf_00040050_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00004050_t 成员函数 **/
static void packet_parse(Itf_00040050_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.m1_id_cmd = packet->data[offset++];
}

static void rtn_and_free(Itf_00040050_t* self, bus_packet *packet, u16 value) {
    u8 data[2];
    WriteU16(data, value);
    BUS_ITA_RTN((*packet), data, 2);
    free(self);
}
