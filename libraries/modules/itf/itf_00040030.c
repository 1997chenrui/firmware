/**
 * @file itf_00040030.c
 * @author wangz
 * @brief 获取信号
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "itf_00040030.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00040030_t *, bus_packet *packet);
static void rtn_and_free(Itf_00040030_t *, bus_packet *packet, u16* value,u8 length);


Itf_00040030_t *itf_00040030_new() {
    Itf_00040030_t *self = (Itf_00040030_t *)malloc(sizeof(Itf_00040030_t));
    memset(self, 0, sizeof(Itf_00040030_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00006000_t 成员函数 **/
static void packet_parse(Itf_00040030_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.group = ReadU16(&packet->data[offset]);
}

static void rtn_and_free(Itf_00040030_t* self, bus_packet *packet, u16* value,u8 length) {
    u8 *data=(u8 *)malloc(sizeof(u16)*length+4);
    WriteU32(data, length);//将长度赋值到data指向的前四个字节的空间中去
	memmove(data+4,value,2*length);//将value指向的数据的2*length个字节传到data指向的前四个字节后的空间中去
    BUS_ITA_RTN((*packet), data, 2*length+4);//通过总线将数据发送给上位机
	free(data);
    free(self);//释放空间
}
