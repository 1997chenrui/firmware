#ifndef MOTOR__H__
#define MOTOR__H__
#include "stm32f4xx.h"
#include <stdbool.h>
#include "mTMC5130Driver.h"
#include "os_implement.h"
#include "macro.h"

#define GET_STATE_BIT(s,n)   ((s>>n)&0x1)    //获取字节中的位  用来判断电机状态寄存器
#define DRI_REVERAL(dri)  ((dri == 0) ? 1:0)   ///< 方向反转

#define STOP_AMAX         65535  ///<停止时的最大加速度
//#define HOLD_CURRENT   3  ///<  HOLD住电机的电流  
#define TCOOL_THRS(n)  (n/32>0 ? n/32: -(int)(n/32))  ///< 计算TCOOL值
#define SGT(n)	        (n * 1.1)      ///< 计算SGT


#define WAIT_NO_SEM   0    //电机不处等待信号状态
#define WAIT_REACH_SEM   1    //等侍位置信号量到达
#define WAIT_TOUCH_SEM    2 //等侍光耦被触发信号量
#define WAIT_REACH_FLG   3  //等侍标志组的信号  用在多电机同时运行中 
#define WAIT_STOP_SEM 4 //等待电机停止信号量
#define WAIT_VELOCITY_SEM 5 //等待电机速度到达信号量
#define WAIT_ERROR_SEM 6 //等侍错误报警信号量


typedef enum 
{
	MOTOR_STATUS_STOP=0,
	MOTOR_STATUS_ERROR_TIME_OUT,
	MOTOR_STATUS_ERROR_MOTORABORT,
	MOTOR_STATUS_ZERO_FAIL,
	MOTOR_STATUS_LISTEN_IO_FAIL,
	MOTOR_STATUS_RESET_FAIL,  
	MOTOR_STATUS_INIT_FAIL, 
	
	MOTOR_STATUS_RESET_START_QUICK,
	MOTOR_STATUS_RESET_START,
	MOTOR_STATUS_RESET_FINISH,

	MOTOR_STATUS_ZERO_START,
	MOTOR_STATUS_ZERO_START_QUICK,
	MOTOR_STATUS_ZERO_START_QUICK_2,
	MOTOR_STATUS_ZERO_START_THROD,
	MOTOR_STATUS_ZERO_FINISH,

	MOTOR_STATUS_LISTEN_IO_START,   	
	MOTOR_STATUS_LISTEN_IO_FINISH,
	
	MOTOR_STATUS_MOVE_FINISH,
	MOTOR_STATUS_RORATE_FINISH,
	MOTOR_STATUS_STOP_FINISH,
	MOTOR_STATUS_RUNING,
	MOTOR_STATUS_INIT_FINISH,
    
    MOTOR_STATUS_LISTEN_STOP_EVENT_START,
    MOTOR_STATUS_LISTEN_STOP_EVENT_STOP
}DEFINESTATUS;

typedef enum {
    MOTOR_NO_ERROR = 0,
    MOTOR_ERROR_LOST_AUTO_LISTEN,
	MOTOR_ERROR_LOST,
	MOTOR_ERROR_RESET_TIME_OUT,
	MOTOR_ERROR_LISTEN_EVENT_NO_TRIGGER,
	MOTOR_ERROR_STOP_TIME_OUT,
	MOTOR_ERROR_UNKNOW
}Motor_errcode_e;

