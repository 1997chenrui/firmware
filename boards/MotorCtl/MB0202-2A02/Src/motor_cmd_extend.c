#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c20.h"
#include "itf_01000c21.h"
#include "itf_01000c23.h"
#include "itf_01000c50.h"
#include "ad_signal_cfg.h"
#include "ad_signal.h"
#include "io_mod.h"
#include "io_thread.h"
#include "stm32f4xx.h"
#include <math.h>
#include "kalman.h"
// 宏定义

#define MEASURE 101325
#define MOTOR_MOVE_MAX_DELAY 400


#define WINDOW_SIZE 3			   // 滑动窗平均算法窗口大小
#define REAGENT_MOTOR_CNT 2
int reagent_err_code = 0;

float volatile kalman;

typedef struct
{
	MOTOR *motor;
	u8 sensor_idx;
	Itf_01000c20_content_t bumb_sample;
	volatile u8 suck_done;
	u32 P0_sample;
	u32 press_real_time_sample;
	f32 motor_intial_position;
	f32 motor_move_volume;
	f32 *press_adjust;
	volatile float kalman;
} ModuleSample_t;

//热力学压力计算吸附试剂
typedef struct
{
	MOTOR *motor;
	u8 sensor_idx;
	
	Itf_01000c23_content_t bumb_sample;
	volatile u8 suck_done;
	// 电机移动至最大位置后的延时，定时器触发次数
	u32 timer_count_motor_move_to_max_position_delay;
	// 错误码
	u32 error_code;
	// 初始气压
	u32 pressure_intial;
	// 0 吸附，1 喷吐
	u8 m0_absorb_expel_flag;
	// 初始体积
	f32 m1_begin_ml;
	// 目标体积
	f32 m2_dest_ml;
	// 电机速度
	f32 m3_speed;
	// 加速度
	f32 m4_acce;
	// 吸附完成后关阀
	u32 m5_done_valve_ctrl;
	//吸附过程中的实时气压
	u32 press_real_time_sample;
	//吸附前电机的位置
	f32 motor_intial_position;
	//电机移动产生的体积
	f32 motor_move_volume;
	//热力学温度校准系数
	f32 temp_power;
	//u32气压校准数组长度
	f32 press_adjust_len;
	//气压校准组参数
	f32 *press_adjust;
	//上位机传过来的初始气压AD
	u32 press_ad;
	//标准气压
	u32 standard_press;
	//kalman
	volatile float kalman;
} Modulereagent_t;

//typedef struct
//{
//	// 电机
//	MOTOR *motor;
//	// 传感器id
//	u8 sensor_idx;
//	// 吸附完成标记位
//	volatile u8 suck_done;
//	// 电机移动至最大位置后的延时，定时器触发次数
//	u32 timer_count_motor_move_to_max_position_delay;
//	// 错误码
//	u32 error_code;
//	// 吸附过程最大压差
//	int Press_differ_Max;
//	// 压力积分
//	float sum_press_intergal;
//	// 数据缓存到数组idx
//	u8 pressure_array_idx;
//	// 平均
//	u16 press_array[WINDOW_SIZE];
//	// 初始气压
//	u32 pressure_intial;
//	// 0 吸附，1 喷吐
//	u8 m0_absorb_expel_flag;
//	// 初始体积
//	f32 m1_begin_ml;
//	// 目标体积
//	f32 m2_dest_ml;
//	// 电机速度
//	f32 m3_speed;
//	// 加速度
//	f32 m4_acce;
//	// 吸附完成后关阀
//	u32 m5_done_valve_ctrl;
//	// 标准积分 
//	f32 m6_standard_integral;
//	// 标准气压
//	f32 m7_standard_press;
//	// 次幂系数
//	f32 m8_power;
//	// 0 普通模式，1 积分模式
//	u8 m9_integral_or_normal;
//	// 上位机下达校准参数
//	f32 press_adjust_k;
//	f32 press_adjust_b;
//	// 电机初始位置
//	f32 inital_position;
//	// 目标积分体积
//	f32 V_dest_intergal;
//	//停止标志气压
//	u32 stop_flag_press;
//} ModuleReagent_t;

// 外部变量
extern MOTOR motors[];
extern Io_writable_t *writable_io;
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
extern TIM_HandleTypeDef htim2;
extern f32 motor_distance_calc(u8 idx, int value);

//ModuleReagent_t module_reagent[REAGENT_MOTOR_CNT] = 
//{
//	{
//		.motor = &motors[1],
//		 .suck_done = 1
//	},
//	{
//		.motor = &motors[2],
//		 .suck_done = 1
//	},
//};

Modulereagent_t reagent[REAGENT_MOTOR_CNT] = 
{
	{
		.motor = &motors[1],
		 .suck_done = 1
	},
	{
		.motor = &motors[2],
		 .suck_done = 1
	},
};

ModuleSample_t sample = {
	
	.motor = &motors[0],
	.suck_done = 1
	
};

u8 m0_event_read(u8 event_id){return sample.suck_done;}
u8 m1_event_read(u8 event_id){return reagent[0].suck_done;}
u8 m2_event_read(u8 event_id){return reagent[1].suck_done;}

