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
	//���ID 1��Ⱦɫ��1�� 0��Ⱦɫ��2��
	u8 motor_id;
	//0������ 1������
	u8 absorb_expel_flag;
	//����ѧ��ʽ�¶�У׼ϵ��
	f32 temp_power;
	//ѹ��У׼���鳤��
	u32 adjust_press_len;
	//ѹ��У׼���������ѹ���������У׼��
	f32 *adjust_press;
	//��ʼ��ѹֵ
	u32 press_intial;
	//��׼����ѹ
	u32 standard_press;
	
} Itf_01000c23_content_t;

typedef struct _Itf_01000c23_t{
    Itf_01000c23_content_t content;
	
    void (*packet_parse)(struct _Itf_01000c23_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c23_t* , bus_packet *, u8 error_code, f32 motor_move_volum, u32 stop_press, u32 final_v, u32 intial_press);
} Itf_01000c23_t;

Itf_01000c23_t* Itf_01000c23_new(void);

#endif

