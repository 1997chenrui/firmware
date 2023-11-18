#ifndef MOTOR__H__
#define MOTOR__H__
#include "stm32f4xx.h"
#include <stdbool.h>
#include "mTMC5130Driver.h"
#include "os_implement.h"
#include "mTMC5130DThread.h"

#define TMC5130 1
#define TMC5160 2

#define GET_STATE_BIT(s,n)   ((s>>n)&0x1)    //获取字节中的位  用来判断电机状态寄存器
#define DRI_REVERAL(dri)  ((dri == 0) ? 1:0)   ///< 方向反转

#define STOP_AMAX         65535  ///<停止时的最大加速度
//#define HOLD_CURRENT   14  ///<  HOLD住电机的电流  
#define TCOOL_THRS(n)  (n/32>0 ? n/32: -(int)(n/32))  ///< 计算TCOOL值
#define SGT(n)	        (n * 1.1)      ///< 计算SGT



#define WAIT_REACH_SEM    1 //等侍位置信号量到达
#define WAIT_TOUCH_SEM    2 //等侍光耦被触发信号量
#define WAIT_REACH_FLG    3 //等侍标志组的信号  用在多电机同时运行中 
#define WAIT_STOP_SEM     4 //等待电机停止信号量
#define WAIT_VELOCITY_SEM 5 //等待电机速度到达信号量
#define WAIT_ERROR_SEM    6 //等侍错误报警信号量

typedef enum 
{
	MOTOR_STATUS_NO_ERROR=0,
	MOTOR_STATUS_ERROR_TIME_OUT,
	MOTOR_STATUS_ERROR_MOTORABORT,
	MOTOR_STATUS_ZERO_FAIL,
	MOTOR_STATUS_LISTEN_IO_FAIL,
	MOTOR_STATUS_RESET_FAIL,  

	MOTOR_STATUS_RESET_START,
	MOTOR_STATUS_RESET_WAIT_SEM,
	MOTOR_STATUS_RESET_WAIT_REACH,
	MOTOR_STATUS_RESET_FINISH,

	MOTOR_STATUS_ZERO_START,
	MOTOR_STATUS_ZERO_WAIT_SEM,
	MOTOR_STATUS_ZERO_FINISH,

	MOTOR_STATUS_LISTEN_IO_START,   	
	MOTOR_STATUS_LISTEN_IO_FINISH,
}DEFINESTATUS;

typedef struct _Motor_cfg
{
	uint32_t             zero_cordinate;        	//零位距离
	uint32_t             max_cordinate;         	//最大行程
	uint32_t             lost_endure_continue;  	//失步忍受,继续运动,返回时通知上位机
	uint32_t			 lost_endure_stop;			//失步忍受,停止  报错		
	uint32_t      		 rs_cordinate;//反向脱离光耦的距离
	uint16_t		     subdivision;     //细分
	uint8_t              currentLevel;      //运行电流
	uint8_t              holdLevel;      //stop电流
	uint8_t              zero_trig_vol  : 1;		//零位触发电压
	uint8_t              dir_vol  : 1;				//正方向电压
	uint8_t              enb_vol  : 1;             //使能电平
	uint8_t              half_vol  : 2;             //半流电平
	uint8_t              is_rotate  : 1;			//是不是旋转电机
}motor_cfg_struct;  

typedef struct _Motor_args
{
	struct _Motor *motor;
	u32     errCode;
	u8      m1_idx;
	u32 	m2_acce;
	u32 	m3_speed;
	u32     m4_distance;
	u8      m5_isRelative;
	u32 	m6_delayms;
	u8      m7_currentLevel;
	u16	   m8_subdivision;
  bool     is_relative;
}motor_multi_move_args_t;


