//------------- 结构声明 ----------------------------------
#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_
#include "bytesBuffer.h"
#include "tool.h"
#include <stdlib.h> 
#include <string.h> 
typedef struct Dsp_DHCP_Send { 
    int m1_uid_Length;
    u8* m1_uid;
    BytesBufferNode * (*ToBytes)( struct Dsp_DHCP_Send *ths ); 
    int (*GetFromBytes)( struct Dsp_DHCP_Send *ths,u8* buff,int offset ); 
}Dsp_DHCP_Send;
typedef struct Dsp_DHCP_Return { 
    int m1_uid_Length;
    u8* m1_uid;
    u8 m2_localId;
    BytesBufferNode * (*ToBytes)( struct Dsp_DHCP_Return *ths ); 
    int (*GetFromBytes)( struct Dsp_DHCP_Return *ths,u8* buff,int offset ); 
}Dsp_DHCP_Return;
typedef struct Ping_Send { 
    u32 m1_test1;
    u32 m2_test2;
    u32 m3_test3;
    u32 m4_test4;
    BytesBufferNode * (*ToBytes)( struct Ping_Send *ths ); 
    int (*GetFromBytes)( struct Ping_Send *ths,u8* buff,int offset ); 
}Ping_Send;
typedef struct Ping_Return { 
    int m1_uniqueId_Length;
    u8* m1_uniqueId;
    BytesBufferNode * (*ToBytes)( struct Ping_Return *ths ); 
    int (*GetFromBytes)( struct Ping_Return *ths,u8* buff,int offset ); 
}Ping_Return;
typedef struct FilePush_Send { 
    u8 m1_cmd;
    u32 m2_fileLen;
    int m3_fileContent_Length;
    u8* m3_fileContent;
    BytesBufferNode * (*ToBytes)( struct FilePush_Send *ths ); 
    int (*GetFromBytes)( struct FilePush_Send *ths,u8* buff,int offset ); 
}FilePush_Send;
typedef struct FilePush_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct FilePush_Return *ths ); 
    int (*GetFromBytes)( struct FilePush_Return *ths,u8* buff,int offset ); 
}FilePush_Return;
typedef struct InstrumentStateReport_Send { 
    int m1_uniqueId_Length;
    u8* m1_uniqueId;
    BytesBufferNode * (*ToBytes)( struct InstrumentStateReport_Send *ths ); 
    int (*GetFromBytes)( struct InstrumentStateReport_Send *ths,u8* buff,int offset ); 
}InstrumentStateReport_Send;
typedef struct InstrumentStateReport_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct InstrumentStateReport_Return *ths ); 
    int (*GetFromBytes)( struct InstrumentStateReport_Return *ths,u8* buff,int offset ); 
}InstrumentStateReport_Return;
typedef struct ProtoTest_Send { 
    int m1_data_Length;
    u8* m1_data;
    u32 m2_data_crc;
    BytesBufferNode * (*ToBytes)( struct ProtoTest_Send *ths ); 
    int (*GetFromBytes)( struct ProtoTest_Send *ths,u8* buff,int offset ); 
}ProtoTest_Send;
typedef struct ProtoTest_Return { 
    int m1_data_Length;
    u8* m1_data;
    u32 m2_data_crc;
    BytesBufferNode * (*ToBytes)( struct ProtoTest_Return *ths ); 
    int (*GetFromBytes)( struct ProtoTest_Return *ths,u8* buff,int offset ); 
}ProtoTest_Return;
typedef struct McuErrorToPC_Send { 
    u32 m1_errCode;
    char *  m2_info;
    u32 m3_reserved;
    u32 m4_reserved;
    u32 m5_reserved;
    BytesBufferNode * (*ToBytes)( struct McuErrorToPC_Send *ths ); 
    int (*GetFromBytes)( struct McuErrorToPC_Send *ths,u8* buff,int offset ); 
}McuErrorToPC_Send;
typedef struct McuErrorToPC_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct McuErrorToPC_Return *ths ); 
    int (*GetFromBytes)( struct McuErrorToPC_Return *ths,u8* buff,int offset ); 
}McuErrorToPC_Return;
typedef struct AutoAdapter_Send { 
    u32 m1_localID;
    BytesBufferNode * (*ToBytes)( struct AutoAdapter_Send *ths ); 
    int (*GetFromBytes)( struct AutoAdapter_Send *ths,u8* buff,int offset ); 
}AutoAdapter_Send;
typedef struct AutoAdapter_Return { 
    u32 m1_writeState;
    BytesBufferNode * (*ToBytes)( struct AutoAdapter_Return *ths ); 
    int (*GetFromBytes)( struct AutoAdapter_Return *ths,u8* buff,int offset ); 
}AutoAdapter_Return;
typedef struct MotorMultiMove_Send_m1_MoveArgs { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u32 m6_delayms;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMove_Send_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct MotorMultiMove_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
}MotorMultiMove_Send_m1_MoveArgs;
typedef struct MotorMultiMove_Send { 
    int m1_MoveArgs_Length;
    MotorMultiMove_Send_m1_MoveArgs* m1_MoveArgs;
    u8 m2_isBackZero;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMove_Send *ths ); 
    int (*GetFromBytes)( struct MotorMultiMove_Send *ths,u8* buff,int offset ); 
}MotorMultiMove_Send;
typedef struct MotorMultiMove_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMove_Return *ths ); 
    int (*GetFromBytes)( struct MotorMultiMove_Return *ths,u8* buff,int offset ); 
}MotorMultiMove_Return;
typedef struct MotorMove_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m6_isBackZero;
    BytesBufferNode * (*ToBytes)( struct MotorMove_Send *ths ); 
    int (*GetFromBytes)( struct MotorMove_Send *ths,u8* buff,int offset ); 
}MotorMove_Send;
typedef struct MotorMove_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMove_Return *ths ); 
    int (*GetFromBytes)( struct MotorMove_Return *ths,u8* buff,int offset ); 
}MotorMove_Return;
typedef struct MotorStatetGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorStatetGet_Send *ths ); 
    int (*GetFromBytes)( struct MotorStatetGet_Send *ths,u8* buff,int offset ); 
}MotorStatetGet_Send;
typedef struct MotorStatetGet_Return { 
    u8 m1_isZeroTriggered;
    u32 m2_currentCordinate;
    u32 m3_lostSteps;
    u8 m4_isBusy;
    BytesBufferNode * (*ToBytes)( struct MotorStatetGet_Return *ths ); 
    int (*GetFromBytes)( struct MotorStatetGet_Return *ths,u8* buff,int offset ); 
}MotorStatetGet_Return;
typedef struct MotorRotateStart_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_isForward;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStart_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateStart_Send *ths,u8* buff,int offset ); 
}MotorRotateStart_Send;
typedef struct MotorRotateStart_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStart_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateStart_Return *ths,u8* buff,int offset ); 
}MotorRotateStart_Return;
typedef struct MotorRotateStop_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStop_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateStop_Send *ths,u8* buff,int offset ); 
}MotorRotateStop_Send;
typedef struct MotorRotateStop_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStop_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateStop_Return *ths,u8* buff,int offset ); 
}MotorRotateStop_Return;
typedef struct MotorRotate_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    BytesBufferNode * (*ToBytes)( struct MotorRotate_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotate_Send *ths,u8* buff,int offset ); 
}MotorRotate_Send;
typedef struct MotorRotate_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotate_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotate_Return *ths,u8* buff,int offset ); 
}MotorRotate_Return;
typedef struct MotorReset_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    BytesBufferNode * (*ToBytes)( struct MotorReset_Send *ths ); 
    int (*GetFromBytes)( struct MotorReset_Send *ths,u8* buff,int offset ); 
}MotorReset_Send;
typedef struct MotorReset_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorReset_Return *ths ); 
    int (*GetFromBytes)( struct MotorReset_Return *ths,u8* buff,int offset ); 
}MotorReset_Return;
typedef struct SensorValueOperation_Send { 
    u16 m1_reg;
    u8 m2_op;
    u32 m3_value;
    u32 m4_t_delay_1;
    u32 m5_t_delay_2;
    BytesBufferNode * (*ToBytes)( struct SensorValueOperation_Send *ths ); 
    int (*GetFromBytes)( struct SensorValueOperation_Send *ths,u8* buff,int offset ); 
}SensorValueOperation_Send;
typedef struct SensorValueOperation_Return { 
    u32 m1_errCode;
    u32 m2_value;
    u32 m3_airpres;
    u32 m4_basepres;
    u32 m5_actpres;
    BytesBufferNode * (*ToBytes)( struct SensorValueOperation_Return *ths ); 
    int (*GetFromBytes)( struct SensorValueOperation_Return *ths,u8* buff,int offset ); 
}SensorValueOperation_Return;
typedef struct ADAutoCaliStart_Send { 
    u32 m1_scan_times_offset;
    u32 m2_f2f;
    u32 m3_f2d;
    u32 m4_d2f;
    u32 m5_f2rc;
    u32 m6_refer_width_times;
    BytesBufferNode * (*ToBytes)( struct ADAutoCaliStart_Send *ths ); 
    int (*GetFromBytes)( struct ADAutoCaliStart_Send *ths,u8* buff,int offset ); 
}ADAutoCaliStart_Send;
typedef struct ADAutoCaliStart_Return { 
    u32 m1_z2f;
    u32 m2_errCode;
    BytesBufferNode * (*ToBytes)( struct ADAutoCaliStart_Return *ths ); 
    int (*GetFromBytes)( struct ADAutoCaliStart_Return *ths,u8* buff,int offset ); 
}ADAutoCaliStart_Return;
typedef struct TemperatureGet_Send { 
    u8 m1_channelId;
    BytesBufferNode * (*ToBytes)( struct TemperatureGet_Send *ths ); 
    int (*GetFromBytes)( struct TemperatureGet_Send *ths,u8* buff,int offset ); 
}TemperatureGet_Send;
typedef struct TemperatureGet_Return { 
    u8 m1_state;
    float m2_current_temp;
    BytesBufferNode * (*ToBytes)( struct TemperatureGet_Return *ths ); 
    int (*GetFromBytes)( struct TemperatureGet_Return *ths,u8* buff,int offset ); 
}TemperatureGet_Return;
typedef struct TemperatureOpen_Send { 
    u8 m1_channelId;
    float m2_targetTemp;
    float m3_p;
    float m4_i;
    float m5_d;
    float m6_offset;
    float m7_from;
    float m8_to;
    BytesBufferNode * (*ToBytes)( struct TemperatureOpen_Send *ths ); 
    int (*GetFromBytes)( struct TemperatureOpen_Send *ths,u8* buff,int offset ); 
}TemperatureOpen_Send;
typedef struct TemperatureOpen_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct TemperatureOpen_Return *ths ); 
    int (*GetFromBytes)( struct TemperatureOpen_Return *ths,u8* buff,int offset ); 
}TemperatureOpen_Return;
typedef struct TemperatureClose_Send { 
    u8 m1_channelId;
    BytesBufferNode * (*ToBytes)( struct TemperatureClose_Send *ths ); 
    int (*GetFromBytes)( struct TemperatureClose_Send *ths,u8* buff,int offset ); 
}TemperatureClose_Send;
typedef struct TemperatureClose_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct TemperatureClose_Return *ths ); 
    int (*GetFromBytes)( struct TemperatureClose_Return *ths,u8* buff,int offset ); 
}TemperatureClose_Return;
typedef struct BarCodeGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct BarCodeGet_Send *ths ); 
    int (*GetFromBytes)( struct BarCodeGet_Send *ths,u8* buff,int offset ); 
}BarCodeGet_Send;
typedef struct BarCodeGet_Return { 
    u32 m1_errCode;
    char *  m2_value;
    BytesBufferNode * (*ToBytes)( struct BarCodeGet_Return *ths ); 
    int (*GetFromBytes)( struct BarCodeGet_Return *ths,u8* buff,int offset ); 
}BarCodeGet_Return;
typedef struct FirmWareConfig_Send { 
    u32 m1_cmd;
    u32 m2_version;
    BytesBufferNode * (*ToBytes)( struct FirmWareConfig_Send *ths ); 
    int (*GetFromBytes)( struct FirmWareConfig_Send *ths,u8* buff,int offset ); 
}FirmWareConfig_Send;
typedef struct FirmWareConfig_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct FirmWareConfig_Return *ths ); 
    int (*GetFromBytes)( struct FirmWareConfig_Return *ths,u8* buff,int offset ); 
}FirmWareConfig_Return;
typedef struct FirmWareTransmitFile_Send { 
    int m1_datas_Length;
    u8* m1_datas;
    u32 m2_crc;
    u32 m3_offset;
    BytesBufferNode * (*ToBytes)( struct FirmWareTransmitFile_Send *ths ); 
    int (*GetFromBytes)( struct FirmWareTransmitFile_Send *ths,u8* buff,int offset ); 
}FirmWareTransmitFile_Send;
typedef struct FirmWareTransmitFile_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct FirmWareTransmitFile_Return *ths ); 
    int (*GetFromBytes)( struct FirmWareTransmitFile_Return *ths,u8* buff,int offset ); 
}FirmWareTransmitFile_Return;
typedef struct McuReset_Send { 
    u8 m1_reserved;
    BytesBufferNode * (*ToBytes)( struct McuReset_Send *ths ); 
    int (*GetFromBytes)( struct McuReset_Send *ths,u8* buff,int offset ); 
}McuReset_Send;
typedef struct McuReset_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct McuReset_Return *ths ); 
    int (*GetFromBytes)( struct McuReset_Return *ths,u8* buff,int offset ); 
}McuReset_Return;
typedef struct FirmWareVersionGet_Send { 
    u8 m1_reserved;
    BytesBufferNode * (*ToBytes)( struct FirmWareVersionGet_Send *ths ); 
    int (*GetFromBytes)( struct FirmWareVersionGet_Send *ths,u8* buff,int offset ); 
}FirmWareVersionGet_Send;
typedef struct FirmWareVersionGet_Return { 
    u32 m1_errCode;
    u32 m2_value;
    BytesBufferNode * (*ToBytes)( struct FirmWareVersionGet_Return *ths ); 
    int (*GetFromBytes)( struct FirmWareVersionGet_Return *ths,u8* buff,int offset ); 
}FirmWareVersionGet_Return;
typedef struct MotorConfigSet_Send { 
    u8 m1_idx;
    u32 m2_zeroCordinate;
    u32 m3_maxCordinate;
    u32 m4_lostEndureContinue;
    u32 m5_lostEndureStop;
    u32 m6_zeroTrigVol;
    u32 m7_dirVol;
    u8 m8_isRotate;
    u8 m9_enableVol;
    u8 m10_halfVol;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSet_Send *ths ); 
    int (*GetFromBytes)( struct MotorConfigSet_Send *ths,u8* buff,int offset ); 
}MotorConfigSet_Send;
typedef struct MotorConfigSet_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSet_Return *ths ); 
    int (*GetFromBytes)( struct MotorConfigSet_Return *ths,u8* buff,int offset ); 
}MotorConfigSet_Return;
typedef struct MotorMoveEvent_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m6_eventId;
    BytesBufferNode * (*ToBytes)( struct MotorMoveEvent_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveEvent_Send *ths,u8* buff,int offset ); 
}MotorMoveEvent_Send;
typedef struct MotorMoveEvent_Return { 
    u32 m1_errCode;
    u32 m2_trigger_pos;
    BytesBufferNode * (*ToBytes)( struct MotorMoveEvent_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveEvent_Return *ths,u8* buff,int offset ); 
}MotorMoveEvent_Return;
typedef struct McuIoStateChangeEvent_Send { 
    int m1_port_pin_state_Length;
    u16* m1_port_pin_state;
    BytesBufferNode * (*ToBytes)( struct McuIoStateChangeEvent_Send *ths ); 
    int (*GetFromBytes)( struct McuIoStateChangeEvent_Send *ths,u8* buff,int offset ); 
}McuIoStateChangeEvent_Send;
typedef struct McuIoStateChangeEvent_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct McuIoStateChangeEvent_Return *ths ); 
    int (*GetFromBytes)( struct McuIoStateChangeEvent_Return *ths,u8* buff,int offset ); 
}McuIoStateChangeEvent_Return;
typedef struct SampleUnitSensorGet_Send { 
    char *  m1_port;
    u16 m2_pin;
    BytesBufferNode * (*ToBytes)( struct SampleUnitSensorGet_Send *ths ); 
    int (*GetFromBytes)( struct SampleUnitSensorGet_Send *ths,u8* buff,int offset ); 
}SampleUnitSensorGet_Send;
typedef struct SampleUnitSensorGet_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct SampleUnitSensorGet_Return *ths ); 
    int (*GetFromBytes)( struct SampleUnitSensorGet_Return *ths,u8* buff,int offset ); 
}SampleUnitSensorGet_Return;
typedef struct AirPumpOpen_Send { 
    u8 m1_channelId;
    u32 m2_pw;
    BytesBufferNode * (*ToBytes)( struct AirPumpOpen_Send *ths ); 
    int (*GetFromBytes)( struct AirPumpOpen_Send *ths,u8* buff,int offset ); 
}AirPumpOpen_Send;
typedef struct AirPumpOpen_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct AirPumpOpen_Return *ths ); 
    int (*GetFromBytes)( struct AirPumpOpen_Return *ths,u8* buff,int offset ); 
}AirPumpOpen_Return;

typedef struct AirPumpClose_Send { 
    u8 m1_channelId;
    BytesBufferNode * (*ToBytes)( struct AirPumpClose_Send *ths ); 
    int (*GetFromBytes)( struct AirPumpClose_Send *ths,u8* buff,int offset ); 
}AirPumpClose_Send;
typedef struct AirPumpClose_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct AirPumpClose_Return *ths ); 
    int (*GetFromBytes)( struct AirPumpClose_Return *ths,u8* buff,int offset ); 
}AirPumpClose_Return;

typedef struct ADSampleRead_Send { 
    u8 m1_channel;
    u16 m2_amp;
    BytesBufferNode * (*ToBytes)( struct ADSampleRead_Send *ths ); 
    int (*GetFromBytes)( struct ADSampleRead_Send *ths,u8* buff,int offset ); 
}ADSampleRead_Send;
typedef struct ADSampleRead_Return { 
    u16 m1_val;
    BytesBufferNode * (*ToBytes)( struct ADSampleRead_Return *ths ); 
    int (*GetFromBytes)( struct ADSampleRead_Return *ths,u8* buff,int offset ); 
}ADSampleRead_Return;

/*
这里增加新的数据接口 结构体
*/

//typedef struct DiskInstalledRed_Send{
//	u8 m1_channel;
//	BytesBufferNode * (*ToBytes)( struct DiskInstalledRed_Send *ths ); 
//  int (*GetFromBytes)( struct DiskInstalledRed_Send *ths,u8* buff,int offset ); 
//	
//}DiskInstalledRed_Send;

typedef struct DiskInstalledRed_Send{
	u16 m1_cnt;
  u16 m2_time_span_ms;
  u16 m3_channel;
	BytesBufferNode * (*ToBytes)( struct DiskInstalledRed_Send *ths ); 
  int (*GetFromBytes)( struct DiskInstalledRed_Send *ths,u8* buff,int offset ); 
	
}DiskInstalledRed_Send;

//上位机需要 选择 检测次数，检测间隔时间 传感器通道  


typedef struct DiskInstalledRed_Return { 
		u32 m1_errCode;
    u16 m2_maxval;
		u16 m3_minval;
    BytesBufferNode * (*ToBytes)( struct DiskInstalledRed_Return *ths ); 
    int (*GetFromBytes)( struct DiskInstalledRed_Return *ths,u8* buff,int offset ); 
}DiskInstalledRed_Return;
//下位机返回 多次采集的最大值和最小值 供上位机判断 盘的安装状态