//// 静态函数
///**
// * @brief 滑动平均
// *
// * @param data 数据
// * @param index 平均前的数据idx
// * @return u16 平均后的值
// */
//static u16 moving_average_filter(ModuleReagent_t* self, u16 pressure)
//{
//	self->pressure_array_idx++;
//	self->pressure_array_idx %= WINDOW_SIZE;
//	self->press_array[self->pressure_array_idx] = pressure;
//	float sum = 0.0;
//	for (u8 i = 0; i < WINDOW_SIZE; i++)
//	{
//		sum += self->press_array[i];
//	}
//	//printf("sum / WINDOW_SIZE = %d\r\n", (u16)(sum / WINDOW_SIZE));
//	return (u16)(sum / WINDOW_SIZE);
//}


/**
 * @brief 读取吸附事件
 *
 * @return u8  0 未吸附完成 1 吸附完成
 */
static u8 module_sample_event_read(void)
{
	return sample.suck_done;
}

/**
 * @brief 样本电机吸附
 * 
 * @param distance 距离
 * @return int 错误码
 */
static int module_sample_suck_start(float distance)
{
	sample.suck_done = 0;
	HAL_TIM_Base_Start_IT(&htim2);
	int coordinate = 0;
	MOTOR *motor = sample.motor;
	u8 idx = motor->id;
	motor->postion = motor_step_calc(idx, distance);
	motor->speed = motor_step_calc(idx, sample.bumb_sample.m3_speed);
	motor->acc = motor_step_calc(idx, sample.bumb_sample.m4_acce);
	motor->isBackZero = 0;
	motor->isRelative = 1;
	int error_code = motor->mTMC5130MotorMoveStopEvent(motor, &coordinate);
	return error_code;
}

///**
// * @brief 试剂模块吸附
// * @param self 试剂模块
// * @param distance 吸附体积
// * @return int 错误码
// */
//static int module_reagent_suck_start(ModuleReagent_t* self, f32 distance, u8 flag)
//{
//	self->suck_done = 0;
//	HAL_TIM_Base_Start_IT(&htim2);
//	int coordinate;
//	MOTOR* motor = self->motor;
//	int idx = motor->id;
//	motor->postion = motor_step_calc(idx, distance);
//	motor->speed = motor_step_calc(idx, self->m3_speed);
//	motor->acc = motor_step_calc(idx, self->m4_acce);
//	motor->isBackZero = 0;
//	motor->isRelative = (flag ? 0 : 1);
//	int error_code = motor->mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
//	return error_code;
//}

/**
 * @brief 试剂模块吸附
 * @param self 试剂模块
 * @param distance 吸附体积
 * @return int 错误码
 */
static int Module_Meagent_Suck_Start(Modulereagent_t* self, f32 distance, u8 flag)
{
	self->suck_done = 0;
	HAL_TIM_Base_Start_IT(&htim2);
	int coordinate;
	MOTOR* motor = self->motor;
	int idx = motor->id;
	printf("idx = %d\r\n",idx);
	motor->postion = motor_step_calc(idx, distance);
	motor->speed = motor_step_calc(idx, self->m3_speed);
	motor->acc = motor_step_calc(idx, self->m4_acce);
	motor->isBackZero = 0;
	motor->isRelative = (flag ? 0 : 1);

	int error_code = motor->mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
	return error_code;
}

///**
// * @brief 气压校准函数
// *
// * @param self 试剂模块
// * @param pressure 校准前的气压
// * @return u32 校准后的气压
// */
//static u32 module_reagent_adjust_pressure(ModuleReagent_t *self, u16 pressure)
//{
//	return (self->press_adjust_k * pressure + self->press_adjust_b);
//}

///**
// * @brief 气压校准函数
// *
// * @param self 试剂模块
// * @param pressure 校准前的气压
// * @return u32 校准后的气压
// */
//static u32 Module_Reagent_Adjust_Pressure(Modulereagent_t *self, u16 pressure)
//{
////	printf("press_ad = %d\r\n",pressure);
//	u32 press = self->press_adjust_k * pressure + self->press_adjust_b;
////	printf("%f,%f\r\n",self->press_adjust_k,self->press_adjust_b);
////	printf("press = %d\r\n",press);
//	return press;
//}


/**
 * @brief 气压校准函数,临界值校准，若大于校准最高气压，则按最后一段气压进行校准，若小于校准最低气压，则按第一段气压进行校准（新）
 *
 * @param 
 * @param pressure 校准前的气压
 * @return f32 校准后的气压
 */
static f32 press_critical_adjust(u16 press_ad, f32 *press_adjust, u8 i)
{
	f32 k,b,press;
//	printf("---%d---\r\n",i);	
	k = (press_adjust[i+3]-press_adjust[i+1])/(press_adjust[i+2]-press_adjust[i]);
	b = press_adjust[i+1]-k*press_adjust[i];
//	printf("k=%f,b=%f\r\n",k,b);
	press = k*press_ad+b;
	return press;	
}

/**
 * @brief 气压校准函数（新）
 *
 * @param 
 * @param pressure 校准前的气压
 * @return f32 校准后的气压
 */
static f32 Press_Adjust(u16 press_ad, f32 *press_adjust, u8 len, u32 standard_press)
{
//	printf("press_ad=%d\r\n",press_ad);
	if(press_ad<press_adjust[0])
	{
		return (press_critical_adjust(press_ad,press_adjust,0)+standard_press);
	}
	else if(press_ad>press_adjust[len])
	{
		return (press_critical_adjust(press_ad,press_adjust,len-2)+standard_press);
	}
	for(int i=0; i<len; i+=2)
	{
		if(press_ad >= press_adjust[i] && press_ad <= press_adjust[i+2])
		{
			f32 k,b,press;
//			printf("---%d---\r\n",i);
			k = (press_adjust[i+3]-press_adjust[i+1])/(press_adjust[i+2]-press_adjust[i]);
			b = press_adjust[i+1]-k*press_adjust[i];
//			printf("k=%f,b=%f\r\n",k,b);
			press = k*press_ad+b+standard_press;
			return press;
		}
		else
		{
			continue;
		}
	}
	return 0;
}