typedef struct _Motor
{
	u8     id;
	u8     dir;             //方向
	u8     is_busy;			//0不忙 1复位 2回零 3运动 4MoveGPIO
	u8     i_hold;           //静止电流  锁住电机时电流
	u8     i_run;            //运行电流
	u8     is_abort;
	u8     Listen_type;
	u8     Listen_state;
	u8     Listen_old_state; 
	u8     zeor_isr_id;      //零位 光耦触发中断号
	u8     reach_isr_id;     //目标位置到达触发中断号
	u8     error_isr_id;     //芯片错误检测中断号
	u8     wait_type;        //等待类开型
	u8     evnt_group;       //事件组
	u8     isBackZero;       //是否回零，1是，0否
	u8     isRelative;       //是否相对运动，1是，0否
	u8     re_current;       //复位时使用的电流
	u16    re_mico;          //复位时使用的细分
	u16    micostep;         //细分
	u32    Listen_pin;	
	u32    speed;
	u32    acc;
	u32    reset_speed;
	u32    reset_acc;
	s32    c_postion;	//当前位置
	u32    postion;	    //步距
	s32    ZeorTouch;   //零位触发位置
	s32    ZeroBrake;
	s32    BrakeStep;	//刹车距离
	s32    Zero_drift;	//零位漂移  回零时测得的零位漂移量  小于0则需要多走|Zero_drift|，大于0则需要少走|Zero_drift|

	u16    RAMPSTAT;
	u32    DRVSTATUS;
	u8     DRVSTATE;

	GPIO_TypeDef *Listen_port;
	DEFINESTATUS status;	            //电机的运行状态
	OS_EVENT * wait_sem;  
	
	motor_cfg_struct  *mot_cfg;   //电要参数配置表
	DRIVER_REG *driver_reg;

	void (*write_io)(pin_id_t id,int value);
	int  (*read_io) (pin_id_t id,int,char);	

	DEFINESTATUS (*mTMC5130Motor_State_Machine)(struct _Motor *);
	
	int (*mTMC5130MotorBackZero)(struct _Motor *);	
	int (*mTMC5130MotorListenIo)(struct _Motor *);
	
	int (*mTMC5130MotorReset)(struct _Motor *);
	int (*mTMC5130MotorMove)(struct _Motor *);
	int (*mTMC5130MotorRotate)(struct _Motor *);
	int (*mTMC5130MotorGetStatus)(struct _Motor *);
	int (*mTMC5130MotorStop)(struct _Motor *);
	int (*mTMC5130Motor_MultiMove)(int ,motor_multi_move_args_t *,bool);
	int (*mTMC5130MotorMoveGPIOEvent)(struct _Motor *,unsigned int* );
}MOTOR;


#define MOTOR_IO_RD_DECLARE(midx,t_port,t_pin,r_port,r_pin,e_port,e_pin)\
static int m##midx##_io_read(pin_id_t id,int isr_no,char flg)\
{\
	int result=0;\
	switch(id)\
	{\
		case PIN_ID_SET_SENSOR:\
		{/*复位的时候执行*/\
			GPIO_InitTypeDef GPIO_InitStruct;\
			GPIO_InitStruct.Pin = t_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
			HAL_GPIO_Init(t_port, &GPIO_InitStruct);\
			/*HAL_NVIC_DisableIRQ((IRQn_Type)isr_no);*/\
			result=HAL_GPIO_ReadPin(t_port,t_pin);\
		}break;\
		case PIN_ID_SET_TOUCH_ISR:\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			GPIO_InitStruct.Pin = t_pin;\
			GPIO_InitStruct.Mode = (flg == 0) ?GPIO_MODE_IT_FALLING:GPIO_MODE_IT_RISING;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			HAL_GPIO_Init(t_port, &GPIO_InitStruct);\
			HAL_NVIC_SetPriority((IRQn_Type)isr_no , 0, 0);\
			HAL_NVIC_EnableIRQ((IRQn_Type)isr_no );\
		}break;\
		case PIN_ID_SET_REACH_ISR:\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			GPIO_InitStruct.Pin = r_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			HAL_GPIO_Init(r_port, &GPIO_InitStruct);\
			HAL_NVIC_SetPriority((IRQn_Type)isr_no , 0, 0);\
			HAL_NVIC_EnableIRQ((IRQn_Type)isr_no );\
		}break;\
		case PIN_ID_SET_ERROR_ISR:\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			GPIO_InitStruct.Pin = e_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			HAL_GPIO_Init(e_port, &GPIO_InitStruct);\
			HAL_NVIC_SetPriority((IRQn_Type)isr_no , 0, 0);\
			HAL_NVIC_EnableIRQ((IRQn_Type)isr_no );\
		}break;\
		case PIN_INIT_TOUCH_ISR:result=Get_ISR_ID(t_pin);break;\
		case PIN_INIT_REACH_ISR:result=Get_ISR_ID(r_pin);break;\
		case PIN_INIT_ERROR_ISR:result=Get_ISR_ID(e_pin);break;\
		case READ_PIN_ID_SENSOR:result=HAL_GPIO_ReadPin(t_port,t_pin);break;\
		case READ_PIN_ID_ERROR_ISR:result=HAL_GPIO_ReadPin(e_port,e_pin);break;\
		case READ_PIN_ID_TOUCH_ISR:result=HAL_GPIO_ReadPin(t_port,t_pin);break;\
		case READ_PIN_ID_REACH_ISR:result=HAL_GPIO_ReadPin(r_port,r_pin);break;\
		default:break;\
	} \
	return result;\
}	
						
