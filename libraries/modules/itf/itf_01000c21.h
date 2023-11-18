#ifndef __ITF_01000c21_H__
#define __ITF_01000c21_H__
#include "macro.h"
#include "bus.h"

typedef struct {
	u8 m0_absorb_expel_flag;//0 吸附，1 喷吐
    f32 m1_begin_ml;
    f32 m2_dest_ml;
    f32 m3_speed;
    f32 m4_acce;
	u32 m5_done_valve_ctrl;//吸附完成后关阀
	f32 m6_standard_integral;// 流阻系数
	f32 m7_standard_press;//标准气压
	f32 m8_power;//次幂系数
	u8 m9_channel_id;//吸附通道
	f32 m10_k;
	f32 m11_b;
	u32 stop_flag_press;//当负压低于此值时吸附结束

} Itf_01000c21_content_t;

typedef struct _Itf_01000c21_t{
    Itf_01000c21_content_t content;
	
    void (*packet_parse)(struct _Itf_01000c21_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c21_t* , bus_packet *, u8 error_code, int differ_max, f32 final_v);
} Itf_01000c21_t;

Itf_01000c21_t* Itf_01000c21_new(void);

#endif