///**
// * @brief 试剂模块初始化
// *
// * @param self 试剂模块
// */
//static void module_reagent_data_init(ModuleReagent_t *self)
//{
//	self->suck_done = 1;
//	self->timer_count_motor_move_to_max_position_delay = 0;
//	self->error_code = 0;
//	self->Press_differ_Max = 1;
//	self->sum_press_intergal = 0;
//	self->pressure_array_idx = 0;
//	self->stop_flag_press = 100;
//}
//
/**
 * @brief 试剂模块初始化
 *
 * @param self 试剂模块
 */
static void Module_Reagent_Data_Init(Modulereagent_t *self)
{
	self->suck_done = 1;
	self->timer_count_motor_move_to_max_position_delay = 0;
	self->error_code = 0;
}

///**
// * @brief 试剂模块接受上位机参数
// *
// * @param module_reagent 试剂模块
// * @param cmd 上位机参数
// */
//static void module_reagent_data_recieve(ModuleReagent_t *self, Itf_01000c21_t *cmd)
//{
//	self->m0_absorb_expel_flag = cmd->content.m0_absorb_expel_flag;
//	self->m1_begin_ml = cmd->content.m1_begin_ml;
//	self->m2_dest_ml = cmd->content.m2_dest_ml;
//	self->m3_speed = 1.3f * cmd->content.m3_speed;
//	self->m4_acce = cmd->content.m4_acce;
//	self->m5_done_valve_ctrl = cmd->content.m5_done_valve_ctrl;
//	self->m6_standard_integral = cmd->content.m6_standard_integral;
//	self->m7_standard_press = cmd->content.m7_standard_press;
//	self->m8_power = cmd->content.m8_power;
//	self->sensor_idx = cmd->content.m9_channel_id == 0 ? 1 : 2;
//	printf("sensor_idx = %d\r\n",self->sensor_idx);
//	self->press_adjust_k = cmd->content.m10_k;
//	self->press_adjust_b = cmd->content.m11_b;
//	self->stop_flag_press = cmd->content.stop_flag_press;
//	printf("m0_absorb_flag=%d,m1_begin_ml=%f,m2_dest_ml=%f,m3_speed=%f,m4_acce=%f,m5_done_valve_ctrl=%d,m6_standard_integral=%f\r\n",
//	cmd->content.m0_absorb_expel_flag,cmd->content.m1_begin_ml,cmd->content.m2_dest_ml,
//	1.3f * cmd->content.m3_speed,cmd->content.m4_acce,cmd->content.m5_done_valve_ctrl,cmd->content.m6_standard_integral);
//	printf("m7_standard_press=%f,m8_power=%f,m9_channel_id=%d,m10_k=%f,m11_b=%f\r\n",
//	cmd->content.m7_standard_press,cmd->content.m8_power,cmd->content.m9_channel_id,cmd->content.m10_k,cmd->content.m11_b);
//}

/**
 * @brief 试剂模块接受上位机参数
 *
 * @param module_reagent 试剂模块
 * @param cmd 上位机参数
 */
static void Module_Reagent_Data_Recieve(Modulereagent_t *self, Itf_01000c23_t *cmd)
{
	self->m0_absorb_expel_flag = cmd->content.absorb_expel_flag;
	self->m1_begin_ml = cmd->content.m1_begin_ml;
	self->m2_dest_ml = cmd->content.m2_dest_ml;
	self->m3_speed = 1.3f * cmd->content.m3_speed;
	self->m4_acce = cmd->content.m4_acce;
	self->m5_done_valve_ctrl = cmd->content.m5_done_valve_ctrl;

	self->sensor_idx = cmd->content.motor_id == 1 ? 1 : 2;
	printf("sensor_idx = %d\r\n",self->sensor_idx);
	self->bumb_sample.motor_id = cmd->content.motor_id;
	self->timer_count_motor_move_to_max_position_delay = 0;
	self->error_code = 0;
	reagent_err_code = 0;
	self->temp_power = cmd->content.temp_power;
	self->press_adjust_len = cmd->content.adjust_press_len;
	self->press_adjust = cmd->content.adjust_press;
	self->press_ad = cmd->content.press_intial;
	self->standard_press = cmd->content.standard_press;
//	memmove(self->press_adjust, &cmd->content.adjust_press, SAMPLE_PRESS_ADJUST_LEN*4);
//	memmove(&reagent[self->bumb_sample.motor_id].press_adjust, &cmd->content.adjust_press, PRESS_ADJUST);
//	printf("m0_absorb_expel_flag = %d\r\n",self->m0_absorb_expel_flag);
//	printf("m1_begin_ml = %f\r\n",self->m1_begin_ml);
//	printf("m2_dest_ml = %f\r\n",self->m2_dest_ml);
//	printf("m3_speed = %f\r\n",self->m3_speed);
//	printf("m4_acce = %f\r\n",self->m4_acce);
//	printf("m5_done_valve_ctrl = %d\r\n",self->m5_done_valve_ctrl);
//	printf("motor_id = %d\r\n",self->bumb_sample.motor_id);
//	printf("temp_power = %f\r\n", self->temp_power);
//	for(int i=0; i<18; i++)
//	{
//		printf("data[%d]=%f,",i,self->press_adjust[i]);
//	}
//	putchar('\n');
}

