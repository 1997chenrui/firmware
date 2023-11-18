#ifndef __itf_01000c33_H__
#define __itf_01000c33_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 m1_option;
} itf_01000c33_content_t;

typedef struct _itf_01000c33_t{
    itf_01000c33_content_t content;
    void (*packet_parse)(struct _itf_01000c33_t* , bus_packet *);
    void (*rtn_and_free)(struct _itf_01000c33_t* , bus_packet *packet, u8 error_code);
} itf_01000c33_t;

itf_01000c33_t* itf_01000c33_new(void);

#endif


