#ifndef __ITF_01000c60_H__
#define __ITF_01000c60_H__
#include "macro.h"
#include "bus.h"

typedef struct _Itf_01000c60_t
{
    void (*rtn_and_free)(struct _Itf_01000c60_t*, bus_packet*, u8 error_code);
}Itf_01000c60_t;

Itf_01000c60_t *itf_01000c60_new(void);

#endif