#define MOTOR_IO_WR_DECLARE(midx,cs_port,cs_pin,en_port,en_pin,em_port,em_pin,refl_port,refl_pin,refr_port,refr_pin)  \
static void m##midx##_io_write(pin_id_t id,int value)  \
{\
	static char is_cs_init=0,is_eme_init=0,is_en_init=0,is_refl_init=0,is_refr_init=0;\
	switch(id)   \
	{                                                                  \
		case PIN_ID_CS:                                                  \
		{         \
			if(is_cs_init <= 0)\
			{\
				is_cs_init=1;\
				GPIO_InitTypeDef GPIO_InitStruct;\
				__HAL_RCC_##cs_port##_CLK_ENABLE();\
				GPIO_InitStruct.Pin = cs_pin;\
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
				GPIO_InitStruct.Pull = GPIO_NOPULL;\
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
				HAL_GPIO_Init(cs_port, &GPIO_InitStruct);\
			}\
			HAL_GPIO_WritePin(cs_port,cs_pin,(GPIO_PinState)(value));    \
			break;                                                     \
		}                                                              \
		case PIN_ID_EME:                                                    \
		{       \
			if(is_eme_init <= 0)\
			{\
				is_eme_init=1;\
				GPIO_InitTypeDef GPIO_InitStruct;\
				__HAL_RCC_##em_port##_CLK_ENABLE();\
				GPIO_InitStruct.Pin = em_pin;\
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
				GPIO_InitStruct.Pull = GPIO_NOPULL;\
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
				HAL_GPIO_Init(em_port, &GPIO_InitStruct);\
			}\
			HAL_GPIO_WritePin(em_port,em_pin,(GPIO_PinState)(value));    \
			break;                                                     \
		}                                                              \
		case PIN_ID_ENABLE:                                                 \
		{                                                              \
			if(is_en_init <= 0)\
			{\
				is_en_init=1;\
				GPIO_InitTypeDef GPIO_InitStruct;\
				__HAL_RCC_##en_port##_CLK_ENABLE();\
				GPIO_InitStruct.Pin = en_pin;\
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
				GPIO_InitStruct.Pull = GPIO_NOPULL;\
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
				HAL_GPIO_Init(en_port, &GPIO_InitStruct);\
			}\
			HAL_GPIO_WritePin(en_port,en_pin,(GPIO_PinState)(value));    \
			break;                                                     \
		} \
		case PIN_ID_REFL:                                                 \
		{                                                              \
			if(is_refl_init <= 0)\
			{\
				is_refl_init=1;\
				GPIO_InitTypeDef GPIO_InitStruct;\
				__HAL_RCC_##refl_port##_CLK_ENABLE();\
				GPIO_InitStruct.Pin = refl_pin;\
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
				GPIO_InitStruct.Pull = GPIO_NOPULL;\
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
				HAL_GPIO_Init(refl_port, &GPIO_InitStruct);\
			}\
			HAL_GPIO_WritePin(refl_port,refl_pin,(GPIO_PinState)(value));    \
			break;                                                     \
		}\
		case PIN_ID_REFR:                                                 \
		{                                                              \
			if(is_refr_init <= 0)\
			{\
				is_refr_init=1;\
				GPIO_InitTypeDef GPIO_InitStruct;\
				__HAL_RCC_##refr_port##_CLK_ENABLE();\
				GPIO_InitStruct.Pin = refr_pin;\
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
				GPIO_InitStruct.Pull = GPIO_NOPULL;\
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;\
				HAL_GPIO_Init(refr_port, &GPIO_InitStruct);\
			}\
			HAL_GPIO_WritePin(refr_port,refr_pin,(GPIO_PinState)(value));    \
			break;                                                     \
		}\
	 default:break;\
	}\
}
	

