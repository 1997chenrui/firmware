#ifndef __itf_01000c32_H__
#define __itf_01000c32_H__
#include "macro.h"
#include "bus.h"

typedef struct {
	u8 laser_id;
    u16 samping_ms;
    u16 time_spam_ms;
} itf_01000c32_content_t;

typedef struct _itf_01000c32_t{
    itf_01000c32_content_t content;
    void (*packet_parse)(struct _itf_01000c32_t* , bus_packet *);
    void (*rtn_and_free)(struct _itf_01000c32_t* , bus_packet *packet, u8 error_code);
} itf_01000c32_t;

itf_01000c32_t* itf_01000c32_new(void);

#endif