///**
// * @brief 试剂吸附
// * 
// * @param self 试剂
// * @return u8 
// */
//static u8 module_reagent_check_run(ModuleReagent_t *self)
//{
//	if (self->suck_done)
//	{
//		return 0;
//	}
//	MOTOR* motor = self->motor;
//	f32 volume_motor_move, volume_current_real, volume_current_theory;
//	int negative_pressure;
//	// 读取传感器压力值，10次平均
//	u8 sen_idx = (self->sensor_idx == 1) ? 2:1 ;
//	//printf("sen_idx = %d\r\n",sen_idx);
//	u16 press_ad = ad_signal.hd_average_signal_read((self->sensor_idx == 1 ? 2:1 ) , 10);
//	
//	//printf("press_ad --%d-- = %d\r\n",self->sensor_idx, press_ad);
//	// 计算滑动平均
//	press_ad = moving_average_filter(self, press_ad);
////	printf("p-ad = %d = \r\n",press_ad);
//	// 校准压力值
//	u32 pressure_current = module_reagent_adjust_pressure(self, press_ad);
//	
////	printf("pc = %d\r\n",pressure_current);
//	//printf("reagent --%d-- pressure_current-after-adjust = %d\r\n",self->sensor_idx , pressure_current);
//	
//	//printf("m[%d]_absorb_expel_flag = %d\r\n",self->sensor_idx, self->m0_absorb_expel_flag);
//	if (!self->m0_absorb_expel_flag)
//	{
//		negative_pressure = self->pressure_intial - pressure_current;
//		// 电机移动产生的体积
//		volume_motor_move = motor_distance_calc(0, mTMC5130DeviceGetPostion(motor, 1));
//		// 理论上的当前体积
//		volume_current_theory = self->m1_begin_ml + volume_motor_move - self->m2_dest_ml;
//	}
//	else
//	{
//		negative_pressure = pressure_current - self->pressure_intial;
//		// 电机移动产生的体积
//		volume_motor_move = self->inital_position - motor_distance_calc(0, mTMC5130DeviceGetPostion(motor, 1));
//		// 理论上的当前体积
//		volume_current_theory = self->m1_begin_ml - volume_motor_move + self->m2_dest_ml;
//	}
//	
//	// 当前吸附的体积
//	volume_current_real = self->pressure_intial * self->m1_begin_ml / pressure_current;
//	self->sum_press_intergal += 1.0 * negative_pressure;
//	
////	printf("spi= %f np = %d\r\n", self->sum_press_intergal, negative_pressure);
//	
//	
//	self->V_dest_intergal = self->sum_press_intergal;
//	
//	u32 pressure_theory = (u32)(1.0 * self->pressure_intial * self->m1_begin_ml / volume_current_theory);
//	
//	if (self->error_code != 0)
//	{
//		self->timer_count_motor_move_to_max_position_delay++; // 若电机走到最大行程还没吸附成功，则等待，最多40s
////		printf("md = %d\r\n",self->timer_count_motor_move_to_max_position_delay);
//	}
//	
//	if (self->Press_differ_Max < negative_pressure)
//	{
//		self->Press_differ_Max = negative_pressure;
//	}
//	
//	f32 standard_compare_integral = self->m6_standard_integral * pow((self->Press_differ_Max * 1.0) / self->m7_standard_press, self->m8_power);

