#include "itf_137.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_137_t* self, bus_packet* packet);
static void rtn_and_free(Itf_137_t* self, bus_packet* packet, u32 error_code, u16 diff, u16 min);

Itf_137_t* Itf_137_new(){
    Itf_137_t* self = (Itf_137_t*)malloc(sizeof(Itf_137_t));
    memset(self, 0, sizeof(Itf_137_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_137_t 成员函数 **/
static void packet_parse(Itf_137_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.cnt = ReadU16(&packet->data[offset]);
    offset += 2;
    self->content.time_span_ms = ReadU16(&packet->data[offset]);
    offset += 2;
    self->content.channel = ReadU16(&packet->data[offset]);
    offset += 2;
}

static void rtn_and_free(Itf_137_t* self, bus_packet* packet, u32 error_code, u16 diff, u16 min){
    u8 data[8];
    u8 offset = 0;
    WriteU32(&data[offset], error_code);
    offset+=4;
    WriteU16(&data[offset], diff);
    offset+=2;
    WriteU16(&data[offset], min);
    offset+=2;
    BUS_ITA_RTN((*packet), data, 8);
    free(self);
}


