/**
 * @file itf_01000920.c
 * @author wangz
 * @brief 获取信号
 * @version 0.1
 * @date 2023-03-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "itf_01000930.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000930_t *, bus_packet *packet);
static void rtn_and_free(Itf_01000930_t *, bus_packet *packet, u8);

Itf_01000930_t *itf_01000930_new() {
    Itf_01000930_t *self = (Itf_01000930_t *)malloc(sizeof(Itf_01000930_t));
    memset(self, 0, sizeof(Itf_01000930_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_01000930_t 成员函数 **/
static void packet_parse(Itf_01000930_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.m1_sen_grade = packet->data[offset++];
}

static void rtn_and_free(Itf_01000930_t* self, bus_packet *packet, u8 m1_errCode) {
    BUS_ITA_RTN((*packet), &m1_errCode, 1);//通过总线将数据发送出去
    free(self);//释放空间
}
