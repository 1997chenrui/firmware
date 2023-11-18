/**
 * @file itf_00010082.c
 * @author wangz
 * @brief 电机复位接口
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "itf_00010082.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010082_t *self, bus_packet *packet);
static void ret_and_free(Itf_00010082_t *self, bus_packet *packet,
                         u8 error_code);

Itf_00010082_t *Itf_00010082_new(void) {
    Itf_00010082_t *self = (Itf_00010082_t *)malloc(sizeof(Itf_00010082_t));
    self->content_array = NULL;
    self->content_len = 0;
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010082_t *self, bus_packet *packet) {
    u8 offset = 4;
    self->content_len = ReadU32(&packet->data[offset]);
    offset += 4;
    self->content_array = (Itf_00010082_content_t *)malloc(
        sizeof(Itf_00010082_content_t) * self->content_len);
    for (int i = 0; i < self->content_len; i++) {
        self->content_array[i].arg_len = ReadU32(&packet->data[offset]);
        offset += 4;
        self->content_array[i].args = (Itf_00010082_arg_t *)malloc(
            sizeof(Itf_00010082_arg_t) * self->content_array[i].arg_len);
        for(int j = 0; j < self->content_array[i].arg_len; j++){
            self->content_array[i].args[j].idx_rel.data = packet->data[offset++];
            self->content_array[i].args[j].acce = ReadF32(&packet->data[offset]);
            offset += 4;
            self->content_array[i].args[j].speed = ReadF32(&packet->data[offset]);
            offset += 4;
            self->content_array[i].args[j].distance = ReadF32(&packet->data[offset]);
            offset += 4;
        }
		self->content_array[i].delay_ms = ReadU16(&packet->data[offset]);
		offset += 2;
    }
}

static void ret_and_free(Itf_00010082_t *self, bus_packet *packet,
                         u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    //逐层释放内存
    for(int i = 0; i< self->content_len; i++){
        free(self->content_array[i].args);
    }
    free(self->content_array);
    free(self);
}
