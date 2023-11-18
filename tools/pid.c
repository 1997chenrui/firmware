#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pid.h"

static f32 pid_position_ctl(Pid_t *self, f32 now);
static f32 pid_increment_ctl(Pid_t *self, f32 now);
static void pid_target_set(Pid_t *self, f32 target);

/** 全局函数 **/

Pid_t* pid_mod_new(Pid_cfg_t cfg){
    Pid_t *self = (Pid_t *)malloc(sizeof(Pid_t));
    memset(self, 0, sizeof(Pid_t));
    self->entity.cfg = cfg;
    self->increment_ctl = pid_increment_ctl;
    self->position_ctl = pid_position_ctl;
    self->target_set = pid_target_set;
    return self;
}

/** Pid_entity_t 成员函数 **/

static f32 entity_limit(Pid_entity_t*self, f32 value){
    f32 min = -self->cfg.limit;
    f32 max = self->cfg.limit;
    if(value < min){
        return min;
    }else if(value > max){
        return max;
    }else{
        return value;
    }
}

static f32 position_out(Pid_entity_t* self, f32 now){
    Pid_cfg_t* cfg =  &self->cfg;
    ASSERT(cfg->type != POSITION, "PID 类型错误 position");
    self->e0 = cfg->target - now;
    self->pe = self->e0;
    self->de = self->e0 - self->e1;
    f32 out = self->pe*cfg->kp + self->de*cfg->kd;
    u8 need_i = self->e0 > -cfg->separate_i_range && self->e0 < cfg->separate_i_range;
    if(need_i){
        self->e_sum += self->e0;
        self->ie = self->e_sum;
        out += self->ie*cfg->ki;
    }else{
        self->e_sum = 0;
        self->ie = 0;
    }
    out = entity_limit(self, out);
    printf("temp = %4f, target = %4f, out = %4f, pe = %4f, ie = %4f, de = %4f\r\n", 
                        now, 
                        self->cfg.target,
                        out,
                        self->pe,
                        self->ie,
                        self->de);
    self->e1 = self->e0;
    return out;
}

static f32 increment_out(Pid_entity_t* self, f32 now){
    Pid_cfg_t* cfg =  &self->cfg;
    ASSERT(cfg->type != INCREMENT, "PID 类型错误 increment");
    self->e0 = cfg->target - now;

    self->pe = self->e0 - self->e1;
    self->ie = self->e0;
    self->de = self->e0 - 2*self->e1 + self->e2;
    f32 out = self->pe*cfg->kp + self->ie*cfg->ki + self->de*cfg->kd;
    printf("temp = %4f, target = %4f, pe = %4f, ie = %4f, de = %4f\r\n", 
                        now, 
                        self->cfg.target,
                        self->pe,
                        self->ie,
                        self->de);
    out = entity_limit(self, out);
    self->e2 = self->e1;
    self->e1 = self->e0;
    return out;
}

/** Pid_t 成员函数 **/
//static void pid_init(u8 idx, u8 type, f32 target, f32 limit){
//    ASSERT(idx >= pid.entity_len, "PID 参数错误");
//    Pid_cfg_t* cfg = &pid.entity[idx].cfg;
//    cfg->type = type;
//    cfg->target = target;    
//    cfg->limit = limit;
//}

static f32 pid_position_ctl(Pid_t *self, f32 now){
    return position_out(&self->entity, now);
}

static f32 pid_increment_ctl(Pid_t *self, f32 now){
    return increment_out(&self->entity, now);
}

static void pid_target_set(Pid_t* self, f32 target){
    self->entity.cfg.target = target;
}
