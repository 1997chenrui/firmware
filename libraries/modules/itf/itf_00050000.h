#ifndef __ITF_00050000_H__
#define __ITF_00050000_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 id;
    u8 *data;
    u16 len;
    bus_packet packet;
} Itf_00050000_content_t;

typedef struct {
    Itf_00050000_content_t *(*packet_parse)(bus_packet *);
    void (*send)(u8 id, u8 *data, u32 len);
    void (*rtn_and_content_del)(Itf_00050000_content_t *, u8 error_code);
} Itf_00050000_t;

Itf_00050000_t* Itf_00050000_new(void);

#endif
