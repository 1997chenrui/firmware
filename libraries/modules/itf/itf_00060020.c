/**
 * @file itf_00060020.c
 * @author wangz
 * @brief 目标温度设置接口
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00060020.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00060020_t *self, bus_packet *packet);
static void rtn_and_free(Itf_00060020_t *self, bus_packet *packet,
                         u8 error_code);

Itf_00060020_t *itf_00060020_new(void) {
    Itf_00060020_t *self = (Itf_00060020_t *)malloc(sizeof(Itf_00060020_t));
    memset(self, 0, sizeof(Itf_00060020_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00060020_t 成员函数 **/
static void packet_parse(Itf_00060020_t *self, bus_packet *packet) {
    u8 offset = 4;
    self->content.id = packet->data[offset];
    offset++;
    self->content.target_temp = ReadF32(&packet->data[offset]);
    offset += 4;
}

static void rtn_and_free(Itf_00060020_t *self, bus_packet *packet,
                         u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
