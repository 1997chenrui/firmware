#include "itf_00030010.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00030010_t*, bus_packet *packet);
static void rtn_and_free(Itf_00030010_t*, bus_packet *packet, u8 error_code);

Itf_00030010_t *itf_00030010_new() {
    Itf_00030010_t *self = (Itf_00030010_t *)malloc(sizeof(Itf_00030010_t));
    memset(self, 0, sizeof(Itf_00030010_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00030010_t 成员函数 **/
static void packet_parse(Itf_00030010_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.idx = packet->data[offset];
    offset++;
    self->content.duty_cycle = ReadU16(&packet->data[offset]);
    offset+=2;
}

static void rtn_and_free(Itf_00030010_t*self, bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
