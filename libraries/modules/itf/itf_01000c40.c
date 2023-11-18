#include "itf_01000c40.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static Itf_01000c40_content_t packet_parse(bus_packet *packet);
static void rtn(bus_packet *packet, u8 error_code);

void itf_01000c40_init(Itf_01000c40_t* self) {
    self->packet_parse = packet_parse;
    self->rtn = rtn;
}

/** Itf_01000c40_t 成员函数 **/
static Itf_01000c40_content_t packet_parse(bus_packet *packet) {
    u8 offset = 4;
    Itf_01000c40_content_t content;
    content.idx_state.value = packet->data[offset++];
    return content;
}

static void rtn(bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
}