#define  MOTOR_STATE_MACHINE(idx)           \
static DEFINESTATUS motor##idx##_state_machine(MOTOR *ths)    \
{ \
	switch(ths->status)  \
	{   \
		case MOTOR_STATUS_RESET_START:  \
		{   \
			ths->c_postion=0;\
			ths->ZeorTouch=0;  \
			if(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) != ths->mot_cfg->zero_trig_vol)  \
			{  \
				printf("zero no triger\r\n");\
				mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->re_mico,ths->re_current); \
				if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_TOUCH_SEM,GetTimeOut(ths->speed,ths->mot_cfg->max_cordinate)))\
				{\
					printf("1 reset time out.....\r\n");\
					return MOTOR_STATUS_ERROR_TIME_OUT;\
				}\
			}\
			else \
			{\
				printf("zero triger\r\n");\
				mTMC5130DevicRotate(ths,ths->speed,ths->acc,DRI_REVERAL(ths->dir),ths->re_mico,ths->re_current);\
				while(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol);\
				mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->mot_cfg->rs_cordinate,DRI_REVERAL(ths->dir),ths->re_mico,ths->re_current);\
				if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,GetTimeOut(ths->speed/4,ths->mot_cfg->rs_cordinate * 2)))\
				{\
					printf("2 reset time out.....\r\n");\
					return MOTOR_STATUS_ERROR_TIME_OUT;\
				}\
				mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->re_mico,ths->re_current); \
				if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_TOUCH_SEM,GetTimeOut(ths->speed/4,ths->mot_cfg->rs_cordinate * 2)))\
				{\
					printf("3 reset time out.....\r\n");\
					return MOTOR_STATUS_ERROR_TIME_OUT;\
				}\
			}\
			ths->ZeorTouch=mTMC5130DeviceGetPostion(ths,0);\
			ths->status=MOTOR_STATUS_RESET_WAIT_SEM;\
		};\
		case MOTOR_STATUS_RESET_WAIT_SEM:\
		{printf("MOTOR_STATUS_RESET_WAIT_SEM\r\n");\
			mTMC5130DevicMoveby(ths,ths->speed/4,ths->acc/4,ths->mot_cfg->rs_cordinate,ths->dir,ths->re_mico,ths->re_current) ;\
			if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,GetTimeOut(ths->speed/6,ths->mot_cfg->zero_cordinate)))\
			{\
				printf("4 reset time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			ths->ZeroBrake=mTMC5130DeviceGetPostion(ths,0);\
			ths->status=MOTOR_STATUS_RESET_WAIT_REACH;\
		};\
		case MOTOR_STATUS_RESET_WAIT_REACH:\
		{printf("MOTOR_STATUS_RESET_WAIT_REACH\r\n");\
			ths->BrakeStep=abs(ths->ZeroBrake - ths->ZeorTouch);\
			printf("ZeorTouch positon= %d\r\n",ths->ZeorTouch);	\
			printf("ZeroBrake= %d\r\n",ths->ZeroBrake);\
			if(abs(ths->BrakeStep) > ths->mot_cfg->rs_cordinate)\
			printf("Brake step distance= %d zero_cordinate= %d\r\n",ths->BrakeStep,ths->mot_cfg->zero_cordinate);  \
			/*printf("The Braking deviation= %x\r\n",abs(abs(ths->BrakeStep) - ths->mot_cfg->zero_cordinate));*/  \
 			if(abs(abs(ths->BrakeStep) - ths->mot_cfg->rs_cordinate) >= 20*(ths->micostep/16))\
			{\
				printf("The Braking distance is too large %d\r\n",abs(abs(ths->BrakeStep) - ths->mot_cfg->rs_cordinate));\
				mTMC5130DevicMoveby(ths,ths->speed/4,ths->acc/4,abs(abs(ths->BrakeStep) - ths->mot_cfg->rs_cordinate),\
													abs(ths->BrakeStep) > ths->mot_cfg->rs_cordinate?DRI_REVERAL(ths->dir):ths->dir,ths->re_mico,ths->re_current) ;\
				if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,GetTimeOut(ths->speed/4,abs(abs(ths->BrakeStep) - ths->mot_cfg->zero_cordinate))))\
				{\
					/*printf("5 reset time out.....\r\n");*/ \
					return MOTOR_STATUS_ERROR_TIME_OUT;\
				}\
				ths->ZeroBrake=mTMC5130DeviceGetPostion(ths,0);\
				ths->BrakeStep=abs(ths->ZeroBrake - ths->ZeorTouch);\
			}\
			ths->status=MOTOR_STATUS_RESET_FINISH;\
			mTMC5130DeviceResetPostion(ths);\
		}break;\
		case MOTOR_STATUS_ZERO_START:\
		{\
			\
			mTMC5130DevicMoveto(ths,ths->speed,ths->acc,0,ths->dir,ths->re_mico,ths->re_current); \
			if( (MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem( ths,WAIT_TOUCH_SEM,GetTimeOut(ths->speed,abs(mTMC5130DeviceGetPostion(ths,0))) + 2000 )) )\
			{\
				printf("1 back zero time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			ths->ZeorTouch=mTMC5130DeviceGetPostion(ths,0);\
			if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,GetTimeOut(ths->speed,ths->mot_cfg->zero_cordinate)+500))\
			{\
				printf("2 back zero time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			ths->Zero_drift=ths->mot_cfg->zero_cordinate - abs(ths->ZeorTouch);\
			/*ths->Zero_drift= ths->mot_cfg->dir_vol ? 0-result:result;*/\
			ths->status=MOTOR_STATUS_ZERO_FINISH;\
		}break;\
		case MOTOR_STATUS_LISTEN_IO_START:\
		{\
			ths->Listen_state=HAL_GPIO_ReadPin((GPIO_TypeDef *)ths->Listen_port,1<<ths->Listen_pin);\
			switch(ths->Listen_type)\
			{\
				case 0:if(ths->Listen_state == 0) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;\
				case 1:if(ths->Listen_state == 1) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;\
				case 2:if(ths->Listen_old_state == 1 && ths->Listen_state==0) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;\
				case 3:if(ths->Listen_old_state == 0 && ths->Listen_state==1) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;\
			}\
			ths->Listen_old_state=ths->Listen_state;\
			break;\
		}\
		case MOTOR_STATUS_ERROR_TIME_OUT:printf("thread %d time out...\r\n",ths->id);break;\
		default:break;\
	}	\
	return ths->status;\
}\


#define  MOTOR_TMC_RESET(idx)           \
static int motor##idx##_reset(MOTOR* ths)  \
{ \
	ths->is_busy = 1;/*表示电机在复位 */\
	if(TURN_TABLE)\
	{\
		ths->mot_cfg->rs_cordinate = 120-87;/*根据SP转盘杯子的位置人为调节，使各针能对准杯子*/\
	}\
	else \
	{\
		ths->mot_cfg->rs_cordinate = ths->mot_cfg->zero_cordinate;\
	}\
	DEFINESTATUS result=MOTOR_STATUS_NO_ERROR;\
	ths->Zero_drift=0;\
	ths->ZeorTouch=0;\
	ths->ZeroBrake=0;\
	ths->BrakeStep=0;\
	ths->status=MOTOR_STATUS_RESET_START;\
	ths->dir= ths->mot_cfg->dir_vol ? ths->mot_cfg->dir_vol:DRI_REVERAL(ths->mot_cfg->dir_vol);\
	if(ths->mot_cfg->max_cordinate < 87430) ths->mot_cfg->max_cordinate = 87430;\
	ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,0);\
	ths->speed=ths->speed/2;\
	ths->acc=ths->speed*2;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) break;\
	}while(result != MOTOR_STATUS_RESET_FINISH);\
	if(result == MOTOR_STATUS_ERROR_TIME_OUT)\
	{\
		mTMC5130DevicStop(ths,STOP_AMAX);\
		ths->status = MOTOR_STATUS_ERROR_TIME_OUT;\
		printf("reset time out.......\r\n");\
	}\
	else \
	{\
		if(!TURN_TABLE )   \
		{ \
			if((abs(ths->BrakeStep) - ths->mot_cfg->zero_cordinate ) <= 25*(ths->micostep/16))\
			{\
				ths->status=MOTOR_STATUS_NO_ERROR;\
				printf("1the motor %d reset successful....\r\n",ths->id);\
			}\
			else \
			{ \
				ths->status=MOTOR_STATUS_RESET_FAIL;\
				printf("1the motor %d reset fail....\r\n",ths->id);\
			} \
		}\
		else \
		{ \
			if ( TURN_TABLE && ((abs(ths->BrakeStep) - ths->mot_cfg->rs_cordinate ) >= 20*(ths->micostep/16)) ) \
			{ \
				ths->status=MOTOR_STATUS_NO_ERROR;\
				printf("2the motor %d reset successful....\r\n",ths->id);\
			} \
			else \
			{ \
				ths->status=MOTOR_STATUS_RESET_FAIL;\
				printf("2the motor %d reset fail....\r\n",ths->id);\
			} \
		} \
	}ths->Zero_drift=0;\
	ths->is_busy = 0;/*表示电机空闲 */\
	return ths->status;\
}


