#ifndef CMD_H_
#define CMD_H_


#define CMD_MotorFinish			    0x0

#define CMD_Ping                   	0x03
#define CMD_ProtoTest             	0x06

#define CMD_MotorMove			    0x13
#define CMD_MotorRotateStart        0x15
#define CMD_MotorRotateStop         0x16
#define CMD_MotorStatetGet          0x14
#define CMD_MotorRotate             0x17
#define CMD_MotorMultiMove          0x12
#define CMD_MotorReset				0x18
#define CMD_SensorValueOperation    0x19
#define CMD_MotorBackZero			0x20
#define CMD_MotorMultiBackZero		0x21

#define CMD_MotorConfigSet		    0x23
#define CMD_MotorMoveEven			0x24
#define ExecutesCtrl				0x2c

#define CMD_ADAutoCaliStart			0x1a

#define CMD_TemperatureGet			0x1b
#define CMD_TemperatureOpen			0x1c
#define CMD_TemperatureClose		0x1d

#define CMD_AirPumpOpen				0x27
#define CMD_AirPumpClose			0x28

#define CMD_ExecutesCtrl			0x2c    //JS1000_Sample直流电机

#define CMD_ADSampleRead			0x29
#define CMD_DISKInstalledRed  		0x137   // 实现判别是否安装 盘
#define CMD_MotorMoveGPIOEvent      0x2a
#define CMD_MotorConfigSetNew       0x2b
#define CMD_ADDetectConfigSet       0x30  
#define CMD_ADDataRead              0x31
#define CMD_ADDataSeriesSend	    0x32
#define CMD_UartTransmit			0x33
#define CMD_UartReceiveEvent		0x34

#define CMD_SeqMotorMultiMove     	0x35 			//序列化的多电机运动	
#define CMD_MotorAbort              0x38		//电机急停指令 直接停掉
#define CMD_MotorMoveSeq            0x40
#define CMD_PWMSet					0x43

#define CMD_PressureConfig          0x36
#define CMD_PressureDataGet         0x37
#define CMD_TurnTableMove			0x44
#define CMD_EncodeTurnTableFindZero 0x45
#define CMD_EncodeTurnTableRotateStart 0x46
#define CMD_EncodeTurnTableRotateStop 0x47
#define CMD_EncodeTurnTableStateGet 0x48
#define CMD_DCAdRead                0x49
#define CMD_JSAdRead				0x50
#define CMD_MotorMoveSyncLevel		0x51		//在设定的步数发送同步信号
#define CMD_MotorCupIDSet          	0x53        //设置转盘 归零处 反应杯的ID(1~CUP_CNT)
#define CMD_MotorCupInfoGet        	0x54        //get 当前转盘的信息(高速旋转时有些数据可能不准确)
#define CMD_JSAdDataGet 			0x55		//JS1000上位机主动读取data
#define CMD_MotorDetectLiquidLevel	0x6D

#define CMD_IoSimple                0x99
#define CMD_IoOperation				0x100
#define CMD_IoBlink					0x101
#define CMD_UartChangeBode		    0x102    //修改串口波特率

#define CMD_ServoMotorRotateShakeRotateDetect   0x106
#define CMD_ServoMotorMultiRotate			0x107
#define CMD_ServoMotorRotateShakeDetect		0x108
#define CMD_ServoMotorRotateTwoDirOnTime 	0x109
#define CMD_ServoStateGet			0x110
#define CMD_ServoRotateStart		0x111
#define CMD_ServoRotateStop			0x112
#define CMD_ServoReset				0x113
#define CMD_ServoAutoPID			0x114
#define CMD_ServoShake				0x115
#define CMD_ServoBaseCtrl			0x116
#define CMD_ServoMove				0x117
#define CMD_RGBSensorGet 			0x118
#define CMD_SerialDataReport 		0x119

#define CMD_GetEPOSType				0x126
#define CMD_SetEPOSType				0x127



#define CMD_FirmWareConfig          0x1F
#define CMD_FirmWareTransmitFile    0x20
#define CMD_McuReset                0x21
#define CMD_FirmWareVersionGet      0x22
#define CMD_FirmWareReadyEvent      0x41

#define CMD_MotorMultiMoveTmc  		0x62 

#define CMD_MotorMoveTmc  			0x63
#define CMD_MotorStatetGetTmc  		0x64
#define CMD_MotorRotateStartTmc  	0x65
#define CMD_MotorRotateStopTmc  	0x66
#define CMD_MotorRotateTmc  		0x67
#define CMD_MotorResetTmc  			0x68
#define CMD_MotorConfigSetTmc  		0x69
#define CMD_MotorMoveGPIOEventTmc  	0x6a
#define CMD_MotorRotateShake  		0x6b//DC cup传送带用


#define CMD_StartCheckPress  		0x122
#define CMD_EndCheckPress 	 		0x123

#define CMD_ShutDown_PC             0x71

#define CMD_RFIDValueOperation		0x80//读RFID卡


#define CMD_DACSet					0x128
#define CMD_LightCtrl				0x129
#define CMD_DCMotorMoveSeq          0x130
#define CMD_DCMotorMoveTmc          0x131
#define CMD_PressureRead 		    0x132
#define CMD_LittleMotorCtrl         0x133
#define CMD_CongestionDetection     0x134 // JS1000全血堵管检测
#define CMD_LaserCtrol				0x135 // JS1000激光开关控制

#define CMD_SensorReadSingle 0x01000003
#define CMD_PressureListen   0x01000002
#define CMD_IOCtrlByEvent    0x01000000
#define CMD_IOWrite          0x01000004

#define CMD_ColorWrite          0x01000b01
#define CMD_VoltageWrite        0x01000b02
#define CMD_TctMixing           0x01000b03


#endif





