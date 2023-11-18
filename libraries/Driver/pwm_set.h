#ifndef PWM_SET_H_
#define PWM_SET_H_
#include <stdint.h>
#include <stdbool.h>

uint32_t updatePwm(uint8_t time_channle,uint32_t period,uint32_t dutyCycle);
bool startPumpPwm(uint8_t time_channle);

#endif
