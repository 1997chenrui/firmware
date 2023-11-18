#ifndef LIGHT_SAMPLER_H_
#define LIGHT_SAMPLER_H_
#include <stdint.h>
#include <stdbool.h>
#include <os_implement.h>
#include "light_type_e.h"

u16 singelRead(LIGHT_E type);
u16 averageRead(LIGHT_E type);
//#include "adc_detect_hw.h"
//#include "autoCai.h"
//#include "adc_detect.h"
//typedef struct
//{
//    AutoCali_t              auto_cali;
//    light_detect_struct     light_detect;
//    u8    is_start;
//    u8    function;         //0粗定标 1 精定标 2 采样
//    u16 (*get_ad_data)(u8 ,u16 *,u8);
//	u16  (*get_sigle_ad_data)(u8 );
//}light_sampler_struct;

//void LightSampler_ZeroTiggerHandle(light_gen_struct *light_gen);
//void LightSampler_TimerTiggerHandle(light_gen_struct *light_gen);
//void LightSampler_Start(u8 f);
#endif

