/**
 * @file itf_00010080.c
 * @author wangz
 * @brief 电机复位接口
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "itf_00010080.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010080_t *self, bus_packet *packet);
static void ret_and_free(Itf_00010080_t *self, bus_packet *packet,
                         u8 error_code);

Itf_00010080_t *Itf_00010080_new(void) {
    Itf_00010080_t *self = (Itf_00010080_t *)malloc(sizeof(Itf_00010080_t));
    self->content_array = NULL;
    self->content_len = 0;
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010080_t *self, bus_packet *packet) {
    u8 offset = 4;
    self->content_len = ReadU32(&packet->data[offset]);
    offset += 4;
    self->content_array = (Itf_00010080_content_t *)malloc(
        sizeof(Itf_00010080_content_t) * self->content_len);
    memset(self->content_array, 0, sizeof(Itf_00010080_content_t) * self->content_len);
    for (int i = 0; i < self->content_len; i++) {
        self->content_array[i].idx_rel.data = packet->data[offset++];
        self->content_array[i].acce = ReadF32(&packet->data[offset]);
        offset += 4;
        self->content_array[i].speed = ReadF32(&packet->data[offset]);
        offset += 4;
        self->content_array[i].distance = ReadF32(&packet->data[offset]);
        offset += 4;
    }
}

static void ret_and_free(Itf_00010080_t *self, bus_packet *packet,
                         u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    self->content_len = 0;
    free(self->content_array);
    free(self);
}
