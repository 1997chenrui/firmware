/**
 * @file itf_00010083.c
 * @author wangz
 * @brief 电机复位接口
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "itf_00010083.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00010083_t *self, bus_packet *packet);
static void ret_and_free(Itf_00010083_t *self, bus_packet *packet,
                         u8 error_code);

Itf_00010083_t *Itf_00010083_new(void) {
    Itf_00010083_t *self = (Itf_00010083_t *)malloc(sizeof(Itf_00010083_t));
    memset(self, 0, sizeof(Itf_00010083_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_00010083_t *self, bus_packet *packet) {
    u8 offset = 4;
    self->content.seq_len = ReadU32(&packet->data[offset]);
    offset += 4;
    Itf_00010083_seq_t *seq_array = (Itf_00010083_seq_t *)malloc(
        sizeof(Itf_00010083_content_t) * self->content.seq_len);
    memset(seq_array, 0, sizeof(Itf_00010083_seq_t) * self->content.seq_len);
    self->content.seq_array = seq_array;

    for (int i = 0; i < self->content.seq_len; i++) {
        seq_array[i].arg_len = ReadU32(&packet->data[offset]);
        offset += 4;
        Itf_00010083_arg_t *arg_array = (Itf_00010083_arg_t *)malloc(
            sizeof(Itf_00010083_arg_t) * seq_array[i].arg_len);
        memset(arg_array, 0, sizeof(Itf_00010083_arg_t) * seq_array[i].arg_len);
        seq_array[i].args = arg_array;
        for (int j = 0; j < seq_array[i].arg_len; j++) {
            arg_array[j].idx_rel.data = packet->data[offset++];
            arg_array[j].acce = ReadF32(&packet->data[offset]);
            offset += 4;
            arg_array[j].speed = ReadF32(&packet->data[offset]);
            offset += 4;
            arg_array[j].distance = ReadF32(&packet->data[offset]);
            offset += 4;
        }
        seq_array[i].delay_ms = ReadU16(&packet->data[offset]);
        offset += 2;
    }
    self->content.signal_id = packet->data[offset++];
    self->content.timeout_ms = ReadU16(&packet->data[offset]);
    offset += 2;
    
}

static void ret_and_free(Itf_00010083_t *self, bus_packet *packet,
                         u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    //逐层释放内存
    for (int i = 0; i < self->content.seq_len; i++) {
        free(self->content.seq_array[i].args);
    }
    free(self->content.seq_array);
    free(self);
}
