#include "itf_00070010.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static Itf_00070010_content_t packet_parse(bus_packet *packet);
static void rtn(bus_packet *packet, u8 error_code);

Itf_00070010_t *itf_00070010_new() {
    Itf_00070010_t *self = (Itf_00070010_t *)malloc(sizeof(Itf_00070010_t));
    memset(self, 0, sizeof(Itf_00070010_t));
    self->packet_parse = packet_parse;
    self->rtn = rtn;
    return self;
}

void itf_00070010_free(Itf_00070010_t *self) {
    free(self);
}

/** Itf_00070010_t 成员函数 **/
static Itf_00070010_content_t packet_parse(bus_packet *packet) {
    u8 offset = 4;
    Itf_00070010_content_t content;
    content.idx = packet->data[offset];
    offset++;
    content.voltage = ReadF32(&packet->data[offset]);
    offset+=4;
    content.time_ms = ReadU16(&packet->data[offset]);
    return content;
}

static void rtn(bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
}