typedef struct _Motor_cfg
{
	uint32_t             zero_cordinate;        	//零位距离
	uint32_t             max_cordinate;         	//最大行程
	uint32_t             lost_endure_continue;  	//失步忍受,继续运动,返回时通知上位机
	uint32_t			 lost_endure_stop;			//失步忍受,停止  报错		
	uint16_t		     subdivision;     //细分
	uint8_t              currentLevel;      //运行电流
	uint8_t              holdLevel;
	uint8_t              zero_trig_vol  : 1;		//零位触发电压
	uint8_t              dir_vol  : 1;				//正方向电压
	uint8_t              enb_vol  : 1;             //使能电平
	uint8_t              half_vol  : 2;             //半流电平
	uint8_t              enable_zero_makeup  : 1;		//开启零位补偿, 非回零时, 越过零位均计算补偿量, 若开机会补偿同时若失步量大则直接报错
	uint8_t							 back_zero_speed_mode:1; // 0:位置模式下回零支持快速回零1:速度模式下回零,与复位一致, 速度不能很快.
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

typedef struct _Motor_seq_args
{
	struct _Motor_args* args;
	u32 args_len;
	u32 m2_delayms;
}motor_seq_move_args_t;

typedef struct _Motor
{
	u8     id;
	u8     dir;             //方向
	u8     is_busy;		
	u8     i_hold;  //静止电流  锁住电机时电流
	u8     i_run;           //运行电流
	u8     is_abort;
	u8     Listen_type;
	u8     Listen_state;
	u8     Listen_old_state; 
	u8     zeor_isr_id;//零位 光耦触发中断号
	u8     reach_isr_id;//目标位置到达触发中断号
	u8     error_isr_id;//芯片错误检测中断号
	u8     wait_type;//等待类开型
	u8     evnt_group;//事件组
	u8     isBackZero;//是否回零
	u8     isRelative;//是相对运动吗
	volatile u8     isReset;//是否是复位
	volatile u8     is_reach_target;//位置检测信号是否触发	
    volatile u8 stop_event_trigger;
	u16    micostep;//细分
	s32    throld;
	u32    Listen_pin;	
	u32    speed;
	u32    acc;
	u32    reset_speed;
	u32    reset_acc;
	s32    c_postion;
	u32    postion;	//步距
    volatile int ZeorTouch;//零位(光耦)触发位置
	int    BrakeStep; //刹车距离
	int    ZeroBrake;//废弃 最终停止位置
	u16    backzero_position; // 废弃 回零时上位机设置的位置
	volatile int Zero_drift; // 零位漂移 在回零时不再使用此参数做运动补偿, 但是在运动过程中若触发了零位可用来做运动补偿

    
	u16    RAMPSTAT;
	u32    DRVSTATUS;
	u8     DRVSTATE;

	GPIO_TypeDef *Listen_port;
	DEFINESTATUS status;	            //电机的运行状态
	OS_EVENT * wait_sem;  

	motor_cfg_struct  *mot_cfg;   //电要参数配置表
	DRIVER_REG  *drvie_reg;
	
	void (*write_io)(pin_id_t id,int value);
	int  (*read_io) (pin_id_t id,int,char);	
    u8  (*read_event) (u8 id);	

	DEFINESTATUS (*mTMC5130Motor_State_Machine)(struct _Motor *);

	int (*mBackZero)(struct _Motor *);	
	int (*mListenIo)(struct _Motor *);	
    int (*mListenEvent)(struct _Motor *);	
	int (*mReset)(struct _Motor *);

	int (*mTMC5130MotorReset)(struct _Motor *);	
	int (*mTMC5130MotorMove)(struct _Motor *);
	int (*mTMC5130MotorRotate)(struct _Motor *);
	int (*mTMC5130MotorGetStatus)(struct _Motor *);
	int (*mTMC5130MotorStop)(struct _Motor *);
	int (*mTMC5130Motor_MultiMove)(int ,motor_multi_move_args_t *,bool);
	int (*mTMC5130Motor_SeqMove)(int cnt, motor_seq_move_args_t *args);
	int (*mTMC5130MotorMoveGPIOEvent)(struct _Motor *,int* );
    int (*mTMC5130MotorMoveStopEvent)(struct _Motor *,int* );
	int (*mTMC5130MotorResetByMethod2)(struct _Motor *);
}MOTOR;


static bool wait_zero_no_trig(MOTOR* ths, int span, int timeout)
{
    int cur = 0; 
    while(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol)
    {
        bus_os_sleep(span);
        cur+=span;
        if(cur > timeout)
        {
            return 1;
        }
    }
    return 0;
}

static void hal_rcc_clk_enable(GPIO_TypeDef  *GPIOx){
    if(GPIOx == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }else if(GPIOx == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }else if(GPIOx == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }else if(GPIOx == GPIOD){
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }else if(GPIOx == GPIOE){
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }else if(GPIOx == GPIOF){
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }else if(GPIOx == GPIOG){
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }else {
        ASSERT(1, "__HAL_RCC_GPIOG_CLK_ENABLE GPIOx Error");
    }
}

#define MOTOR_IO_RD_DECLARE(midx, t_port, t_pin, r_port, r_pin, e_port, e_pin)\
static int m##midx##_io_read(pin_id_t id, int isr_no, char flg)\
{\
	int result=0;\
	switch(id)\
	{\
		case PIN_ID_SET_SENSOR:\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			hal_rcc_clk_enable(t_port);\
			GPIO_InitStruct.Pin = t_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;\
			GPIO_InitStruct.Pull = GPIO_NOPULL;\
			HAL_GPIO_Init(t_port, &GPIO_InitStruct);\
			/*if(flg==0)HAL_NVIC_DisableIRQ((IRQn_Type)isr_no);*/\
			result=mGPIOReadPin(t_port,t_pin);\
		}break;\
        case PIN_ID_SET_REACH_SENSOR:\
        {\
            GPIO_InitTypeDef GPIO_InitStruct;\
			hal_rcc_clk_enable(r_port);\
			GPIO_InitStruct.Pin = r_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;\
			GPIO_InitStruct.Pull = GPIO_NOPULL;\
			HAL_GPIO_Init(r_port, &GPIO_InitStruct);\
			/*if(flg==0)HAL_NVIC_DisableIRQ((IRQn_Type)isr_no);*/\
			result=mGPIOReadPin(r_port,r_pin);\
        }break;\
		case PIN_ID_SET_TOUCH_ISR:/*光耦触发中断*/\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			hal_rcc_clk_enable(t_port);\
			GPIO_InitStruct.Pin = t_pin;\
			GPIO_InitStruct.Mode = (flg == 0) ?GPIO_MODE_IT_FALLING:GPIO_MODE_IT_RISING;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			HAL_GPIO_Init(t_port, &GPIO_InitStruct);\
			HAL_NVIC_SetPriority((IRQn_Type)isr_no , 0, 0);\
			__HAL_GPIO_EXTI_CLEAR_IT(t_pin);/*wangzhen*/\
			HAL_NVIC_EnableIRQ((IRQn_Type)isr_no );\
		}break;\
		case PIN_ID_SET_REACH_ISR:/*位置到达中断*/\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			hal_rcc_clk_enable(r_port);\
			GPIO_InitStruct.Pin = r_pin;\
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;\
			GPIO_InitStruct.Pull = GPIO_PULLUP;\
			HAL_GPIO_Init(r_port, &GPIO_InitStruct);\
			HAL_NVIC_SetPriority((IRQn_Type)isr_no , 0, 0);\
			if(flg==0) __HAL_GPIO_EXTI_CLEAR_IT(r_pin);/*wangzhen*/\
			HAL_NVIC_EnableIRQ((IRQn_Type)isr_no );\
		}break;\
		case PIN_ID_SET_ERROR_ISR:\
		{\
			GPIO_InitTypeDef GPIO_InitStruct;\
			hal_rcc_clk_enable(e_port);\
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
		case READ_PIN_ID_SENSOR:result=mGPIOReadPin(t_port,t_pin);break;\
		case READ_PIN_ID_ERROR_ISR:result=mGPIOReadPin(e_port,e_pin);break;\
		case READ_PIN_ID_TOUCH_ISR:result=mGPIOReadPin(t_port,t_pin);break;\
		case READ_PIN_ID_REACH_ISR:result=mGPIOReadPin(r_port,r_pin);break;\
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
                hal_rcc_clk_enable(cs_port);\
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
				hal_rcc_clk_enable(em_port);\
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
				hal_rcc_clk_enable(en_port);\
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
				hal_rcc_clk_enable(refl_port);\
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
				hal_rcc_clk_enable(refr_port);\
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
	
#define TIME_OUT_RESET(coordinate,acc,speed) timeOut_Reset(coordinate,acc,speed) 
#define  MOTOR_STATE_MACHINE(idx)           \
static DEFINESTATUS motor##idx##_state_machine(MOTOR *ths)    \
{ \
	switch(ths->status)  \
	{   \
		case MOTOR_STATUS_RESET_START_QUICK:/*快速复位*/ \
		{ \
			int coordinate;\
			if(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol)  \
			{\
				/*向光耦不触发的方向运动*/\
				mTMC5130DevicRotate(ths,ths->speed/5,ths->acc/5,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run);\
				if(wait_zero_no_trig(ths, 200, TIME_OUT_RESET(ths->mot_cfg->max_cordinate,ths->acc/5,ths->speed/5)) != 0)\
                {                                                                                   \
                    return MOTOR_STATUS_ERROR_TIME_OUT;                                             \
                }                                                                                   \
				/*以零位距离减速下来*/\
				mTMC5130DevicMoveby(ths,ths->speed/10,ths->acc/10,ths->mot_cfg->lost_endure_continue,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run);\
				if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0))\
				{\
					return MOTOR_STATUS_ERROR_TIME_OUT;\
				}\
				coordinate = ths->mot_cfg->lost_endure_continue;\
			} else \
			{\
				coordinate=ths->mot_cfg->max_cordinate;\
			}\
			/*向零位触发方向运动*/\
			mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->micostep,ths->i_run); \
			if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC5130DevicWait_Sem(ths,WAIT_TOUCH_SEM,TIME_OUT_RESET(coordinate,ths->acc,ths->speed)))\
			{\
				printf("1 back zero time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			/*零位触发后停止*/\
			mTMC5130DevicStop(ths,ths->reset_acc);\
			while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1) OSTimeDlyHMSM(0,0,0,300);\
			ths->BrakeStep=abs(ths->ZeorTouch - mTMC5130DeviceGetPostion(ths,0));\
			printf("reset zero touch = %d\r\n", ths->ZeorTouch);\
			printf("BrakeStep%d\r\n", ths->BrakeStep);\
            /*bus_os_sleep(5000);*/\
            /*在零位停下*/\
            /*if(ths->BrakeStep != 0) mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->BrakeStep,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run);*/\
			/*ths->BrakeStep=0;最终停止距离相对光耦的位置 此处修改原因, 若在移动过程中发生严重失步, 会将Zero_drift设置为错误的值,导致后面已连续的失败*/\
			ths->write_io(PIN_ID_REFR,0);\
			coordinate = ths->mot_cfg->zero_cordinate - ths->BrakeStep;\
			mTMC5130DeviceSetPostion(ths,ths->mot_cfg->dir_vol==1? (0-coordinate):coordinate);\
			ths->write_io(PIN_ID_REFR,1);\
			printf("current：%d\r\n", ths->mot_cfg->dir_vol==1? 0-coordinate:coordinate);\
			/*ths->Zero_drift=ths->BrakeStep;*/\
			ths->status=MOTOR_STATUS_RESET_FINISH;\
		}break;\
		case MOTOR_STATUS_LISTEN_IO_START:/*监测IO*/\
		{\
			if(ths->is_reach_target == 1) \
			{\
				printf("1 listen io time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			ths->Listen_state=mGPIOReadPin((GPIO_TypeDef *)ths->Listen_port,1<<ths->Listen_pin);\
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
        case MOTOR_STATUS_LISTEN_STOP_EVENT_START:/*监测IO*/\
		{\
            if(ths->is_reach_target == 1) \
			{\
				printf("1 listen io time out.....\r\n");\
				return MOTOR_STATUS_ERROR_TIME_OUT;\
			}\
			if(ths->read_event(ths->Listen_type) == 1)\
			{\
				ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;\
			}\
			break;\
		}\
		case MOTOR_STATUS_ERROR_TIME_OUT:printf("thread %d time out...\r\n",ths->id);break;\
		default:break;\
	}\
	return ths->status;\
}\


