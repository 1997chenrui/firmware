/**
 * @file itf_00040010.c
 * @author wangz
 * @brief 获取信号
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00040010.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040010_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040010_t *, bus_packet *packet, u32 value);

Itf_00040010_t *itf_00040010_new() {
    Itf_00040010_t *self = (Itf_00040010_t *)malloc(sizeof(Itf_00040010_t));
    memset(self, 0, sizeof(Itf_00040010_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00006000_t 成员函数 **/
static void packet_parse(Itf_00040010_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.sensor_id = packet->data[offset++];
}

static void rtn_and_free(Itf_00040010_t* self, bus_packet *packet, u32 value) {
    u8 data[4];
    WriteU32(data, value);//将value赋值给data
    BUS_ITA_RTN((*packet), data, 4);//通过总线将数据发送出去
    free(self);//释放空间
}
