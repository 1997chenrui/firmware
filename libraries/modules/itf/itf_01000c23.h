#ifndef __ITF_01000c23_H__
#define __ITF_01000c23_H__
#include "macro.h"
#include "bus.h"

#define REAGENT_PRESS_ADJUST_LEN 18

typedef struct {
	
    f32 m1_begin_ml;
    f32 m2_dest_ml;
    f32 m3_speed;
    f32 m4_acce;
	u32 m5_done_valve_ctrl;
	//电机ID 1：染色池1区 0：染色池2区
	u8 motor_id;
	//0：吸附 1：喷吐
	u8 absorb_expel_flag;
	//热力学公式温度校准系数
	f32 temp_power;
	//压力校准数组长度
	u32 adjust_press_len;
	//压力校准组参数（分压力区间进行校准）
	f32 *adjust_press;
	//初始气压值
	u32 press_intial;
	//标准大气压
	u32 standard_press;
	
} Itf_01000c23_content_t;

typedef struct _Itf_01000c23_t{
    Itf_01000c23_content_t content;
	
    void (*packet_parse)(struct _Itf_01000c23_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c23_t* , bus_packet *, u8 error_code, f32 motor_move_volum, u32 stop_press, u32 final_v, u32 intial_press);
} Itf_01000c23_t;

Itf_01000c23_t* Itf_01000c23_new(void);

#endif