//	//printf("m6_standard_integral = %f, Press_differ_Max = %d, m7_standard_press = %f, m8_power = %f\r\n",
//	//self->m6_standard_integral, self->Press_differ_Max, self->m7_standard_press, self->m8_power);
////	printf("sci= %f\r\n",standard_compare_integral);
//	//printf("\r\n\r\n ecode = %d\r\n", self->error_code);
//	
//	if(self->timer_count_motor_move_to_max_position_delay == 0)
//	{
//		if ((!self->m0_absorb_expel_flag) && (self->sum_press_intergal >= self->m6_standard_integral))
//		{
////			pressure_buffer_done(&buffer_array[self->sensor_idx]);
//			printf("Min_final = %d\r\n", self->Press_differ_Max);
//			self->suck_done = 1;											 // 吸附完成后标记置1，电机停止
//			printf("sdt=%d\r\n", OSTimeGet());
//			self->error_code = 0;											 // 执行成功
//			writable_io->group_write(writable_io, self->m5_done_valve_ctrl); // 吸附完成后，打开阀。
//			printf("reagent --%d-- standard_press = %f\r\n", self->sensor_idx,  self->m7_standard_press);
//			printf("reagent --%d-- self->sum_press_intergal = %f\r\n", self->sensor_idx, self->sum_press_intergal);
//			printf("reagent --%d-- standard_integral = %f\r\n", self->sensor_idx, self->m6_standard_integral);
//			printf("reagent --%d-- stand_compare_integral = %f\r\n", self->sensor_idx, standard_compare_integral);
//			//			printf("pressure_current = %d volume_motor_move = %f volume_current_theory = %f flag = %d pressure_theory = %d\r\n",pressure_current,volume_motor_move,volume_current_theory,absorb_expel_flag,pressure_theory);
//			printf("reagent --%d-- absorb successfully\r\nV_dest_intergal_1 = %f\r\n", self->sensor_idx, self->V_dest_intergal);
//			return 0;
//		}
//		else if((self->m0_absorb_expel_flag) && (self->sum_press_intergal >= self->m6_standard_integral))
//		{
////			pressure_buffer_done(&buffer_array[self->sensor_idx]);
//			printf("reagent --%d-- Press_differ_Max = %d\r\n", self->sensor_idx, self->Press_differ_Max);
//			self->suck_done = 1;											 // 吸附完成后标记置1，电机停止
//			self->error_code = 0;				// 执行成功
//			
//			printf("valve_ctrl = %d\r\n", self->m5_done_valve_ctrl);
//			writable_io->group_write(writable_io, self->m5_done_valve_ctrl); // 吸附完成后，打开阀。
//			printf("reagent --%d-- self->sum_press_intergal = %f standard_compare_integral= %f \r\n", self->sensor_idx, self->sum_press_intergal, standard_compare_integral);
//			printf("reagent --%d-- expel successfully\r\nV_dest_intergal_expel_integral = %f\r\n", self->sensor_idx, self->V_dest_intergal);
//			return 0;
//		}
//	}
//	/*standard_compare_integral = 标准积分*（实时最低气压/标准最低气压）^n*/
//	
//	//printf("timer_count_motor_move_to_max_position_delay = %d\r\n",self->timer_count_motor_move_to_max_position_delay);
//	else if (self->timer_count_motor_move_to_max_position_delay > 0 && self->timer_count_motor_move_to_max_position_delay < MOTOR_MOVE_MAX_DELAY) // 若在电机移动或电机走到最大行程后等待20s中
//	{
////		f32 standard_compare_integral = self->m6_standard_integral * pow((self->Press_differ_Max * 1.0) / self->m7_standard_press, self->m8_power);
//		if (((!self->m0_absorb_expel_flag) && (self->sum_press_intergal >= standard_compare_integral))) //|| (self->stop_flag_press > negative_pressure)
//		{
////			pressure_buffer_done(&buffer_array[self->sensor_idx]);
//			printf("reagent --%d-- Press_Min_final = %d\r\n", self->sensor_idx, self->Press_differ_Max);
//			self->suck_done = 1;											 // 吸附完成后标记置1，电机停止
//			self->error_code = 0;											 // 执行成功
//			writable_io->group_write(writable_io, self->m5_done_valve_ctrl); // 吸附完成后，打开阀。
//			printf("reagent --%d-- standard_press = %f\r\n", self->sensor_idx,  self->m7_standard_press);
//			printf("reagent --%d-- self->sum_press_intergal = %f\r\n", self->sensor_idx, self->sum_press_intergal);
//			printf("reagent --%d-- standard_integral = %f\r\n", self->sensor_idx, self->m6_standard_integral);
//			printf("reagent --%d-- stand_compare_integral = %f\r\n", self->sensor_idx, standard_compare_integral);
//			//			printf("pressure_current = %d volume_motor_move = %f volume_current_theory = %f flag = %d pressure_theory = %d\r\n",pressure_current,volume_motor_move,volume_current_theory,absorb_expel_flag,pressure_theory);
//			printf("reagent --%d-- absorb successfully\r\nV_dest_intergal_1 = %f\r\n", self->sensor_idx, self->V_dest_intergal);
//			return 0;
//		}
//		else if(((self->m0_absorb_expel_flag) && (self->sum_press_intergal >= standard_compare_integral)) ) //|| ((self->stop_flag_press > negative_pressure))
//		{
////			pressure_buffer_done(&buffer_array[self->sensor_idx]);
//			printf("reagent --%d-- Press_differ_Max = %d\r\n", self->sensor_idx, self->Press_differ_Max);
//			self->suck_done = 1;											 // 吸附完成后标记置1，电机停止
//			self->error_code = 0;				// 执行成功
//			
//			printf("valve_ctrl = %d\r\n", self->m5_done_valve_ctrl);
//			writable_io->group_write(writable_io, self->m5_done_valve_ctrl); // 吸附完成后，打开阀。
//			printf("reagent --%d-- self->sum_press_intergal = %f standard_compare_integral= %f \r\n", self->sensor_idx, self->sum_press_intergal, standard_compare_integral);
//			printf("reagent --%d-- expel successfully\r\nV_dest_intergal_expel_integral = %f\r\n", self->sensor_idx, self->V_dest_intergal);
//			return 0;
//		}
//	}
//	else if ((self->timer_count_motor_move_to_max_position_delay >= MOTOR_MOVE_MAX_DELAY) ) // || ((self->stop_flag_press > negative_pressure))等待超过20s，则认定吸附失败
//	{
////		pressure_buffer_done(&buffer_array[self->sensor_idx]);
//		self->suck_done = 1;											 // 吸附完成后标记置1，电机停止
//		self->error_code = 1;											 // 执行成功
//		writable_io->group_write(writable_io, self->m5_done_valve_ctrl); // 吸附完成后，打开阀。
//		printf("reagent --%d-- absorb failed\r\n", self->sensor_idx);
//		return 0;
//	}
//	return 1;
//}

