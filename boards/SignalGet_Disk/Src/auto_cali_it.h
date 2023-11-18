#ifndef __CALIIT_H
#define __CALIIT_H
#include "auto_caliPKG.h"
extern TIM_HandleTypeDef htim2;
void close_tim(void);
void open_tim(void);
void close_exti(void);
void open_exti(void);
void set_tim_cnt(uint32_t value);
typedef struct auto_it auto_it;
struct auto_it
{
    unsigned char z_count;
    void (*set_timer)(uint32_t us);
	void (*open_timer)(void);
	void (*close_timer)(void);
	void (*open_exit)(void);
	void (*close_exit)(void);
};

void auto_it_Init(auto_it *me,void(*opentim)(void),void(*closetim)(void),void(*openexti)(void),void(*closeexti)(void),void(*settime)(uint32_t us));
#endif