#define  MOTOR_TMC_RESET(idx)           \
static int motor##idx##_reset(MOTOR* ths)  \
{ \
	DEFINESTATUS result=MOTOR_STATUS_STOP;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) break;\
	}while(result != MOTOR_STATUS_RESET_FINISH);\
	return result;\
}


#define  MOTOR_TMC_BACKZERO(idx)           \
static int motor##idx##_backzeor(MOTOR* ths)  \
{\
	DEFINESTATUS result=MOTOR_STATUS_STOP;\
	/*ths->status=MOTOR_STATUS_ZERO_START;*/ \
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) break;\
	}while(result != MOTOR_STATUS_ZERO_FINISH);\
	return result;\
}


#define MOTOR_TMC_LISTEN_EVENT(idx)  \
static int motor##idx##_listen_event(MOTOR* ths)  \
{   \
	DEFINESTATUS result=MOTOR_STATUS_STOP;\
	ths->status=MOTOR_STATUS_LISTEN_STOP_EVENT_START;\
	ths->stop_event_trigger=0;\
    ths->read_io(PIN_ID_SET_REACH_ISR,ths->reach_isr_id,0);\
    ths->is_reach_target=0;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		OSTimeDlyHMSM(0,0,0,2);\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT || result == MOTOR_STATUS_LISTEN_IO_FAIL) break;\
	}while(result != MOTOR_STATUS_LISTEN_IO_FINISH);\
	return result;\
}