#define  MOTOR_TMC_BACKZERO(idx)           \
static int motor##idx##_backzeor(MOTOR* ths)  \
{	printf("motor%d back_zero\r\n",ths->id);\
	u8 err=0;\
	DEFINESTATUS result=MOTOR_STATUS_NO_ERROR;\
	ths->status=MOTOR_STATUS_ZERO_START;\
	ths->dir=ths->mot_cfg->dir_vol;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);printf("2ths->ZeorTouch=%d  Zero_drift=%d\r\n",ths->ZeorTouch,ths->Zero_drift);\
		/*if(result == MOTOR_STATUS_ZERO_FINISH) { ths->status=MOTOR_STATUS_NO_ERROR; return ths->status; }*/\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) break;\
	}while(result != MOTOR_STATUS_ZERO_FINISH);\
	if(result == MOTOR_STATUS_ERROR_TIME_OUT)\
	{\
		mTMC5130DevicStop(ths,STOP_AMAX);\
		ths->status=MOTOR_STATUS_ERROR_TIME_OUT;\
		/*if( abs(mTMC5130DeviceGetPostion(ths,0)) == 0 )*/\
		if(ths->wait_sem != NULL)	\
		{\
			OSSemDel(ths->wait_sem,OS_DEL_ALWAYS,&err);\
			ths->wait_sem=NULL;\
		}\
	}\
	else \
	{\
		while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),POSTION_READHED) != 1) ;\
		if(abs(ths->Zero_drift) <= ths->mot_cfg->lost_endure_continue)\
		{/*由于目前的失步补偿不能够清零寄存器，只能通过多走/少走的方式进行补偿，也就是加减寄存器里的数值*/\
			if( abs(mTMC5130DeviceGetPostion(ths,0))<= ths->mot_cfg->lost_endure_continue )\
			{\
				printf("back zero successful.......\r\n");\
			}\
			else \
			{\
				ths->mTMC5130MotorReset(ths);\
				printf("lost_endure_continue\r\n");\
			}\
			ths->status=MOTOR_STATUS_NO_ERROR;\
		}\
		else if(abs(ths->Zero_drift) >= ths->mot_cfg->lost_endure_stop)\
		{\
			printf("lost_endure_stop\r\n");\
			ths->status=MOTOR_STATUS_ZERO_FAIL;\
		}\
	}printf("ths->status %d\r\n",ths->status);\
	if( abs(ths->Zero_drift) >= ths->mot_cfg->lost_endure_stop )\
	{\
		ths->status=MOTOR_STATUS_ZERO_FAIL;\
		printf("MOTOR_STATUS_ZERO_FAIL\r\n");\
	}\
	if(ths->status == MOTOR_STATUS_ZERO_FINISH)\
		ths->status=MOTOR_STATUS_NO_ERROR;\
	return ths->status;\
}


