#ifndef __PB7_43_H__
#define __PB7_43_H__
#include "macro.h"

typedef struct{
    f32 temperature;
    f32 r_value;
}Pb7_43_cfg_t;

typedef struct _Pb7_43_t{
    f32 (*mfunc_read_r_value)(void);
    f32 (*read_temperature)(struct _Pb7_43_t *);
}Pb7_43_t;

Pb7_43_t* pb7_43_new(f32 (*read_r_value)(void));

#endif
