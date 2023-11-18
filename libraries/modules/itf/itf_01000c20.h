#ifndef __ITF_01000c20_H__
#define __ITF_01000c20_H__
#include "macro.h"
#include "bus.h"

#define SAMPLE_PRESS_ADJUST_LEN 8
typedef struct {
    f32 m1_begin_ml;
    f32 m2_dest_ml;
    f32 m3_speed;
    f32 m4_acce;
	u32 m5_done_valve_ctrl;
	u32 adjust_press_len;
	f32 *press_adjust;
	//初始气压AD;
	u32 press_ad;
	//标准大气压
	u32 standard_press;
} Itf_01000c20_content_t;

typedef struct _Itf_01000c20_t{
    Itf_01000c20_content_t content;
	
    void (*packet_parse)(struct _Itf_01000c20_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c20_t* , bus_packet *, u8 error_code, u32 final_v);
} Itf_01000c20_t;

Itf_01000c20_t* Itf_01000c20_new(void);

#endif