#define MOTOR_TMC_LISTEN_IO(idx)  \
static int motor##idx##_listen_io(MOTOR* ths)  \
{   \
	DEFINESTATUS result=MOTOR_STATUS_STOP;\
	ths->status=MOTOR_STATUS_LISTEN_IO_START;\
	ths->Listen_old_state=mGPIOReadPin((GPIO_TypeDef *)ths->Listen_port,1<<ths->Listen_pin);\
	ths->read_io(PIN_ID_SET_REACH_ISR,ths->reach_isr_id,0);\
	ths->is_reach_target=0;\
	do\
	{\
		result=ths->mTMC5130Motor_State_Machine(ths);\
		OSTimeDlyHMSM(0,0,0,2);\
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) break;\
	}while(result != MOTOR_STATUS_LISTEN_IO_FINISH);\
	return result;\
}


#define MOTOR_TMC_INIT(midx) \
mTMC5130Motor_Init(midx,	\
					&motors[midx],\
					&motors_cfg[midx],\
					&motors_reg[midx],\
					&m##midx##_io_write,\
					&m##midx##_io_read,\
                    &m##midx##_event_read,\
					&motor##midx##_state_machine,\
					&motor##midx##_backzeor,\
					&motor##midx##_reset,\
					&motor##midx##_listen_io,\
                    &motor##midx##_listen_event,\
					&mTMC5130Motor_Move,\
					&mTMC5130Motor_Rotate,\
					&mTMC5130Motor_GetStatus,\
					&mTMC5130Motor_Stop,\
					&mTMC5130Motor_MultiMove,\
					&mTMC5130Motor_SeqMove,\
					&mTMC5130Motor_MoveGPIOEvent,\
                    &mTMC5130Motor_MoveStopEvent,\
					&mTMC5130Motor_Reset\
					)

