#ifndef ERR_CODE_H
#define ERR_CODE_H

#define ERR_OK	0x0

/*.....................UCOS 系统错误...........................................................................*/
#define ERR_OS_THREAD_CREATE_FAILED     0x400  //线程创建失败
#define ERR_OS_SEM_CREATE_FAILED        0x401  //信号量创建失败
#define ERR_OS_SEM_PEND_FAILED          0x402  //等待信号量失败
#define ERR_OS_SEM_POST_FAILED          0x403  //发送信号量失败
#define ERR_OS_SEM_PEND_TIMEOUT         0x404  //等待信号量超时
#define ERR_OS_SEM_DEL_FAILED           0x405  //删除信量失败
#define ERR_OS_TIMER_CREATE_FAILED      0x406  //创建定时器失败

/*..........................通讯总线错误..........................................................................*/
#define ERR_BUG_OCCURE                      0x500  //总线出错
#define ERR_BUS_REPONSE_ACK_TIMOUT          0x501   //等待总线回复响应超时 
#define ERR_BUS_ITA_NO_ACK                  0x502   //ita没有回应
#define ERR_BUS_RETURN_NO_ACK               0x503  //总线没有回零应答信号
#define ERR_ACK_MAP_FULL                    0x504
#define ERR_HEART_BEAT_TIMOUT               0x505  //总线心跑超时

/* ......................固件操作错误....................................................................................*/
#define ERR_FLASH_ERROR                     0x600 //写FLASH出错 
#define ERR_CRC_ERROR                       0x601  //CRC校验错误

#define ERR_APP_PRAM_ERR 0x610  //PWM参数出错

/*.........................AD采值错误.......................................................................................*/
#define ERR_CALIBRATE_ERR   0x722  			//光值小于设定值(15000)
#define ERR_CALIBRATE_STOP_ERR   0x723  			//采值定标异常停止

/*..........................电机错误..............................................................................................*/
#define ERR_MOTOR_NO   0x0  //无错误

#define ERR_MOTOR_ARGS_ERROR            	0x5ac   //给定的参数是不正确的
#define ERR_MOTOR_LOST_STEP             	0x5ad  //电机失步
#define ERR_MOTOR_SENSOR_BROKEN        	0x5ae    //传感器失败
#define ERR_MOTOR_LOST_CONTINUE         	0x5af //连续失败
#define ERR_MOTOR_NO_RESET              	0x5b0 //电机没有返回
#define ERR_MOTOR_EVENT_NO_TRIG         	0x5b1 //事件没有触发
#define ERR_MOTOR_BUSY         			0x5b2  //电机忙
#define ERR_MOTOR_RESET_OUT_OF_MAX        0x5b3 //参数过大 
#define ERR_MOTOR_RESET_IMPACT            0x5b4  //返回错误
#define ERR_MOTOR_MOTORABORT   0x5b5  //电机急停

/*.......................步进电机错误............................................................................*/
#define ERR_MOTOR_LISTEN_IO_FAIL 0x5d0  //电机监听IO事件失败
#define ERR_MOTOR_RESET_FAIL  0x5d1  //电机复位失败
#define ERR_MOTOR_TIME_OUT   0x5d2//等待信号量超时 
#define ERR_MOTOR_INIT_FAIL  0x5d3  //电机初始化错误码 
/*.......................伺服电机错误.............................................................................*/
#define ERR_SERVO_DRIVER_ACK				0x701 //伺服没返回
#define ERR_SERVO_DRIVER_CRC				0x702 //伺服CRC校验出错
#define ERR_SERVO_DRIVER_DATALEN			0x703  //伺服移动失败epos2

#define ERR_WAIT_REACH_TIME_OUT				0x720  //EPOS4通读超时
#define ERR_EPOS4_ERROR      0X721  			//EPOS4硬件错误

#define ERR_RESULT_ERR	                    0x1004

/*.......................液面探测错误.............................................................................*/
#define DET_AIR_ERR 0x1100//参考气压异常
#define DET_FAIL    0x1101//探液失败

#define ERR_RFID_KEY_ERROR		0x1200 // RFID密钥验证失败
#define ERR_RFID_READ_ERROR		0x1201 // RFID读失败
#define ERR_RFID_WRITE_ERROR	0x1202 // RFID写失败

#define ERR_SEM_WAIT_TIMEOUT    0x1203 // 信号量等待超时

#define ERR_CONGESTION_DETECTION	0x1204 // 堵管检测错误

#endif



