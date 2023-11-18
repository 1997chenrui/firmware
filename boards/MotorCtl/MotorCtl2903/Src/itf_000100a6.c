#include "itf_000100a6.h"
#include "bus.h"
#include <stdlib.h>

static void packet_parse(Itf_000100a6_t* self, bus_packet* packet);
static void ret_and_free(Itf_000100a6_t* self, bus_packet* packet,u8 errcode, u8 last,u8 current);

Itf_000100a6_t* Itf_000100a6_new(void){
    Itf_000100a6_t* self = (Itf_000100a6_t*)malloc(sizeof(Itf_000100a6_t));
    memset(&self->content, 0, sizeof(Itf_000100a6_content_t));
    self->content.cal_distance = 0;
    self->packet_parse = packet_parse;
    self->rtn_and_free = ret_and_free;
    return self;
}

static void packet_parse(Itf_000100a6_t* self, bus_packet* packet){
    u8 offset = 4;
    self->content.cal_distance = ReadF32(&packet->data[offset]);
}

static void ret_and_free(Itf_000100a6_t* self, bus_packet* packet, u8 errcode,u8 last,u8 current){
	
	u8 rt[3];
	rt[0]=errcode;
	rt[1]=last;
	rt[2]=current;
	
    BUS_ITA_RTN((*packet), rt, 3);
    free(self);
}