GPIO_PinState mGPIOReadPin(GPIO_TypeDef* port, uint16_t pin);
void Delay_Nus(unsigned int nus);
char mTMC5130DevicStop(MOTOR*,int);
char mTMC5130DevicRotate(MOTOR*,int,int,char,int,char);
char mTMC5130DevicMoveto(MOTOR*,int,int,int,char,int,char);
char mTMC5130DevicMoveby(MOTOR* ,int ,int ,int ,char ,int ,char );//相对运动

char mTMC5130Device_GetPara(MOTOR*  );
int mTMC5130DeviceGetRAMPSTAT(MOTOR * );
int mTMC5130DeviceGetDRVSTATUS(MOTOR * );
int mTMC5130DeviceSETSWMODE(MOTOR *,int );
char mTMC5130DeviceGetState(MOTOR* );
int  mTMC5130DeviceGetPostion(MOTOR* ,char );
int  mTMC5130DeviceGetPostionIf(MOTOR* ,char, bool );
int mTMC5130DeviceGetSpeed(MOTOR*  );
void mTMC5130DeviceResetPostion(MOTOR* );
int mTMC5130DeviceReadWriteRegister(MOTOR* ,char ,char ,int );//获取寄存器信息
int mTMC5130Motor_EmergencyStop(MOTOR* ths, bool flag);
void mTMC5130DeviceSetPostion(MOTOR* ,int );//清零位置寄存器  使之等于零;

