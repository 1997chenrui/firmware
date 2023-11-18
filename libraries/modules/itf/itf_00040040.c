/**
 * @file itf_00040040.c
 * @author Chenr
 * @brief 获取信号
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_00040040.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040040_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040040_t *, bus_packet *packet, u16* value, u16 value_length);

Itf_00040040_t *itf_00040040_new() {
    Itf_00040040_t *self = (Itf_00040040_t *)malloc(sizeof(Itf_00040040_t));
    memset(self, 0, sizeof(Itf_00040040_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00004040_t 成员函数 **/
static void packet_parse(Itf_00040040_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.m1_id = packet->data[offset++];
}

static void rtn_and_free(Itf_00040040_t* self, bus_packet *packet, u16* value, u16 value_length) {
    u8 *data = (u8*)malloc(sizeof(u16)*value_length);
	memmove(data, value, value_length*2);
    BUS_ITA_RTN((*packet), data, value_length*2);
	free(data);
    free(self);
}
