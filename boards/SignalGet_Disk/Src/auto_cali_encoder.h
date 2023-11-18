#ifndef __ENCODER_H
#define __ENCODER_H
#include "auto_caliPKG.h"
#include "light_type_e.h"
#include "signal_det.h"

/*设置中断时间100us  500ms / 360°  */

typedef struct auto_encoder  auto_encoder;
struct auto_encoder
{
    bool postsem;
    unsigned short light_value;
    unsigned short encoder_value1;//保存光值第一次小于15000时候的和再次大于15000的值
    volatile unsigned short c_time1;
    volatile unsigned short c_time2;
    unsigned short tim_value1;//保存对应的定时器的值
};
unsigned short read_light(auto_encoder *me,LIGHT_E type);

void auto_encoder_Iint(auto_encoder *me);

#endif
