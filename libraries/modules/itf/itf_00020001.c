#include "itf_00020001.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00020001_t*, bus_packet *packet);
static void rtn_and_free(Itf_00020001_t*, bus_packet *packet, u8 error_code);

Itf_00020001_t *itf_00020001_new() {
    Itf_00020001_t *self = (Itf_00020001_t *)malloc(sizeof(Itf_00020001_t));
    memset(self, 0, sizeof(Itf_00020001_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}


static void packet_parse(Itf_00020001_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.group_idx = packet->data[offset];
    offset++;
    self->content.m1_state = ReadU32(&packet->data[offset]);
}

static void rtn_and_free(Itf_00020001_t*self, bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
