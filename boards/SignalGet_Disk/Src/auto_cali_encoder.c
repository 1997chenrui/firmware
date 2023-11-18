#include "auto_cali_encoder.h"

unsigned short read_light(auto_encoder *me,LIGHT_E type)
{
    return me->light_value = singelRead(type);
}
//unsigned short read_code();
//unsigned short get_pulse();
//unsigned short get_time();
void auto_encoder_Iint(auto_encoder *me)
{
    me->postsem = false;
}