int mTMC5130DevicWait_Sem(MOTOR* ,char ,unsigned int );
void mTMC5130ProcessSemPost(MOTOR *,int );
char Get_ISR_ID(u32 pin);
unsigned int GetTimeOut(int ,int );//计算超时 时间
unsigned int timeOut_Reset(u32 coordinate, u32 acce, u32 speed);

void mTMC5130Device_Init(MOTOR* );
void mTMC5130Device_SetDefault(MOTOR*  ); //配置
int mTMC5130Motor_Reset(MOTOR* ths) ;
int mTMC5130Motor_Move(MOTOR* ths);
int mTMC5130Motor_Rotate(MOTOR* ths);
int mTMC5130Motor_GetStatus(MOTOR* ths);
int mTMC5130Motor_Stop(MOTOR* ths);
int mTMC5130Motor_MultiMove(int cnt,motor_multi_move_args_t *margs,bool is_move_to_zero);
int mTMC5130Motor_SeqMove(int cnt, motor_seq_move_args_t *args);
int mTMC5130Motor_MoveGPIOEvent(MOTOR* ths,int*);
int mTMC5130Motor_MoveStopEvent(MOTOR* ths,int*);
void mTMC5130Motor_ZeroTouch(MOTOR* ths);
int mTMC5130ConvertToAbsPosition(MOTOR* ths, int position);

void mTMC5130Motor_Init(int idx, \
						MOTOR *ths,\
						motor_cfg_struct * cfg,\
						DRIVER_REG *drive_reg,\
						void (*write_io)(pin_id_t ,int ),\
						int  (*read_io) (pin_id_t ,int,char),\
                        u8 (*read_event)(u8 id),\
						DEFINESTATUS (*motor_state_machine)(MOTOR *),\
						int (*motor_backzeor)(MOTOR *),\
						int (*motor_reset)(MOTOR *),\
						int (*motor_listen_io)(MOTOR *),\
                        int (*mListenEvent)(struct _Motor *),\
						int (*Move)(MOTOR* ths),\
						int (*Rotate)(MOTOR* ths),\
						int (*GetStatus)(MOTOR* ths),\
						int (*Stop)(MOTOR* ths),\
						int (*MultiMove)(int,motor_multi_move_args_t *,bool ),\
						int (*SeqMove)(int, motor_seq_move_args_t*),\
						int (*MoveGPIOEvent)(MOTOR* ,int*),\
                        int (*MoveStopEvent)(MOTOR* ,int*),\
						int (*MotorReset)(MOTOR* )\
						);				
#endif

