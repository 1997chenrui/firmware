#ifndef __ITF_01000f10_H__
#define __ITF_01000f10_H__
#include "macro.h"
#include "bus.h"


#define ITF_01000F10_OK 0
#define ITF_01000F10_ERR 1
#define ITF_01000F10_UNCLOSED 2
typedef struct {
    f32 acc;
    f32 speed;
	f32 distance;
    u16 time_spam_ms;
} Itf_01000f10_content_t;

typedef struct _Itf_01000f10_t{
    Itf_01000f10_content_t content;
    void (*packet_parse)(struct _Itf_01000f10_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000f10_t* self, bus_packet *packet, u8 error_code);
} Itf_01000f10_t;

Itf_01000f10_t* Itf_01000f10_new(void);

#endif