typedef struct MotorMoveGPIOEvent_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u16 m6_state_port_pin;
    BytesBufferNode * (*ToBytes)( struct MotorMoveGPIOEvent_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveGPIOEvent_Send *ths,u8* buff,int offset ); 
}MotorMoveGPIOEvent_Send;
typedef struct MotorMoveGPIOEvent_Return { 
    u32 m1_errCode;
    u32 m2_trigger_pos;
    BytesBufferNode * (*ToBytes)( struct MotorMoveGPIOEvent_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveGPIOEvent_Return *ths,u8* buff,int offset ); 
}MotorMoveGPIOEvent_Return;
typedef struct MotorConfigSetNew_Send { 
    u16 m1_idx_zero_dir_isR_en_hf;
    u16 m2_maxCordinate;
    u16 m3_zeroCordinate;
    u16 m4_lostEndureContinue;
    u16 m5_lostEndureStop;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSetNew_Send *ths ); 
    int (*GetFromBytes)( struct MotorConfigSetNew_Send *ths,u8* buff,int offset ); 
}MotorConfigSetNew_Send;
typedef struct MotorConfigSetNew_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSetNew_Return *ths ); 
    int (*GetFromBytes)( struct MotorConfigSetNew_Return *ths,u8* buff,int offset ); 
}MotorConfigSetNew_Return;
typedef struct ExecutesCtrl_Send { 
    u8 m1_channelId;
    u8 m2_isOpen;
    BytesBufferNode * (*ToBytes)( struct ExecutesCtrl_Send *ths ); 
    int (*GetFromBytes)( struct ExecutesCtrl_Send *ths,u8* buff,int offset ); 
}ExecutesCtrl_Send;
typedef struct ExecutesCtrl_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ExecutesCtrl_Return *ths ); 
    int (*GetFromBytes)( struct ExecutesCtrl_Return *ths,u8* buff,int offset ); 
}ExecutesCtrl_Return;
typedef struct ADDetectConfigSet_Send { 
    int m1_amp_Length;
    u16* m1_amp;
    u8 m2_filterCnt;
    u32 m3_light_offset;
    int m4_channel_Length;
    u16* m4_channel;
    u32 m5_light_motor_speed;
    BytesBufferNode * (*ToBytes)( struct ADDetectConfigSet_Send *ths ); 
    int (*GetFromBytes)( struct ADDetectConfigSet_Send *ths,u8* buff,int offset ); 
}ADDetectConfigSet_Send;
typedef struct ADDetectConfigSet_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ADDetectConfigSet_Return *ths ); 
    int (*GetFromBytes)( struct ADDetectConfigSet_Return *ths,u8* buff,int offset ); 
}ADDetectConfigSet_Return;
typedef struct ADDataRead_Send { 
    u8 m1_filterId;
    u8 m2_method;
    u8 m3_aver_cnt;
    u8 m4_channel;
    u32 m5_detect_circle_cnt;
    u32 m6_z2f;
	u32 m7_time_per_degree;
    BytesBufferNode * (*ToBytes)( struct ADDataRead_Send *ths ); 
    int (*GetFromBytes)( struct ADDataRead_Send *ths,u8* buff,int offset ); 
}ADDataRead_Send;
typedef struct ADDataRead_Return { 
    int m1_val_Length;
    u16* m1_val;
    BytesBufferNode * (*ToBytes)( struct ADDataRead_Return *ths ); 
    int (*GetFromBytes)( struct ADDataRead_Return *ths,u8* buff,int offset ); 
}ADDataRead_Return;
typedef struct ADDataSeriesSend_Send { 
    int m1_data_Length;
    u16* m1_data;
    u16 m2_current_detect_circle;
    BytesBufferNode * (*ToBytes)( struct ADDataSeriesSend_Send *ths ); 
    int (*GetFromBytes)( struct ADDataSeriesSend_Send *ths,u8* buff,int offset ); 
}ADDataSeriesSend_Send;
typedef struct ADDataSeriesSend_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ADDataSeriesSend_Return *ths ); 
    int (*GetFromBytes)( struct ADDataSeriesSend_Return *ths,u8* buff,int offset ); 
}ADDataSeriesSend_Return;
typedef struct UartTransmit_Send { 
    u8 m1_uart_id;
    int m2_data_Length;
    u8* m2_data;
    BytesBufferNode * (*ToBytes)( struct UartTransmit_Send *ths ); 
    int (*GetFromBytes)( struct UartTransmit_Send *ths,u8* buff,int offset ); 
}UartTransmit_Send;
typedef struct UartTransmit_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct UartTransmit_Return *ths ); 
    int (*GetFromBytes)( struct UartTransmit_Return *ths,u8* buff,int offset ); 
}UartTransmit_Return;
typedef struct UartReceiveEvent_Send { 
    u8 m1_uart_id;
    int m2_data_Length;
    u8* m2_data;
    BytesBufferNode * (*ToBytes)( struct UartReceiveEvent_Send *ths ); 
    int (*GetFromBytes)( struct UartReceiveEvent_Send *ths,u8* buff,int offset ); 
}UartReceiveEvent_Send;
typedef struct UartReceiveEvent_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct UartReceiveEvent_Return *ths ); 
    int (*GetFromBytes)( struct UartReceiveEvent_Return *ths,u8* buff,int offset ); 
}UartReceiveEvent_Return;
typedef struct SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    BytesBufferNode * (*ToBytes)( struct SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths,u8* buff,int offset ); 
}SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs;
typedef struct SeqMotorMultiMove_Send_m1_Sequence { 
    int m1_MoveArgs_Length;
    SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* m1_MoveArgs;
    u32 m2_delayms;
    BytesBufferNode * (*ToBytes)( struct SeqMotorMultiMove_Send_m1_Sequence *ths ); 
    int (*GetFromBytes)( struct SeqMotorMultiMove_Send_m1_Sequence *ths,u8* buff,int offset ); 
}SeqMotorMultiMove_Send_m1_Sequence;
typedef struct SeqMotorMultiMove_Send { 
    int m1_Sequence_Length;
    SeqMotorMultiMove_Send_m1_Sequence* m1_Sequence;
    BytesBufferNode * (*ToBytes)( struct SeqMotorMultiMove_Send *ths ); 
    int (*GetFromBytes)( struct SeqMotorMultiMove_Send *ths,u8* buff,int offset ); 
}SeqMotorMultiMove_Send;
typedef struct SeqMotorMultiMove_Return { 
    u32 m1_errCode;
    u8 m2_errIdx;
    BytesBufferNode * (*ToBytes)( struct SeqMotorMultiMove_Return *ths ); 
    int (*GetFromBytes)( struct SeqMotorMultiMove_Return *ths,u8* buff,int offset ); 
}SeqMotorMultiMove_Return;
typedef struct PressureConfig_Send { 
    u8 m1_cmd;
    u8 m2_averageCnt;
    u32 m3_tick;
    u32 m4_maxSampleCnt;
    u32 m5_threshold;
    BytesBufferNode * (*ToBytes)( struct PressureConfig_Send *ths ); 
    int (*GetFromBytes)( struct PressureConfig_Send *ths,u8* buff,int offset ); 
}PressureConfig_Send;
typedef struct PressureConfig_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct PressureConfig_Return *ths ); 
    int (*GetFromBytes)( struct PressureConfig_Return *ths,u8* buff,int offset ); 
}PressureConfig_Return;
typedef struct PressureDataGet_Send { 
    u8 m1_cmd;
    u32 m2_data_offset;
    BytesBufferNode * (*ToBytes)( struct PressureDataGet_Send *ths ); 
    int (*GetFromBytes)( struct PressureDataGet_Send *ths,u8* buff,int offset ); 
}PressureDataGet_Send;
typedef struct PressureDataGet_Return { 
    u32 m1_errCode;
    u32 m2_remainLen;
    int m3_pressData_Length;
    u16* m3_pressData;
    BytesBufferNode * (*ToBytes)( struct PressureDataGet_Return *ths ); 
    int (*GetFromBytes)( struct PressureDataGet_Return *ths,u8* buff,int offset ); 
}PressureDataGet_Return;
typedef struct PressureListen_Send { 
    u8 m1_id_flag;
    u32 m2_diff_threthold;
    u16 m3_listen_time_span;
    u32 m4_error_range;
    u16 m5_listen_time;
    BytesBufferNode * (*ToBytes)( struct PressureListen_Send *ths ); 
    int (*GetFromBytes)( struct PressureListen_Send *ths,u8* buff,int offset ); 
}PressureListen_Send;
typedef struct PressureListen_Return { 
    u8 m1_error_code;
    u16 m2_continue_time;
    u16 m3_end_pressure;
    int m4_pressure_data_Length;
    u16* m4_pressure_data;
    BytesBufferNode * (*ToBytes)( struct PressureListen_Return *ths ); 
    int (*GetFromBytes)( struct PressureListen_Return *ths,u8* buff,int offset ); 
}PressureListen_Return;
typedef struct MotorAbort_Send { 
    u32 m1_idx_mask;
    BytesBufferNode * (*ToBytes)( struct MotorAbort_Send *ths ); 
    int (*GetFromBytes)( struct MotorAbort_Send *ths,u8* buff,int offset ); 
}MotorAbort_Send;
typedef struct MotorAbort_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorAbort_Return *ths ); 
    int (*GetFromBytes)( struct MotorAbort_Return *ths,u8* buff,int offset ); 
}MotorAbort_Return;
typedef struct PressureEvent_Send { 
    u8 m1_eventId;
    BytesBufferNode * (*ToBytes)( struct PressureEvent_Send *ths ); 
    int (*GetFromBytes)( struct PressureEvent_Send *ths,u8* buff,int offset ); 
}PressureEvent_Send;
typedef struct PressureEvent_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct PressureEvent_Return *ths ); 
    int (*GetFromBytes)( struct PressureEvent_Return *ths,u8* buff,int offset ); 
}PressureEvent_Return;
typedef struct MotorMoveSeq_Send_m1_MoveArgs { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u32 m6_delayms;
    u8 m7_currentLevel;
    u8 m2_isBackZero;
    BytesBufferNode * (*ToBytes)( struct MotorMoveSeq_Send_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct MotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
}MotorMoveSeq_Send_m1_MoveArgs;
typedef struct MotorMoveSeq_Send { 
    int m1_MoveArgs_Length;
    MotorMoveSeq_Send_m1_MoveArgs* m1_MoveArgs;
    BytesBufferNode * (*ToBytes)( struct MotorMoveSeq_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveSeq_Send *ths,u8* buff,int offset ); 
}MotorMoveSeq_Send;
typedef struct MotorMoveSeq_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMoveSeq_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveSeq_Return *ths,u8* buff,int offset ); 
}MotorMoveSeq_Return;
typedef struct FirmWareReadyEvent_Send { 
    u32 m1_reserved;
    BytesBufferNode * (*ToBytes)( struct FirmWareReadyEvent_Send *ths ); 
    int (*GetFromBytes)( struct FirmWareReadyEvent_Send *ths,u8* buff,int offset ); 
}FirmWareReadyEvent_Send;
typedef struct FirmWareReadyEvent_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct FirmWareReadyEvent_Return *ths ); 
    int (*GetFromBytes)( struct FirmWareReadyEvent_Return *ths,u8* buff,int offset ); 
}FirmWareReadyEvent_Return;
typedef struct MotorConfigGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorConfigGet_Send *ths ); 
    int (*GetFromBytes)( struct MotorConfigGet_Send *ths,u8* buff,int offset ); 
}MotorConfigGet_Send;
typedef struct MotorConfigGet_Return { 
    u32 m1_errCode;
    u32 m2_zeroCordinate;
    u32 m3_maxCordinate;
    u32 m4_lostEndureContinue;
    u32 m5_lostEndureStop;
    u8 m6_zeroTrigVol;
    u8 m7_dirVol;
    u8 m8_isRotate;
    u8 m9_enableVol;
    u8 m10_halfVol;
    BytesBufferNode * (*ToBytes)( struct MotorConfigGet_Return *ths ); 
    int (*GetFromBytes)( struct MotorConfigGet_Return *ths,u8* buff,int offset ); 
}MotorConfigGet_Return;
typedef struct PWM_Set_Send { 
    u8 m1_time_channle;
    u32 m2_period;
    u32 m3_dutyCycle;
    BytesBufferNode * (*ToBytes)( struct PWM_Set_Send *ths ); 
    int (*GetFromBytes)( struct PWM_Set_Send *ths,u8* buff,int offset ); 
}PWM_Set_Send;
typedef struct PWM_Set_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct PWM_Set_Return *ths ); 
    int (*GetFromBytes)( struct PWM_Set_Return *ths,u8* buff,int offset ); 
}PWM_Set_Return;
typedef struct TurnTable_Move_Send { 
    u32 m1_cali_location;
    u8 m2_idx;
    u32 m3_steps;
    u32 m4_speed;
    u32 m5_acce;
    BytesBufferNode * (*ToBytes)( struct TurnTable_Move_Send *ths ); 
    int (*GetFromBytes)( struct TurnTable_Move_Send *ths,u8* buff,int offset ); 
}TurnTable_Move_Send;
typedef struct TurnTable_Move_Return { 
    u32 m1_errCode;
    u8 m2_filter_id;
    u16 m3_ad_data;
    BytesBufferNode * (*ToBytes)( struct TurnTable_Move_Return *ths ); 
    int (*GetFromBytes)( struct TurnTable_Move_Return *ths,u8* buff,int offset ); 
}TurnTable_Move_Return;
typedef struct EncodeTurnTableFindZero_Send { 
    u8 m1_idx;
    u32 m2_cali_location;
    u32 m3_speed;
    u32 m4_acce;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableFindZero_Send *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableFindZero_Send *ths,u8* buff,int offset ); 
}EncodeTurnTableFindZero_Send;
typedef struct EncodeTurnTableFindZero_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableFindZero_Return *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableFindZero_Return *ths,u8* buff,int offset ); 
}EncodeTurnTableFindZero_Return;
typedef struct EncodeTurnTableRotateStart_Send { 
    u8 m1_idx;
    u32 m2_cali_location;
    u32 m3_speed;
    u32 m4_acce;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableRotateStart_Send *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableRotateStart_Send *ths,u8* buff,int offset ); 
}EncodeTurnTableRotateStart_Send;
typedef struct EncodeTurnTableRotateStart_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableRotateStart_Return *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableRotateStart_Return *ths,u8* buff,int offset ); 
}EncodeTurnTableRotateStart_Return;
typedef struct EncodeTurnTableRotateStop_Send { 
    u8 m1_idx;
    u32 m2_dest_id;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableRotateStop_Send *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableRotateStop_Send *ths,u8* buff,int offset ); 
}EncodeTurnTableRotateStop_Send;
typedef struct EncodeTurnTableRotateStop_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableRotateStop_Return *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableRotateStop_Return *ths,u8* buff,int offset ); 
}EncodeTurnTableRotateStop_Return;
typedef struct EncodeTurnTableStateGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableStateGet_Send *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableStateGet_Send *ths,u8* buff,int offset ); 
}EncodeTurnTableStateGet_Send;
typedef struct EncodeTurnTableStateGet_Return { 
    u8 m1_isZeroTriggered;
    u8 m2_is_busy;
    u32 m3_current_id;
    BytesBufferNode * (*ToBytes)( struct EncodeTurnTableStateGet_Return *ths ); 
    int (*GetFromBytes)( struct EncodeTurnTableStateGet_Return *ths,u8* buff,int offset ); 
}EncodeTurnTableStateGet_Return;
typedef struct DCAdRead_Send { 
    u8 m1_cmd;
    u32 m2_channel;
    BytesBufferNode * (*ToBytes)( struct DCAdRead_Send *ths ); 
    int (*GetFromBytes)( struct DCAdRead_Send *ths,u8* buff,int offset ); 
}DCAdRead_Send;
typedef struct DCAdRead_Return { 
    u16 m1_remainCnt;
    int m2_adValue_Length;
    u16* m2_adValue;
    BytesBufferNode * (*ToBytes)( struct DCAdRead_Return *ths ); 
    int (*GetFromBytes)( struct DCAdRead_Return *ths,u8* buff,int offset ); 
}DCAdRead_Return;
typedef struct JSAdRead_Send { 
    u8 m1_channel_id;
    u32 m2_time_span;
    u16 m3_cnt_read;
    u16 m4_dac_data;
    BytesBufferNode * (*ToBytes)( struct JSAdRead_Send *ths ); 
    int (*GetFromBytes)( struct JSAdRead_Send *ths,u8* buff,int offset ); 
}JSAdRead_Send;
typedef struct JSAdRead_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct JSAdRead_Return *ths ); 
    int (*GetFromBytes)( struct JSAdRead_Return *ths,u8* buff,int offset ); 
}JSAdRead_Return;
typedef struct MotorMoveSyncLevel_Send { 
    u8 m1_idx;
    u32 m2_steps;
    u32 m3_sync_steps;
    u32 m4_speed;
    u32 m5_acce;
    BytesBufferNode * (*ToBytes)( struct MotorMoveSyncLevel_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveSyncLevel_Send *ths,u8* buff,int offset ); 
}MotorMoveSyncLevel_Send;
typedef struct MotorMoveSyncLevel_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMoveSyncLevel_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveSyncLevel_Return *ths,u8* buff,int offset ); 
}MotorMoveSyncLevel_Return;
typedef struct DCMotor_Send { 
    u32 m1_Idx;
    u8 m2_isOpen;
    BytesBufferNode * (*ToBytes)( struct DCMotor_Send *ths ); 
    int (*GetFromBytes)( struct DCMotor_Send *ths,u8* buff,int offset ); 
}DCMotor_Send;
typedef struct DCMotor_Return { 
    u32 errCode;
    BytesBufferNode * (*ToBytes)( struct DCMotor_Return *ths ); 
    int (*GetFromBytes)( struct DCMotor_Return *ths,u8* buff,int offset ); 
}DCMotor_Return;
typedef struct MotorCupIDSet_Send { 
    u16 m1_CupID;
    u32 m2_speed;
    u32 m3_acce;
    BytesBufferNode * (*ToBytes)( struct MotorCupIDSet_Send *ths ); 
    int (*GetFromBytes)( struct MotorCupIDSet_Send *ths,u8* buff,int offset ); 
}MotorCupIDSet_Send;
typedef struct MotorCupIDSet_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorCupIDSet_Return *ths ); 
    int (*GetFromBytes)( struct MotorCupIDSet_Return *ths,u8* buff,int offset ); 
}MotorCupIDSet_Return;
typedef struct MotorCupIDInfoGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorCupIDInfoGet_Send *ths ); 
    int (*GetFromBytes)( struct MotorCupIDInfoGet_Send *ths,u8* buff,int offset ); 
}MotorCupIDInfoGet_Send;
typedef struct MotorCupIDInfoGet_Return { 
    u8 m1_idx;
    u32 m2_errCode;
    u32 m3_turntableStatus;
    u16 m4_nowCupID;
    u8 m5_isinit;
    BytesBufferNode * (*ToBytes)( struct MotorCupIDInfoGet_Return *ths ); 
    int (*GetFromBytes)( struct MotorCupIDInfoGet_Return *ths,u8* buff,int offset ); 
}MotorCupIDInfoGet_Return;
typedef struct JSAdDataGet_Send { 
    u8 m1_channelIdx;
    u16 m2_timeSpan;
    u16 m3_dataCnt;
    BytesBufferNode * (*ToBytes)( struct JSAdDataGet_Send *ths ); 
    int (*GetFromBytes)( struct JSAdDataGet_Send *ths,u8* buff,int offset ); 
}JSAdDataGet_Send;
typedef struct JSAdDataGet_Return { 
    int m1_data_Length;
    u16* m1_data;
    BytesBufferNode * (*ToBytes)( struct JSAdDataGet_Return *ths ); 
    int (*GetFromBytes)( struct JSAdDataGet_Return *ths,u8* buff,int offset ); 
}JSAdDataGet_Return;
typedef struct MotorMultiMoveTmc_Send_m1_MoveArgs { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u32 m6_delayms;
    u8 m7_currentLevel;
    u16 m8_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMoveTmc_Send_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct MotorMultiMoveTmc_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
}MotorMultiMoveTmc_Send_m1_MoveArgs;
typedef struct MotorMultiMoveTmc_Send { 
    int m1_MoveArgs_Length;
    MotorMultiMoveTmc_Send_m1_MoveArgs* m1_MoveArgs;
    u8 m2_isBackZero;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMoveTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorMultiMoveTmc_Send *ths,u8* buff,int offset ); 
}MotorMultiMoveTmc_Send;
typedef struct MotorMultiMoveTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMultiMoveTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorMultiMoveTmc_Return *ths,u8* buff,int offset ); 
}MotorMultiMoveTmc_Return;
typedef struct MotorMoveTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m6_isBackZero;
    u8 m7_currentLevel;
    u16 m8_subdivision;
    u16 m9_backzero_position;
    BytesBufferNode * (*ToBytes)( struct MotorMoveTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveTmc_Send *ths,u8* buff,int offset ); 
}MotorMoveTmc_Send;
typedef struct MotorMoveTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorMoveTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveTmc_Return *ths,u8* buff,int offset ); 
}MotorMoveTmc_Return;
typedef struct MotorStatetGetTmc_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorStatetGetTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorStatetGetTmc_Send *ths,u8* buff,int offset ); 
}MotorStatetGetTmc_Send;
typedef struct MotorStatetGetTmc_Return { 
    u8 m1_isZeroTriggered;
    u32 m2_currentCordinate;
    u32 m3_lostSteps;
    u8 m4_isBusy;
    u8 m5_currentLevel;
    u16 m6_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorStatetGetTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorStatetGetTmc_Return *ths,u8* buff,int offset ); 
}MotorStatetGetTmc_Return;
typedef struct MotorRotateStartTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_isForward;
    u8 m5_currentLevel;
    u16 m6_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStartTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateStartTmc_Send *ths,u8* buff,int offset ); 
}MotorRotateStartTmc_Send;
typedef struct MotorRotateStartTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStartTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateStartTmc_Return *ths,u8* buff,int offset ); 
}MotorRotateStartTmc_Return;
typedef struct MotorRotateStopTmc_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStopTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateStopTmc_Send *ths,u8* buff,int offset ); 
}MotorRotateStopTmc_Send;
typedef struct MotorRotateStopTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateStopTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateStopTmc_Return *ths,u8* buff,int offset ); 
}MotorRotateStopTmc_Return;
typedef struct MotorRotateTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m6_currentLevel;
    u16 m7_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorRotateTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateTmc_Send *ths,u8* buff,int offset ); 
}MotorRotateTmc_Send;
typedef struct MotorRotateTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateTmc_Return *ths,u8* buff,int offset ); 
}MotorRotateTmc_Return;
typedef struct MotorResetTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_currentLevel;
    u16 m5_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorResetTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorResetTmc_Send *ths,u8* buff,int offset ); 
}MotorResetTmc_Send;
typedef struct MotorResetTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorResetTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorResetTmc_Return *ths,u8* buff,int offset ); 
}MotorResetTmc_Return;
typedef struct MotorConfigSetTmc_Send { 
    u8 m1_idx;
    u32 m2_zeroCordinate;
    u32 m3_maxCordinate;
    u32 m4_lostEndureContinue;
    u32 m5_lostEndureStop;
    u32 m6_zeroTrigVol;
    u32 m7_dirVol;
    u8 m8_isRotate;
    u8 m9_enableVol;
    u8 m10_halfVol;
    u8 m11_currentLevel;
    u16 m12_subdivision;
    u8 m13_holdLevel;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSetTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorConfigSetTmc_Send *ths,u8* buff,int offset ); 
}MotorConfigSetTmc_Send;
typedef struct MotorConfigSetTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorConfigSetTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorConfigSetTmc_Return *ths,u8* buff,int offset ); 
}MotorConfigSetTmc_Return;
typedef struct MotorMoveGPIOEventTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u16 m6_state_port_pin;
    u8 m7_currentLevel;
    u16 m8_subdivision;
    u16 m9_isdetect;
    u16 m10_movedelay;
    BytesBufferNode * (*ToBytes)( struct MotorMoveGPIOEventTmc_Send *ths ); 
    int (*GetFromBytes)( struct MotorMoveGPIOEventTmc_Send *ths,u8* buff,int offset ); 
}MotorMoveGPIOEventTmc_Send;
typedef struct MotorMoveGPIOEventTmc_Return { 
    u32 m1_errCode;
    u32 m2_trigger_pos;
    u32 m3_distance;
    BytesBufferNode * (*ToBytes)( struct MotorMoveGPIOEventTmc_Return *ths ); 
    int (*GetFromBytes)( struct MotorMoveGPIOEventTmc_Return *ths,u8* buff,int offset ); 
}MotorMoveGPIOEventTmc_Return;
typedef struct MotorRotateShake_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_dir;
    u8 m5_currentLevel;
    u16 m6_subdivision;
    u32 m7_step;
    BytesBufferNode * (*ToBytes)( struct MotorRotateShake_Send *ths ); 
    int (*GetFromBytes)( struct MotorRotateShake_Send *ths,u8* buff,int offset ); 
}MotorRotateShake_Send;
typedef struct MotorRotateShake_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct MotorRotateShake_Return *ths ); 
    int (*GetFromBytes)( struct MotorRotateShake_Return *ths,u8* buff,int offset ); 
}MotorRotateShake_Return;
typedef struct LiquidLevelResultGet_Send { 
    u32 m1_data;
    BytesBufferNode * (*ToBytes)( struct LiquidLevelResultGet_Send *ths ); 
    int (*GetFromBytes)( struct LiquidLevelResultGet_Send *ths,u8* buff,int offset ); 
}LiquidLevelResultGet_Send;
typedef struct LiquidLevelResultGet_Return { 
    u32 m1_trigger_pos;
    BytesBufferNode * (*ToBytes)( struct LiquidLevelResultGet_Return *ths ); 
    int (*GetFromBytes)( struct LiquidLevelResultGet_Return *ths,u8* buff,int offset ); 
}LiquidLevelResultGet_Return;
typedef struct MotorDetectLiquidLevel_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m7_currentLevel;
    u16 m8_subdivision;
    u8 m10_idx;
    u32 m11_acce;
    u32 m12_speed;
    u32 m13_distance;
    u8 m14_isRelative;
    u8 m15_currentLevel;
    u16 m16_subdivision;
    BytesBufferNode * (*ToBytes)( struct MotorDetectLiquidLevel_Send *ths ); 
    int (*GetFromBytes)( struct MotorDetectLiquidLevel_Send *ths,u8* buff,int offset ); 
}MotorDetectLiquidLevel_Send;
typedef struct MotorDetectLiquidLevel_Return { 
    u32 m1_errCode;
    u32 m3_distance;
    BytesBufferNode * (*ToBytes)( struct MotorDetectLiquidLevel_Return *ths ); 
    int (*GetFromBytes)( struct MotorDetectLiquidLevel_Return *ths,u8* buff,int offset ); 
}MotorDetectLiquidLevel_Return;
typedef struct RFIDValueOperation_Send { 
    u8 m1_op;
    int data_Length;
    u8* data;
    BytesBufferNode * (*ToBytes)( struct RFIDValueOperation_Send *ths ); 
    int (*GetFromBytes)( struct RFIDValueOperation_Send *ths,u8* buff,int offset ); 
}RFIDValueOperation_Send;
typedef struct RFIDValueOperation_Return { 
    u32 m1_errCode;
    int data_Length;
    u8* data;
    BytesBufferNode * (*ToBytes)( struct RFIDValueOperation_Return *ths ); 
    int (*GetFromBytes)( struct RFIDValueOperation_Return *ths,u8* buff,int offset ); 
}RFIDValueOperation_Return;
typedef struct IoSimple_Send { 
    u16 m1_port_pin_arg;
    BytesBufferNode * (*ToBytes)( struct IoSimple_Send *ths ); 
    int (*GetFromBytes)( struct IoSimple_Send *ths,u8* buff,int offset ); 
}IoSimple_Send;
typedef struct IoSimple_Return { 
    u32 m1_errCode_value;
    BytesBufferNode * (*ToBytes)( struct IoSimple_Return *ths ); 
    int (*GetFromBytes)( struct IoSimple_Return *ths,u8* buff,int offset ); 
}IoSimple_Return;
typedef struct IoOperation_Send_m1_IoArgs_m1_Controls { 
    u16 m1_port_pin;
    u8 m2_cmd_arg;
    BytesBufferNode * (*ToBytes)( struct IoOperation_Send_m1_IoArgs_m1_Controls *ths ); 
    int (*GetFromBytes)( struct IoOperation_Send_m1_IoArgs_m1_Controls *ths,u8* buff,int offset ); 
}IoOperation_Send_m1_IoArgs_m1_Controls;
typedef struct IoOperation_Send_m1_IoArgs { 
    int m1_Controls_Length;
    IoOperation_Send_m1_IoArgs_m1_Controls* m1_Controls;
    u32 m2_delay;
    BytesBufferNode * (*ToBytes)( struct IoOperation_Send_m1_IoArgs *ths ); 
    int (*GetFromBytes)( struct IoOperation_Send_m1_IoArgs *ths,u8* buff,int offset ); 
}IoOperation_Send_m1_IoArgs;
typedef struct IoOperation_Send { 
    int m1_IoArgs_Length;
    IoOperation_Send_m1_IoArgs* m1_IoArgs;
    BytesBufferNode * (*ToBytes)( struct IoOperation_Send *ths ); 
    int (*GetFromBytes)( struct IoOperation_Send *ths,u8* buff,int offset ); 
}IoOperation_Send;
typedef struct IoOperation_Return { 
    u32 m1_errCode;
    u32 m2_value;
    BytesBufferNode * (*ToBytes)( struct IoOperation_Return *ths ); 
    int (*GetFromBytes)( struct IoOperation_Return *ths,u8* buff,int offset ); 
}IoOperation_Return;
typedef struct IoBlink_Send { 
    u8 m1_port;
    u16 m2_pin;
    u8 m3_state;
    u32 m4_frequence;
    BytesBufferNode * (*ToBytes)( struct IoBlink_Send *ths ); 
    int (*GetFromBytes)( struct IoBlink_Send *ths,u8* buff,int offset ); 
}IoBlink_Send;
typedef struct IoBlink_Return { 
    u32 errCode;
    BytesBufferNode * (*ToBytes)( struct IoBlink_Return *ths ); 
    int (*GetFromBytes)( struct IoBlink_Return *ths,u8* buff,int offset ); 
}IoBlink_Return;
typedef struct UartChangeBode_Send { 
    u32 m1_uartIdx;
    u32 m2_bode;
    BytesBufferNode * (*ToBytes)( struct UartChangeBode_Send *ths ); 
    int (*GetFromBytes)( struct UartChangeBode_Send *ths,u8* buff,int offset ); 
}UartChangeBode_Send;
typedef struct UartChangeBode_Return { 
    u32 errCode;
    BytesBufferNode * (*ToBytes)( struct UartChangeBode_Return *ths ); 
    int (*GetFromBytes)( struct UartChangeBode_Return *ths,u8* buff,int offset ); 
}UartChangeBode_Return;
typedef struct ServoMotorMultiRotate_Send_m1_MoveArgs { 
    u8 m1_idx;
    u8 m2_dir;
    u32 m3_speed;
    u32 m4_acce;
    u32 m5_delayms;
    u8 m6_isSendSyncSigle;
    BytesBufferNode * (*ToBytes)( struct ServoMotorMultiRotate_Send_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct ServoMotorMultiRotate_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
}ServoMotorMultiRotate_Send_m1_MoveArgs;
typedef struct ServoMotorMultiRotate_Send { 
    int m1_MoveArgs_Length;
    ServoMotorMultiRotate_Send_m1_MoveArgs* m1_MoveArgs;
    BytesBufferNode * (*ToBytes)( struct ServoMotorMultiRotate_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorMultiRotate_Send *ths,u8* buff,int offset ); 
}ServoMotorMultiRotate_Send;
typedef struct ServoMotorMultiRotate_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorMultiRotate_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorMultiRotate_Return *ths,u8* buff,int offset ); 
}ServoMotorMultiRotate_Return;
typedef struct ServoMotorRotateShakeDetect_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_time;
    u8 m5_isForward;
    u32 m6_time;
    u8 m7_frequency;
    u32 m8_amplitude;
    u32 m9_acce;
    u32 m10_speed;
    u8 m11_isForward;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateShakeDetect_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateShakeDetect_Send *ths,u8* buff,int offset ); 
}ServoMotorRotateShakeDetect_Send;
typedef struct ServoMotorRotateShakeDetect_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateShakeDetect_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateShakeDetect_Return *ths,u8* buff,int offset ); 
}ServoMotorRotateShakeDetect_Return;
typedef struct ServoMotorRotateShakeRotateDetect_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_time;
    u8 m5_isForward;
    u32 m6_time;
    u8 m7_frequency;
    u32 m8_amplitude;
    u32 m9_acce;
    u32 m10_speed;
    u8 m11_isForward;
    u32 m12_time;
    u32 m13_acce;
    u32 m14_speed;
    u8 m15_isForward;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateShakeRotateDetect_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateShakeRotateDetect_Send *ths,u8* buff,int offset ); 
}ServoMotorRotateShakeRotateDetect_Send;
typedef struct ServoMotorRotateShakeRotateDetect_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateShakeRotateDetect_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateShakeRotateDetect_Return *ths,u8* buff,int offset ); 
}ServoMotorRotateShakeRotateDetect_Return;
typedef struct ServoMotorRotateTwoDirOnTime_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_isForward;
    u32 m5_time;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateTwoDirOnTime_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateTwoDirOnTime_Send *ths,u8* buff,int offset ); 
}ServoMotorRotateTwoDirOnTime_Send;
typedef struct ServoMotorRotateTwoDirOnTime_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateTwoDirOnTime_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateTwoDirOnTime_Return *ths,u8* buff,int offset ); 
}ServoMotorRotateTwoDirOnTime_Return;
typedef struct ServoMotorStatetGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct ServoMotorStatetGet_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorStatetGet_Send *ths,u8* buff,int offset ); 
}ServoMotorStatetGet_Send;
typedef struct ServoMotorStatetGet_Return { 
    u32 m1_errCode;
    u8 m2_isZeroTriggered;
    u32 m3_currentCordinate;
    u32 m4_currentSpeed;
    BytesBufferNode * (*ToBytes)( struct ServoMotorStatetGet_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorStatetGet_Return *ths,u8* buff,int offset ); 
}ServoMotorStatetGet_Return;
typedef struct ServoMotorRotateStart_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u8 m4_isForward;
    u8 m5_isSendSyncSigle;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateStart_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateStart_Send *ths,u8* buff,int offset ); 
}ServoMotorRotateStart_Send;
typedef struct ServoMotorRotateStart_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateStart_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateStart_Return *ths,u8* buff,int offset ); 
}ServoMotorRotateStart_Return;
typedef struct ServoMotorRotateStop_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateStop_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateStop_Send *ths,u8* buff,int offset ); 
}ServoMotorRotateStop_Send;
typedef struct ServoMotorRotateStop_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorRotateStop_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorRotateStop_Return *ths,u8* buff,int offset ); 
}ServoMotorRotateStop_Return;
typedef struct ServoMotorReset_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    BytesBufferNode * (*ToBytes)( struct ServoMotorReset_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorReset_Send *ths,u8* buff,int offset ); 
}ServoMotorReset_Send;
typedef struct ServoMotorReset_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorReset_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorReset_Return *ths,u8* buff,int offset ); 
}ServoMotorReset_Return;
typedef struct ServoMotorAutoCalibratio_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct ServoMotorAutoCalibratio_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorAutoCalibratio_Send *ths,u8* buff,int offset ); 
}ServoMotorAutoCalibratio_Send;
typedef struct ServoMotorAutoCalibratio_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorAutoCalibratio_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorAutoCalibratio_Return *ths,u8* buff,int offset ); 
}ServoMotorAutoCalibratio_Return;
typedef struct ServoMotorShake_Send { 
    u8 m1_idx;
    u32 m2_time;
    u8 m3_frequency;
    u32 m4_amplitude;
    BytesBufferNode * (*ToBytes)( struct ServoMotorShake_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorShake_Send *ths,u8* buff,int offset ); 
}ServoMotorShake_Send;
typedef struct ServoMotorShake_Return { 
    u32 m1_maxCordinate;
    u32 m2_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorShake_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorShake_Return *ths,u8* buff,int offset ); 
}ServoMotorShake_Return;
typedef struct ServoMotorBaseCtrl_Send { 
    u8 m1_idx;
    u8 m2_option;
    BytesBufferNode * (*ToBytes)( struct ServoMotorBaseCtrl_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorBaseCtrl_Send *ths,u8* buff,int offset ); 
}ServoMotorBaseCtrl_Send;
typedef struct ServoMotorBaseCtrl_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorBaseCtrl_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorBaseCtrl_Return *ths,u8* buff,int offset ); 
}ServoMotorBaseCtrl_Return;
typedef struct ServoMotorMove_Send { 
    u8 m1_idx;
    u32 m2_steps;
    u32 m3_speed;
    u32 m4_acce;
    u8 m5_isAbsolute;
    BytesBufferNode * (*ToBytes)( struct ServoMotorMove_Send *ths ); 
    int (*GetFromBytes)( struct ServoMotorMove_Send *ths,u8* buff,int offset ); 
}ServoMotorMove_Send;
typedef struct ServoMotorMove_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct ServoMotorMove_Return *ths ); 
    int (*GetFromBytes)( struct ServoMotorMove_Return *ths,u8* buff,int offset ); 
}ServoMotorMove_Return;
typedef struct RGBSensorGet_Send { 
    u8 m1_idx;
    BytesBufferNode * (*ToBytes)( struct RGBSensorGet_Send *ths ); 
    int (*GetFromBytes)( struct RGBSensorGet_Send *ths,u8* buff,int offset ); 
}RGBSensorGet_Send;
typedef struct RGBSensorGet_Return { 
    u32 m1_errCode;
    u32 m2_R;
    u32 m3_G;
    u32 m4_B;
    u32 m5_C;
    BytesBufferNode * (*ToBytes)( struct RGBSensorGet_Return *ths ); 
    int (*GetFromBytes)( struct RGBSensorGet_Return *ths,u8* buff,int offset ); 
}RGBSensorGet_Return;
typedef struct SerialDataReport_Send { 
    int m1_data_Length;
    u8* m1_data;
    BytesBufferNode * (*ToBytes)( struct SerialDataReport_Send *ths ); 
    int (*GetFromBytes)( struct SerialDataReport_Send *ths,u8* buff,int offset ); 
}SerialDataReport_Send;
typedef struct SerialDataReport_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct SerialDataReport_Return *ths ); 
    int (*GetFromBytes)( struct SerialDataReport_Return *ths,u8* buff,int offset ); 
}SerialDataReport_Return;
typedef struct AddressBroadcastSend_Send { 
    u32 m1_mode;
    u32 m2_address;
    u32 m3_count;
    int m4_addrRestrict_Length;
    u8* m4_addrRestrict;
    BytesBufferNode * (*ToBytes)( struct AddressBroadcastSend_Send *ths ); 
    int (*GetFromBytes)( struct AddressBroadcastSend_Send *ths,u8* buff,int offset ); 
}AddressBroadcastSend_Send;
typedef struct AddressBroadcastSend_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct AddressBroadcastSend_Return *ths ); 
    int (*GetFromBytes)( struct AddressBroadcastSend_Return *ths,u8* buff,int offset ); 
}AddressBroadcastSend_Return;
typedef struct AddressBroadcastReceive_Send { 
    u32 m1_address;
    BytesBufferNode * (*ToBytes)( struct AddressBroadcastReceive_Send *ths ); 
    int (*GetFromBytes)( struct AddressBroadcastReceive_Send *ths,u8* buff,int offset ); 
}AddressBroadcastReceive_Send;
typedef struct AddressBroadcastReceive_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct AddressBroadcastReceive_Return *ths ); 
    int (*GetFromBytes)( struct AddressBroadcastReceive_Return *ths,u8* buff,int offset ); 
}AddressBroadcastReceive_Return;
typedef struct StartCheckPress_Send { 
    u32 m1_address;
    BytesBufferNode * (*ToBytes)( struct StartCheckPress_Send *ths ); 
    int (*GetFromBytes)( struct StartCheckPress_Send *ths,u8* buff,int offset ); 
}StartCheckPress_Send;
typedef struct StartCheckPress_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct StartCheckPress_Return *ths ); 
    int (*GetFromBytes)( struct StartCheckPress_Return *ths,u8* buff,int offset ); 
}StartCheckPress_Return;
typedef struct EndCheckPress_Send { 
    u32 m1_address;
    BytesBufferNode * (*ToBytes)( struct EndCheckPress_Send *ths ); 
    int (*GetFromBytes)( struct EndCheckPress_Send *ths,u8* buff,int offset ); 
}EndCheckPress_Send;
typedef struct EndCheckPress_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct EndCheckPress_Return *ths ); 
    int (*GetFromBytes)( struct EndCheckPress_Return *ths,u8* buff,int offset ); 
}EndCheckPress_Return;
typedef struct Blending_Send { 
    u32 m1_address;
    u32 m2_speed;
    u32 m3_time;
    u32 m4_dir;
    BytesBufferNode * (*ToBytes)( struct Blending_Send *ths ); 
    int (*GetFromBytes)( struct Blending_Send *ths,u8* buff,int offset ); 
}Blending_Send;
typedef struct Blending_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct Blending_Return *ths ); 
    int (*GetFromBytes)( struct Blending_Return *ths,u8* buff,int offset ); 
}Blending_Return;
typedef struct GetEPOSType_Send { 
    u32 m1_idx;
    BytesBufferNode * (*ToBytes)( struct GetEPOSType_Send *ths ); 
    int (*GetFromBytes)( struct GetEPOSType_Send *ths,u8* buff,int offset ); 
}GetEPOSType_Send;
typedef struct GetEPOSType_Return { 
    u8 m1_motorType;
    u32 m2_errCode;
    BytesBufferNode * (*ToBytes)( struct GetEPOSType_Return *ths ); 
    int (*GetFromBytes)( struct GetEPOSType_Return *ths,u8* buff,int offset ); 
}GetEPOSType_Return;
typedef struct SetEPOSType_Send { 
    u32 m1_idx;
    u8 m2_typeId;
    BytesBufferNode * (*ToBytes)( struct SetEPOSType_Send *ths ); 
    int (*GetFromBytes)( struct SetEPOSType_Send *ths,u8* buff,int offset ); 
}SetEPOSType_Send;
typedef struct SetEPOSType_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct SetEPOSType_Return *ths ); 
    int (*GetFromBytes)( struct SetEPOSType_Return *ths,u8* buff,int offset ); 
}SetEPOSType_Return;
typedef struct DACSet_Send { 
    u16 m1_value;
    BytesBufferNode * (*ToBytes)( struct DACSet_Send *ths ); 
    int (*GetFromBytes)( struct DACSet_Send *ths,u8* buff,int offset ); 
}DACSet_Send;
typedef struct DACSet_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct DACSet_Return *ths ); 
    int (*GetFromBytes)( struct DACSet_Return *ths,u8* buff,int offset ); 
}DACSet_Return;
typedef struct LightCtrl_Send { 
    u8 idx;
    u32 value;
    BytesBufferNode * (*ToBytes)( struct LightCtrl_Send *ths ); 
    int (*GetFromBytes)( struct LightCtrl_Send *ths,u8* buff,int offset ); 
}LightCtrl_Send;
typedef struct LightCtrl_Return { 
    u32 errCode;
    BytesBufferNode * (*ToBytes)( struct LightCtrl_Return *ths ); 
    int (*GetFromBytes)( struct LightCtrl_Return *ths,u8* buff,int offset ); 
}LightCtrl_Return;
typedef struct DCMotorMoveSeq_Send_m1_MoveArgs { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u32 m6_delayms;
    u8 m7_currentLevel;
    u8 m2_isBackZero;
    BytesBufferNode * (*ToBytes)( struct DCMotorMoveSeq_Send_m1_MoveArgs *ths ); 
    int (*GetFromBytes)( struct DCMotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
}DCMotorMoveSeq_Send_m1_MoveArgs;
typedef struct DCMotorMoveSeq_Send { 
    int m1_MoveArgs_Length;
    DCMotorMoveSeq_Send_m1_MoveArgs* m1_MoveArgs;
    u32 m2_WaitSyncTime;
    BytesBufferNode * (*ToBytes)( struct DCMotorMoveSeq_Send *ths ); 
    int (*GetFromBytes)( struct DCMotorMoveSeq_Send *ths,u8* buff,int offset ); 
}DCMotorMoveSeq_Send;
typedef struct DCMotorMoveSeq_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct DCMotorMoveSeq_Return *ths ); 
    int (*GetFromBytes)( struct DCMotorMoveSeq_Return *ths,u8* buff,int offset ); 
}DCMotorMoveSeq_Return;
typedef struct DCMotorMoveTmc_Send { 
    u8 m1_idx;
    u32 m2_acce;
    u32 m3_speed;
    u32 m4_distance;
    u8 m5_isRelative;
    u8 m6_isBackZero;
    u8 m7_currentLevel;
    u16 m8_subdivision;
    u16 m9_backzero_position;
    u8 m10_writeSyncFlag;
    BytesBufferNode * (*ToBytes)( struct DCMotorMoveTmc_Send *ths ); 
    int (*GetFromBytes)( struct DCMotorMoveTmc_Send *ths,u8* buff,int offset ); 
}DCMotorMoveTmc_Send;
typedef struct DCMotorMoveTmc_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct DCMotorMoveTmc_Return *ths ); 
    int (*GetFromBytes)( struct DCMotorMoveTmc_Return *ths,u8* buff,int offset ); 
}DCMotorMoveTmc_Return;
typedef struct PressureRead_Send { 
    u8 m1_id;
    BytesBufferNode * (*ToBytes)( struct PressureRead_Send *ths ); 
    int (*GetFromBytes)( struct PressureRead_Send *ths,u8* buff,int offset ); 
}PressureRead_Send;
typedef struct PressureRead_Return { 
    float m1_pressure1;
    float m2_pressure2;
    u32 m3_errCode;
    BytesBufferNode * (*ToBytes)( struct PressureRead_Return *ths ); 
    int (*GetFromBytes)( struct PressureRead_Return *ths,u8* buff,int offset ); 
}PressureRead_Return;
typedef struct LittleMotorCtrl_Send { 
    u8 m1_flag;
    u32 m2_period;
    u32 m3_dutyCycle;
    BytesBufferNode * (*ToBytes)( struct LittleMotorCtrl_Send *ths ); 
    int (*GetFromBytes)( struct LittleMotorCtrl_Send *ths,u8* buff,int offset ); 
}LittleMotorCtrl_Send;
typedef struct LittleMotorCtrl_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct LittleMotorCtrl_Return *ths ); 
    int (*GetFromBytes)( struct LittleMotorCtrl_Return *ths,u8* buff,int offset ); 
}LittleMotorCtrl_Return;
typedef struct CongestionDetection_Send { 
    u8 m1_status;
    u16 m2_threshold;
    BytesBufferNode * (*ToBytes)( struct CongestionDetection_Send *ths ); 
    int (*GetFromBytes)( struct CongestionDetection_Send *ths,u8* buff,int offset ); 
}CongestionDetection_Send;
typedef struct CongestionDetection_Return { 
    u8 m1_status;
    u16 m2_ad_value;
    u32 m3_errCode;
    BytesBufferNode * (*ToBytes)( struct CongestionDetection_Return *ths ); 
    int (*GetFromBytes)( struct CongestionDetection_Return *ths,u8* buff,int offset ); 
}CongestionDetection_Return;
typedef struct LaserCtrol_Send { 
    u8 m1_status;
    BytesBufferNode * (*ToBytes)( struct LaserCtrol_Send *ths ); 
    int (*GetFromBytes)( struct LaserCtrol_Send *ths,u8* buff,int offset ); 
}LaserCtrol_Send;
typedef struct LaserCtrol_Return { 
    u32 m1_errCode;
    BytesBufferNode * (*ToBytes)( struct LaserCtrol_Return *ths ); 
    int (*GetFromBytes)( struct LaserCtrol_Return *ths,u8* buff,int offset ); 
}LaserCtrol_Return;
typedef struct NFL_ReadConfig_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadConfig_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ReadConfig_Send *ths,u8* buff,int offset ); 
}NFL_ReadConfig_Send;
typedef struct NFL_ReadConfig_Return { 
    u32 cmd;
    u32 ErrorCode;
    float offset0;
    float volt0;
    u16 delay0;
    float offset1;
    float volt1;
    u16 delay1;
    float offset2;
    float volt2;
    u16 delay2;
    float offset3;
    float volt3;
    u16 delay3;
    float offset4;
    float volt4;
    u16 delay4;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadConfig_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ReadConfig_Return *ths,u8* buff,int offset ); 
}NFL_ReadConfig_Return;
typedef struct NFL_WriteConfig_Send { 
    float offset0;
    float volt0;
    u16 delay0;
    float offset1;
    float volt1;
    u16 delay1;
    float offset2;
    float volt2;
    u16 delay2;
    float offset3;
    float volt3;
    u16 delay3;
    float offset4;
    float volt4;
    u16 delay4;
    BytesBufferNode * (*ToBytes)( struct NFL_WriteConfig_Send *ths ); 
    int (*GetFromBytes)( struct NFL_WriteConfig_Send *ths,u8* buff,int offset ); 
}NFL_WriteConfig_Send;
typedef struct NFL_WriteConfig_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_WriteConfig_Return *ths ); 
    int (*GetFromBytes)( struct NFL_WriteConfig_Return *ths,u8* buff,int offset ); 
}NFL_WriteConfig_Return;
typedef struct NFL_ReadDataStatus_Send { 
    u32 wait_time_ms;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadDataStatus_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ReadDataStatus_Send *ths,u8* buff,int offset ); 
}NFL_ReadDataStatus_Send;
typedef struct NFL_ReadDataStatus_Return { 
    u32 cmd;
    u32 ErrorCode;
    u32 data_status;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadDataStatus_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ReadDataStatus_Return *ths,u8* buff,int offset ); 
}NFL_ReadDataStatus_Return;
typedef struct NFL_ReadData_Send { 
    u32 channel;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadData_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ReadData_Send *ths,u8* buff,int offset ); 
}NFL_ReadData_Send;
typedef struct NFL_ReadData_Return { 
    u32 cmd;
    u32 ErrorCode;
    int data_Length;
    u8* data;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadData_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ReadData_Return *ths,u8* buff,int offset ); 
}NFL_ReadData_Return;
typedef struct NFL_DataCollectStart_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_DataCollectStart_Send *ths ); 
    int (*GetFromBytes)( struct NFL_DataCollectStart_Send *ths,u8* buff,int offset ); 
}NFL_DataCollectStart_Send;
typedef struct NFL_DataCollectStart_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_DataCollectStart_Return *ths ); 
    int (*GetFromBytes)( struct NFL_DataCollectStart_Return *ths,u8* buff,int offset ); 
}NFL_DataCollectStart_Return;
typedef struct NFL_DataCollectStop_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_DataCollectStop_Send *ths ); 
    int (*GetFromBytes)( struct NFL_DataCollectStop_Send *ths,u8* buff,int offset ); 
}NFL_DataCollectStop_Send;
typedef struct NFL_DataCollectStop_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_DataCollectStop_Return *ths ); 
    int (*GetFromBytes)( struct NFL_DataCollectStop_Return *ths,u8* buff,int offset ); 
}NFL_DataCollectStop_Return;
typedef struct NFL_ThresholdConfig_Send { 
    u16 threshold;
    BytesBufferNode * (*ToBytes)( struct NFL_ThresholdConfig_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ThresholdConfig_Send *ths,u8* buff,int offset ); 
}NFL_ThresholdConfig_Send;
typedef struct NFL_ThresholdConfig_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_ThresholdConfig_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ThresholdConfig_Return *ths,u8* buff,int offset ); 
}NFL_ThresholdConfig_Return;
typedef struct NFL_StandbyConfig_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_StandbyConfig_Send *ths ); 
    int (*GetFromBytes)( struct NFL_StandbyConfig_Send *ths,u8* buff,int offset ); 
}NFL_StandbyConfig_Send;
typedef struct NFL_StandbyConfig_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_StandbyConfig_Return *ths ); 
    int (*GetFromBytes)( struct NFL_StandbyConfig_Return *ths,u8* buff,int offset ); 
}NFL_StandbyConfig_Return;
typedef struct NFL_ActivateConfig_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_ActivateConfig_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ActivateConfig_Send *ths,u8* buff,int offset ); 
}NFL_ActivateConfig_Send;
typedef struct NFL_ActivateConfig_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_ActivateConfig_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ActivateConfig_Return *ths,u8* buff,int offset ); 
}NFL_ActivateConfig_Return;
typedef struct NFL_ReadTemperature_Send { 
    u32 data;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadTemperature_Send *ths ); 
    int (*GetFromBytes)( struct NFL_ReadTemperature_Send *ths,u8* buff,int offset ); 
}NFL_ReadTemperature_Send;
typedef struct NFL_ReadTemperature_Return { 
    u32 cmd;
    u32 ErrorCode;
    float temperature;
    BytesBufferNode * (*ToBytes)( struct NFL_ReadTemperature_Return *ths ); 
    int (*GetFromBytes)( struct NFL_ReadTemperature_Return *ths,u8* buff,int offset ); 
}NFL_ReadTemperature_Return;
typedef struct NFL_SendEvent_Send { 
    u32 ErrorCode;
    u32 length;
    int data_Length;
    u8* data;
    BytesBufferNode * (*ToBytes)( struct NFL_SendEvent_Send *ths ); 
    int (*GetFromBytes)( struct NFL_SendEvent_Send *ths,u8* buff,int offset ); 
}NFL_SendEvent_Send;
typedef struct NFL_SendEvent_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct NFL_SendEvent_Return *ths ); 
    int (*GetFromBytes)( struct NFL_SendEvent_Return *ths,u8* buff,int offset ); 
}NFL_SendEvent_Return;
typedef struct PowerCtrl_Send { 
    u16 PcPower;
    u16 MotorPower;
    u16 TemperaturePower;
    u16 DataCollectionPower;
    BytesBufferNode * (*ToBytes)( struct PowerCtrl_Send *ths ); 
    int (*GetFromBytes)( struct PowerCtrl_Send *ths,u8* buff,int offset ); 
}PowerCtrl_Send;
typedef struct PowerCtrl_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct PowerCtrl_Return *ths ); 
    int (*GetFromBytes)( struct PowerCtrl_Return *ths,u8* buff,int offset ); 
}PowerCtrl_Return;
typedef struct SendErrorCode_Send { 
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct SendErrorCode_Send *ths ); 
    int (*GetFromBytes)( struct SendErrorCode_Send *ths,u8* buff,int offset ); 
}SendErrorCode_Send;
typedef struct SendErrorCode_Return { 
    u32 cmd;
    u32 ErrorCode;
    BytesBufferNode * (*ToBytes)( struct SendErrorCode_Return *ths ); 
    int (*GetFromBytes)( struct SendErrorCode_Return *ths,u8* buff,int offset ); 
}SendErrorCode_Return;
typedef struct IOCtrlByEvent_Send_m1_PortArgs { 
    u32 m1_state;
    u16 m2_delay;
    BytesBufferNode * (*ToBytes)( struct IOCtrlByEvent_Send_m1_PortArgs *ths ); 
    int (*GetFromBytes)( struct IOCtrlByEvent_Send_m1_PortArgs *ths,u8* buff,int offset ); 
}IOCtrlByEvent_Send_m1_PortArgs;
typedef struct IOCtrlByEvent_Send_m2_PortArgs { 
    u32 m1_state;
    u16 m2_delay;
    BytesBufferNode * (*ToBytes)( struct IOCtrlByEvent_Send_m2_PortArgs *ths ); 
    int (*GetFromBytes)( struct IOCtrlByEvent_Send_m2_PortArgs *ths,u8* buff,int offset ); 
}IOCtrlByEvent_Send_m2_PortArgs;
typedef struct IOCtrlByEvent_Send { 
    int m1_PortArgs_Length;
    IOCtrlByEvent_Send_m1_PortArgs* m1_PortArgs;
    int m2_PortArgs_Length;
    IOCtrlByEvent_Send_m2_PortArgs* m2_PortArgs;
    u32 m3_id_event_threshold;
    u32 m4_timeout;
    BytesBufferNode * (*ToBytes)( struct IOCtrlByEvent_Send *ths ); 
    int (*GetFromBytes)( struct IOCtrlByEvent_Send *ths,u8* buff,int offset ); 
}IOCtrlByEvent_Send;
typedef struct IOCtrlByEvent_Return { 
    u8 m1_error_code;
    BytesBufferNode * (*ToBytes)( struct IOCtrlByEvent_Return *ths ); 
    int (*GetFromBytes)( struct IOCtrlByEvent_Return *ths,u8* buff,int offset ); 
}IOCtrlByEvent_Return;
//------------- 函数声明 ----------------------------------
BytesBufferNode * Dsp_DHCP_Send_ToBytes(Dsp_DHCP_Send *ths ); 
BytesBufferNode * Dsp_DHCP_Return_ToBytes(Dsp_DHCP_Return *ths ); 
BytesBufferNode * Ping_Send_ToBytes(Ping_Send *ths ); 
BytesBufferNode * Ping_Return_ToBytes(Ping_Return *ths ); 
BytesBufferNode * FilePush_Send_ToBytes(FilePush_Send *ths ); 
BytesBufferNode * FilePush_Return_ToBytes(FilePush_Return *ths ); 
BytesBufferNode * InstrumentStateReport_Send_ToBytes(InstrumentStateReport_Send *ths ); 
BytesBufferNode * InstrumentStateReport_Return_ToBytes(InstrumentStateReport_Return *ths ); 
BytesBufferNode * ProtoTest_Send_ToBytes(ProtoTest_Send *ths ); 
BytesBufferNode * ProtoTest_Return_ToBytes(ProtoTest_Return *ths ); 
BytesBufferNode * McuErrorToPC_Send_ToBytes(McuErrorToPC_Send *ths ); 
BytesBufferNode * McuErrorToPC_Return_ToBytes(McuErrorToPC_Return *ths ); 
BytesBufferNode * AutoAdapter_Send_ToBytes(AutoAdapter_Send *ths ); 
BytesBufferNode * AutoAdapter_Return_ToBytes(AutoAdapter_Return *ths ); 
BytesBufferNode * MotorMultiMove_Send_m1_MoveArgs_ToBytes(MotorMultiMove_Send_m1_MoveArgs *ths ); 
BytesBufferNode * MotorMultiMove_Send_ToBytes(MotorMultiMove_Send *ths ); 
BytesBufferNode * MotorMultiMove_Return_ToBytes(MotorMultiMove_Return *ths ); 
BytesBufferNode * MotorMove_Send_ToBytes(MotorMove_Send *ths ); 
BytesBufferNode * MotorMove_Return_ToBytes(MotorMove_Return *ths ); 
BytesBufferNode * MotorStatetGet_Send_ToBytes(MotorStatetGet_Send *ths ); 
BytesBufferNode * MotorStatetGet_Return_ToBytes(MotorStatetGet_Return *ths ); 
BytesBufferNode * MotorRotateStart_Send_ToBytes(MotorRotateStart_Send *ths ); 
BytesBufferNode * MotorRotateStart_Return_ToBytes(MotorRotateStart_Return *ths ); 
BytesBufferNode * MotorRotateStop_Send_ToBytes(MotorRotateStop_Send *ths ); 
BytesBufferNode * MotorRotateStop_Return_ToBytes(MotorRotateStop_Return *ths ); 
BytesBufferNode * MotorRotate_Send_ToBytes(MotorRotate_Send *ths ); 
BytesBufferNode * MotorRotate_Return_ToBytes(MotorRotate_Return *ths ); 
BytesBufferNode * MotorReset_Send_ToBytes(MotorReset_Send *ths ); 
BytesBufferNode * MotorReset_Return_ToBytes(MotorReset_Return *ths ); 
BytesBufferNode * SensorValueOperation_Send_ToBytes(SensorValueOperation_Send *ths ); 
BytesBufferNode * SensorValueOperation_Return_ToBytes(SensorValueOperation_Return *ths ); 
BytesBufferNode * ADAutoCaliStart_Send_ToBytes(ADAutoCaliStart_Send *ths ); 
BytesBufferNode * ADAutoCaliStart_Return_ToBytes(ADAutoCaliStart_Return *ths ); 
BytesBufferNode * TemperatureGet_Send_ToBytes(TemperatureGet_Send *ths ); 
BytesBufferNode * TemperatureGet_Return_ToBytes(TemperatureGet_Return *ths ); 
BytesBufferNode * TemperatureOpen_Send_ToBytes(TemperatureOpen_Send *ths ); 
BytesBufferNode * TemperatureOpen_Return_ToBytes(TemperatureOpen_Return *ths ); 
BytesBufferNode * TemperatureClose_Send_ToBytes(TemperatureClose_Send *ths ); 
BytesBufferNode * TemperatureClose_Return_ToBytes(TemperatureClose_Return *ths ); 
BytesBufferNode * BarCodeGet_Send_ToBytes(BarCodeGet_Send *ths ); 
BytesBufferNode * BarCodeGet_Return_ToBytes(BarCodeGet_Return *ths ); 
BytesBufferNode * FirmWareConfig_Send_ToBytes(FirmWareConfig_Send *ths ); 
BytesBufferNode * FirmWareConfig_Return_ToBytes(FirmWareConfig_Return *ths ); 
BytesBufferNode * FirmWareTransmitFile_Send_ToBytes(FirmWareTransmitFile_Send *ths ); 
BytesBufferNode * FirmWareTransmitFile_Return_ToBytes(FirmWareTransmitFile_Return *ths ); 
BytesBufferNode * McuReset_Send_ToBytes(McuReset_Send *ths ); 
BytesBufferNode * McuReset_Return_ToBytes(McuReset_Return *ths ); 
BytesBufferNode * FirmWareVersionGet_Send_ToBytes(FirmWareVersionGet_Send *ths ); 
BytesBufferNode * FirmWareVersionGet_Return_ToBytes(FirmWareVersionGet_Return *ths ); 
BytesBufferNode * MotorConfigSet_Send_ToBytes(MotorConfigSet_Send *ths ); 
BytesBufferNode * MotorConfigSet_Return_ToBytes(MotorConfigSet_Return *ths ); 
BytesBufferNode * MotorMoveEvent_Send_ToBytes(MotorMoveEvent_Send *ths ); 
BytesBufferNode * MotorMoveEvent_Return_ToBytes(MotorMoveEvent_Return *ths ); 
BytesBufferNode * McuIoStateChangeEvent_Send_ToBytes(McuIoStateChangeEvent_Send *ths ); 
BytesBufferNode * McuIoStateChangeEvent_Return_ToBytes(McuIoStateChangeEvent_Return *ths ); 
BytesBufferNode * SampleUnitSensorGet_Send_ToBytes(SampleUnitSensorGet_Send *ths ); 
BytesBufferNode * SampleUnitSensorGet_Return_ToBytes(SampleUnitSensorGet_Return *ths ); 
BytesBufferNode * AirPumpOpen_Send_ToBytes(AirPumpOpen_Send *ths ); 
BytesBufferNode * AirPumpOpen_Return_ToBytes(AirPumpOpen_Return *ths ); 
BytesBufferNode * AirPumpClose_Send_ToBytes(AirPumpClose_Send *ths ); 
BytesBufferNode * AirPumpClose_Return_ToBytes(AirPumpClose_Return *ths ); 
BytesBufferNode * ADSampleRead_Send_ToBytes(ADSampleRead_Send *ths ); 
BytesBufferNode * ADSampleRead_Return_ToBytes(ADSampleRead_Return *ths ); 
BytesBufferNode * MotorMoveGPIOEvent_Send_ToBytes(MotorMoveGPIOEvent_Send *ths ); 
BytesBufferNode * MotorMoveGPIOEvent_Return_ToBytes(MotorMoveGPIOEvent_Return *ths ); 
BytesBufferNode * MotorConfigSetNew_Send_ToBytes(MotorConfigSetNew_Send *ths ); 
BytesBufferNode * MotorConfigSetNew_Return_ToBytes(MotorConfigSetNew_Return *ths ); 
BytesBufferNode * ExecutesCtrl_Send_ToBytes(ExecutesCtrl_Send *ths ); 
BytesBufferNode * ExecutesCtrl_Return_ToBytes(ExecutesCtrl_Return *ths ); 
BytesBufferNode * ADDetectConfigSet_Send_ToBytes(ADDetectConfigSet_Send *ths ); 
BytesBufferNode * ADDetectConfigSet_Return_ToBytes(ADDetectConfigSet_Return *ths ); 
BytesBufferNode * ADDataRead_Send_ToBytes(ADDataRead_Send *ths ); 
BytesBufferNode * ADDataRead_Return_ToBytes(ADDataRead_Return *ths ); 
/*
这里添加数据接口 Tobytes成员函数的定义
*/

