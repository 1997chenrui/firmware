#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c20.h"
#include "ad_signal_cfg.h"
#include "ad_signal.h"
#include "io_mod.h"
#include "io_thread.h"
#include "stm32f4xx.h"
#define MEASURE 101325
extern TIM_HandleTypeDef htim2;
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
static int timer_cnt=0;
static u32 P1;

typedef struct _Pressure{
	u16 cnt;
	u8 pressure_buffer[2048];
}Pressure;

Pressure pressure;

extern Io_writable_t* writable_io;

Itf_01000c20_content_t bumb;

volatile u8 suck_done = 0;

u32 P0;

u8 m2_event_read(u8 event_id){return suck_done;}

static void pressure_clear()
{
	pressure.cnt = 9;
}

static void pressure_set_success(u8 success)//错误码
{
	pressure.pressure_buffer[0] = success;
}

static void pressure_set_final_volume(u32 fianl_v)//吸附体积
{
	WriteU32(&pressure.pressure_buffer[1], fianl_v);
}

static void pressure_set_value_cnt()//气压值数
{
	WriteU32(&pressure.pressure_buffer[5], (pressure.cnt-9)>>1);
}

static void pressure_write_pressure(u16 value)//气压数组
{
	if(pressure.cnt + 2 > 2048 - 9)
	{
		ASSERT(1, "pressure.cnt + 2 > 2048 - 9");
	}
	WriteU16(&pressure.pressure_buffer[pressure.cnt], value);
	pressure.cnt += 2;
}

/*
	依据公式：P0*V0=P1*V1
	P1I = 初始体积*初始气压/（初始体积+电机移动增大的体积-目标体积）
	吸附完成判断条件：实时气压>P1I
*/
void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	u16 pressure_value = ad_signal.hd_signal_read(0);
	P1=MEASURE - pressure_value;
	timer_cnt++;
	pressure_write_pressure(pressure_value);

//	printf("Press->cnt = %d\r\n",pressure.cnt);

	f32 V1 = timer_cnt*0.02*bumb.m3_speed;

//	printf("P1 = %d V1 = %f\r\n",P1,V1);
	
	u32 P1i = (u32)(1.0*P0*bumb.m1_begin_ml/(bumb.m1_begin_ml+V1-bumb.m2_dest_ml));

//	printf("timer_cnt = %d\r\n",timer_cnt);
//	printf("P1i = %d\r\n",P1i);

	if(P1i<P1)
	{
		suck_done = 1;//吸附完成后标记置1，电机停止
//		printf("successfully timer_cnt = %d\r\n!!!\r\n", timer_cnt);
		writable_io->group_write( writable_io,bumb.m5_done_valve_ctrl);//吸附完成后，打开阀。
		HAL_TIM_Base_Stop_IT(&htim2);
		timer_cnt = 0;
	}
}

static int check(u8 idx, f32 speed, f32 acce, f32 distance)
{
	suck_done = 0;
	int coordinate;
	motors[idx].id = idx;
	motors[idx].postion = motor_step_calc(idx, distance);
	motors[idx].speed = motor_step_calc(idx, speed);
	motors[idx].acc = motor_step_calc(idx, acce);
	motors[idx].isBackZero = 0;
	motors[idx].isRelative = 1;
	int error_code = motors[idx].mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
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
		queue_ele_enter(0, itf_code, packet, cmd);//进入队列，开线程，电机移动耗时操作
		break;
	}
	default:
		return 1;
	}
	return 0;
}
/*
	压差与AD转换公式：Vout = VS x (0.09 x P + 0.08)（这里VS = 3.3v，P = 大气压-绝对气压）
	VS = 3.0 V ± 0.30 Vdc
*/

u8 motor_thread_exe_extend(Motor_queue_ele_t *ele)
{
	switch (ele->cmd_code)
	{
	case 0x01000c20:
	{
		pressure_clear();
		u16 pressure_value = ad_signal.hd_signal_read(0);
		P0=MEASURE - pressure_value;
		
//		printf("P0 = %d\r\n",P0);

		pressure_write_pressure(pressure_value);
		Itf_01000c20_t *cmd = ele->p_cmd;
		bumb=cmd->content;
		HAL_TIM_Base_Start_IT(&htim2);//开定时器中断，在中断中处理
		u8 error_code = check(2,1.3*cmd->content.m3_speed,cmd->content.m4_acce,40);//1.3*cmd->content.m3_speed：实际速度
		
//		printf("bumb.m1_begin_ml=%f bumb.m2_dest_ml=%f cmd->content.m3_speed = %f\r\n",bumb.m1_begin_ml,bumb.m2_dest_ml,cmd->content.m3_speed);
		
		pressure_set_success(error_code==0?0:1);
		u32 final_v;
		if(error_code)
		{
			final_v = bumb.m1_begin_ml+timer_cnt*0.02*bumb.m3_speed*1000-1000.0*P0*bumb.m1_begin_ml/P1;	
		}
		else
		{
			final_v = bumb.m2_dest_ml*1000;
		}			
		pressure_set_final_volume(final_v);
		
//		printf("error_code = %d\r\n",error_code);

		pressure_set_value_cnt();
		cmd->rtn_and_free(cmd, &ele->packet_head, pressure.cnt, pressure.pressure_buffer);
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
