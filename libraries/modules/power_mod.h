#ifndef __POWER_MOD_H__
#define __POWER_MOD_H__
#include "macro.h"

typedef void (*POWER_MOD_CTRL)(u8 on);
typedef u8 (*POWER_MOD_BTN_TRIG)(void);



typedef enum{
    MOD_ON,
    MOD_OFF
}Powermod_state_e;

typedef enum{
    SWITCH_ON,
    SWITCH_OFF,
    SWITCH_IDLE
}Powermod_switch_e;

typedef struct{
    u8 state;
    u8 switch_state;
    u16 trig_time;
    void (*run)(void);
	void (*run1)(void);
    void (*run2)(void);
}Powermod_t;


void powermod_init(POWER_MOD_CTRL ctrl, POWER_MOD_BTN_TRIG read_trig);
void powermod_init_with_state(POWER_MOD_CTRL ctrl, POWER_MOD_BTN_TRIG read_trig, Powermod_switch_e state);
Powermod_t* powermod_get_as_ref(void);

#endif