#define MOTOR_TMC_LISTEN_IO(idx)  \
static int motor##idx##_listen_io(MOTOR* ths)  \
{   int time=0;\
	DEFINESTATUS result=MOTOR_STATUS_NO_ERROR;\
	ths->status=MOTOR_STATUS_LISTEN_IO_START;\
	ths->Listen_old_state=0;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		OSTimeDlyHMSM(0,0,0,20);\
		if(++time > 900)/*18秒之内没有监听到就退出*/\
		{\
			result=MOTOR_STATUS_LISTEN_IO_FAIL;\
		}\
	}while(result != MOTOR_STATUS_LISTEN_IO_FINISH && result != MOTOR_STATUS_LISTEN_IO_FAIL);\
	if(result == MOTOR_STATUS_LISTEN_IO_FINISH)\
	{\
		mTMC5130DevicStop(ths,STOP_AMAX);\
		while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1) ;\
		ths->status=MOTOR_STATUS_NO_ERROR;\
	}\
	else \
	{\
		while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1) ;\
		ths->status=result;\
	}\
	return ths->status;\
}


#define MOTOR_TMC_INIT(midx) \
mTMC5130Motor_Init( &motors[midx],\
					&motors_cfg[midx],\
					&driver_reg[midx],\
					&m##midx##_io_write,\
					&m##midx##_io_read,\
					&motor##midx##_state_machine,\
					&motor##midx##_backzeor,\
					&motor##midx##_reset,\
					&motor##midx##_listen_io,\
					&mTMC5130Motor_Move,\
					&mTMC5130Motor_Rotate,\
					&mTMC5130Motor_GetStatus,\
					&mTMC5130Motor_Stop,\
					&mTMC5130Motor_MultiMove,\
					&mTMC5130Motor_MoveGPIOEvent\
					)