BytesBufferNode * DiskInstalledRed_Send_ToBytes(DiskInstalledRed_Send *ths);
BytesBufferNode * DiskInstalledRed_Return_ToBytes(DiskInstalledRed_Return *ths ); 



BytesBufferNode * ADDataSeriesSend_Send_ToBytes(ADDataSeriesSend_Send *ths ); 
BytesBufferNode * ADDataSeriesSend_Return_ToBytes(ADDataSeriesSend_Return *ths ); 
BytesBufferNode * UartTransmit_Send_ToBytes(UartTransmit_Send *ths ); 
BytesBufferNode * UartTransmit_Return_ToBytes(UartTransmit_Return *ths ); 
BytesBufferNode * UartReceiveEvent_Send_ToBytes(UartReceiveEvent_Send *ths ); 
BytesBufferNode * UartReceiveEvent_Return_ToBytes(UartReceiveEvent_Return *ths ); 
BytesBufferNode * SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_ToBytes(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths ); 
BytesBufferNode * SeqMotorMultiMove_Send_m1_Sequence_ToBytes(SeqMotorMultiMove_Send_m1_Sequence *ths ); 
BytesBufferNode * SeqMotorMultiMove_Send_ToBytes(SeqMotorMultiMove_Send *ths ); 
BytesBufferNode * SeqMotorMultiMove_Return_ToBytes(SeqMotorMultiMove_Return *ths ); 
BytesBufferNode * PressureConfig_Send_ToBytes(PressureConfig_Send *ths ); 
BytesBufferNode * PressureConfig_Return_ToBytes(PressureConfig_Return *ths ); 
BytesBufferNode * PressureDataGet_Send_ToBytes(PressureDataGet_Send *ths ); 
BytesBufferNode * PressureDataGet_Return_ToBytes(PressureDataGet_Return *ths ); 
BytesBufferNode * MotorAbort_Send_ToBytes(MotorAbort_Send *ths ); 
BytesBufferNode * MotorAbort_Return_ToBytes(MotorAbort_Return *ths ); 
BytesBufferNode * PressureEvent_Send_ToBytes(PressureEvent_Send *ths ); 
BytesBufferNode * PressureEvent_Return_ToBytes(PressureEvent_Return *ths ); 
BytesBufferNode * MotorMoveSeq_Send_m1_MoveArgs_ToBytes(MotorMoveSeq_Send_m1_MoveArgs *ths ); 
BytesBufferNode * MotorMoveSeq_Send_ToBytes(MotorMoveSeq_Send *ths ); 
BytesBufferNode * MotorMoveSeq_Return_ToBytes(MotorMoveSeq_Return *ths ); 
BytesBufferNode * FirmWareReadyEvent_Send_ToBytes(FirmWareReadyEvent_Send *ths ); 
BytesBufferNode * FirmWareReadyEvent_Return_ToBytes(FirmWareReadyEvent_Return *ths ); 
BytesBufferNode * MotorConfigGet_Send_ToBytes(MotorConfigGet_Send *ths ); 
BytesBufferNode * MotorConfigGet_Return_ToBytes(MotorConfigGet_Return *ths ); 
BytesBufferNode * PWM_Set_Send_ToBytes(PWM_Set_Send *ths ); 
BytesBufferNode * PWM_Set_Return_ToBytes(PWM_Set_Return *ths ); 
BytesBufferNode * TurnTable_Move_Send_ToBytes(TurnTable_Move_Send *ths ); 
BytesBufferNode * TurnTable_Move_Return_ToBytes(TurnTable_Move_Return *ths ); 
BytesBufferNode * EncodeTurnTableFindZero_Send_ToBytes(EncodeTurnTableFindZero_Send *ths ); 
BytesBufferNode * EncodeTurnTableFindZero_Return_ToBytes(EncodeTurnTableFindZero_Return *ths ); 
BytesBufferNode * EncodeTurnTableRotateStart_Send_ToBytes(EncodeTurnTableRotateStart_Send *ths ); 
BytesBufferNode * EncodeTurnTableRotateStart_Return_ToBytes(EncodeTurnTableRotateStart_Return *ths ); 
BytesBufferNode * EncodeTurnTableRotateStop_Send_ToBytes(EncodeTurnTableRotateStop_Send *ths ); 
BytesBufferNode * EncodeTurnTableRotateStop_Return_ToBytes(EncodeTurnTableRotateStop_Return *ths ); 
BytesBufferNode * EncodeTurnTableStateGet_Send_ToBytes(EncodeTurnTableStateGet_Send *ths ); 
BytesBufferNode * EncodeTurnTableStateGet_Return_ToBytes(EncodeTurnTableStateGet_Return *ths ); 
BytesBufferNode * DCAdRead_Send_ToBytes(DCAdRead_Send *ths ); 
BytesBufferNode * DCAdRead_Return_ToBytes(DCAdRead_Return *ths ); 
BytesBufferNode * JSAdRead_Send_ToBytes(JSAdRead_Send *ths ); 
BytesBufferNode * JSAdRead_Return_ToBytes(JSAdRead_Return *ths ); 
BytesBufferNode * MotorMoveSyncLevel_Send_ToBytes(MotorMoveSyncLevel_Send *ths ); 
BytesBufferNode * MotorMoveSyncLevel_Return_ToBytes(MotorMoveSyncLevel_Return *ths ); 
BytesBufferNode * DCMotor_Send_ToBytes(DCMotor_Send *ths ); 
BytesBufferNode * DCMotor_Return_ToBytes(DCMotor_Return *ths ); 
BytesBufferNode * MotorCupIDSet_Send_ToBytes(MotorCupIDSet_Send *ths ); 
BytesBufferNode * MotorCupIDSet_Return_ToBytes(MotorCupIDSet_Return *ths ); 
BytesBufferNode * MotorCupIDInfoGet_Send_ToBytes(MotorCupIDInfoGet_Send *ths ); 
BytesBufferNode * MotorCupIDInfoGet_Return_ToBytes(MotorCupIDInfoGet_Return *ths ); 
BytesBufferNode * JSAdDataGet_Send_ToBytes(JSAdDataGet_Send *ths ); 
BytesBufferNode * JSAdDataGet_Return_ToBytes(JSAdDataGet_Return *ths ); 
BytesBufferNode * MotorMultiMoveTmc_Send_m1_MoveArgs_ToBytes(MotorMultiMoveTmc_Send_m1_MoveArgs *ths ); 
BytesBufferNode * MotorMultiMoveTmc_Send_ToBytes(MotorMultiMoveTmc_Send *ths ); 
BytesBufferNode * MotorMultiMoveTmc_Return_ToBytes(MotorMultiMoveTmc_Return *ths ); 
BytesBufferNode * MotorMoveTmc_Send_ToBytes(MotorMoveTmc_Send *ths ); 
BytesBufferNode * MotorMoveTmc_Return_ToBytes(MotorMoveTmc_Return *ths ); 
BytesBufferNode * MotorStatetGetTmc_Send_ToBytes(MotorStatetGetTmc_Send *ths ); 
BytesBufferNode * MotorStatetGetTmc_Return_ToBytes(MotorStatetGetTmc_Return *ths ); 
BytesBufferNode * MotorRotateStartTmc_Send_ToBytes(MotorRotateStartTmc_Send *ths ); 
BytesBufferNode * MotorRotateStartTmc_Return_ToBytes(MotorRotateStartTmc_Return *ths ); 
BytesBufferNode * MotorRotateStopTmc_Send_ToBytes(MotorRotateStopTmc_Send *ths ); 
BytesBufferNode * MotorRotateStopTmc_Return_ToBytes(MotorRotateStopTmc_Return *ths ); 
BytesBufferNode * MotorRotateTmc_Send_ToBytes(MotorRotateTmc_Send *ths ); 
BytesBufferNode * MotorRotateTmc_Return_ToBytes(MotorRotateTmc_Return *ths ); 
BytesBufferNode * MotorResetTmc_Send_ToBytes(MotorResetTmc_Send *ths ); 
BytesBufferNode * MotorResetTmc_Return_ToBytes(MotorResetTmc_Return *ths ); 
BytesBufferNode * MotorConfigSetTmc_Send_ToBytes(MotorConfigSetTmc_Send *ths ); 
BytesBufferNode * MotorConfigSetTmc_Return_ToBytes(MotorConfigSetTmc_Return *ths ); 
BytesBufferNode * MotorMoveGPIOEventTmc_Send_ToBytes(MotorMoveGPIOEventTmc_Send *ths ); 
BytesBufferNode * MotorMoveGPIOEventTmc_Return_ToBytes(MotorMoveGPIOEventTmc_Return *ths ); 
BytesBufferNode * MotorRotateShake_Send_ToBytes(MotorRotateShake_Send *ths ); 
BytesBufferNode * MotorRotateShake_Return_ToBytes(MotorRotateShake_Return *ths ); 
BytesBufferNode * LiquidLevelResultGet_Send_ToBytes(LiquidLevelResultGet_Send *ths ); 
BytesBufferNode * LiquidLevelResultGet_Return_ToBytes(LiquidLevelResultGet_Return *ths ); 
BytesBufferNode * MotorDetectLiquidLevel_Send_ToBytes(MotorDetectLiquidLevel_Send *ths ); 
BytesBufferNode * MotorDetectLiquidLevel_Return_ToBytes(MotorDetectLiquidLevel_Return *ths ); 
BytesBufferNode * RFIDValueOperation_Send_ToBytes(RFIDValueOperation_Send *ths ); 
BytesBufferNode * RFIDValueOperation_Return_ToBytes(RFIDValueOperation_Return *ths ); 
BytesBufferNode * IoSimple_Send_ToBytes(IoSimple_Send *ths ); 
BytesBufferNode * IoSimple_Return_ToBytes(IoSimple_Return *ths ); 
BytesBufferNode * IoOperation_Send_m1_IoArgs_m1_Controls_ToBytes(IoOperation_Send_m1_IoArgs_m1_Controls *ths ); 
BytesBufferNode * IoOperation_Send_m1_IoArgs_ToBytes(IoOperation_Send_m1_IoArgs *ths ); 
BytesBufferNode * IoOperation_Send_ToBytes(IoOperation_Send *ths ); 
BytesBufferNode * IoOperation_Return_ToBytes(IoOperation_Return *ths ); 
BytesBufferNode * IoBlink_Send_ToBytes(IoBlink_Send *ths ); 
BytesBufferNode * IoBlink_Return_ToBytes(IoBlink_Return *ths ); 
BytesBufferNode * UartChangeBode_Send_ToBytes(UartChangeBode_Send *ths ); 
BytesBufferNode * UartChangeBode_Return_ToBytes(UartChangeBode_Return *ths ); 
BytesBufferNode * ServoMotorMultiRotate_Send_m1_MoveArgs_ToBytes(ServoMotorMultiRotate_Send_m1_MoveArgs *ths ); 
BytesBufferNode * ServoMotorMultiRotate_Send_ToBytes(ServoMotorMultiRotate_Send *ths ); 
BytesBufferNode * ServoMotorMultiRotate_Return_ToBytes(ServoMotorMultiRotate_Return *ths ); 
BytesBufferNode * ServoMotorRotateShakeDetect_Send_ToBytes(ServoMotorRotateShakeDetect_Send *ths ); 
BytesBufferNode * ServoMotorRotateShakeDetect_Return_ToBytes(ServoMotorRotateShakeDetect_Return *ths ); 
BytesBufferNode * ServoMotorRotateShakeRotateDetect_Send_ToBytes(ServoMotorRotateShakeRotateDetect_Send *ths ); 
BytesBufferNode * ServoMotorRotateShakeRotateDetect_Return_ToBytes(ServoMotorRotateShakeRotateDetect_Return *ths ); 
BytesBufferNode * ServoMotorRotateTwoDirOnTime_Send_ToBytes(ServoMotorRotateTwoDirOnTime_Send *ths ); 
BytesBufferNode * ServoMotorRotateTwoDirOnTime_Return_ToBytes(ServoMotorRotateTwoDirOnTime_Return *ths ); 
BytesBufferNode * ServoMotorStatetGet_Send_ToBytes(ServoMotorStatetGet_Send *ths ); 
BytesBufferNode * ServoMotorStatetGet_Return_ToBytes(ServoMotorStatetGet_Return *ths ); 
BytesBufferNode * ServoMotorRotateStart_Send_ToBytes(ServoMotorRotateStart_Send *ths ); 
BytesBufferNode * ServoMotorRotateStart_Return_ToBytes(ServoMotorRotateStart_Return *ths ); 
BytesBufferNode * ServoMotorRotateStop_Send_ToBytes(ServoMotorRotateStop_Send *ths ); 
BytesBufferNode * ServoMotorRotateStop_Return_ToBytes(ServoMotorRotateStop_Return *ths ); 
BytesBufferNode * ServoMotorReset_Send_ToBytes(ServoMotorReset_Send *ths ); 
BytesBufferNode * ServoMotorReset_Return_ToBytes(ServoMotorReset_Return *ths ); 
BytesBufferNode * ServoMotorAutoCalibratio_Send_ToBytes(ServoMotorAutoCalibratio_Send *ths ); 
BytesBufferNode * ServoMotorAutoCalibratio_Return_ToBytes(ServoMotorAutoCalibratio_Return *ths ); 
BytesBufferNode * ServoMotorShake_Send_ToBytes(ServoMotorShake_Send *ths ); 
BytesBufferNode * ServoMotorShake_Return_ToBytes(ServoMotorShake_Return *ths ); 
BytesBufferNode * ServoMotorBaseCtrl_Send_ToBytes(ServoMotorBaseCtrl_Send *ths ); 
BytesBufferNode * ServoMotorBaseCtrl_Return_ToBytes(ServoMotorBaseCtrl_Return *ths ); 
BytesBufferNode * ServoMotorMove_Send_ToBytes(ServoMotorMove_Send *ths ); 
BytesBufferNode * ServoMotorMove_Return_ToBytes(ServoMotorMove_Return *ths ); 
BytesBufferNode * RGBSensorGet_Send_ToBytes(RGBSensorGet_Send *ths ); 
BytesBufferNode * RGBSensorGet_Return_ToBytes(RGBSensorGet_Return *ths ); 
BytesBufferNode * SerialDataReport_Send_ToBytes(SerialDataReport_Send *ths ); 
BytesBufferNode * SerialDataReport_Return_ToBytes(SerialDataReport_Return *ths ); 
BytesBufferNode * AddressBroadcastSend_Send_ToBytes(AddressBroadcastSend_Send *ths ); 
BytesBufferNode * AddressBroadcastSend_Return_ToBytes(AddressBroadcastSend_Return *ths ); 
BytesBufferNode * AddressBroadcastReceive_Send_ToBytes(AddressBroadcastReceive_Send *ths ); 
BytesBufferNode * AddressBroadcastReceive_Return_ToBytes(AddressBroadcastReceive_Return *ths ); 
BytesBufferNode * StartCheckPress_Send_ToBytes(StartCheckPress_Send *ths ); 
BytesBufferNode * StartCheckPress_Return_ToBytes(StartCheckPress_Return *ths ); 
BytesBufferNode * EndCheckPress_Send_ToBytes(EndCheckPress_Send *ths ); 
BytesBufferNode * EndCheckPress_Return_ToBytes(EndCheckPress_Return *ths ); 
BytesBufferNode * Blending_Send_ToBytes(Blending_Send *ths ); 
BytesBufferNode * Blending_Return_ToBytes(Blending_Return *ths ); 
BytesBufferNode * GetEPOSType_Send_ToBytes(GetEPOSType_Send *ths ); 
BytesBufferNode * GetEPOSType_Return_ToBytes(GetEPOSType_Return *ths ); 
BytesBufferNode * SetEPOSType_Send_ToBytes(SetEPOSType_Send *ths ); 
BytesBufferNode * SetEPOSType_Return_ToBytes(SetEPOSType_Return *ths ); 
BytesBufferNode * DACSet_Send_ToBytes(DACSet_Send *ths ); 
BytesBufferNode * DACSet_Return_ToBytes(DACSet_Return *ths ); 
BytesBufferNode * LightCtrl_Send_ToBytes(LightCtrl_Send *ths ); 
BytesBufferNode * LightCtrl_Return_ToBytes(LightCtrl_Return *ths ); 
BytesBufferNode * DCMotorMoveSeq_Send_m1_MoveArgs_ToBytes(DCMotorMoveSeq_Send_m1_MoveArgs *ths ); 
BytesBufferNode * DCMotorMoveSeq_Send_ToBytes(DCMotorMoveSeq_Send *ths ); 
BytesBufferNode * DCMotorMoveSeq_Return_ToBytes(DCMotorMoveSeq_Return *ths ); 
BytesBufferNode * DCMotorMoveTmc_Send_ToBytes(DCMotorMoveTmc_Send *ths ); 
BytesBufferNode * DCMotorMoveTmc_Return_ToBytes(DCMotorMoveTmc_Return *ths ); 
BytesBufferNode * PressureRead_Send_ToBytes(PressureRead_Send *ths ); 
BytesBufferNode * PressureRead_Return_ToBytes(PressureRead_Return *ths ); 
BytesBufferNode * LittleMotorCtrl_Send_ToBytes(LittleMotorCtrl_Send *ths ); 
BytesBufferNode * LittleMotorCtrl_Return_ToBytes(LittleMotorCtrl_Return *ths ); 
BytesBufferNode * CongestionDetection_Send_ToBytes(CongestionDetection_Send *ths ); 
BytesBufferNode * CongestionDetection_Return_ToBytes(CongestionDetection_Return *ths ); 
BytesBufferNode * LaserCtrol_Send_ToBytes(LaserCtrol_Send *ths ); 
BytesBufferNode * LaserCtrol_Return_ToBytes(LaserCtrol_Return *ths ); 
BytesBufferNode * NFL_ReadConfig_Send_ToBytes(NFL_ReadConfig_Send *ths ); 
BytesBufferNode * NFL_ReadConfig_Return_ToBytes(NFL_ReadConfig_Return *ths ); 
BytesBufferNode * NFL_WriteConfig_Send_ToBytes(NFL_WriteConfig_Send *ths ); 
BytesBufferNode * NFL_WriteConfig_Return_ToBytes(NFL_WriteConfig_Return *ths ); 
BytesBufferNode * NFL_ReadDataStatus_Send_ToBytes(NFL_ReadDataStatus_Send *ths ); 
BytesBufferNode * NFL_ReadDataStatus_Return_ToBytes(NFL_ReadDataStatus_Return *ths ); 
BytesBufferNode * NFL_ReadData_Send_ToBytes(NFL_ReadData_Send *ths ); 
BytesBufferNode * NFL_ReadData_Return_ToBytes(NFL_ReadData_Return *ths ); 
BytesBufferNode * NFL_DataCollectStart_Send_ToBytes(NFL_DataCollectStart_Send *ths ); 
BytesBufferNode * NFL_DataCollectStart_Return_ToBytes(NFL_DataCollectStart_Return *ths ); 
BytesBufferNode * NFL_DataCollectStop_Send_ToBytes(NFL_DataCollectStop_Send *ths ); 
BytesBufferNode * NFL_DataCollectStop_Return_ToBytes(NFL_DataCollectStop_Return *ths ); 
BytesBufferNode * NFL_ThresholdConfig_Send_ToBytes(NFL_ThresholdConfig_Send *ths ); 
BytesBufferNode * NFL_ThresholdConfig_Return_ToBytes(NFL_ThresholdConfig_Return *ths ); 
BytesBufferNode * NFL_StandbyConfig_Send_ToBytes(NFL_StandbyConfig_Send *ths ); 
BytesBufferNode * NFL_StandbyConfig_Return_ToBytes(NFL_StandbyConfig_Return *ths ); 
BytesBufferNode * NFL_ActivateConfig_Send_ToBytes(NFL_ActivateConfig_Send *ths ); 
BytesBufferNode * NFL_ActivateConfig_Return_ToBytes(NFL_ActivateConfig_Return *ths ); 
BytesBufferNode * NFL_ReadTemperature_Send_ToBytes(NFL_ReadTemperature_Send *ths ); 
BytesBufferNode * NFL_ReadTemperature_Return_ToBytes(NFL_ReadTemperature_Return *ths ); 
BytesBufferNode * NFL_SendEvent_Send_ToBytes(NFL_SendEvent_Send *ths ); 
BytesBufferNode * NFL_SendEvent_Return_ToBytes(NFL_SendEvent_Return *ths ); 
BytesBufferNode * PowerCtrl_Send_ToBytes(PowerCtrl_Send *ths ); 
BytesBufferNode * PowerCtrl_Return_ToBytes(PowerCtrl_Return *ths ); 
BytesBufferNode * SendErrorCode_Send_ToBytes(SendErrorCode_Send *ths ); 
BytesBufferNode * SendErrorCode_Return_ToBytes(SendErrorCode_Return *ths ); 
BytesBufferNode * IOCtrlByEvent_Send_m1_PortArgs_ToBytes(IOCtrlByEvent_Send_m1_PortArgs *ths ); 
BytesBufferNode * IOCtrlByEvent_Send_m2_PortArgs_ToBytes(IOCtrlByEvent_Send_m2_PortArgs *ths ); 
BytesBufferNode * IOCtrlByEvent_Send_ToBytes(IOCtrlByEvent_Send *ths ); 
BytesBufferNode * IOCtrlByEvent_Return_ToBytes(IOCtrlByEvent_Return *ths ); 
int Dsp_DHCP_Send_GetFromBytes(Dsp_DHCP_Send *ths,u8* buff,int offset ); 
int Dsp_DHCP_Return_GetFromBytes(Dsp_DHCP_Return *ths,u8* buff,int offset ); 
int Ping_Send_GetFromBytes(Ping_Send *ths,u8* buff,int offset ); 
int Ping_Return_GetFromBytes(Ping_Return *ths,u8* buff,int offset ); 
int FilePush_Send_GetFromBytes(FilePush_Send *ths,u8* buff,int offset ); 
int FilePush_Return_GetFromBytes(FilePush_Return *ths,u8* buff,int offset ); 
int InstrumentStateReport_Send_GetFromBytes(InstrumentStateReport_Send *ths,u8* buff,int offset ); 
int InstrumentStateReport_Return_GetFromBytes(InstrumentStateReport_Return *ths,u8* buff,int offset ); 
int ProtoTest_Send_GetFromBytes(ProtoTest_Send *ths,u8* buff,int offset ); 
int ProtoTest_Return_GetFromBytes(ProtoTest_Return *ths,u8* buff,int offset ); 
int McuErrorToPC_Send_GetFromBytes(McuErrorToPC_Send *ths,u8* buff,int offset ); 
int McuErrorToPC_Return_GetFromBytes(McuErrorToPC_Return *ths,u8* buff,int offset ); 
int AutoAdapter_Send_GetFromBytes(AutoAdapter_Send *ths,u8* buff,int offset ); 
int AutoAdapter_Return_GetFromBytes(AutoAdapter_Return *ths,u8* buff,int offset ); 
int MotorMultiMove_Send_m1_MoveArgs_GetFromBytes(MotorMultiMove_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
int MotorMultiMove_Send_GetFromBytes(MotorMultiMove_Send *ths,u8* buff,int offset ); 
int MotorMultiMove_Return_GetFromBytes(MotorMultiMove_Return *ths,u8* buff,int offset ); 
int MotorMove_Send_GetFromBytes(MotorMove_Send *ths,u8* buff,int offset ); 
int MotorMove_Return_GetFromBytes(MotorMove_Return *ths,u8* buff,int offset ); 
int MotorStatetGet_Send_GetFromBytes(MotorStatetGet_Send *ths,u8* buff,int offset ); 
int MotorStatetGet_Return_GetFromBytes(MotorStatetGet_Return *ths,u8* buff,int offset ); 
int MotorRotateStart_Send_GetFromBytes(MotorRotateStart_Send *ths,u8* buff,int offset ); 
int MotorRotateStart_Return_GetFromBytes(MotorRotateStart_Return *ths,u8* buff,int offset ); 
int MotorRotateStop_Send_GetFromBytes(MotorRotateStop_Send *ths,u8* buff,int offset ); 
int MotorRotateStop_Return_GetFromBytes(MotorRotateStop_Return *ths,u8* buff,int offset ); 
int MotorRotate_Send_GetFromBytes(MotorRotate_Send *ths,u8* buff,int offset ); 
int MotorRotate_Return_GetFromBytes(MotorRotate_Return *ths,u8* buff,int offset ); 
int MotorReset_Send_GetFromBytes(MotorReset_Send *ths,u8* buff,int offset ); 
int MotorReset_Return_GetFromBytes(MotorReset_Return *ths,u8* buff,int offset ); 
int SensorValueOperation_Send_GetFromBytes(SensorValueOperation_Send *ths,u8* buff,int offset ); 
int SensorValueOperation_Return_GetFromBytes(SensorValueOperation_Return *ths,u8* buff,int offset ); 
int ADAutoCaliStart_Send_GetFromBytes(ADAutoCaliStart_Send *ths,u8* buff,int offset ); 
int ADAutoCaliStart_Return_GetFromBytes(ADAutoCaliStart_Return *ths,u8* buff,int offset ); 
int TemperatureGet_Send_GetFromBytes(TemperatureGet_Send *ths,u8* buff,int offset ); 
int TemperatureGet_Return_GetFromBytes(TemperatureGet_Return *ths,u8* buff,int offset ); 
int TemperatureOpen_Send_GetFromBytes(TemperatureOpen_Send *ths,u8* buff,int offset ); 
int TemperatureOpen_Return_GetFromBytes(TemperatureOpen_Return *ths,u8* buff,int offset ); 
int TemperatureClose_Send_GetFromBytes(TemperatureClose_Send *ths,u8* buff,int offset ); 
int TemperatureClose_Return_GetFromBytes(TemperatureClose_Return *ths,u8* buff,int offset ); 
int BarCodeGet_Send_GetFromBytes(BarCodeGet_Send *ths,u8* buff,int offset ); 
int BarCodeGet_Return_GetFromBytes(BarCodeGet_Return *ths,u8* buff,int offset ); 
int FirmWareConfig_Send_GetFromBytes(FirmWareConfig_Send *ths,u8* buff,int offset ); 
int FirmWareConfig_Return_GetFromBytes(FirmWareConfig_Return *ths,u8* buff,int offset ); 
int FirmWareTransmitFile_Send_GetFromBytes(FirmWareTransmitFile_Send *ths,u8* buff,int offset ); 
int FirmWareTransmitFile_Return_GetFromBytes(FirmWareTransmitFile_Return *ths,u8* buff,int offset ); 
int McuReset_Send_GetFromBytes(McuReset_Send *ths,u8* buff,int offset ); 
int McuReset_Return_GetFromBytes(McuReset_Return *ths,u8* buff,int offset ); 
int FirmWareVersionGet_Send_GetFromBytes(FirmWareVersionGet_Send *ths,u8* buff,int offset ); 
int FirmWareVersionGet_Return_GetFromBytes(FirmWareVersionGet_Return *ths,u8* buff,int offset ); 
int MotorConfigSet_Send_GetFromBytes(MotorConfigSet_Send *ths,u8* buff,int offset ); 
int MotorConfigSet_Return_GetFromBytes(MotorConfigSet_Return *ths,u8* buff,int offset ); 
int MotorMoveEvent_Send_GetFromBytes(MotorMoveEvent_Send *ths,u8* buff,int offset ); 
int MotorMoveEvent_Return_GetFromBytes(MotorMoveEvent_Return *ths,u8* buff,int offset ); 
int McuIoStateChangeEvent_Send_GetFromBytes(McuIoStateChangeEvent_Send *ths,u8* buff,int offset ); 
int McuIoStateChangeEvent_Return_GetFromBytes(McuIoStateChangeEvent_Return *ths,u8* buff,int offset ); 
int SampleUnitSensorGet_Send_GetFromBytes(SampleUnitSensorGet_Send *ths,u8* buff,int offset ); 
int SampleUnitSensorGet_Return_GetFromBytes(SampleUnitSensorGet_Return *ths,u8* buff,int offset ); 
int AirPumpOpen_Send_GetFromBytes(AirPumpOpen_Send *ths,u8* buff,int offset ); 
int AirPumpOpen_Return_GetFromBytes(AirPumpOpen_Return *ths,u8* buff,int offset ); 
int AirPumpClose_Send_GetFromBytes(AirPumpClose_Send *ths,u8* buff,int offset ); 
int AirPumpClose_Return_GetFromBytes(AirPumpClose_Return *ths,u8* buff,int offset ); 
int ADSampleRead_Send_GetFromBytes(ADSampleRead_Send *ths,u8* buff,int offset ); 
int ADSampleRead_Return_GetFromBytes(ADSampleRead_Return *ths,u8* buff,int offset ); 
int MotorMoveGPIOEvent_Send_GetFromBytes(MotorMoveGPIOEvent_Send *ths,u8* buff,int offset ); 
int MotorMoveGPIOEvent_Return_GetFromBytes(MotorMoveGPIOEvent_Return *ths,u8* buff,int offset ); 
int MotorConfigSetNew_Send_GetFromBytes(MotorConfigSetNew_Send *ths,u8* buff,int offset ); 
int MotorConfigSetNew_Return_GetFromBytes(MotorConfigSetNew_Return *ths,u8* buff,int offset ); 
int ExecutesCtrl_Send_GetFromBytes(ExecutesCtrl_Send *ths,u8* buff,int offset ); 
int ExecutesCtrl_Return_GetFromBytes(ExecutesCtrl_Return *ths,u8* buff,int offset ); 
int ADDetectConfigSet_Send_GetFromBytes(ADDetectConfigSet_Send *ths,u8* buff,int offset ); 
int ADDetectConfigSet_Return_GetFromBytes(ADDetectConfigSet_Return *ths,u8* buff,int offset ); 
int ADDataRead_Send_GetFromBytes(ADDataRead_Send *ths,u8* buff,int offset ); 
int ADDataRead_Return_GetFromBytes(ADDataRead_Return *ths,u8* buff,int offset ); 
/*
这里添加 数据接口GetFromBytes成员函数的定义
*/

int  DiskInstalledRed_Send_GetFromBytes(DiskInstalledRed_Send *ths,u8* buff,int offset);
int  DiskInstalledRed_Return_GetFromBytes(DiskInstalledRed_Return *ths,u8* buff,int offset); 


int ADDataSeriesSend_Send_GetFromBytes(ADDataSeriesSend_Send *ths,u8* buff,int offset ); 
int ADDataSeriesSend_Return_GetFromBytes(ADDataSeriesSend_Return *ths,u8* buff,int offset ); 
int UartTransmit_Send_GetFromBytes(UartTransmit_Send *ths,u8* buff,int offset ); 
int UartTransmit_Return_GetFromBytes(UartTransmit_Return *ths,u8* buff,int offset ); 
int UartReceiveEvent_Send_GetFromBytes(UartReceiveEvent_Send *ths,u8* buff,int offset ); 
int UartReceiveEvent_Return_GetFromBytes(UartReceiveEvent_Return *ths,u8* buff,int offset ); 
int SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_GetFromBytes(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths,u8* buff,int offset ); 
int SeqMotorMultiMove_Send_m1_Sequence_GetFromBytes(SeqMotorMultiMove_Send_m1_Sequence *ths,u8* buff,int offset ); 
int SeqMotorMultiMove_Send_GetFromBytes(SeqMotorMultiMove_Send *ths,u8* buff,int offset ); 
int SeqMotorMultiMove_Return_GetFromBytes(SeqMotorMultiMove_Return *ths,u8* buff,int offset ); 
int PressureConfig_Send_GetFromBytes(PressureConfig_Send *ths,u8* buff,int offset ); 
int PressureConfig_Return_GetFromBytes(PressureConfig_Return *ths,u8* buff,int offset ); 
int PressureDataGet_Send_GetFromBytes(PressureDataGet_Send *ths,u8* buff,int offset ); 
int PressureDataGet_Return_GetFromBytes(PressureDataGet_Return *ths,u8* buff,int offset ); 
int MotorAbort_Send_GetFromBytes(MotorAbort_Send *ths,u8* buff,int offset ); 
int MotorAbort_Return_GetFromBytes(MotorAbort_Return *ths,u8* buff,int offset ); 
int PressureEvent_Send_GetFromBytes(PressureEvent_Send *ths,u8* buff,int offset ); 
int PressureEvent_Return_GetFromBytes(PressureEvent_Return *ths,u8* buff,int offset ); 
int MotorMoveSeq_Send_m1_MoveArgs_GetFromBytes(MotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
int MotorMoveSeq_Send_GetFromBytes(MotorMoveSeq_Send *ths,u8* buff,int offset ); 
int MotorMoveSeq_Return_GetFromBytes(MotorMoveSeq_Return *ths,u8* buff,int offset ); 
int FirmWareReadyEvent_Send_GetFromBytes(FirmWareReadyEvent_Send *ths,u8* buff,int offset ); 
int FirmWareReadyEvent_Return_GetFromBytes(FirmWareReadyEvent_Return *ths,u8* buff,int offset ); 
int MotorConfigGet_Send_GetFromBytes(MotorConfigGet_Send *ths,u8* buff,int offset ); 
int MotorConfigGet_Return_GetFromBytes(MotorConfigGet_Return *ths,u8* buff,int offset ); 
int PWM_Set_Send_GetFromBytes(PWM_Set_Send *ths,u8* buff,int offset ); 
int PWM_Set_Return_GetFromBytes(PWM_Set_Return *ths,u8* buff,int offset ); 
int TurnTable_Move_Send_GetFromBytes(TurnTable_Move_Send *ths,u8* buff,int offset ); 
int TurnTable_Move_Return_GetFromBytes(TurnTable_Move_Return *ths,u8* buff,int offset ); 
int EncodeTurnTableFindZero_Send_GetFromBytes(EncodeTurnTableFindZero_Send *ths,u8* buff,int offset ); 
int EncodeTurnTableFindZero_Return_GetFromBytes(EncodeTurnTableFindZero_Return *ths,u8* buff,int offset ); 
int EncodeTurnTableRotateStart_Send_GetFromBytes(EncodeTurnTableRotateStart_Send *ths,u8* buff,int offset ); 
int EncodeTurnTableRotateStart_Return_GetFromBytes(EncodeTurnTableRotateStart_Return *ths,u8* buff,int offset ); 
int EncodeTurnTableRotateStop_Send_GetFromBytes(EncodeTurnTableRotateStop_Send *ths,u8* buff,int offset ); 
int EncodeTurnTableRotateStop_Return_GetFromBytes(EncodeTurnTableRotateStop_Return *ths,u8* buff,int offset ); 
int EncodeTurnTableStateGet_Send_GetFromBytes(EncodeTurnTableStateGet_Send *ths,u8* buff,int offset ); 
int EncodeTurnTableStateGet_Return_GetFromBytes(EncodeTurnTableStateGet_Return *ths,u8* buff,int offset ); 
int DCAdRead_Send_GetFromBytes(DCAdRead_Send *ths,u8* buff,int offset ); 
int DCAdRead_Return_GetFromBytes(DCAdRead_Return *ths,u8* buff,int offset ); 
int JSAdRead_Send_GetFromBytes(JSAdRead_Send *ths,u8* buff,int offset ); 
int JSAdRead_Return_GetFromBytes(JSAdRead_Return *ths,u8* buff,int offset ); 
int MotorMoveSyncLevel_Send_GetFromBytes(MotorMoveSyncLevel_Send *ths,u8* buff,int offset ); 
int MotorMoveSyncLevel_Return_GetFromBytes(MotorMoveSyncLevel_Return *ths,u8* buff,int offset ); 
int DCMotor_Send_GetFromBytes(DCMotor_Send *ths,u8* buff,int offset ); 
int DCMotor_Return_GetFromBytes(DCMotor_Return *ths,u8* buff,int offset ); 
int MotorCupIDSet_Send_GetFromBytes(MotorCupIDSet_Send *ths,u8* buff,int offset ); 
int MotorCupIDSet_Return_GetFromBytes(MotorCupIDSet_Return *ths,u8* buff,int offset ); 
int MotorCupIDInfoGet_Send_GetFromBytes(MotorCupIDInfoGet_Send *ths,u8* buff,int offset ); 
int MotorCupIDInfoGet_Return_GetFromBytes(MotorCupIDInfoGet_Return *ths,u8* buff,int offset ); 
int JSAdDataGet_Send_GetFromBytes(JSAdDataGet_Send *ths,u8* buff,int offset ); 
int JSAdDataGet_Return_GetFromBytes(JSAdDataGet_Return *ths,u8* buff,int offset ); 
int MotorMultiMoveTmc_Send_m1_MoveArgs_GetFromBytes(MotorMultiMoveTmc_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
int MotorMultiMoveTmc_Send_GetFromBytes(MotorMultiMoveTmc_Send *ths,u8* buff,int offset ); 
int MotorMultiMoveTmc_Return_GetFromBytes(MotorMultiMoveTmc_Return *ths,u8* buff,int offset ); 
int MotorMoveTmc_Send_GetFromBytes(MotorMoveTmc_Send *ths,u8* buff,int offset ); 
int MotorMoveTmc_Return_GetFromBytes(MotorMoveTmc_Return *ths,u8* buff,int offset ); 
int MotorStatetGetTmc_Send_GetFromBytes(MotorStatetGetTmc_Send *ths,u8* buff,int offset ); 
int MotorStatetGetTmc_Return_GetFromBytes(MotorStatetGetTmc_Return *ths,u8* buff,int offset ); 
int MotorRotateStartTmc_Send_GetFromBytes(MotorRotateStartTmc_Send *ths,u8* buff,int offset ); 
int MotorRotateStartTmc_Return_GetFromBytes(MotorRotateStartTmc_Return *ths,u8* buff,int offset ); 
int MotorRotateStopTmc_Send_GetFromBytes(MotorRotateStopTmc_Send *ths,u8* buff,int offset ); 
int MotorRotateStopTmc_Return_GetFromBytes(MotorRotateStopTmc_Return *ths,u8* buff,int offset ); 
int MotorRotateTmc_Send_GetFromBytes(MotorRotateTmc_Send *ths,u8* buff,int offset ); 
int MotorRotateTmc_Return_GetFromBytes(MotorRotateTmc_Return *ths,u8* buff,int offset ); 
int MotorResetTmc_Send_GetFromBytes(MotorResetTmc_Send *ths,u8* buff,int offset ); 
int MotorResetTmc_Return_GetFromBytes(MotorResetTmc_Return *ths,u8* buff,int offset ); 
int MotorConfigSetTmc_Send_GetFromBytes(MotorConfigSetTmc_Send *ths,u8* buff,int offset ); 
int MotorConfigSetTmc_Return_GetFromBytes(MotorConfigSetTmc_Return *ths,u8* buff,int offset ); 
int MotorMoveGPIOEventTmc_Send_GetFromBytes(MotorMoveGPIOEventTmc_Send *ths,u8* buff,int offset ); 
int MotorMoveGPIOEventTmc_Return_GetFromBytes(MotorMoveGPIOEventTmc_Return *ths,u8* buff,int offset ); 
int MotorRotateShake_Send_GetFromBytes(MotorRotateShake_Send *ths,u8* buff,int offset ); 
int MotorRotateShake_Return_GetFromBytes(MotorRotateShake_Return *ths,u8* buff,int offset ); 
int LiquidLevelResultGet_Send_GetFromBytes(LiquidLevelResultGet_Send *ths,u8* buff,int offset ); 
int LiquidLevelResultGet_Return_GetFromBytes(LiquidLevelResultGet_Return *ths,u8* buff,int offset ); 
int MotorDetectLiquidLevel_Send_GetFromBytes(MotorDetectLiquidLevel_Send *ths,u8* buff,int offset ); 
int MotorDetectLiquidLevel_Return_GetFromBytes(MotorDetectLiquidLevel_Return *ths,u8* buff,int offset ); 
int RFIDValueOperation_Send_GetFromBytes(RFIDValueOperation_Send *ths,u8* buff,int offset ); 
int RFIDValueOperation_Return_GetFromBytes(RFIDValueOperation_Return *ths,u8* buff,int offset ); 
int IoSimple_Send_GetFromBytes(IoSimple_Send *ths,u8* buff,int offset ); 
int IoSimple_Return_GetFromBytes(IoSimple_Return *ths,u8* buff,int offset ); 
int IoOperation_Send_m1_IoArgs_m1_Controls_GetFromBytes(IoOperation_Send_m1_IoArgs_m1_Controls *ths,u8* buff,int offset ); 
int IoOperation_Send_m1_IoArgs_GetFromBytes(IoOperation_Send_m1_IoArgs *ths,u8* buff,int offset ); 
int IoOperation_Send_GetFromBytes(IoOperation_Send *ths,u8* buff,int offset ); 
int IoOperation_Return_GetFromBytes(IoOperation_Return *ths,u8* buff,int offset ); 
int IoBlink_Send_GetFromBytes(IoBlink_Send *ths,u8* buff,int offset ); 
int IoBlink_Return_GetFromBytes(IoBlink_Return *ths,u8* buff,int offset ); 
int UartChangeBode_Send_GetFromBytes(UartChangeBode_Send *ths,u8* buff,int offset ); 
int UartChangeBode_Return_GetFromBytes(UartChangeBode_Return *ths,u8* buff,int offset ); 
int ServoMotorMultiRotate_Send_m1_MoveArgs_GetFromBytes(ServoMotorMultiRotate_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
int ServoMotorMultiRotate_Send_GetFromBytes(ServoMotorMultiRotate_Send *ths,u8* buff,int offset ); 
int ServoMotorMultiRotate_Return_GetFromBytes(ServoMotorMultiRotate_Return *ths,u8* buff,int offset ); 
int ServoMotorRotateShakeDetect_Send_GetFromBytes(ServoMotorRotateShakeDetect_Send *ths,u8* buff,int offset ); 
int ServoMotorRotateShakeDetect_Return_GetFromBytes(ServoMotorRotateShakeDetect_Return *ths,u8* buff,int offset ); 
int ServoMotorRotateShakeRotateDetect_Send_GetFromBytes(ServoMotorRotateShakeRotateDetect_Send *ths,u8* buff,int offset ); 
int ServoMotorRotateShakeRotateDetect_Return_GetFromBytes(ServoMotorRotateShakeRotateDetect_Return *ths,u8* buff,int offset ); 
int ServoMotorRotateTwoDirOnTime_Send_GetFromBytes(ServoMotorRotateTwoDirOnTime_Send *ths,u8* buff,int offset ); 
int ServoMotorRotateTwoDirOnTime_Return_GetFromBytes(ServoMotorRotateTwoDirOnTime_Return *ths,u8* buff,int offset ); 
int ServoMotorStatetGet_Send_GetFromBytes(ServoMotorStatetGet_Send *ths,u8* buff,int offset ); 
int ServoMotorStatetGet_Return_GetFromBytes(ServoMotorStatetGet_Return *ths,u8* buff,int offset ); 
int ServoMotorRotateStart_Send_GetFromBytes(ServoMotorRotateStart_Send *ths,u8* buff,int offset ); 
int ServoMotorRotateStart_Return_GetFromBytes(ServoMotorRotateStart_Return *ths,u8* buff,int offset ); 
int ServoMotorRotateStop_Send_GetFromBytes(ServoMotorRotateStop_Send *ths,u8* buff,int offset ); 
int ServoMotorRotateStop_Return_GetFromBytes(ServoMotorRotateStop_Return *ths,u8* buff,int offset ); 
int ServoMotorReset_Send_GetFromBytes(ServoMotorReset_Send *ths,u8* buff,int offset ); 
int ServoMotorReset_Return_GetFromBytes(ServoMotorReset_Return *ths,u8* buff,int offset ); 
int ServoMotorAutoCalibratio_Send_GetFromBytes(ServoMotorAutoCalibratio_Send *ths,u8* buff,int offset ); 
int ServoMotorAutoCalibratio_Return_GetFromBytes(ServoMotorAutoCalibratio_Return *ths,u8* buff,int offset ); 
int ServoMotorShake_Send_GetFromBytes(ServoMotorShake_Send *ths,u8* buff,int offset ); 
int ServoMotorShake_Return_GetFromBytes(ServoMotorShake_Return *ths,u8* buff,int offset ); 
int ServoMotorBaseCtrl_Send_GetFromBytes(ServoMotorBaseCtrl_Send *ths,u8* buff,int offset ); 
int ServoMotorBaseCtrl_Return_GetFromBytes(ServoMotorBaseCtrl_Return *ths,u8* buff,int offset ); 
int ServoMotorMove_Send_GetFromBytes(ServoMotorMove_Send *ths,u8* buff,int offset ); 
int ServoMotorMove_Return_GetFromBytes(ServoMotorMove_Return *ths,u8* buff,int offset ); 
int RGBSensorGet_Send_GetFromBytes(RGBSensorGet_Send *ths,u8* buff,int offset ); 
int RGBSensorGet_Return_GetFromBytes(RGBSensorGet_Return *ths,u8* buff,int offset ); 
int SerialDataReport_Send_GetFromBytes(SerialDataReport_Send *ths,u8* buff,int offset ); 
int SerialDataReport_Return_GetFromBytes(SerialDataReport_Return *ths,u8* buff,int offset ); 
int AddressBroadcastSend_Send_GetFromBytes(AddressBroadcastSend_Send *ths,u8* buff,int offset ); 
int AddressBroadcastSend_Return_GetFromBytes(AddressBroadcastSend_Return *ths,u8* buff,int offset ); 
int AddressBroadcastReceive_Send_GetFromBytes(AddressBroadcastReceive_Send *ths,u8* buff,int offset ); 
int AddressBroadcastReceive_Return_GetFromBytes(AddressBroadcastReceive_Return *ths,u8* buff,int offset ); 
int StartCheckPress_Send_GetFromBytes(StartCheckPress_Send *ths,u8* buff,int offset ); 
int StartCheckPress_Return_GetFromBytes(StartCheckPress_Return *ths,u8* buff,int offset ); 
int EndCheckPress_Send_GetFromBytes(EndCheckPress_Send *ths,u8* buff,int offset ); 
int EndCheckPress_Return_GetFromBytes(EndCheckPress_Return *ths,u8* buff,int offset ); 
int Blending_Send_GetFromBytes(Blending_Send *ths,u8* buff,int offset ); 
int Blending_Return_GetFromBytes(Blending_Return *ths,u8* buff,int offset ); 
int GetEPOSType_Send_GetFromBytes(GetEPOSType_Send *ths,u8* buff,int offset ); 
int GetEPOSType_Return_GetFromBytes(GetEPOSType_Return *ths,u8* buff,int offset ); 
int SetEPOSType_Send_GetFromBytes(SetEPOSType_Send *ths,u8* buff,int offset ); 
int SetEPOSType_Return_GetFromBytes(SetEPOSType_Return *ths,u8* buff,int offset ); 
int DACSet_Send_GetFromBytes(DACSet_Send *ths,u8* buff,int offset ); 
int DACSet_Return_GetFromBytes(DACSet_Return *ths,u8* buff,int offset ); 
int LightCtrl_Send_GetFromBytes(LightCtrl_Send *ths,u8* buff,int offset ); 
int LightCtrl_Return_GetFromBytes(LightCtrl_Return *ths,u8* buff,int offset ); 
int DCMotorMoveSeq_Send_m1_MoveArgs_GetFromBytes(DCMotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ); 
int DCMotorMoveSeq_Send_GetFromBytes(DCMotorMoveSeq_Send *ths,u8* buff,int offset ); 
int DCMotorMoveSeq_Return_GetFromBytes(DCMotorMoveSeq_Return *ths,u8* buff,int offset ); 
int DCMotorMoveTmc_Send_GetFromBytes(DCMotorMoveTmc_Send *ths,u8* buff,int offset ); 
int DCMotorMoveTmc_Return_GetFromBytes(DCMotorMoveTmc_Return *ths,u8* buff,int offset ); 
int PressureRead_Send_GetFromBytes(PressureRead_Send *ths,u8* buff,int offset ); 
int PressureRead_Return_GetFromBytes(PressureRead_Return *ths,u8* buff,int offset ); 
int LittleMotorCtrl_Send_GetFromBytes(LittleMotorCtrl_Send *ths,u8* buff,int offset ); 
int LittleMotorCtrl_Return_GetFromBytes(LittleMotorCtrl_Return *ths,u8* buff,int offset ); 
int CongestionDetection_Send_GetFromBytes(CongestionDetection_Send *ths,u8* buff,int offset ); 
int CongestionDetection_Return_GetFromBytes(CongestionDetection_Return *ths,u8* buff,int offset ); 
int LaserCtrol_Send_GetFromBytes(LaserCtrol_Send *ths,u8* buff,int offset ); 
int LaserCtrol_Return_GetFromBytes(LaserCtrol_Return *ths,u8* buff,int offset ); 
int NFL_ReadConfig_Send_GetFromBytes(NFL_ReadConfig_Send *ths,u8* buff,int offset ); 
int NFL_ReadConfig_Return_GetFromBytes(NFL_ReadConfig_Return *ths,u8* buff,int offset ); 
int NFL_WriteConfig_Send_GetFromBytes(NFL_WriteConfig_Send *ths,u8* buff,int offset ); 
int NFL_WriteConfig_Return_GetFromBytes(NFL_WriteConfig_Return *ths,u8* buff,int offset ); 
int NFL_ReadDataStatus_Send_GetFromBytes(NFL_ReadDataStatus_Send *ths,u8* buff,int offset ); 
int NFL_ReadDataStatus_Return_GetFromBytes(NFL_ReadDataStatus_Return *ths,u8* buff,int offset ); 
int NFL_ReadData_Send_GetFromBytes(NFL_ReadData_Send *ths,u8* buff,int offset ); 
int NFL_ReadData_Return_GetFromBytes(NFL_ReadData_Return *ths,u8* buff,int offset ); 
int NFL_DataCollectStart_Send_GetFromBytes(NFL_DataCollectStart_Send *ths,u8* buff,int offset ); 
int NFL_DataCollectStart_Return_GetFromBytes(NFL_DataCollectStart_Return *ths,u8* buff,int offset ); 
int NFL_DataCollectStop_Send_GetFromBytes(NFL_DataCollectStop_Send *ths,u8* buff,int offset ); 
int NFL_DataCollectStop_Return_GetFromBytes(NFL_DataCollectStop_Return *ths,u8* buff,int offset ); 
int NFL_ThresholdConfig_Send_GetFromBytes(NFL_ThresholdConfig_Send *ths,u8* buff,int offset ); 
int NFL_ThresholdConfig_Return_GetFromBytes(NFL_ThresholdConfig_Return *ths,u8* buff,int offset ); 
int NFL_StandbyConfig_Send_GetFromBytes(NFL_StandbyConfig_Send *ths,u8* buff,int offset ); 
int NFL_StandbyConfig_Return_GetFromBytes(NFL_StandbyConfig_Return *ths,u8* buff,int offset ); 
int NFL_ActivateConfig_Send_GetFromBytes(NFL_ActivateConfig_Send *ths,u8* buff,int offset ); 
int NFL_ActivateConfig_Return_GetFromBytes(NFL_ActivateConfig_Return *ths,u8* buff,int offset ); 
int NFL_ReadTemperature_Send_GetFromBytes(NFL_ReadTemperature_Send *ths,u8* buff,int offset ); 
int NFL_ReadTemperature_Return_GetFromBytes(NFL_ReadTemperature_Return *ths,u8* buff,int offset ); 
int NFL_SendEvent_Send_GetFromBytes(NFL_SendEvent_Send *ths,u8* buff,int offset ); 
int NFL_SendEvent_Return_GetFromBytes(NFL_SendEvent_Return *ths,u8* buff,int offset ); 
int PowerCtrl_Send_GetFromBytes(PowerCtrl_Send *ths,u8* buff,int offset ); 
int PowerCtrl_Return_GetFromBytes(PowerCtrl_Return *ths,u8* buff,int offset ); 
int SendErrorCode_Send_GetFromBytes(SendErrorCode_Send *ths,u8* buff,int offset ); 
int SendErrorCode_Return_GetFromBytes(SendErrorCode_Return *ths,u8* buff,int offset ); 
int IOCtrlByEvent_Send_m1_PortArgs_GetFromBytes(IOCtrlByEvent_Send_m1_PortArgs *ths,u8* buff,int offset ); 
int IOCtrlByEvent_Send_m2_PortArgs_GetFromBytes(IOCtrlByEvent_Send_m2_PortArgs *ths,u8* buff,int offset ); 
int IOCtrlByEvent_Send_GetFromBytes(IOCtrlByEvent_Send *ths,u8* buff,int offset ); 
int IOCtrlByEvent_Return_GetFromBytes(IOCtrlByEvent_Return *ths,u8* buff,int offset ); 
Dsp_DHCP_Send* New_Dsp_DHCP_Send(void);
Dsp_DHCP_Return* New_Dsp_DHCP_Return(void);
Ping_Send* New_Ping_Send(void);
Ping_Return* New_Ping_Return(void);
FilePush_Send* New_FilePush_Send(void);
FilePush_Return* New_FilePush_Return(void);
InstrumentStateReport_Send* New_InstrumentStateReport_Send(void);
InstrumentStateReport_Return* New_InstrumentStateReport_Return(void);
ProtoTest_Send* New_ProtoTest_Send(void);
ProtoTest_Return* New_ProtoTest_Return(void);
McuErrorToPC_Send* New_McuErrorToPC_Send(void);
McuErrorToPC_Return* New_McuErrorToPC_Return(void);
AutoAdapter_Send* New_AutoAdapter_Send(void);
AutoAdapter_Return* New_AutoAdapter_Return(void);
MotorMultiMove_Send_m1_MoveArgs* New_MotorMultiMove_Send_m1_MoveArgs(void);
MotorMultiMove_Send* New_MotorMultiMove_Send(void);
MotorMultiMove_Return* New_MotorMultiMove_Return(void);
MotorMove_Send* New_MotorMove_Send(void);
MotorMove_Return* New_MotorMove_Return(void);
MotorStatetGet_Send* New_MotorStatetGet_Send(void);
MotorStatetGet_Return* New_MotorStatetGet_Return(void);
MotorRotateStart_Send* New_MotorRotateStart_Send(void);
MotorRotateStart_Return* New_MotorRotateStart_Return(void);
MotorRotateStop_Send* New_MotorRotateStop_Send(void);
MotorRotateStop_Return* New_MotorRotateStop_Return(void);
MotorRotate_Send* New_MotorRotate_Send(void);
MotorRotate_Return* New_MotorRotate_Return(void);
MotorReset_Send* New_MotorReset_Send(void);
MotorReset_Return* New_MotorReset_Return(void);
SensorValueOperation_Send* New_SensorValueOperation_Send(void);
SensorValueOperation_Return* New_SensorValueOperation_Return(void);
ADAutoCaliStart_Send* New_ADAutoCaliStart_Send(void);
ADAutoCaliStart_Return* New_ADAutoCaliStart_Return(void);
TemperatureGet_Send* New_TemperatureGet_Send(void);
TemperatureGet_Return* New_TemperatureGet_Return(void);
TemperatureOpen_Send* New_TemperatureOpen_Send(void);
TemperatureOpen_Return* New_TemperatureOpen_Return(void);
TemperatureClose_Send* New_TemperatureClose_Send(void);
TemperatureClose_Return* New_TemperatureClose_Return(void);
BarCodeGet_Send* New_BarCodeGet_Send(void);
BarCodeGet_Return* New_BarCodeGet_Return(void);
FirmWareConfig_Send* New_FirmWareConfig_Send(void);
FirmWareConfig_Return* New_FirmWareConfig_Return(void);
FirmWareTransmitFile_Send* New_FirmWareTransmitFile_Send(void);
FirmWareTransmitFile_Return* New_FirmWareTransmitFile_Return(void);
McuReset_Send* New_McuReset_Send(void);
McuReset_Return* New_McuReset_Return(void);
FirmWareVersionGet_Send* New_FirmWareVersionGet_Send(void);
FirmWareVersionGet_Return* New_FirmWareVersionGet_Return(void);
MotorConfigSet_Send* New_MotorConfigSet_Send(void);
MotorConfigSet_Return* New_MotorConfigSet_Return(void);
MotorMoveEvent_Send* New_MotorMoveEvent_Send(void);
MotorMoveEvent_Return* New_MotorMoveEvent_Return(void);
McuIoStateChangeEvent_Send* New_McuIoStateChangeEvent_Send(void);
McuIoStateChangeEvent_Return* New_McuIoStateChangeEvent_Return(void);
SampleUnitSensorGet_Send* New_SampleUnitSensorGet_Send(void);
SampleUnitSensorGet_Return* New_SampleUnitSensorGet_Return(void);
AirPumpOpen_Send* New_AirPumpOpen_Send(void);
AirPumpOpen_Return* New_AirPumpOpen_Return(void);
AirPumpClose_Send* New_AirPumpClose_Send(void);
AirPumpClose_Return* New_AirPumpClose_Return(void);
ADSampleRead_Send* New_ADSampleRead_Send(void);
ADSampleRead_Return* New_ADSampleRead_Return(void);
MotorMoveGPIOEvent_Send* New_MotorMoveGPIOEvent_Send(void);
MotorMoveGPIOEvent_Return* New_MotorMoveGPIOEvent_Return(void);
MotorConfigSetNew_Send* New_MotorConfigSetNew_Send(void);
MotorConfigSetNew_Return* New_MotorConfigSetNew_Return(void);
ExecutesCtrl_Send* New_ExecutesCtrl_Send(void);
ExecutesCtrl_Return* New_ExecutesCtrl_Return(void);
ADDetectConfigSet_Send* New_ADDetectConfigSet_Send(void);
ADDetectConfigSet_Return* New_ADDetectConfigSet_Return(void);

ADDataRead_Send* New_ADDataRead_Send(void);
ADDataRead_Return* New_ADDataRead_Return(void);
/*
这里添加 数据接口 的构造函数
*/

DiskInstalledRed_Send* New_DiskInstalledRed_Send(void);
DiskInstalledRed_Return* New_DiskInstalledRed_Return(void);




ADDataSeriesSend_Send* New_ADDataSeriesSend_Send(void);
ADDataSeriesSend_Return* New_ADDataSeriesSend_Return(void);
UartTransmit_Send* New_UartTransmit_Send(void);
UartTransmit_Return* New_UartTransmit_Return(void);
UartReceiveEvent_Send* New_UartReceiveEvent_Send(void);
UartReceiveEvent_Return* New_UartReceiveEvent_Return(void);
SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* New_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs(void);
SeqMotorMultiMove_Send_m1_Sequence* New_SeqMotorMultiMove_Send_m1_Sequence(void);
SeqMotorMultiMove_Send* New_SeqMotorMultiMove_Send(void);
SeqMotorMultiMove_Return* New_SeqMotorMultiMove_Return(void);
PressureConfig_Send* New_PressureConfig_Send(void);
PressureConfig_Return* New_PressureConfig_Return(void);
PressureDataGet_Send* New_PressureDataGet_Send(void);
PressureDataGet_Return* New_PressureDataGet_Return(void);
MotorAbort_Send* New_MotorAbort_Send(void);
MotorAbort_Return* New_MotorAbort_Return(void);
PressureEvent_Send* New_PressureEvent_Send(void);
PressureEvent_Return* New_PressureEvent_Return(void);
MotorMoveSeq_Send_m1_MoveArgs* New_MotorMoveSeq_Send_m1_MoveArgs(void);
MotorMoveSeq_Send* New_MotorMoveSeq_Send(void);
MotorMoveSeq_Return* New_MotorMoveSeq_Return(void);
FirmWareReadyEvent_Send* New_FirmWareReadyEvent_Send(void);
FirmWareReadyEvent_Return* New_FirmWareReadyEvent_Return(void);
MotorConfigGet_Send* New_MotorConfigGet_Send(void);
MotorConfigGet_Return* New_MotorConfigGet_Return(void);
PWM_Set_Send* New_PWM_Set_Send(void);
PWM_Set_Return* New_PWM_Set_Return(void);
TurnTable_Move_Send* New_TurnTable_Move_Send(void);
TurnTable_Move_Return* New_TurnTable_Move_Return(void);
EncodeTurnTableFindZero_Send* New_EncodeTurnTableFindZero_Send(void);
EncodeTurnTableFindZero_Return* New_EncodeTurnTableFindZero_Return(void);
EncodeTurnTableRotateStart_Send* New_EncodeTurnTableRotateStart_Send(void);
EncodeTurnTableRotateStart_Return* New_EncodeTurnTableRotateStart_Return(void);
EncodeTurnTableRotateStop_Send* New_EncodeTurnTableRotateStop_Send(void);
EncodeTurnTableRotateStop_Return* New_EncodeTurnTableRotateStop_Return(void);
EncodeTurnTableStateGet_Send* New_EncodeTurnTableStateGet_Send(void);
EncodeTurnTableStateGet_Return* New_EncodeTurnTableStateGet_Return(void);
DCAdRead_Send* New_DCAdRead_Send(void);
DCAdRead_Return* New_DCAdRead_Return(void);
JSAdRead_Send* New_JSAdRead_Send(void);
JSAdRead_Return* New_JSAdRead_Return(void);
MotorMoveSyncLevel_Send* New_MotorMoveSyncLevel_Send(void);
MotorMoveSyncLevel_Return* New_MotorMoveSyncLevel_Return(void);
DCMotor_Send* New_DCMotor_Send(void);
DCMotor_Return* New_DCMotor_Return(void);
MotorCupIDSet_Send* New_MotorCupIDSet_Send(void);
MotorCupIDSet_Return* New_MotorCupIDSet_Return(void);
MotorCupIDInfoGet_Send* New_MotorCupIDInfoGet_Send(void);
MotorCupIDInfoGet_Return* New_MotorCupIDInfoGet_Return(void);
JSAdDataGet_Send* New_JSAdDataGet_Send(void);
JSAdDataGet_Return* New_JSAdDataGet_Return(void);
MotorMultiMoveTmc_Send_m1_MoveArgs* New_MotorMultiMoveTmc_Send_m1_MoveArgs(void);
MotorMultiMoveTmc_Send* New_MotorMultiMoveTmc_Send(void);
MotorMultiMoveTmc_Return* New_MotorMultiMoveTmc_Return(void);
MotorMoveTmc_Send* New_MotorMoveTmc_Send(void);
MotorMoveTmc_Return* New_MotorMoveTmc_Return(void);
MotorStatetGetTmc_Send* New_MotorStatetGetTmc_Send(void);
MotorStatetGetTmc_Return* New_MotorStatetGetTmc_Return(void);
MotorRotateStartTmc_Send* New_MotorRotateStartTmc_Send(void);
MotorRotateStartTmc_Return* New_MotorRotateStartTmc_Return(void);
MotorRotateStopTmc_Send* New_MotorRotateStopTmc_Send(void);
MotorRotateStopTmc_Return* New_MotorRotateStopTmc_Return(void);
MotorRotateTmc_Send* New_MotorRotateTmc_Send(void);
MotorRotateTmc_Return* New_MotorRotateTmc_Return(void);
MotorResetTmc_Send* New_MotorResetTmc_Send(void);
MotorResetTmc_Return* New_MotorResetTmc_Return(void);
MotorConfigSetTmc_Send* New_MotorConfigSetTmc_Send(void);
MotorConfigSetTmc_Return* New_MotorConfigSetTmc_Return(void);
MotorMoveGPIOEventTmc_Send* New_MotorMoveGPIOEventTmc_Send(void);
MotorMoveGPIOEventTmc_Return* New_MotorMoveGPIOEventTmc_Return(void);
MotorRotateShake_Send* New_MotorRotateShake_Send(void);
MotorRotateShake_Return* New_MotorRotateShake_Return(void);
LiquidLevelResultGet_Send* New_LiquidLevelResultGet_Send(void);
LiquidLevelResultGet_Return* New_LiquidLevelResultGet_Return(void);
MotorDetectLiquidLevel_Send* New_MotorDetectLiquidLevel_Send(void);
MotorDetectLiquidLevel_Return* New_MotorDetectLiquidLevel_Return(void);
RFIDValueOperation_Send* New_RFIDValueOperation_Send(void);
RFIDValueOperation_Return* New_RFIDValueOperation_Return(void);
IoSimple_Send* New_IoSimple_Send(void);
IoSimple_Return* New_IoSimple_Return(void);
IoOperation_Send_m1_IoArgs_m1_Controls* New_IoOperation_Send_m1_IoArgs_m1_Controls(void);
IoOperation_Send_m1_IoArgs* New_IoOperation_Send_m1_IoArgs(void);
IoOperation_Send* New_IoOperation_Send(void);
IoOperation_Return* New_IoOperation_Return(void);
IoBlink_Send* New_IoBlink_Send(void);
IoBlink_Return* New_IoBlink_Return(void);
UartChangeBode_Send* New_UartChangeBode_Send(void);
UartChangeBode_Return* New_UartChangeBode_Return(void);
ServoMotorMultiRotate_Send_m1_MoveArgs* New_ServoMotorMultiRotate_Send_m1_MoveArgs(void);
ServoMotorMultiRotate_Send* New_ServoMotorMultiRotate_Send(void);
ServoMotorMultiRotate_Return* New_ServoMotorMultiRotate_Return(void);
ServoMotorRotateShakeDetect_Send* New_ServoMotorRotateShakeDetect_Send(void);
ServoMotorRotateShakeDetect_Return* New_ServoMotorRotateShakeDetect_Return(void);
ServoMotorRotateShakeRotateDetect_Send* New_ServoMotorRotateShakeRotateDetect_Send(void);
ServoMotorRotateShakeRotateDetect_Return* New_ServoMotorRotateShakeRotateDetect_Return(void);
ServoMotorRotateTwoDirOnTime_Send* New_ServoMotorRotateTwoDirOnTime_Send(void);
ServoMotorRotateTwoDirOnTime_Return* New_ServoMotorRotateTwoDirOnTime_Return(void);
ServoMotorStatetGet_Send* New_ServoMotorStatetGet_Send(void);
ServoMotorStatetGet_Return* New_ServoMotorStatetGet_Return(void);
ServoMotorRotateStart_Send* New_ServoMotorRotateStart_Send(void);
ServoMotorRotateStart_Return* New_ServoMotorRotateStart_Return(void);
ServoMotorRotateStop_Send* New_ServoMotorRotateStop_Send(void);
ServoMotorRotateStop_Return* New_ServoMotorRotateStop_Return(void);
ServoMotorReset_Send* New_ServoMotorReset_Send(void);
ServoMotorReset_Return* New_ServoMotorReset_Return(void);
ServoMotorAutoCalibratio_Send* New_ServoMotorAutoCalibratio_Send(void);
ServoMotorAutoCalibratio_Return* New_ServoMotorAutoCalibratio_Return(void);
ServoMotorShake_Send* New_ServoMotorShake_Send(void);
ServoMotorShake_Return* New_ServoMotorShake_Return(void);
ServoMotorBaseCtrl_Send* New_ServoMotorBaseCtrl_Send(void);
ServoMotorBaseCtrl_Return* New_ServoMotorBaseCtrl_Return(void);
ServoMotorMove_Send* New_ServoMotorMove_Send(void);
ServoMotorMove_Return* New_ServoMotorMove_Return(void);
RGBSensorGet_Send* New_RGBSensorGet_Send(void);
RGBSensorGet_Return* New_RGBSensorGet_Return(void);
SerialDataReport_Send* New_SerialDataReport_Send(void);
SerialDataReport_Return* New_SerialDataReport_Return(void);
AddressBroadcastSend_Send* New_AddressBroadcastSend_Send(void);
AddressBroadcastSend_Return* New_AddressBroadcastSend_Return(void);
AddressBroadcastReceive_Send* New_AddressBroadcastReceive_Send(void);
AddressBroadcastReceive_Return* New_AddressBroadcastReceive_Return(void);
StartCheckPress_Send* New_StartCheckPress_Send(void);
StartCheckPress_Return* New_StartCheckPress_Return(void);
EndCheckPress_Send* New_EndCheckPress_Send(void);
EndCheckPress_Return* New_EndCheckPress_Return(void);
Blending_Send* New_Blending_Send(void);
Blending_Return* New_Blending_Return(void);
GetEPOSType_Send* New_GetEPOSType_Send(void);
GetEPOSType_Return* New_GetEPOSType_Return(void);
SetEPOSType_Send* New_SetEPOSType_Send(void);
SetEPOSType_Return* New_SetEPOSType_Return(void);
DACSet_Send* New_DACSet_Send(void);
DACSet_Return* New_DACSet_Return(void);
LightCtrl_Send* New_LightCtrl_Send(void);
LightCtrl_Return* New_LightCtrl_Return(void);
DCMotorMoveSeq_Send_m1_MoveArgs* New_DCMotorMoveSeq_Send_m1_MoveArgs(void);
DCMotorMoveSeq_Send* New_DCMotorMoveSeq_Send(void);
DCMotorMoveSeq_Return* New_DCMotorMoveSeq_Return(void);
DCMotorMoveTmc_Send* New_DCMotorMoveTmc_Send(void);
DCMotorMoveTmc_Return* New_DCMotorMoveTmc_Return(void);
PressureRead_Send* New_PressureRead_Send(void);
PressureRead_Return* New_PressureRead_Return(void);
LittleMotorCtrl_Send* New_LittleMotorCtrl_Send(void);
LittleMotorCtrl_Return* New_LittleMotorCtrl_Return(void);
CongestionDetection_Send* New_CongestionDetection_Send(void);
CongestionDetection_Return* New_CongestionDetection_Return(void);
LaserCtrol_Send* New_LaserCtrol_Send(void);
LaserCtrol_Return* New_LaserCtrol_Return(void);
NFL_ReadConfig_Send* New_NFL_ReadConfig_Send(void);
NFL_ReadConfig_Return* New_NFL_ReadConfig_Return(void);
NFL_WriteConfig_Send* New_NFL_WriteConfig_Send(void);
NFL_WriteConfig_Return* New_NFL_WriteConfig_Return(void);
NFL_ReadDataStatus_Send* New_NFL_ReadDataStatus_Send(void);
NFL_ReadDataStatus_Return* New_NFL_ReadDataStatus_Return(void);
NFL_ReadData_Send* New_NFL_ReadData_Send(void);
NFL_ReadData_Return* New_NFL_ReadData_Return(void);
NFL_DataCollectStart_Send* New_NFL_DataCollectStart_Send(void);
NFL_DataCollectStart_Return* New_NFL_DataCollectStart_Return(void);
NFL_DataCollectStop_Send* New_NFL_DataCollectStop_Send(void);
NFL_DataCollectStop_Return* New_NFL_DataCollectStop_Return(void);
NFL_ThresholdConfig_Send* New_NFL_ThresholdConfig_Send(void);
NFL_ThresholdConfig_Return* New_NFL_ThresholdConfig_Return(void);
NFL_StandbyConfig_Send* New_NFL_StandbyConfig_Send(void);
NFL_StandbyConfig_Return* New_NFL_StandbyConfig_Return(void);
NFL_ActivateConfig_Send* New_NFL_ActivateConfig_Send(void);
NFL_ActivateConfig_Return* New_NFL_ActivateConfig_Return(void);
NFL_ReadTemperature_Send* New_NFL_ReadTemperature_Send(void);
NFL_ReadTemperature_Return* New_NFL_ReadTemperature_Return(void);
NFL_SendEvent_Send* New_NFL_SendEvent_Send(void);
NFL_SendEvent_Return* New_NFL_SendEvent_Return(void);
PowerCtrl_Send* New_PowerCtrl_Send(void);
PowerCtrl_Return* New_PowerCtrl_Return(void);
SendErrorCode_Send* New_SendErrorCode_Send(void);
SendErrorCode_Return* New_SendErrorCode_Return(void);
IOCtrlByEvent_Send_m1_PortArgs* New_IOCtrlByEvent_Send_m1_PortArgs(void);
IOCtrlByEvent_Send_m2_PortArgs* New_IOCtrlByEvent_Send_m2_PortArgs(void);
IOCtrlByEvent_Send* New_IOCtrlByEvent_Send(void);
IOCtrlByEvent_Return* New_IOCtrlByEvent_Return(void);
Dsp_DHCP_Send* New_Dsp_DHCP_Send_Array(int cnt);
Dsp_DHCP_Return* New_Dsp_DHCP_Return_Array(int cnt);
Ping_Send* New_Ping_Send_Array(int cnt);
Ping_Return* New_Ping_Return_Array(int cnt);
FilePush_Send* New_FilePush_Send_Array(int cnt);
FilePush_Return* New_FilePush_Return_Array(int cnt);
InstrumentStateReport_Send* New_InstrumentStateReport_Send_Array(int cnt);
InstrumentStateReport_Return* New_InstrumentStateReport_Return_Array(int cnt);
ProtoTest_Send* New_ProtoTest_Send_Array(int cnt);
ProtoTest_Return* New_ProtoTest_Return_Array(int cnt);
McuErrorToPC_Send* New_McuErrorToPC_Send_Array(int cnt);
McuErrorToPC_Return* New_McuErrorToPC_Return_Array(int cnt);
AutoAdapter_Send* New_AutoAdapter_Send_Array(int cnt);
AutoAdapter_Return* New_AutoAdapter_Return_Array(int cnt);
MotorMultiMove_Send_m1_MoveArgs* New_MotorMultiMove_Send_m1_MoveArgs_Array(int cnt);
MotorMultiMove_Send* New_MotorMultiMove_Send_Array(int cnt);
MotorMultiMove_Return* New_MotorMultiMove_Return_Array(int cnt);
MotorMove_Send* New_MotorMove_Send_Array(int cnt);
MotorMove_Return* New_MotorMove_Return_Array(int cnt);
MotorStatetGet_Send* New_MotorStatetGet_Send_Array(int cnt);
MotorStatetGet_Return* New_MotorStatetGet_Return_Array(int cnt);
MotorRotateStart_Send* New_MotorRotateStart_Send_Array(int cnt);
MotorRotateStart_Return* New_MotorRotateStart_Return_Array(int cnt);
MotorRotateStop_Send* New_MotorRotateStop_Send_Array(int cnt);
MotorRotateStop_Return* New_MotorRotateStop_Return_Array(int cnt);
MotorRotate_Send* New_MotorRotate_Send_Array(int cnt);
MotorRotate_Return* New_MotorRotate_Return_Array(int cnt);
MotorReset_Send* New_MotorReset_Send_Array(int cnt);
MotorReset_Return* New_MotorReset_Return_Array(int cnt);
SensorValueOperation_Send* New_SensorValueOperation_Send_Array(int cnt);
SensorValueOperation_Return* New_SensorValueOperation_Return_Array(int cnt);
ADAutoCaliStart_Send* New_ADAutoCaliStart_Send_Array(int cnt);
ADAutoCaliStart_Return* New_ADAutoCaliStart_Return_Array(int cnt);
TemperatureGet_Send* New_TemperatureGet_Send_Array(int cnt);
TemperatureGet_Return* New_TemperatureGet_Return_Array(int cnt);
TemperatureOpen_Send* New_TemperatureOpen_Send_Array(int cnt);
TemperatureOpen_Return* New_TemperatureOpen_Return_Array(int cnt);
TemperatureClose_Send* New_TemperatureClose_Send_Array(int cnt);
TemperatureClose_Return* New_TemperatureClose_Return_Array(int cnt);
BarCodeGet_Send* New_BarCodeGet_Send_Array(int cnt);
BarCodeGet_Return* New_BarCodeGet_Return_Array(int cnt);
FirmWareConfig_Send* New_FirmWareConfig_Send_Array(int cnt);
FirmWareConfig_Return* New_FirmWareConfig_Return_Array(int cnt);
FirmWareTransmitFile_Send* New_FirmWareTransmitFile_Send_Array(int cnt);
FirmWareTransmitFile_Return* New_FirmWareTransmitFile_Return_Array(int cnt);
McuReset_Send* New_McuReset_Send_Array(int cnt);
McuReset_Return* New_McuReset_Return_Array(int cnt);
FirmWareVersionGet_Send* New_FirmWareVersionGet_Send_Array(int cnt);
FirmWareVersionGet_Return* New_FirmWareVersionGet_Return_Array(int cnt);
MotorConfigSet_Send* New_MotorConfigSet_Send_Array(int cnt);
MotorConfigSet_Return* New_MotorConfigSet_Return_Array(int cnt);
MotorMoveEvent_Send* New_MotorMoveEvent_Send_Array(int cnt);
MotorMoveEvent_Return* New_MotorMoveEvent_Return_Array(int cnt);
McuIoStateChangeEvent_Send* New_McuIoStateChangeEvent_Send_Array(int cnt);
McuIoStateChangeEvent_Return* New_McuIoStateChangeEvent_Return_Array(int cnt);
SampleUnitSensorGet_Send* New_SampleUnitSensorGet_Send_Array(int cnt);
SampleUnitSensorGet_Return* New_SampleUnitSensorGet_Return_Array(int cnt);
AirPumpOpen_Send* New_AirPumpOpen_Send_Array(int cnt);
AirPumpOpen_Return* New_AirPumpOpen_Return_Array(int cnt);
AirPumpClose_Send* New_AirPumpClose_Send_Array(int cnt);
AirPumpClose_Return* New_AirPumpClose_Return_Array(int cnt);

ADSampleRead_Send* New_ADSampleRead_Send_Array(int cnt);
ADSampleRead_Return* New_ADSampleRead_Return_Array(int cnt);

MotorMoveGPIOEvent_Send* New_MotorMoveGPIOEvent_Send_Array(int cnt);
MotorMoveGPIOEvent_Return* New_MotorMoveGPIOEvent_Return_Array(int cnt);
MotorConfigSetNew_Send* New_MotorConfigSetNew_Send_Array(int cnt);
MotorConfigSetNew_Return* New_MotorConfigSetNew_Return_Array(int cnt);
ExecutesCtrl_Send* New_ExecutesCtrl_Send_Array(int cnt);
ExecutesCtrl_Return* New_ExecutesCtrl_Return_Array(int cnt);
ADDetectConfigSet_Send* New_ADDetectConfigSet_Send_Array(int cnt);
ADDetectConfigSet_Return* New_ADDetectConfigSet_Return_Array(int cnt);

ADDataRead_Send* New_ADDataRead_Send_Array(int cnt);
ADDataRead_Return* New_ADDataRead_Return_Array(int cnt);
/*
这里数据接口 构造数组函数
*/
DiskInstalledRed_Send* New_DiskInstalledRed_Send_Array(int cnt);
DiskInstalledRed_Return* New_DiskInstalledRed_Return_Array(int cnt);


ADDataSeriesSend_Send* New_ADDataSeriesSend_Send_Array(int cnt);
ADDataSeriesSend_Return* New_ADDataSeriesSend_Return_Array(int cnt);
UartTransmit_Send* New_UartTransmit_Send_Array(int cnt);
UartTransmit_Return* New_UartTransmit_Return_Array(int cnt);
UartReceiveEvent_Send* New_UartReceiveEvent_Send_Array(int cnt);
UartReceiveEvent_Return* New_UartReceiveEvent_Return_Array(int cnt);
SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* New_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_Array(int cnt);
SeqMotorMultiMove_Send_m1_Sequence* New_SeqMotorMultiMove_Send_m1_Sequence_Array(int cnt);
SeqMotorMultiMove_Send* New_SeqMotorMultiMove_Send_Array(int cnt);
SeqMotorMultiMove_Return* New_SeqMotorMultiMove_Return_Array(int cnt);
PressureConfig_Send* New_PressureConfig_Send_Array(int cnt);
PressureConfig_Return* New_PressureConfig_Return_Array(int cnt);
PressureDataGet_Send* New_PressureDataGet_Send_Array(int cnt);
PressureDataGet_Return* New_PressureDataGet_Return_Array(int cnt);
MotorAbort_Send* New_MotorAbort_Send_Array(int cnt);
MotorAbort_Return* New_MotorAbort_Return_Array(int cnt);
PressureEvent_Send* New_PressureEvent_Send_Array(int cnt);
PressureEvent_Return* New_PressureEvent_Return_Array(int cnt);
MotorMoveSeq_Send_m1_MoveArgs* New_MotorMoveSeq_Send_m1_MoveArgs_Array(int cnt);
MotorMoveSeq_Send* New_MotorMoveSeq_Send_Array(int cnt);
MotorMoveSeq_Return* New_MotorMoveSeq_Return_Array(int cnt);
FirmWareReadyEvent_Send* New_FirmWareReadyEvent_Send_Array(int cnt);
FirmWareReadyEvent_Return* New_FirmWareReadyEvent_Return_Array(int cnt);
MotorConfigGet_Send* New_MotorConfigGet_Send_Array(int cnt);
MotorConfigGet_Return* New_MotorConfigGet_Return_Array(int cnt);
PWM_Set_Send* New_PWM_Set_Send_Array(int cnt);
PWM_Set_Return* New_PWM_Set_Return_Array(int cnt);
TurnTable_Move_Send* New_TurnTable_Move_Send_Array(int cnt);
TurnTable_Move_Return* New_TurnTable_Move_Return_Array(int cnt);
EncodeTurnTableFindZero_Send* New_EncodeTurnTableFindZero_Send_Array(int cnt);
EncodeTurnTableFindZero_Return* New_EncodeTurnTableFindZero_Return_Array(int cnt);
EncodeTurnTableRotateStart_Send* New_EncodeTurnTableRotateStart_Send_Array(int cnt);
EncodeTurnTableRotateStart_Return* New_EncodeTurnTableRotateStart_Return_Array(int cnt);
EncodeTurnTableRotateStop_Send* New_EncodeTurnTableRotateStop_Send_Array(int cnt);
EncodeTurnTableRotateStop_Return* New_EncodeTurnTableRotateStop_Return_Array(int cnt);
EncodeTurnTableStateGet_Send* New_EncodeTurnTableStateGet_Send_Array(int cnt);
EncodeTurnTableStateGet_Return* New_EncodeTurnTableStateGet_Return_Array(int cnt);
DCAdRead_Send* New_DCAdRead_Send_Array(int cnt);
DCAdRead_Return* New_DCAdRead_Return_Array(int cnt);
JSAdRead_Send* New_JSAdRead_Send_Array(int cnt);
JSAdRead_Return* New_JSAdRead_Return_Array(int cnt);
MotorMoveSyncLevel_Send* New_MotorMoveSyncLevel_Send_Array(int cnt);
MotorMoveSyncLevel_Return* New_MotorMoveSyncLevel_Return_Array(int cnt);
DCMotor_Send* New_DCMotor_Send_Array(int cnt);
DCMotor_Return* New_DCMotor_Return_Array(int cnt);
MotorCupIDSet_Send* New_MotorCupIDSet_Send_Array(int cnt);
MotorCupIDSet_Return* New_MotorCupIDSet_Return_Array(int cnt);
MotorCupIDInfoGet_Send* New_MotorCupIDInfoGet_Send_Array(int cnt);
MotorCupIDInfoGet_Return* New_MotorCupIDInfoGet_Return_Array(int cnt);
JSAdDataGet_Send* New_JSAdDataGet_Send_Array(int cnt);
JSAdDataGet_Return* New_JSAdDataGet_Return_Array(int cnt);
MotorMultiMoveTmc_Send_m1_MoveArgs* New_MotorMultiMoveTmc_Send_m1_MoveArgs_Array(int cnt);
MotorMultiMoveTmc_Send* New_MotorMultiMoveTmc_Send_Array(int cnt);
MotorMultiMoveTmc_Return* New_MotorMultiMoveTmc_Return_Array(int cnt);
MotorMoveTmc_Send* New_MotorMoveTmc_Send_Array(int cnt);
MotorMoveTmc_Return* New_MotorMoveTmc_Return_Array(int cnt);
MotorStatetGetTmc_Send* New_MotorStatetGetTmc_Send_Array(int cnt);
MotorStatetGetTmc_Return* New_MotorStatetGetTmc_Return_Array(int cnt);
MotorRotateStartTmc_Send* New_MotorRotateStartTmc_Send_Array(int cnt);
MotorRotateStartTmc_Return* New_MotorRotateStartTmc_Return_Array(int cnt);
MotorRotateStopTmc_Send* New_MotorRotateStopTmc_Send_Array(int cnt);
MotorRotateStopTmc_Return* New_MotorRotateStopTmc_Return_Array(int cnt);
MotorRotateTmc_Send* New_MotorRotateTmc_Send_Array(int cnt);
MotorRotateTmc_Return* New_MotorRotateTmc_Return_Array(int cnt);
MotorResetTmc_Send* New_MotorResetTmc_Send_Array(int cnt);
MotorResetTmc_Return* New_MotorResetTmc_Return_Array(int cnt);
MotorConfigSetTmc_Send* New_MotorConfigSetTmc_Send_Array(int cnt);
MotorConfigSetTmc_Return* New_MotorConfigSetTmc_Return_Array(int cnt);
MotorMoveGPIOEventTmc_Send* New_MotorMoveGPIOEventTmc_Send_Array(int cnt);
MotorMoveGPIOEventTmc_Return* New_MotorMoveGPIOEventTmc_Return_Array(int cnt);
MotorRotateShake_Send* New_MotorRotateShake_Send_Array(int cnt);
MotorRotateShake_Return* New_MotorRotateShake_Return_Array(int cnt);
LiquidLevelResultGet_Send* New_LiquidLevelResultGet_Send_Array(int cnt);
LiquidLevelResultGet_Return* New_LiquidLevelResultGet_Return_Array(int cnt);
MotorDetectLiquidLevel_Send* New_MotorDetectLiquidLevel_Send_Array(int cnt);
MotorDetectLiquidLevel_Return* New_MotorDetectLiquidLevel_Return_Array(int cnt);
RFIDValueOperation_Send* New_RFIDValueOperation_Send_Array(int cnt);
RFIDValueOperation_Return* New_RFIDValueOperation_Return_Array(int cnt);
IoSimple_Send* New_IoSimple_Send_Array(int cnt);
IoSimple_Return* New_IoSimple_Return_Array(int cnt);
IoOperation_Send_m1_IoArgs_m1_Controls* New_IoOperation_Send_m1_IoArgs_m1_Controls_Array(int cnt);
IoOperation_Send_m1_IoArgs* New_IoOperation_Send_m1_IoArgs_Array(int cnt);
IoOperation_Send* New_IoOperation_Send_Array(int cnt);
IoOperation_Return* New_IoOperation_Return_Array(int cnt);
IoBlink_Send* New_IoBlink_Send_Array(int cnt);
IoBlink_Return* New_IoBlink_Return_Array(int cnt);
UartChangeBode_Send* New_UartChangeBode_Send_Array(int cnt);
UartChangeBode_Return* New_UartChangeBode_Return_Array(int cnt);
ServoMotorMultiRotate_Send_m1_MoveArgs* New_ServoMotorMultiRotate_Send_m1_MoveArgs_Array(int cnt);
ServoMotorMultiRotate_Send* New_ServoMotorMultiRotate_Send_Array(int cnt);
ServoMotorMultiRotate_Return* New_ServoMotorMultiRotate_Return_Array(int cnt);
ServoMotorRotateShakeDetect_Send* New_ServoMotorRotateShakeDetect_Send_Array(int cnt);
ServoMotorRotateShakeDetect_Return* New_ServoMotorRotateShakeDetect_Return_Array(int cnt);
ServoMotorRotateShakeRotateDetect_Send* New_ServoMotorRotateShakeRotateDetect_Send_Array(int cnt);
ServoMotorRotateShakeRotateDetect_Return* New_ServoMotorRotateShakeRotateDetect_Return_Array(int cnt);
ServoMotorRotateTwoDirOnTime_Send* New_ServoMotorRotateTwoDirOnTime_Send_Array(int cnt);
ServoMotorRotateTwoDirOnTime_Return* New_ServoMotorRotateTwoDirOnTime_Return_Array(int cnt);
ServoMotorStatetGet_Send* New_ServoMotorStatetGet_Send_Array(int cnt);
ServoMotorStatetGet_Return* New_ServoMotorStatetGet_Return_Array(int cnt);
ServoMotorRotateStart_Send* New_ServoMotorRotateStart_Send_Array(int cnt);
ServoMotorRotateStart_Return* New_ServoMotorRotateStart_Return_Array(int cnt);
ServoMotorRotateStop_Send* New_ServoMotorRotateStop_Send_Array(int cnt);
ServoMotorRotateStop_Return* New_ServoMotorRotateStop_Return_Array(int cnt);
ServoMotorReset_Send* New_ServoMotorReset_Send_Array(int cnt);
ServoMotorReset_Return* New_ServoMotorReset_Return_Array(int cnt);
ServoMotorAutoCalibratio_Send* New_ServoMotorAutoCalibratio_Send_Array(int cnt);
ServoMotorAutoCalibratio_Return* New_ServoMotorAutoCalibratio_Return_Array(int cnt);
ServoMotorShake_Send* New_ServoMotorShake_Send_Array(int cnt);
ServoMotorShake_Return* New_ServoMotorShake_Return_Array(int cnt);
ServoMotorBaseCtrl_Send* New_ServoMotorBaseCtrl_Send_Array(int cnt);
ServoMotorBaseCtrl_Return* New_ServoMotorBaseCtrl_Return_Array(int cnt);
ServoMotorMove_Send* New_ServoMotorMove_Send_Array(int cnt);
ServoMotorMove_Return* New_ServoMotorMove_Return_Array(int cnt);
RGBSensorGet_Send* New_RGBSensorGet_Send_Array(int cnt);
RGBSensorGet_Return* New_RGBSensorGet_Return_Array(int cnt);
SerialDataReport_Send* New_SerialDataReport_Send_Array(int cnt);
SerialDataReport_Return* New_SerialDataReport_Return_Array(int cnt);
AddressBroadcastSend_Send* New_AddressBroadcastSend_Send_Array(int cnt);
AddressBroadcastSend_Return* New_AddressBroadcastSend_Return_Array(int cnt);
AddressBroadcastReceive_Send* New_AddressBroadcastReceive_Send_Array(int cnt);
AddressBroadcastReceive_Return* New_AddressBroadcastReceive_Return_Array(int cnt);
StartCheckPress_Send* New_StartCheckPress_Send_Array(int cnt);
StartCheckPress_Return* New_StartCheckPress_Return_Array(int cnt);
EndCheckPress_Send* New_EndCheckPress_Send_Array(int cnt);
EndCheckPress_Return* New_EndCheckPress_Return_Array(int cnt);
Blending_Send* New_Blending_Send_Array(int cnt);
Blending_Return* New_Blending_Return_Array(int cnt);
GetEPOSType_Send* New_GetEPOSType_Send_Array(int cnt);
GetEPOSType_Return* New_GetEPOSType_Return_Array(int cnt);
SetEPOSType_Send* New_SetEPOSType_Send_Array(int cnt);
SetEPOSType_Return* New_SetEPOSType_Return_Array(int cnt);
DACSet_Send* New_DACSet_Send_Array(int cnt);
DACSet_Return* New_DACSet_Return_Array(int cnt);
LightCtrl_Send* New_LightCtrl_Send_Array(int cnt);
LightCtrl_Return* New_LightCtrl_Return_Array(int cnt);
DCMotorMoveSeq_Send_m1_MoveArgs* New_DCMotorMoveSeq_Send_m1_MoveArgs_Array(int cnt);
DCMotorMoveSeq_Send* New_DCMotorMoveSeq_Send_Array(int cnt);
DCMotorMoveSeq_Return* New_DCMotorMoveSeq_Return_Array(int cnt);
DCMotorMoveTmc_Send* New_DCMotorMoveTmc_Send_Array(int cnt);
DCMotorMoveTmc_Return* New_DCMotorMoveTmc_Return_Array(int cnt);
PressureRead_Send* New_PressureRead_Send_Array(int cnt);
PressureRead_Return* New_PressureRead_Return_Array(int cnt);
LittleMotorCtrl_Send* New_LittleMotorCtrl_Send_Array(int cnt);
LittleMotorCtrl_Return* New_LittleMotorCtrl_Return_Array(int cnt);
CongestionDetection_Send* New_CongestionDetection_Send_Array(int cnt);
CongestionDetection_Return* New_CongestionDetection_Return_Array(int cnt);
LaserCtrol_Send* New_LaserCtrol_Send_Array(int cnt);
LaserCtrol_Return* New_LaserCtrol_Return_Array(int cnt);
NFL_ReadConfig_Send* New_NFL_ReadConfig_Send_Array(int cnt);
NFL_ReadConfig_Return* New_NFL_ReadConfig_Return_Array(int cnt);
NFL_WriteConfig_Send* New_NFL_WriteConfig_Send_Array(int cnt);
NFL_WriteConfig_Return* New_NFL_WriteConfig_Return_Array(int cnt);
NFL_ReadDataStatus_Send* New_NFL_ReadDataStatus_Send_Array(int cnt);
NFL_ReadDataStatus_Return* New_NFL_ReadDataStatus_Return_Array(int cnt);
NFL_ReadData_Send* New_NFL_ReadData_Send_Array(int cnt);
NFL_ReadData_Return* New_NFL_ReadData_Return_Array(int cnt);
NFL_DataCollectStart_Send* New_NFL_DataCollectStart_Send_Array(int cnt);
NFL_DataCollectStart_Return* New_NFL_DataCollectStart_Return_Array(int cnt);
NFL_DataCollectStop_Send* New_NFL_DataCollectStop_Send_Array(int cnt);
NFL_DataCollectStop_Return* New_NFL_DataCollectStop_Return_Array(int cnt);
NFL_ThresholdConfig_Send* New_NFL_ThresholdConfig_Send_Array(int cnt);
NFL_ThresholdConfig_Return* New_NFL_ThresholdConfig_Return_Array(int cnt);
NFL_StandbyConfig_Send* New_NFL_StandbyConfig_Send_Array(int cnt);
NFL_StandbyConfig_Return* New_NFL_StandbyConfig_Return_Array(int cnt);
NFL_ActivateConfig_Send* New_NFL_ActivateConfig_Send_Array(int cnt);
NFL_ActivateConfig_Return* New_NFL_ActivateConfig_Return_Array(int cnt);
NFL_ReadTemperature_Send* New_NFL_ReadTemperature_Send_Array(int cnt);
NFL_ReadTemperature_Return* New_NFL_ReadTemperature_Return_Array(int cnt);
NFL_SendEvent_Send* New_NFL_SendEvent_Send_Array(int cnt);
NFL_SendEvent_Return* New_NFL_SendEvent_Return_Array(int cnt);
PowerCtrl_Send* New_PowerCtrl_Send_Array(int cnt);
PowerCtrl_Return* New_PowerCtrl_Return_Array(int cnt);
SendErrorCode_Send* New_SendErrorCode_Send_Array(int cnt);
SendErrorCode_Return* New_SendErrorCode_Return_Array(int cnt);
IOCtrlByEvent_Send_m1_PortArgs* New_IOCtrlByEvent_Send_m1_PortArgs_Array(int cnt);
IOCtrlByEvent_Send_m2_PortArgs* New_IOCtrlByEvent_Send_m2_PortArgs_Array(int cnt);
IOCtrlByEvent_Send* New_IOCtrlByEvent_Send_Array(int cnt);
IOCtrlByEvent_Return* New_IOCtrlByEvent_Return_Array(int cnt);
void Delete_Dsp_DHCP_Send(Dsp_DHCP_Send* ths);
void Delete_Dsp_DHCP_Return(Dsp_DHCP_Return* ths);
void Delete_Ping_Send(Ping_Send* ths);
void Delete_Ping_Return(Ping_Return* ths);
void Delete_FilePush_Send(FilePush_Send* ths);
void Delete_FilePush_Return(FilePush_Return* ths);
void Delete_InstrumentStateReport_Send(InstrumentStateReport_Send* ths);
void Delete_InstrumentStateReport_Return(InstrumentStateReport_Return* ths);
void Delete_ProtoTest_Send(ProtoTest_Send* ths);
void Delete_ProtoTest_Return(ProtoTest_Return* ths);
void Delete_McuErrorToPC_Send(McuErrorToPC_Send* ths);
void Delete_McuErrorToPC_Return(McuErrorToPC_Return* ths);
void Delete_AutoAdapter_Send(AutoAdapter_Send* ths);
void Delete_AutoAdapter_Return(AutoAdapter_Return* ths);
void Delete_MotorMultiMove_Send_m1_MoveArgs(MotorMultiMove_Send_m1_MoveArgs* ths);
void Delete_MotorMultiMove_Send(MotorMultiMove_Send* ths);
void Delete_MotorMultiMove_Return(MotorMultiMove_Return* ths);
void Delete_MotorMove_Send(MotorMove_Send* ths);
void Delete_MotorMove_Return(MotorMove_Return* ths);
void Delete_MotorStatetGet_Send(MotorStatetGet_Send* ths);
void Delete_MotorStatetGet_Return(MotorStatetGet_Return* ths);
void Delete_MotorRotateStart_Send(MotorRotateStart_Send* ths);
void Delete_MotorRotateStart_Return(MotorRotateStart_Return* ths);
void Delete_MotorRotateStop_Send(MotorRotateStop_Send* ths);
void Delete_MotorRotateStop_Return(MotorRotateStop_Return* ths);
void Delete_MotorRotate_Send(MotorRotate_Send* ths);
void Delete_MotorRotate_Return(MotorRotate_Return* ths);
void Delete_MotorReset_Send(MotorReset_Send* ths);
void Delete_MotorReset_Return(MotorReset_Return* ths);
void Delete_SensorValueOperation_Send(SensorValueOperation_Send* ths);
void Delete_SensorValueOperation_Return(SensorValueOperation_Return* ths);
void Delete_ADAutoCaliStart_Send(ADAutoCaliStart_Send* ths);
void Delete_ADAutoCaliStart_Return(ADAutoCaliStart_Return* ths);
void Delete_TemperatureGet_Send(TemperatureGet_Send* ths);
void Delete_TemperatureGet_Return(TemperatureGet_Return* ths);
void Delete_TemperatureOpen_Send(TemperatureOpen_Send* ths);
void Delete_TemperatureOpen_Return(TemperatureOpen_Return* ths);
void Delete_TemperatureClose_Send(TemperatureClose_Send* ths);
void Delete_TemperatureClose_Return(TemperatureClose_Return* ths);
void Delete_BarCodeGet_Send(BarCodeGet_Send* ths);
void Delete_BarCodeGet_Return(BarCodeGet_Return* ths);
void Delete_FirmWareConfig_Send(FirmWareConfig_Send* ths);
void Delete_FirmWareConfig_Return(FirmWareConfig_Return* ths);
void Delete_FirmWareTransmitFile_Send(FirmWareTransmitFile_Send* ths);
void Delete_FirmWareTransmitFile_Return(FirmWareTransmitFile_Return* ths);
void Delete_McuReset_Send(McuReset_Send* ths);
void Delete_McuReset_Return(McuReset_Return* ths);
void Delete_FirmWareVersionGet_Send(FirmWareVersionGet_Send* ths);
void Delete_FirmWareVersionGet_Return(FirmWareVersionGet_Return* ths);
void Delete_MotorConfigSet_Send(MotorConfigSet_Send* ths);
void Delete_MotorConfigSet_Return(MotorConfigSet_Return* ths);
void Delete_MotorMoveEvent_Send(MotorMoveEvent_Send* ths);
void Delete_MotorMoveEvent_Return(MotorMoveEvent_Return* ths);
void Delete_McuIoStateChangeEvent_Send(McuIoStateChangeEvent_Send* ths);
void Delete_McuIoStateChangeEvent_Return(McuIoStateChangeEvent_Return* ths);
void Delete_SampleUnitSensorGet_Send(SampleUnitSensorGet_Send* ths);
void Delete_SampleUnitSensorGet_Return(SampleUnitSensorGet_Return* ths);
void Delete_AirPumpOpen_Send(AirPumpOpen_Send* ths);
void Delete_AirPumpOpen_Return(AirPumpOpen_Return* ths);
void Delete_AirPumpClose_Send(AirPumpClose_Send* ths);
void Delete_AirPumpClose_Return(AirPumpClose_Return* ths);
void Delete_ADSampleRead_Send(ADSampleRead_Send* ths);
void Delete_ADSampleRead_Return(ADSampleRead_Return* ths);
void Delete_MotorMoveGPIOEvent_Send(MotorMoveGPIOEvent_Send* ths);
void Delete_MotorMoveGPIOEvent_Return(MotorMoveGPIOEvent_Return* ths);
void Delete_MotorConfigSetNew_Send(MotorConfigSetNew_Send* ths);
void Delete_MotorConfigSetNew_Return(MotorConfigSetNew_Return* ths);
void Delete_ExecutesCtrl_Send(ExecutesCtrl_Send* ths);
void Delete_ExecutesCtrl_Return(ExecutesCtrl_Return* ths);
void Delete_ADDetectConfigSet_Send(ADDetectConfigSet_Send* ths);
void Delete_ADDetectConfigSet_Return(ADDetectConfigSet_Return* ths);

void Delete_ADDataRead_Send(ADDataRead_Send* ths);
void Delete_ADDataRead_Return(ADDataRead_Return* ths);
/*
这里数据接口 析构函数
*/
void Delete_DiskInstalledRed_Send(DiskInstalledRed_Send* ths);
void Delete_DiskInstalledRed_Return(DiskInstalledRed_Return* ths);


void Delete_ADDataSeriesSend_Send(ADDataSeriesSend_Send* ths);
void Delete_ADDataSeriesSend_Return(ADDataSeriesSend_Return* ths);
void Delete_UartTransmit_Send(UartTransmit_Send* ths);
void Delete_UartTransmit_Return(UartTransmit_Return* ths);
void Delete_UartReceiveEvent_Send(UartReceiveEvent_Send* ths);
void Delete_UartReceiveEvent_Return(UartReceiveEvent_Return* ths);
void Delete_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* ths);
void Delete_SeqMotorMultiMove_Send_m1_Sequence(SeqMotorMultiMove_Send_m1_Sequence* ths);
void Delete_SeqMotorMultiMove_Send(SeqMotorMultiMove_Send* ths);
void Delete_SeqMotorMultiMove_Return(SeqMotorMultiMove_Return* ths);
void Delete_PressureConfig_Send(PressureConfig_Send* ths);
void Delete_PressureConfig_Return(PressureConfig_Return* ths);
void Delete_PressureDataGet_Send(PressureDataGet_Send* ths);
void Delete_PressureDataGet_Return(PressureDataGet_Return* ths);
void Delete_MotorAbort_Send(MotorAbort_Send* ths);
void Delete_MotorAbort_Return(MotorAbort_Return* ths);
void Delete_PressureEvent_Send(PressureEvent_Send* ths);
void Delete_PressureEvent_Return(PressureEvent_Return* ths);
void Delete_MotorMoveSeq_Send_m1_MoveArgs(MotorMoveSeq_Send_m1_MoveArgs* ths);
void Delete_MotorMoveSeq_Send(MotorMoveSeq_Send* ths);
void Delete_MotorMoveSeq_Return(MotorMoveSeq_Return* ths);
void Delete_FirmWareReadyEvent_Send(FirmWareReadyEvent_Send* ths);
void Delete_FirmWareReadyEvent_Return(FirmWareReadyEvent_Return* ths);
void Delete_MotorConfigGet_Send(MotorConfigGet_Send* ths);
void Delete_MotorConfigGet_Return(MotorConfigGet_Return* ths);
void Delete_PWM_Set_Send(PWM_Set_Send* ths);
void Delete_PWM_Set_Return(PWM_Set_Return* ths);
void Delete_TurnTable_Move_Send(TurnTable_Move_Send* ths);
void Delete_TurnTable_Move_Return(TurnTable_Move_Return* ths);
void Delete_EncodeTurnTableFindZero_Send(EncodeTurnTableFindZero_Send* ths);
void Delete_EncodeTurnTableFindZero_Return(EncodeTurnTableFindZero_Return* ths);
void Delete_EncodeTurnTableRotateStart_Send(EncodeTurnTableRotateStart_Send* ths);
void Delete_EncodeTurnTableRotateStart_Return(EncodeTurnTableRotateStart_Return* ths);
void Delete_EncodeTurnTableRotateStop_Send(EncodeTurnTableRotateStop_Send* ths);
void Delete_EncodeTurnTableRotateStop_Return(EncodeTurnTableRotateStop_Return* ths);
void Delete_EncodeTurnTableStateGet_Send(EncodeTurnTableStateGet_Send* ths);
void Delete_EncodeTurnTableStateGet_Return(EncodeTurnTableStateGet_Return* ths);
void Delete_DCAdRead_Send(DCAdRead_Send* ths);
void Delete_DCAdRead_Return(DCAdRead_Return* ths);
void Delete_JSAdRead_Send(JSAdRead_Send* ths);
void Delete_JSAdRead_Return(JSAdRead_Return* ths);
void Delete_MotorMoveSyncLevel_Send(MotorMoveSyncLevel_Send* ths);
void Delete_MotorMoveSyncLevel_Return(MotorMoveSyncLevel_Return* ths);
void Delete_DCMotor_Send(DCMotor_Send* ths);
void Delete_DCMotor_Return(DCMotor_Return* ths);
void Delete_MotorCupIDSet_Send(MotorCupIDSet_Send* ths);
void Delete_MotorCupIDSet_Return(MotorCupIDSet_Return* ths);
void Delete_MotorCupIDInfoGet_Send(MotorCupIDInfoGet_Send* ths);
void Delete_MotorCupIDInfoGet_Return(MotorCupIDInfoGet_Return* ths);
void Delete_JSAdDataGet_Send(JSAdDataGet_Send* ths);
void Delete_JSAdDataGet_Return(JSAdDataGet_Return* ths);
void Delete_MotorMultiMoveTmc_Send_m1_MoveArgs(MotorMultiMoveTmc_Send_m1_MoveArgs* ths);
void Delete_MotorMultiMoveTmc_Send(MotorMultiMoveTmc_Send* ths);
void Delete_MotorMultiMoveTmc_Return(MotorMultiMoveTmc_Return* ths);
void Delete_MotorMoveTmc_Send(MotorMoveTmc_Send* ths);
void Delete_MotorMoveTmc_Return(MotorMoveTmc_Return* ths);
void Delete_MotorStatetGetTmc_Send(MotorStatetGetTmc_Send* ths);
void Delete_MotorStatetGetTmc_Return(MotorStatetGetTmc_Return* ths);
void Delete_MotorRotateStartTmc_Send(MotorRotateStartTmc_Send* ths);
void Delete_MotorRotateStartTmc_Return(MotorRotateStartTmc_Return* ths);
void Delete_MotorRotateStopTmc_Send(MotorRotateStopTmc_Send* ths);
void Delete_MotorRotateStopTmc_Return(MotorRotateStopTmc_Return* ths);
void Delete_MotorRotateTmc_Send(MotorRotateTmc_Send* ths);
void Delete_MotorRotateTmc_Return(MotorRotateTmc_Return* ths);
void Delete_MotorResetTmc_Send(MotorResetTmc_Send* ths);
void Delete_MotorResetTmc_Return(MotorResetTmc_Return* ths);
void Delete_MotorConfigSetTmc_Send(MotorConfigSetTmc_Send* ths);
void Delete_MotorConfigSetTmc_Return(MotorConfigSetTmc_Return* ths);
void Delete_MotorMoveGPIOEventTmc_Send(MotorMoveGPIOEventTmc_Send* ths);
void Delete_MotorMoveGPIOEventTmc_Return(MotorMoveGPIOEventTmc_Return* ths);
void Delete_MotorRotateShake_Send(MotorRotateShake_Send* ths);
void Delete_MotorRotateShake_Return(MotorRotateShake_Return* ths);
void Delete_LiquidLevelResultGet_Send(LiquidLevelResultGet_Send* ths);
void Delete_LiquidLevelResultGet_Return(LiquidLevelResultGet_Return* ths);
void Delete_MotorDetectLiquidLevel_Send(MotorDetectLiquidLevel_Send* ths);
void Delete_MotorDetectLiquidLevel_Return(MotorDetectLiquidLevel_Return* ths);
void Delete_RFIDValueOperation_Send(RFIDValueOperation_Send* ths);
void Delete_RFIDValueOperation_Return(RFIDValueOperation_Return* ths);
void Delete_IoSimple_Send(IoSimple_Send* ths);
void Delete_IoSimple_Return(IoSimple_Return* ths);
void Delete_IoOperation_Send_m1_IoArgs_m1_Controls(IoOperation_Send_m1_IoArgs_m1_Controls* ths);
void Delete_IoOperation_Send_m1_IoArgs(IoOperation_Send_m1_IoArgs* ths);
void Delete_IoOperation_Send(IoOperation_Send* ths);
void Delete_IoOperation_Return(IoOperation_Return* ths);
void Delete_IoBlink_Send(IoBlink_Send* ths);
void Delete_IoBlink_Return(IoBlink_Return* ths);
void Delete_UartChangeBode_Send(UartChangeBode_Send* ths);
void Delete_UartChangeBode_Return(UartChangeBode_Return* ths);
void Delete_ServoMotorMultiRotate_Send_m1_MoveArgs(ServoMotorMultiRotate_Send_m1_MoveArgs* ths);
void Delete_ServoMotorMultiRotate_Send(ServoMotorMultiRotate_Send* ths);
void Delete_ServoMotorMultiRotate_Return(ServoMotorMultiRotate_Return* ths);
void Delete_ServoMotorRotateShakeDetect_Send(ServoMotorRotateShakeDetect_Send* ths);
void Delete_ServoMotorRotateShakeDetect_Return(ServoMotorRotateShakeDetect_Return* ths);
void Delete_ServoMotorRotateShakeRotateDetect_Send(ServoMotorRotateShakeRotateDetect_Send* ths);
void Delete_ServoMotorRotateShakeRotateDetect_Return(ServoMotorRotateShakeRotateDetect_Return* ths);
void Delete_ServoMotorRotateTwoDirOnTime_Send(ServoMotorRotateTwoDirOnTime_Send* ths);
void Delete_ServoMotorRotateTwoDirOnTime_Return(ServoMotorRotateTwoDirOnTime_Return* ths);
void Delete_ServoMotorStatetGet_Send(ServoMotorStatetGet_Send* ths);
void Delete_ServoMotorStatetGet_Return(ServoMotorStatetGet_Return* ths);
void Delete_ServoMotorRotateStart_Send(ServoMotorRotateStart_Send* ths);
void Delete_ServoMotorRotateStart_Return(ServoMotorRotateStart_Return* ths);
void Delete_ServoMotorRotateStop_Send(ServoMotorRotateStop_Send* ths);
void Delete_ServoMotorRotateStop_Return(ServoMotorRotateStop_Return* ths);
void Delete_ServoMotorReset_Send(ServoMotorReset_Send* ths);
void Delete_ServoMotorReset_Return(ServoMotorReset_Return* ths);
void Delete_ServoMotorAutoCalibratio_Send(ServoMotorAutoCalibratio_Send* ths);
void Delete_ServoMotorAutoCalibratio_Return(ServoMotorAutoCalibratio_Return* ths);
void Delete_ServoMotorShake_Send(ServoMotorShake_Send* ths);
void Delete_ServoMotorShake_Return(ServoMotorShake_Return* ths);
void Delete_ServoMotorBaseCtrl_Send(ServoMotorBaseCtrl_Send* ths);
void Delete_ServoMotorBaseCtrl_Return(ServoMotorBaseCtrl_Return* ths);
void Delete_ServoMotorMove_Send(ServoMotorMove_Send* ths);
void Delete_ServoMotorMove_Return(ServoMotorMove_Return* ths);
void Delete_RGBSensorGet_Send(RGBSensorGet_Send* ths);
void Delete_RGBSensorGet_Return(RGBSensorGet_Return* ths);
void Delete_SerialDataReport_Send(SerialDataReport_Send* ths);
void Delete_SerialDataReport_Return(SerialDataReport_Return* ths);
void Delete_AddressBroadcastSend_Send(AddressBroadcastSend_Send* ths);
void Delete_AddressBroadcastSend_Return(AddressBroadcastSend_Return* ths);
void Delete_AddressBroadcastReceive_Send(AddressBroadcastReceive_Send* ths);
void Delete_AddressBroadcastReceive_Return(AddressBroadcastReceive_Return* ths);
void Delete_StartCheckPress_Send(StartCheckPress_Send* ths);
void Delete_StartCheckPress_Return(StartCheckPress_Return* ths);
void Delete_EndCheckPress_Send(EndCheckPress_Send* ths);
void Delete_EndCheckPress_Return(EndCheckPress_Return* ths);
void Delete_Blending_Send(Blending_Send* ths);
void Delete_Blending_Return(Blending_Return* ths);
void Delete_GetEPOSType_Send(GetEPOSType_Send* ths);
void Delete_GetEPOSType_Return(GetEPOSType_Return* ths);
void Delete_SetEPOSType_Send(SetEPOSType_Send* ths);
void Delete_SetEPOSType_Return(SetEPOSType_Return* ths);
void Delete_DACSet_Send(DACSet_Send* ths);
void Delete_DACSet_Return(DACSet_Return* ths);
void Delete_LightCtrl_Send(LightCtrl_Send* ths);
void Delete_LightCtrl_Return(LightCtrl_Return* ths);
void Delete_DCMotorMoveSeq_Send_m1_MoveArgs(DCMotorMoveSeq_Send_m1_MoveArgs* ths);
void Delete_DCMotorMoveSeq_Send(DCMotorMoveSeq_Send* ths);
void Delete_DCMotorMoveSeq_Return(DCMotorMoveSeq_Return* ths);
void Delete_DCMotorMoveTmc_Send(DCMotorMoveTmc_Send* ths);
void Delete_DCMotorMoveTmc_Return(DCMotorMoveTmc_Return* ths);
void Delete_PressureRead_Send(PressureRead_Send* ths);
void Delete_PressureRead_Return(PressureRead_Return* ths);
void Delete_LittleMotorCtrl_Send(LittleMotorCtrl_Send* ths);
void Delete_LittleMotorCtrl_Return(LittleMotorCtrl_Return* ths);
void Delete_CongestionDetection_Send(CongestionDetection_Send* ths);
void Delete_CongestionDetection_Return(CongestionDetection_Return* ths);
void Delete_LaserCtrol_Send(LaserCtrol_Send* ths);
void Delete_LaserCtrol_Return(LaserCtrol_Return* ths);
void Delete_NFL_ReadConfig_Send(NFL_ReadConfig_Send* ths);
void Delete_NFL_ReadConfig_Return(NFL_ReadConfig_Return* ths);
void Delete_NFL_WriteConfig_Send(NFL_WriteConfig_Send* ths);
void Delete_NFL_WriteConfig_Return(NFL_WriteConfig_Return* ths);
void Delete_NFL_ReadDataStatus_Send(NFL_ReadDataStatus_Send* ths);
void Delete_NFL_ReadDataStatus_Return(NFL_ReadDataStatus_Return* ths);
void Delete_NFL_ReadData_Send(NFL_ReadData_Send* ths);
void Delete_NFL_ReadData_Return(NFL_ReadData_Return* ths);
void Delete_NFL_DataCollectStart_Send(NFL_DataCollectStart_Send* ths);
void Delete_NFL_DataCollectStart_Return(NFL_DataCollectStart_Return* ths);
void Delete_NFL_DataCollectStop_Send(NFL_DataCollectStop_Send* ths);
void Delete_NFL_DataCollectStop_Return(NFL_DataCollectStop_Return* ths);
void Delete_NFL_ThresholdConfig_Send(NFL_ThresholdConfig_Send* ths);
void Delete_NFL_ThresholdConfig_Return(NFL_ThresholdConfig_Return* ths);
void Delete_NFL_StandbyConfig_Send(NFL_StandbyConfig_Send* ths);
void Delete_NFL_StandbyConfig_Return(NFL_StandbyConfig_Return* ths);
void Delete_NFL_ActivateConfig_Send(NFL_ActivateConfig_Send* ths);
void Delete_NFL_ActivateConfig_Return(NFL_ActivateConfig_Return* ths);
void Delete_NFL_ReadTemperature_Send(NFL_ReadTemperature_Send* ths);
void Delete_NFL_ReadTemperature_Return(NFL_ReadTemperature_Return* ths);
void Delete_NFL_SendEvent_Send(NFL_SendEvent_Send* ths);
void Delete_NFL_SendEvent_Return(NFL_SendEvent_Return* ths);
void Delete_PowerCtrl_Send(PowerCtrl_Send* ths);
void Delete_PowerCtrl_Return(PowerCtrl_Return* ths);
void Delete_SendErrorCode_Send(SendErrorCode_Send* ths);
void Delete_SendErrorCode_Return(SendErrorCode_Return* ths);
void Delete_IOCtrlByEvent_Send_m1_PortArgs(IOCtrlByEvent_Send_m1_PortArgs* ths);
void Delete_IOCtrlByEvent_Send_m2_PortArgs(IOCtrlByEvent_Send_m2_PortArgs* ths);
void Delete_IOCtrlByEvent_Send(IOCtrlByEvent_Send* ths);
void Delete_IOCtrlByEvent_Return(IOCtrlByEvent_Return* ths);
#endif