/*
 *
 *	依据公式：P0*V0=P1*V1
 *	P1I = 初始体积*初始气压/（初始体积+电机移动增大的体积-目标体积）
 *	吸附完成判断条件：实时气压>P1I
 *	@return 1 正在运行 0 运行结束
*/
static u8 module_sample_check_run()
{
	ModuleSample_t* self = &sample;
	if (self->suck_done)
	{
		return 0;
	}
	Itf_01000c20_content_t* bumb_sample = &self->bumb_sample;
	u16 press_ad = ad_signal.hd_signal_read_average(self->sensor_idx ,10);
//	printf("%d,",press_ad);
	press_ad = KalmanFilter(press_ad,&self->kalman);
//	printf("%d,",press_ad);
	self->press_real_time_sample = Press_Adjust(press_ad, bumb_sample->press_adjust, bumb_sample->adjust_press_len-2, bumb_sample->standard_press);
	self->motor_move_volume = motor_distance_calc(0, mTMC5130DeviceGetPostion(&motors[0],1)) - self->motor_intial_position;
	u32 P1i = (u32)(1.0 * self->P0_sample * bumb_sample->m1_begin_ml / (bumb_sample->m1_begin_ml + self->motor_move_volume - bumb_sample->m2_dest_ml));
//	printf("P1i = %d, self->press_real_time_sample = %d\r\n",P1i, self->press_real_time_sample);
	u32 final_v = sample.bumb_sample.m1_begin_ml*1000 + sample.motor_move_volume * 1000 - 1000.0 * sample.P0_sample * sample.bumb_sample.m1_begin_ml / sample.press_real_time_sample;
//	printf("final_v = %d",final_v);
	if (P1i < self->press_real_time_sample)
	{
		self->suck_done = 1;													   // 吸附完成后标记置1，电机停止
		writable_io->group_write(writable_io, bumb_sample->m5_done_valve_ctrl); // 吸附完成后，打开阀。
		printf("sample absorb successfully\r\n");
		return 0;
	}
	return 1;
}

/*
 *
 *	依据公式：P0*V0=P1*V1
 *	P1I = 初始体积*初始气压/（初始体积+电机移动增大的体积-目标体积）
 *	吸附完成判断条件：实时气压>P1I
 *	@return 1 正在运行 0 运行结束
*/
static u8 Module_Reagent_Check_Run(Modulereagent_t *self)
{
	if (self->suck_done)
	{
		return 0;
	}
	Itf_01000c23_content_t* bumb_sample = &self->bumb_sample;
	u32 Ptheory;
	f32 VR,V_reall;
	u16 press_ad = ad_signal.hd_signal_read_average(self->sensor_idx ,10);
//	printf("%d,",press_ad);
	press_ad = KalmanFilter(press_ad,&self->kalman);
//	printf("%d,",press_ad);
	self->press_real_time_sample = Press_Adjust(press_ad, self->press_adjust, self->press_adjust_len-2, self->standard_press);
//	printf("%d\r\n",self->press_real_time_sample);
	if(!self->m0_absorb_expel_flag)
	{
		self->motor_move_volume = motor_distance_calc(self->motor->id, mTMC5130DeviceGetPostion(&motors[self->motor->id],1)) - self->motor_intial_position;
		V_reall = (bumb_sample->m1_begin_ml + self->motor_move_volume - bumb_sample->m2_dest_ml);
		VR = 1.0 * self->pressure_intial * bumb_sample->m1_begin_ml * self->temp_power;
//		printf("absrob:\r\n");
//		printf("%d,%f,%f\r\n",self->pressure_intial, bumb_sample->m1_begin_ml, self->temp_power);
//		printf("%f,%f\r\n",self->motor_move_volume, bumb_sample->m2_dest_ml);
//		printf("%f,%f\r\n",VR,V_reall);
		Ptheory = (u32)(VR / V_reall);
	}
	else
	{
//		printf("expel:\r\n");
		self->motor_move_volume = self->motor_intial_position - motor_distance_calc(self->motor->id, mTMC5130DeviceGetPostion(&motors[self->motor->id],1));
		V_reall = (bumb_sample->m1_begin_ml - self->motor_move_volume + bumb_sample->m2_dest_ml);
		VR = 1.0 * self->pressure_intial * bumb_sample->m1_begin_ml * self->temp_power;
//		printf("%d,%f,%f\r\n",self->pressure_intial, bumb_sample->m1_begin_ml, self->temp_power);
//		printf("%f,%f\r\n",self->motor_move_volume, bumb_sample->m2_dest_ml);
//		printf("%f,%f\r\n",VR,V_reall);
		Ptheory = (u32)(VR / V_reall);
	}
//	printf("mmv = %fself->motor_move_volume\r\n",self->motor_move_volume);
//	printf("prt = %d\r\n", self->press_real_time_sample);
//	printf("%f,%f,%d\r\n",V_reall, VR, Ptheory);
	if(reagent_err_code)
	{
		self->timer_count_motor_move_to_max_position_delay ++;
//		printf("cnt = %d\r\n",self->timer_count_motor_move_to_max_position_delay);
	}
	
//	printf("Ptheory = %d,prt = %d\r\n",Ptheory,self->press_real_time_sample);
	u32 final_v;
	if ((!self->m0_absorb_expel_flag))
	{
		final_v = 1000*self->m1_begin_ml + self->motor_move_volume * 1000 - 1000.0 * self->pressure_intial * self->m1_begin_ml / self->press_real_time_sample;
	}
	else if(self->m0_absorb_expel_flag)
	{
		final_v = 1000.0 * self->pressure_intial * self->m1_begin_ml / self->press_real_time_sample + self->motor_move_volume * 1000 - self->bumb_sample.m1_begin_ml*1000; 
	}
//	printf("fv = %d\r\n",final_v);
//	printf("bm = %f,mmv = %f,pi = %d,prt = %d\r\n",self->m1_begin_ml, self->motor_move_volume, self->pressure_intial, self->press_real_time_sample);
	if ((Ptheory <= self->press_real_time_sample)&&(!self->m0_absorb_expel_flag)&&(self->timer_count_motor_move_to_max_position_delay < 400))
	{
		self->suck_done = 1;													   // 吸附完成后标记置1，电机停止
		self->error_code = 0;
		writable_io->group_write(writable_io, bumb_sample->m5_done_valve_ctrl); // 吸附完成后，打开阀。
		printf("sample absorb successfully\r\n");
		return 0;
	}
	else if((Ptheory >= self->press_real_time_sample)&&(self->m0_absorb_expel_flag)&&(self->timer_count_motor_move_to_max_position_delay <400))
	{
		self->suck_done = 1;													   // 吸附完成后标记置1，电机停止
		self->error_code = 0;
		writable_io->group_write(writable_io, bumb_sample->m5_done_valve_ctrl); // 吸附完成后，打开阀。
		printf("sample expel successfully\r\n");
		return 0;
	}
	else if(self->timer_count_motor_move_to_max_position_delay >= 400)
	{
		self->suck_done = 1;													   // 吸附完成后标记置1，电机停止
		self->error_code = 1;
		writable_io->group_write(writable_io, bumb_sample->m5_done_valve_ctrl); // 吸附完成后，打开阀。
		printf("sample absorb expel failed\r\n");
	}
	return 1;
}


