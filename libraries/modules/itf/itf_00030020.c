#include "itf_00030020.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00030020_t*, bus_packet *packet);
static void rtn_and_free(Itf_00030020_t*, bus_packet *packet, u8 error_code);

Itf_00030020_t *itf_00030020_new() {
    Itf_00030020_t *self = (Itf_00030020_t *)malloc(sizeof(Itf_00030020_t));
    memset(self, 0, sizeof(Itf_00030020_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

/** Itf_00030020_t 成员函数 **/
static void packet_parse(Itf_00030020_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.group_idx = ReadU32(&packet->data[offset]);
    offset+=4;
    self->content.duty_cycle = ReadU16(&packet->data[offset]);
    offset+=2;
}

static void rtn_and_free(Itf_00030020_t*self, bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
