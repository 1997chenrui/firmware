#include "itf_01000c60.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static void packet_parse(Itf_01000c60_t*, bus_packet *packet);
static void rtn_and_free(Itf_01000c60_t*, bus_packet *packet, u8 error_code);

Itf_01000c60_t *itf_01000c60_new() {
    Itf_01000c60_t *self = (Itf_01000c60_t *)malloc(sizeof(Itf_01000c60_t));
    memset(self, 0, sizeof(Itf_01000c60_t));
    self->rtn_and_free = rtn_and_free;
    return self;
}

static void rtn_and_free(Itf_01000c60_t*self, bus_packet *packet, u8 error_code) {
    BUS_ITA_RTN((*packet), &error_code, 1);
    free(self);
}
