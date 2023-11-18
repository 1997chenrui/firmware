#include "itf_00070001.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_00070001_t*, bus_packet *packet);
static void rtn_and_free(Itf_00070001_t*, bus_packet *packet, u8 error_code);

Itf_00070001_t *itf_00070001_new() {
    Itf_00070001_t *self = (Itf_00070001_t *)malloc(sizeof(Itf_00070001_t));
    memset(self, 0, sizeof(Itf_00070001_t));
    self->packet_parse = packet_parse;
    self->rtn_and_free = rtn_and_free;
    return self;
}

void itf_00070001_free(Itf_00070001_t *self) {
    free(self);
}

/** Itf_00070001_t 成员函数 **/
static void packet_parse(Itf_00070001_t*self, bus_packet *packet) {
    u8 offset = 4;
    self->content.idx = packet->data[offset++];
    self->content.persent = packet->data[offset++];
}

static void rtn_and_free(Itf_00070001_t*self, bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