/*
	依据公式：P0*V0=P1*V1
	P1I = 初始体积*初始气压/（初始体积+电机移动增大的体积-目标体积）
	吸附完成判断条件：实时气压>P1I
*/
void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	u8 has_run = 0;
	
	for (u8 i = 0; i < 2; i++)
	{
//		has_run |= module_reagent_check_run(&module_reagent[i]);
		//printf("run[%d]= = %d\r\n",i, has_run);
		has_run |= Module_Reagent_Check_Run(&reagent[i]);
	}
	has_run |= module_sample_check_run();
	//printf("sample-suck_done = %d\r\n",sample.suck_done);
	if (!has_run)
	{
		printf("run[2] = %d\r\n", has_run);
		printf("all succ\r\n");
		HAL_TIM_Base_Stop_IT(&htim2); // 开定时器中断，在中断中处理
	}
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
		case 0x01000c20:
		{
			Itf_01000c20_t *cmd = Itf_01000c20_new();
			cmd->packet_parse(cmd, packet);
			queue_ele_enter(0, itf_code, packet, cmd); // 进入队列，开线程，电机移动耗时操作
			break;
		}
		
		case 0x01000c21:
		{
			Itf_01000c21_t *cmd = Itf_01000c21_new();
			cmd->packet_parse(cmd, packet);
			u8 motor_idx = cmd->content.m9_channel_id == 0 ? 1 : 2;
			queue_ele_enter(motor_idx, itf_code, packet, cmd); // 进入队列，开线程，电机移动耗时操作
			break;
		}

		case 0x01000c23:
		{
			Itf_01000c23_t *cmd = Itf_01000c23_new();
			cmd->packet_parse(cmd, packet);
			u8 motor_idx = cmd->content.motor_id;
			queue_ele_enter(motor_idx, itf_code, packet, cmd); // 进入队列，开线程，电机移动耗时操作
			break;
		}
//		case 0x01000c50:
//		{
//			Itf_01000c50_t *cmd = Itf_01000c50_new();
//			cmd->packet_parse(cmd, packet);
//			u8 motor_id = cmd->content.motor_id;
//			ASSERT(motor_id < 1 || motor_id > 2, "motor_id error\r\n");
////			module_reagent[motor_id].press_adjust_k = cmd->content.m1_k;
////			module_reagent[motor_id].press_adjust_b = cmd->content.m2_b;
////			printf("press_adjust_k = %f press_adjust_b = %f\r\n", module_reagent[motor_id].press_adjust_k, module_reagent[motor_id].press_adjust_b);
//			cmd->rtn_and_free(cmd, packet, 0);
//			break;
//		}
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
			Itf_01000c20_t *cmd = ele->p_cmd;
			sample.bumb_sample = cmd->content;
			u16 press_ad = sample.bumb_sample.press_ad;
			sample.kalman = press_ad;
//			printf("p_i_ad=%d,",press_ad);
			press_ad = KalmanFilter(press_ad,&sample.kalman);
//			printf("%d,",press_ad);
//			u16 press_ad = ad_signal.hd_signal_read_average(0 ,10);
//			for(int i=0; i<sample.bumb_sample.adjust_press_len; i++)
//			{
//				printf("%f,",sample.bumb_sample.press_adjust[i]);
//			}
			sample.P0_sample = Press_Adjust(press_ad, sample.bumb_sample.press_adjust, cmd->content.adjust_press_len-2,sample.bumb_sample.standard_press);
//			printf("P0 = %d\r\n", sample.P0_sample);
//			printf("press_ad=%d,press_standard=%d\r\n",sample.bumb_sample.press_ad,sample.bumb_sample.standard_press);
			sample.motor_intial_position = motor_distance_calc(0, mTMC5130DeviceGetPostion(sample.motor,1));
//			printf("m_i_p = %f\r\n",sample.motor_intial_position);
			// 1.3*cmd->content.m3_speed：实际速度
			sample.bumb_sample.m3_speed *= 1.3;
//			printf("b_m=%f b_s=%f s = %f\r\n", sample.bumb_sample.m1_begin_ml, sample.bumb_sample.m2_dest_ml, cmd->content.m3_speed);