int mTMC5130DevicMotorStopbyIo(MOTOR*);
void mTMC5130DevicStop(MOTOR*,int);
void mTMC5130DevicRotate(MOTOR*,int,int,char,int,char);
void mTMC5130DevicMoveto(MOTOR*,int,int,int,char,int,char);
void mTMC5130DevicMoveby(MOTOR* ,int ,int ,int ,char ,int ,char );//相对运动
void mTMC5130DeviceOverZeroMove(MOTOR* ,int ,int ,int ,char ,int,char);

int mTMC5130DeviceGetRAMPSTAT(MOTOR *);
int mTMC5130DeviceGetDRVSTATUS(MOTOR *);
char mTMC5130DeviceGetState(MOTOR* );
int  mTMC5130DeviceGetPostion(MOTOR* ,char );
void mTMC5130DeviceResetPostion(MOTOR* );
int mTMC5130DeviceReadWriteRegister(MOTOR* ,char ,char ,int );//获取寄存器信息
void mTMC5130DeviceSetMicoStep(MOTOR* ,short );
void mTMC5130DeviceSetCurrent(MOTOR* ,char );
void mTMC5130DeviceOscillation(MOTOR* ,int,int,int,int);
void mTMC5130DeviceSetDefaultRegister(MOTOR *,char); //配置5130寄存器参数
void mTMC5130DeviceResetStopSpeed(MOTOR* ths);
void mTMC5130DeviceSetGCONF(MOTOR *,char );

int mTMC5130DevicWait_Sem(MOTOR* ,char ,unsigned int );
void mTMC5130ProcessSemPost(MOTOR *,int );
char Get_ISR_ID(u32 pin);
unsigned int GetTimeOut(int ,int );//计算超时 时间

void mTMC5130Device_Init(MOTOR* );


int mTMC5130Motor_Move(MOTOR* ths);
int mTMC5130Motor_Rotate(MOTOR* ths);
int mTMC5130Motor_GetStatus(MOTOR* ths);
int mTMC5130Motor_Stop(MOTOR* ths);
int mTMC5130Motor_MultiMove(int cnt,motor_multi_move_args_t *margs,bool is_move_to_zero);
int mTMC5130Motor_MoveGPIOEvent(MOTOR* ths,unsigned int*);

void mTMC5130Motor_Init(MOTOR *ths,
						motor_cfg_struct * cfg,\
						DRIVER_REG *reg,\
						void (*write_io)(pin_id_t ,int ),\
						int  (*read_io) (pin_id_t ,int,char),\
						DEFINESTATUS (*motor_state_machine)(MOTOR *),\
						int (*motor_backzeor)(MOTOR *),\
						int (*motor_reset)(MOTOR *),\
						int (*motor_listen_io)(MOTOR *),\
						int (*Move)(MOTOR* ths),\
						int (*Rotate)(MOTOR* ths),\
						int (*GetStatus)(MOTOR* ths),\
						int (*Stop)(MOTOR* ths),\
						int (*MultiMove)(int,motor_multi_move_args_t *,bool ),\
						int (*MoveGPIOEvent)(MOTOR* ,unsigned int*)\
						);				
#endif

