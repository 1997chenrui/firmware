
#include "itf_01000f10.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000f10_t* self, bus_packet* packet);
static void ret_and_free(Itf_01000f10_t* self, bus_packet *packet, u8 error_code);

Itf_01000f10_t* Itf_01000f10_new(void){
    Itf_01000f10_t* self = (Itf_01000f10_t*)malloc(sizeof(Itf_01000f10_t));
    memset(&self->content, 0, sizeof(Itf_01000f10_content_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_01000f10_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.acc = ReadF32(&packet->data[offset]);
	offset+=4;
    self->content.speed = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.distance = ReadF32(&packet->data[offset]);
    offset+=4;
    self->content.time_spam_ms = ReadU16(&packet->data[offset]);
    offset+=2;
}

static void ret_and_free(Itf_01000f10_t* self, bus_packet *packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
