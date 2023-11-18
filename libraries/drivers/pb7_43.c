#include "pb7_43.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tool.h"
#define MAP_CNT 51

static float calc_temperature(Pb7_43_t *self);
static float read_temperature_median_filter(Pb7_43_t *self);

const Pb7_43_cfg_t map[MAP_CNT] = {
    {-50, 408.0},  {-45, 301.4},  {-40, 225.1},  {-35, 169.8},  {-30, 129.3},
    {-25, 99.32},  {-20, 76.96},  {-15, 60.13},  {-10, 47.37},  {-5, 37.55},
    {0, 30},       {5, 24.13},    {10, 19.53},   {15, 15.91},   {20, 13.03},
    {25, 10.74},   {30, 8.896},   {35, 7.409},   {40, 6.201},   {45, 5.215},
    {50, 4.406},   {55, 3.739},   {60, 3.186},   {65, 2.727},   {70, 2.343},
    {75, 2.021},   {80, 1.749},   {85, 1.520},   {90, 1.325},   {95, 1.159},
    {100, 1.017},  {105, 0.8947}, {110, 0.7898}, {115, 0.6992}, {120, 0.6208},
    {125, 0.5527}, {130, 0.4933}, {135, 0.4414}, {140, 0.3960}, {145, 0.3561},
    {150, 0.3209}, {155, 0.2899}, {160, 0.2625}, {165, 0.2381}, {170, 0.2165},
    {175, 0.1972}, {180, 0.1800}, {185, 0.1646}, {190, 0.1508}, {195, 0.1384},
    {200, 0.1272},
};

Pb7_43_t* pb7_43_new(f32 (*read_r_value)(void)){
	Pb7_43_t* self = (Pb7_43_t*)malloc(sizeof(Pb7_43_t));
	memset(self, 0, sizeof(Pb7_43_t));
	self->mfunc_read_r_value = read_r_value;
	self->read_temperature = read_temperature_median_filter; 
	return self;
}

/**
 * @brief 将两点之间视为线性，细分5℃
 * 
 * @param self 
 * @param r_big 高电阻值
 * @param r_small 低电阻值
 * @param r 当前电阻值
 * @return float 补偿的温度值
 */
static float calc_temperature_makeup(Pb7_43_t *self, float r_big, float r_small,
                              float r) {
    float temp = 5.0 * (r_big - r) / (r_big - r_small);
    return temp;
}

/**
 * @brief 计算温度
 * 
 * @param self 
 * @return float 最终温度值
 */
static float calc_temperature(Pb7_43_t *self) {
	float r_value = self->mfunc_read_r_value();
    for (int i = 0; i < MAP_CNT; i++) {
        if (r_value == map[i].r_value) {
            return map[i].temperature;
        } else if (r_value > map[i].r_value) {
            if (i == 0) {
                return map[i].temperature;
            } else {
                return map[i - 1].temperature +
                       calc_temperature_makeup(self, map[i - 1].r_value,
                                               map[i].r_value, r_value);
            }
        }
    }
    return map[MAP_CNT - 1].temperature;
}

static float read_temperature_median_filter(Pb7_43_t *self) {
    float temp_array[3];
    for (int i = 0; i < 3; i++) {
        temp_array[i] = calc_temperature(self);
    }
    return median_filter_float(temp_array, sizeof(temp_array) / sizeof(float));
}
