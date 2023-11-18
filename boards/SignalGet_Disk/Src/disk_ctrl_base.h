#ifndef ADC_DETECT_HW_H_
#define ADC_DETECT_HW_H_

#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "os_implement.h"
//共有多少孔
#define     DISK_HOLE_CNT              30

//四项到四项孔
#define		S2SHOLE_TIME			   13153  //9.47

//两个波长孔间距
#define     L2LHOLE_TIME			   26320  //18.95

#define     M_S2LHOLE_TIME    30708  //扇区之间
typedef enum
{
  Light_ExitNoTri = 0,
  Light_ExitTri
}Ligth_ExitITState;

typedef struct light_ctrl_base_t{
	void (*set_timer)(uint32_t us);
	void (*start_timer)(void);
	void (*stop_timer)(void);
	void (*start_exit2)(void);
	void (*stop_exit2)(void);
}light_ctrl_base_t;


void adc_start_exit2_it(void);
void adc_stop_exit2_it(void);
void adc_set_tim2_cnt(uint32_t value);
void adc_start_tim2(void);         
void adc_stop_tim2(void);
void disk_ctrl_base_init(void);



extern light_ctrl_base_t 		lightCtrlBase;


#endif

