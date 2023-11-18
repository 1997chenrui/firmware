#ifndef __PID_H__
#define __PID_H__
#include "macro.h"

typedef enum{
    POSITION = 0,
    INCREMENT = 1
}Pid_type_e;

typedef struct{
    u8 type;
    f32 target;
    f32 kp;
    f32 ki;
    f32 kd;
    f32 limit;
    // 积分区间
    f32 separate_i_range;
}Pid_cfg_t;

typedef struct{
    Pid_cfg_t cfg;
    f32 e_sum;
    f32 e0;
    f32 e1;
    f32 e2;
    f32 pe;
    f32 ie;
    f32 de;
}Pid_entity_t;

typedef struct _Pid_t
{
    Pid_entity_t entity;
    f32 (*position_ctl)(struct _Pid_t *, f32 now);
    f32 (*increment_ctl)(struct _Pid_t *, f32 now);
    void (*target_set)(struct _Pid_t *, f32 target);
}Pid_t;

Pid_t* pid_mod_new(Pid_cfg_t cfg);

#endif
