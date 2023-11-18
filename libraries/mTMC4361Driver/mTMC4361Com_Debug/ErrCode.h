#ifndef ERR_CODE_H
#define ERR_CODE_H

#define ERR_OS_THREAD_CREATE_FAILED     0x400
#define ERR_OS_SEM_CREATE_FAILED        0x401
#define ERR_OS_SEM_PEND_FAILED          0x402
#define ERR_OS_SEM_POST_FAILED          0x403
#define ERR_OS_SEM_PEND_TIMEOUT         0x404
#define ERR_OS_SEM_DEL_FAILED           0x405
#define ERR_OS_TIMER_CREATE_FAILED      0x406

#define ERR_BUG_OCCURE                      0x500
#define ERR_BUS_REPONSE_ACK_TIMOUT          0x501   //等待回复响应超时 
#define ERR_BUS_ITA_NO_ACK                  0x502   //ita没有回应
#define ERR_BUS_RETURN_NO_ACK               0x503
//#define ERR_ACK_MAP_FULL                    0x504
#define ERR_HEART_BEAT_TIMOUT               0x505


#define ERR_MOTOR_ARGS_ERROR            	0x5ac   //给定的参数是不正确的
#define ERR_MOTOR_LOST_STEP             	0x5ad
#define ERR_MOTOR_SENSOR_BROKEN        	0x5ae    //传感器失败
#define ERR_MOTOR_LOST_CONTINUE         	0x5af
#define ERR_MOTOR_NO_RESET              	0x5b0
#define ERR_MOTOR_EVENT_NO_TRIG         	0x5b1
#define ERR_MOTOR_BUSY         			0x5b2
#define ERR_MOTOR_RESET_OUT_OF_MAX        0x5b3
#define ERR_MOTOR_RESET_IMPACT            0x5b4
#define ERR_MOTOR_NO_RNU           		  0x5b5
#define ERR_MOTOR_ABORT           		  0x5b6

#define ERR_FLASH_ERROR                     0x600
#define ERR_CRC_ERROR                       0x601

#define ERR_SERVO_DRIVER_ACK				0x701
#define ERR_SERVO_DRIVER_CRC				0x702
#define ERR_SERVO_DRIVER_DATALEN			0x703

#define ERR_WAIT_REACH_TIME_OUT				0x1002
#define ERR_APP_PRAM_ERR                    0x1003

























#endif



