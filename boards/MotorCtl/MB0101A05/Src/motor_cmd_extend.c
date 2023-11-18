#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c20.h"
#include "ad_signal_cfg.h"
#include "ad_signal.h"
#include "io_mod.h"
#include "io_thread.h"

extern TIM_HandleTypeDef htim2;
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
static int timer_cnt=-1;

extern Io_writable_t* writable_io;

Itf_01000c20_content_t bumb;

volatile u8 suck_done = 0;

u16 P0;

u8 m2_event_read(u8 event_id){return suck_done;}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
	
  if(htim==&htim2)
  { 
	  u16 P1=ad_signal.hd_signal_read(0);
	  timer_cnt++;
	  u16 V1 = (P0*(bumb.m1_begin_ml))/P1;
	  if((bumb.m1_begin_ml+timer_cnt*0.1*bumb.m3_speed-V1)>bumb.m2_dest_ml)
	  {
		  suck_done = 1;
		  writable_io->group_write( writable_io,bumb.m5_done_valve_ctrl);
		  HAL_TIM_Base_Stop_IT(&htim2);
		  timer_cnt = 0;
	  }
  }
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

static u8 check(u8 idx, f32 speed, f32 acce, f32 distance)
{
	suck_done = 0;
	int coordinate;
	motors[idx].id = idx;
	motors[idx].postion = motor_step_calc(idx, distance);
	motors[idx].speed = motor_step_calc(idx, speed);
	motors[idx].acc = motor_step_calc(idx, acce);
	motors[idx].isBackZero = 0;
	motors[idx].isRelative = 1;
	u8 error_code = motors[idx].mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
	return error_code;
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
	case 0x01000c20:
	{
		Itf_01000c20_t *cmd = Itf_01000c20_new();		
		cmd->packet_parse(cmd, packet);
		queue_ele_enter(0, itf_code, packet, cmd);
		break;
	}
	default:
		return 1;
	}
	return 0;
}


u8 motor_thread_exe_extend(Motor_queue_ele_t *ele)
{
	switch (ele->cmd_code)
	{
	case 0x01000c20:
	{
		P0=ad_signal.hd_signal_read(0);
		Itf_01000c20_t *cmd = ele->p_cmd;
		bumb=cmd->content;
		HAL_TIM_Base_Start_IT(&htim2);//开定时器中断，在中断中处理
		u8 error_code = check(2,cmd->content.m3_speed,cmd->content.m4_acce,2*cmd->content.m2_dest_ml);
		cmd->rtn_and_free(cmd, &ele->packet_head, error_code);
		break;
	}
	default:
		return 1;
	}
	return 0;
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet *packet)
{
	switch (func_code)
	{
	case 0x01000c20:
	{
		motor_cmd_extend(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}