//			printf("ac = %f, ctl = %d\r\n", cmd->content.m4_acce, cmd->content.m5_done_valve_ctrl);
			int error_code = module_sample_suck_start(50);
			
			u32 final_v;
//			printf("m_b = %f, m_m_v = %f, P0 = %d,p_r_t = %d\r\n",sample.bumb_sample.m1_begin_ml, sample.motor_move_volume, sample.P0_sample, sample.press_real_time_sample);
			if (error_code != 0)
			{
				final_v = sample.bumb_sample.m1_begin_ml*1000 + sample.motor_move_volume * 1000 - 1000.0 * sample.P0_sample * sample.bumb_sample.m1_begin_ml / sample.press_real_time_sample;
			}
			else
			{
				final_v = sample.bumb_sample.m2_dest_ml * 1000;
			}

			printf("error_code = %d\r\n", error_code);
			cmd->rtn_and_free(cmd, &ele->packet_head, error_code, final_v);
//			printf("20_return succ\r\n");
			break;
		}
		
//		case 0x01000c21:
//		{
//			Itf_01000c21_t *cmd = ele->p_cmd;
//			ModuleReagent_t* self = &module_reagent[cmd->content.m9_channel_id];
//			module_reagent_data_init(self);
//			module_reagent_data_recieve(self, cmd);
//			printf("si = %d", self->sensor_idx);
//			u16 press_ad = ad_signal.hd_average_signal_read((self->sensor_idx == 1 ? 2:1 ), 10);
//			for(u8 i = 0; i < WINDOW_SIZE; i++)
//			{
//				self->press_array[i] = press_ad;
//			}
//			printf("p-ad = %d\r\n",press_ad);
//			self->pressure_intial = module_reagent_adjust_pressure(self, press_ad);
//			self->inital_position = motor_distance_calc(0, mTMC5130DeviceGetPostion(self->motor, 1)); // 计算电机的初始位置
//			printf("mip = %f\r\n",self->inital_position);
//			printf("pi = %d\r\n", self->pressure_intial);
//			self->error_code = module_reagent_suck_start(self, (self->m0_absorb_expel_flag ? 5 : 50), self->m0_absorb_expel_flag);
//			printf("sdt1=%d\r\n", OSTimeGet());
//			printf("e1code=%d\r\n",self->error_code);
//			while ((!self->suck_done)){OSTimeDly(20);}
//			printf("ec = %d pdm = %d vdi = %f\r\n",self->error_code, self->Press_differ_Max, self->V_dest_intergal);
//			cmd->rtn_and_free(cmd, &ele->packet_head, self->error_code, self->Press_differ_Max, self->V_dest_intergal);
//			break;
//		}
		case 0x01000c23:
		{
			Itf_01000c23_t *cmd = ele->p_cmd;
			
			Modulereagent_t* self = &reagent[cmd->content.motor_id];
			
			Module_Reagent_Data_Recieve(self,cmd);
			
			self->sensor_idx = cmd->content.motor_id == 1 ? 1 : 2;
			
			
//			u16 pressure_value_ad = ad_signal.hd_signal_read_ad(self->sensor_idx);
			
			u16 press_ad = self->press_ad;
			self->kalman = press_ad;
			
//			printf("p_i_ad=%d,",press_ad);
			press_ad = KalmanFilter(press_ad,&self->kalman);
//			printf("%d,",press_ad);
			
//			printf("pressure_value = %d\r\n",pressure_value);
			
			self->pressure_intial = Press_Adjust(press_ad, self->press_adjust, self->press_adjust_len-2, self->standard_press);
//			printf("%d\r\n",self->pressure_intial);
			self->motor_intial_position = motor_distance_calc(self->motor->id, mTMC5130DeviceGetPostion(self->motor,1));
			
			self->bumb_sample = cmd->content;
			// 1.3*cmd->content.m3_speed：实际速度
			self->bumb_sample.m3_speed *= 1.3;
			
			reagent_err_code = Module_Meagent_Suck_Start(self, (cmd->content.absorb_expel_flag ? 5 : 50), cmd->content.absorb_expel_flag);

			u32 final_v;
			
			while(!self->suck_done){OSTimeDly(20);}
//			
//			printf("mb = %f,mmv = %f,pi = %d, prt =%d",self->m1_begin_ml,self->motor_move_volume, self->pressure_intial, self->press_real_time_sample);
			if (!self->m0_absorb_expel_flag)
			{
				final_v = 1000*self->m1_begin_ml + self->motor_move_volume * 1000 - 1000.0 * self->pressure_intial * self->m1_begin_ml / self->press_real_time_sample;
			}
			else if(self->m0_absorb_expel_flag)
			{
				final_v = 1000.0 * self->pressure_intial * self->m1_begin_ml / self->press_real_time_sample + self->motor_move_volume * 1000 - self->bumb_sample.m1_begin_ml*1000; 
			}
			
//			printf("er = %d,mmv = %f,prt = %d,fv = %d",self->error_code, self->motor_move_volume, self->press_real_time_sample,final_v);
//			printf("intial_press = %d\r\n",self->pressure_intial);
			cmd->rtn_and_free(cmd, &ele->packet_head, self->error_code, self->motor_move_volume, self->press_real_time_sample, final_v, self->pressure_intial);
//			printf("23_return succ\r\n");
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
//		case 0x01000c21:
		case 0x01000c23:
//		case 0x01000c50:
		{
			motor_cmd_extend(func_code, packet);
			break;
		}
	default:
		return 1;
	}
	return 0;
}

