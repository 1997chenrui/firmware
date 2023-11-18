//------------- 函数定义 ----------------------------------
#include "Protocols.h"
//#define PTR_ASSERT(ptr)  if((ptr)==NULL)   while(1) printf("mf=%d\r\n",__LINE__)  
#define PTR_ASSERT(ptr)  do{\
    if(ptr == NULL)\
    {\
        int cnt = 0;\
        while(1)\
        {\
            if(malloc(8) != NULL)\
            {\
                cnt++;\
            }else\
            {\
                while(1) printf("mf=%d,cnt=%d\r\n",__LINE__,cnt);\
            }\
        }\
    }\
}while(0)

BytesBufferNode * Dsp_DHCP_Send_ToBytes(Dsp_DHCP_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uid_Length),sizeof(ths->m1_uid_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_uid), ths->m1_uid_Length *sizeof(ths->m1_uid[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * Dsp_DHCP_Return_ToBytes(Dsp_DHCP_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uid_Length),sizeof(ths->m1_uid_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_uid), ths->m1_uid_Length *sizeof(ths->m1_uid[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_localId),sizeof(ths->m2_localId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * Ping_Send_ToBytes(Ping_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_test1),sizeof(ths->m1_test1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_test2),sizeof(ths->m2_test2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_test3),sizeof(ths->m3_test3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_test4),sizeof(ths->m4_test4) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * Ping_Return_ToBytes(Ping_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uniqueId_Length),sizeof(ths->m1_uniqueId_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_uniqueId), ths->m1_uniqueId_Length *sizeof(ths->m1_uniqueId[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FilePush_Send_ToBytes(FilePush_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cmd),sizeof(ths->m1_cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_fileLen),sizeof(ths->m2_fileLen) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_fileContent_Length),sizeof(ths->m3_fileContent_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m3_fileContent), ths->m3_fileContent_Length *sizeof(ths->m3_fileContent[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FilePush_Return_ToBytes(FilePush_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * InstrumentStateReport_Send_ToBytes(InstrumentStateReport_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uniqueId_Length),sizeof(ths->m1_uniqueId_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_uniqueId), ths->m1_uniqueId_Length *sizeof(ths->m1_uniqueId[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * InstrumentStateReport_Return_ToBytes(InstrumentStateReport_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ProtoTest_Send_ToBytes(ProtoTest_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data_Length),sizeof(ths->m1_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_data), ths->m1_data_Length *sizeof(ths->m1_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_data_crc),sizeof(ths->m2_data_crc) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ProtoTest_Return_ToBytes(ProtoTest_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data_Length),sizeof(ths->m1_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_data), ths->m1_data_Length *sizeof(ths->m1_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_data_crc),sizeof(ths->m2_data_crc) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuErrorToPC_Send_ToBytes(McuErrorToPC_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m2_info),strlen(ths->m2_info) + 1 );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_reserved),sizeof(ths->m3_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_reserved),sizeof(ths->m4_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_reserved),sizeof(ths->m5_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuErrorToPC_Return_ToBytes(McuErrorToPC_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AutoAdapter_Send_ToBytes(AutoAdapter_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_localID),sizeof(ths->m1_localID) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AutoAdapter_Return_ToBytes(AutoAdapter_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_writeState),sizeof(ths->m1_writeState) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMove_Send_m1_MoveArgs_ToBytes(MotorMultiMove_Send_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_delayms),sizeof(ths->m6_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMove_Send_ToBytes(MotorMultiMove_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isBackZero),sizeof(ths->m2_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMove_Return_ToBytes(MotorMultiMove_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMove_Send_ToBytes(MotorMove_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_isBackZero),sizeof(ths->m6_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMove_Return_ToBytes(MotorMove_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorStatetGet_Send_ToBytes(MotorStatetGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorStatetGet_Return_ToBytes(MotorStatetGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_isZeroTriggered),sizeof(ths->m1_isZeroTriggered) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_currentCordinate),sizeof(ths->m2_currentCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_lostSteps),sizeof(ths->m3_lostSteps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isBusy),sizeof(ths->m4_isBusy) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStart_Send_ToBytes(MotorRotateStart_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isForward),sizeof(ths->m4_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStart_Return_ToBytes(MotorRotateStart_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStop_Send_ToBytes(MotorRotateStop_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStop_Return_ToBytes(MotorRotateStop_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotate_Send_ToBytes(MotorRotate_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotate_Return_ToBytes(MotorRotate_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorReset_Send_ToBytes(MotorReset_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorReset_Return_ToBytes(MotorReset_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SensorValueOperation_Send_ToBytes(SensorValueOperation_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_reg),sizeof(ths->m1_reg) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_op),sizeof(ths->m2_op) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_value),sizeof(ths->m3_value) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_t_delay_1),sizeof(ths->m4_t_delay_1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_t_delay_2),sizeof(ths->m5_t_delay_2) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SensorValueOperation_Return_ToBytes(SensorValueOperation_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_value),sizeof(ths->m2_value) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_airpres),sizeof(ths->m3_airpres) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_basepres),sizeof(ths->m4_basepres) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_actpres),sizeof(ths->m5_actpres) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADAutoCaliStart_Send_ToBytes(ADAutoCaliStart_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_scan_times_offset),sizeof(ths->m1_scan_times_offset) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_f2f),sizeof(ths->m2_f2f) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_f2d),sizeof(ths->m3_f2d) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_d2f),sizeof(ths->m4_d2f) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_f2rc),sizeof(ths->m5_f2rc) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_refer_width_times),sizeof(ths->m6_refer_width_times) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADAutoCaliStart_Return_ToBytes(ADAutoCaliStart_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_z2f),sizeof(ths->m1_z2f) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_errCode),sizeof(ths->m2_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureGet_Send_ToBytes(TemperatureGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureGet_Return_ToBytes(TemperatureGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_state),sizeof(ths->m1_state) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_current_temp),sizeof(ths->m2_current_temp) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureOpen_Send_ToBytes(TemperatureOpen_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_targetTemp),sizeof(ths->m2_targetTemp) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_p),sizeof(ths->m3_p) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_i),sizeof(ths->m4_i) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_d),sizeof(ths->m5_d) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_offset),sizeof(ths->m6_offset) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_from),sizeof(ths->m7_from) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_to),sizeof(ths->m8_to) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureOpen_Return_ToBytes(TemperatureOpen_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureClose_Send_ToBytes(TemperatureClose_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TemperatureClose_Return_ToBytes(TemperatureClose_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * BarCodeGet_Send_ToBytes(BarCodeGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * BarCodeGet_Return_ToBytes(BarCodeGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m2_value),strlen(ths->m2_value) + 1 );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareConfig_Send_ToBytes(FirmWareConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cmd),sizeof(ths->m1_cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_version),sizeof(ths->m2_version) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareConfig_Return_ToBytes(FirmWareConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareTransmitFile_Send_ToBytes(FirmWareTransmitFile_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_datas_Length),sizeof(ths->m1_datas_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_datas), ths->m1_datas_Length *sizeof(ths->m1_datas[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_crc),sizeof(ths->m2_crc) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_offset),sizeof(ths->m3_offset) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareTransmitFile_Return_ToBytes(FirmWareTransmitFile_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuReset_Send_ToBytes(McuReset_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_reserved),sizeof(ths->m1_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuReset_Return_ToBytes(McuReset_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareVersionGet_Send_ToBytes(FirmWareVersionGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_reserved),sizeof(ths->m1_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareVersionGet_Return_ToBytes(FirmWareVersionGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_value),sizeof(ths->m2_value) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSet_Send_ToBytes(MotorConfigSet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_zeroCordinate),sizeof(ths->m2_zeroCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_maxCordinate),sizeof(ths->m3_maxCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_lostEndureContinue),sizeof(ths->m4_lostEndureContinue) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_lostEndureStop),sizeof(ths->m5_lostEndureStop) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_zeroTrigVol),sizeof(ths->m6_zeroTrigVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_dirVol),sizeof(ths->m7_dirVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_isRotate),sizeof(ths->m8_isRotate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_enableVol),sizeof(ths->m9_enableVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_halfVol),sizeof(ths->m10_halfVol) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSet_Return_ToBytes(MotorConfigSet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveEvent_Send_ToBytes(MotorMoveEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_eventId),sizeof(ths->m6_eventId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveEvent_Return_ToBytes(MotorMoveEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_trigger_pos),sizeof(ths->m2_trigger_pos) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuIoStateChangeEvent_Send_ToBytes(McuIoStateChangeEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_port_pin_state_Length),sizeof(ths->m1_port_pin_state_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_port_pin_state), ths->m1_port_pin_state_Length *sizeof(ths->m1_port_pin_state[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * McuIoStateChangeEvent_Return_ToBytes(McuIoStateChangeEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SampleUnitSensorGet_Send_ToBytes(SampleUnitSensorGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_port),strlen(ths->m1_port) + 1 );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_pin),sizeof(ths->m2_pin) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SampleUnitSensorGet_Return_ToBytes(SampleUnitSensorGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AirPumpOpen_Send_ToBytes(AirPumpOpen_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_pw),sizeof(ths->m2_pw) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AirPumpOpen_Return_ToBytes(AirPumpOpen_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AirPumpClose_Send_ToBytes(AirPumpClose_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AirPumpClose_Return_ToBytes(AirPumpClose_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}

BytesBufferNode * ADSampleRead_Send_ToBytes(ADSampleRead_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channel),sizeof(ths->m1_channel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_amp),sizeof(ths->m2_amp) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADSampleRead_Return_ToBytes(ADSampleRead_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_val),sizeof(ths->m1_val) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}


/*
这里实现 数据接口ToBytes成员函数
*/

BytesBufferNode * DiskInstalledRed_Send_ToBytes(DiskInstalledRed_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cnt),sizeof(ths->m1_cnt) );
        BytesBufferNode_Append(&list,temp);
		}
		{
			  BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_time_span_ms),sizeof(ths->m2_time_span_ms) );
        BytesBufferNode_Append(&list,temp);
		}
		{
			  BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_channel),sizeof(ths->m3_channel) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DiskInstalledRed_Return_ToBytes(DiskInstalledRed_Return *ths ) 
{

		BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
		
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_maxval),sizeof(ths->m2_maxval) );
        BytesBufferNode_Append(&list,temp);
    }
		{
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_minval),sizeof(ths->m3_minval) );
        BytesBufferNode_Append(&list,temp);
    }
				
    return list;
}

BytesBufferNode * MotorMoveGPIOEvent_Send_ToBytes(MotorMoveGPIOEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_state_port_pin),sizeof(ths->m6_state_port_pin) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveGPIOEvent_Return_ToBytes(MotorMoveGPIOEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_trigger_pos),sizeof(ths->m2_trigger_pos) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSetNew_Send_ToBytes(MotorConfigSetNew_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx_zero_dir_isR_en_hf),sizeof(ths->m1_idx_zero_dir_isR_en_hf) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_maxCordinate),sizeof(ths->m2_maxCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_zeroCordinate),sizeof(ths->m3_zeroCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_lostEndureContinue),sizeof(ths->m4_lostEndureContinue) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_lostEndureStop),sizeof(ths->m5_lostEndureStop) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSetNew_Return_ToBytes(MotorConfigSetNew_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ExecutesCtrl_Send_ToBytes(ExecutesCtrl_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelId),sizeof(ths->m1_channelId) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isOpen),sizeof(ths->m2_isOpen) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ExecutesCtrl_Return_ToBytes(ExecutesCtrl_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADDetectConfigSet_Send_ToBytes(ADDetectConfigSet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_amp_Length),sizeof(ths->m1_amp_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_amp), ths->m1_amp_Length *sizeof(ths->m1_amp[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_filterCnt),sizeof(ths->m2_filterCnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_light_offset),sizeof(ths->m3_light_offset) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_channel_Length),sizeof(ths->m4_channel_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m4_channel), ths->m4_channel_Length *sizeof(ths->m4_channel[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_light_motor_speed),sizeof(ths->m5_light_motor_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADDetectConfigSet_Return_ToBytes(ADDetectConfigSet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADDataRead_Send_ToBytes(ADDataRead_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_filterId),sizeof(ths->m1_filterId) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_method),sizeof(ths->m2_method) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_aver_cnt),sizeof(ths->m3_aver_cnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_channel),sizeof(ths->m4_channel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_detect_circle_cnt),sizeof(ths->m5_detect_circle_cnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_z2f),sizeof(ths->m6_z2f) );
        BytesBufferNode_Append(&list,temp);
    }
	 {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_time_per_degree),sizeof(ths->m7_time_per_degree) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADDataRead_Return_ToBytes(ADDataRead_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_val_Length),sizeof(ths->m1_val_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_val), ths->m1_val_Length *sizeof(ths->m1_val[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}


BytesBufferNode * ADDataSeriesSend_Send_ToBytes(ADDataSeriesSend_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data_Length),sizeof(ths->m1_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_data), ths->m1_data_Length *sizeof(ths->m1_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_current_detect_circle),sizeof(ths->m2_current_detect_circle) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ADDataSeriesSend_Return_ToBytes(ADDataSeriesSend_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartTransmit_Send_ToBytes(UartTransmit_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uart_id),sizeof(ths->m1_uart_id) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_data_Length),sizeof(ths->m2_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m2_data), ths->m2_data_Length *sizeof(ths->m2_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartTransmit_Return_ToBytes(UartTransmit_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartReceiveEvent_Send_ToBytes(UartReceiveEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uart_id),sizeof(ths->m1_uart_id) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_data_Length),sizeof(ths->m2_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m2_data), ths->m2_data_Length *sizeof(ths->m2_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartReceiveEvent_Return_ToBytes(UartReceiveEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_ToBytes(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SeqMotorMultiMove_Send_m1_Sequence_ToBytes(SeqMotorMultiMove_Send_m1_Sequence *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_delayms),sizeof(ths->m2_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SeqMotorMultiMove_Send_ToBytes(SeqMotorMultiMove_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_Sequence_Length),sizeof(ths->m1_Sequence_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_Sequence_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_Sequence[i].ToBytes( &(ths->m1_Sequence[i])   ));
    }
    return list;
}
BytesBufferNode * SeqMotorMultiMove_Return_ToBytes(SeqMotorMultiMove_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_errIdx),sizeof(ths->m2_errIdx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureConfig_Send_ToBytes(PressureConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cmd),sizeof(ths->m1_cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_averageCnt),sizeof(ths->m2_averageCnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_tick),sizeof(ths->m3_tick) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_maxSampleCnt),sizeof(ths->m4_maxSampleCnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_threshold),sizeof(ths->m5_threshold) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureConfig_Return_ToBytes(PressureConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureDataGet_Send_ToBytes(PressureDataGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cmd),sizeof(ths->m1_cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_data_offset),sizeof(ths->m2_data_offset) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureDataGet_Return_ToBytes(PressureDataGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_remainLen),sizeof(ths->m2_remainLen) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_pressData_Length),sizeof(ths->m3_pressData_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m3_pressData), ths->m3_pressData_Length *sizeof(ths->m3_pressData[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorAbort_Send_ToBytes(MotorAbort_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx_mask),sizeof(ths->m1_idx_mask) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorAbort_Return_ToBytes(MotorAbort_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureEvent_Send_ToBytes(PressureEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_eventId),sizeof(ths->m1_eventId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureEvent_Return_ToBytes(PressureEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveSeq_Send_m1_MoveArgs_ToBytes(MotorMoveSeq_Send_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_delayms),sizeof(ths->m6_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isBackZero),sizeof(ths->m2_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveSeq_Send_ToBytes(MotorMoveSeq_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    return list;
}
BytesBufferNode * MotorMoveSeq_Return_ToBytes(MotorMoveSeq_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareReadyEvent_Send_ToBytes(FirmWareReadyEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_reserved),sizeof(ths->m1_reserved) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * FirmWareReadyEvent_Return_ToBytes(FirmWareReadyEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigGet_Send_ToBytes(MotorConfigGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigGet_Return_ToBytes(MotorConfigGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_zeroCordinate),sizeof(ths->m2_zeroCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_maxCordinate),sizeof(ths->m3_maxCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_lostEndureContinue),sizeof(ths->m4_lostEndureContinue) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_lostEndureStop),sizeof(ths->m5_lostEndureStop) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_zeroTrigVol),sizeof(ths->m6_zeroTrigVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_dirVol),sizeof(ths->m7_dirVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_isRotate),sizeof(ths->m8_isRotate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_enableVol),sizeof(ths->m9_enableVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_halfVol),sizeof(ths->m10_halfVol) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PWM_Set_Send_ToBytes(PWM_Set_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_time_channle),sizeof(ths->m1_time_channle) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_period),sizeof(ths->m2_period) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_dutyCycle),sizeof(ths->m3_dutyCycle) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PWM_Set_Return_ToBytes(PWM_Set_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TurnTable_Move_Send_ToBytes(TurnTable_Move_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cali_location),sizeof(ths->m1_cali_location) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_idx),sizeof(ths->m2_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_steps),sizeof(ths->m3_steps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_speed),sizeof(ths->m4_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_acce),sizeof(ths->m5_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * TurnTable_Move_Return_ToBytes(TurnTable_Move_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_filter_id),sizeof(ths->m2_filter_id) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_ad_data),sizeof(ths->m3_ad_data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableFindZero_Send_ToBytes(EncodeTurnTableFindZero_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_cali_location),sizeof(ths->m2_cali_location) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_acce),sizeof(ths->m4_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableFindZero_Return_ToBytes(EncodeTurnTableFindZero_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableRotateStart_Send_ToBytes(EncodeTurnTableRotateStart_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_cali_location),sizeof(ths->m2_cali_location) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_acce),sizeof(ths->m4_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableRotateStart_Return_ToBytes(EncodeTurnTableRotateStart_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableRotateStop_Send_ToBytes(EncodeTurnTableRotateStop_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_dest_id),sizeof(ths->m2_dest_id) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableRotateStop_Return_ToBytes(EncodeTurnTableRotateStop_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableStateGet_Send_ToBytes(EncodeTurnTableStateGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EncodeTurnTableStateGet_Return_ToBytes(EncodeTurnTableStateGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_isZeroTriggered),sizeof(ths->m1_isZeroTriggered) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_is_busy),sizeof(ths->m2_is_busy) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_current_id),sizeof(ths->m3_current_id) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCAdRead_Send_ToBytes(DCAdRead_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_cmd),sizeof(ths->m1_cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_channel),sizeof(ths->m2_channel) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCAdRead_Return_ToBytes(DCAdRead_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_remainCnt),sizeof(ths->m1_remainCnt) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_adValue_Length),sizeof(ths->m2_adValue_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m2_adValue), ths->m2_adValue_Length *sizeof(ths->m2_adValue[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * JSAdRead_Send_ToBytes(JSAdRead_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channel_id),sizeof(ths->m1_channel_id) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_time_span),sizeof(ths->m2_time_span) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_cnt_read),sizeof(ths->m3_cnt_read) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_dac_data),sizeof(ths->m4_dac_data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * JSAdRead_Return_ToBytes(JSAdRead_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveSyncLevel_Send_ToBytes(MotorMoveSyncLevel_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_steps),sizeof(ths->m2_steps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_sync_steps),sizeof(ths->m3_sync_steps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_speed),sizeof(ths->m4_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_acce),sizeof(ths->m5_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveSyncLevel_Return_ToBytes(MotorMoveSyncLevel_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotor_Send_ToBytes(DCMotor_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_Idx),sizeof(ths->m1_Idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isOpen),sizeof(ths->m2_isOpen) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotor_Return_ToBytes(DCMotor_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->errCode),sizeof(ths->errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorCupIDSet_Send_ToBytes(MotorCupIDSet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_CupID),sizeof(ths->m1_CupID) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_speed),sizeof(ths->m2_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_acce),sizeof(ths->m3_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorCupIDSet_Return_ToBytes(MotorCupIDSet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorCupIDInfoGet_Send_ToBytes(MotorCupIDInfoGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorCupIDInfoGet_Return_ToBytes(MotorCupIDInfoGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_errCode),sizeof(ths->m2_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_turntableStatus),sizeof(ths->m3_turntableStatus) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_nowCupID),sizeof(ths->m4_nowCupID) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isinit),sizeof(ths->m5_isinit) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * JSAdDataGet_Send_ToBytes(JSAdDataGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_channelIdx),sizeof(ths->m1_channelIdx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_timeSpan),sizeof(ths->m2_timeSpan) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_dataCnt),sizeof(ths->m3_dataCnt) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * JSAdDataGet_Return_ToBytes(JSAdDataGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data_Length),sizeof(ths->m1_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_data), ths->m1_data_Length *sizeof(ths->m1_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMoveTmc_Send_m1_MoveArgs_ToBytes(MotorMultiMoveTmc_Send_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_delayms),sizeof(ths->m6_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_subdivision),sizeof(ths->m8_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMoveTmc_Send_ToBytes(MotorMultiMoveTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isBackZero),sizeof(ths->m2_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMultiMoveTmc_Return_ToBytes(MotorMultiMoveTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveTmc_Send_ToBytes(MotorMoveTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_isBackZero),sizeof(ths->m6_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_subdivision),sizeof(ths->m8_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_backzero_position),sizeof(ths->m9_backzero_position) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveTmc_Return_ToBytes(MotorMoveTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorStatetGetTmc_Send_ToBytes(MotorStatetGetTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorStatetGetTmc_Return_ToBytes(MotorStatetGetTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_isZeroTriggered),sizeof(ths->m1_isZeroTriggered) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_currentCordinate),sizeof(ths->m2_currentCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_lostSteps),sizeof(ths->m3_lostSteps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isBusy),sizeof(ths->m4_isBusy) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_currentLevel),sizeof(ths->m5_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_subdivision),sizeof(ths->m6_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStartTmc_Send_ToBytes(MotorRotateStartTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isForward),sizeof(ths->m4_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_currentLevel),sizeof(ths->m5_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_subdivision),sizeof(ths->m6_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStartTmc_Return_ToBytes(MotorRotateStartTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStopTmc_Send_ToBytes(MotorRotateStopTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateStopTmc_Return_ToBytes(MotorRotateStopTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateTmc_Send_ToBytes(MotorRotateTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_currentLevel),sizeof(ths->m6_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_subdivision),sizeof(ths->m7_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateTmc_Return_ToBytes(MotorRotateTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorResetTmc_Send_ToBytes(MotorResetTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_currentLevel),sizeof(ths->m4_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_subdivision),sizeof(ths->m5_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorResetTmc_Return_ToBytes(MotorResetTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSetTmc_Send_ToBytes(MotorConfigSetTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_zeroCordinate),sizeof(ths->m2_zeroCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_maxCordinate),sizeof(ths->m3_maxCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_lostEndureContinue),sizeof(ths->m4_lostEndureContinue) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_lostEndureStop),sizeof(ths->m5_lostEndureStop) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_zeroTrigVol),sizeof(ths->m6_zeroTrigVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_dirVol),sizeof(ths->m7_dirVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_isRotate),sizeof(ths->m8_isRotate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_enableVol),sizeof(ths->m9_enableVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_halfVol),sizeof(ths->m10_halfVol) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m11_currentLevel),sizeof(ths->m11_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m12_subdivision),sizeof(ths->m12_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m13_holdLevel),sizeof(ths->m13_holdLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorConfigSetTmc_Return_ToBytes(MotorConfigSetTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveGPIOEventTmc_Send_ToBytes(MotorMoveGPIOEventTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_state_port_pin),sizeof(ths->m6_state_port_pin) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_subdivision),sizeof(ths->m8_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_isdetect),sizeof(ths->m9_isdetect) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_movedelay),sizeof(ths->m10_movedelay) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorMoveGPIOEventTmc_Return_ToBytes(MotorMoveGPIOEventTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_trigger_pos),sizeof(ths->m2_trigger_pos) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_distance),sizeof(ths->m3_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateShake_Send_ToBytes(MotorRotateShake_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_dir),sizeof(ths->m4_dir) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_currentLevel),sizeof(ths->m5_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_subdivision),sizeof(ths->m6_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_step),sizeof(ths->m7_step) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorRotateShake_Return_ToBytes(MotorRotateShake_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LiquidLevelResultGet_Send_ToBytes(LiquidLevelResultGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data),sizeof(ths->m1_data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LiquidLevelResultGet_Return_ToBytes(LiquidLevelResultGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_trigger_pos),sizeof(ths->m1_trigger_pos) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorDetectLiquidLevel_Send_ToBytes(MotorDetectLiquidLevel_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_subdivision),sizeof(ths->m8_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_idx),sizeof(ths->m10_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m11_acce),sizeof(ths->m11_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m12_speed),sizeof(ths->m12_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m13_distance),sizeof(ths->m13_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m14_isRelative),sizeof(ths->m14_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m15_currentLevel),sizeof(ths->m15_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m16_subdivision),sizeof(ths->m16_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * MotorDetectLiquidLevel_Return_ToBytes(MotorDetectLiquidLevel_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_distance),sizeof(ths->m3_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * RFIDValueOperation_Send_ToBytes(RFIDValueOperation_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_op),sizeof(ths->m1_op) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data_Length),sizeof(ths->data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->data), ths->data_Length *sizeof(ths->data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * RFIDValueOperation_Return_ToBytes(RFIDValueOperation_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data_Length),sizeof(ths->data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->data), ths->data_Length *sizeof(ths->data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoSimple_Send_ToBytes(IoSimple_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_port_pin_arg),sizeof(ths->m1_port_pin_arg) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoSimple_Return_ToBytes(IoSimple_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode_value),sizeof(ths->m1_errCode_value) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoOperation_Send_m1_IoArgs_m1_Controls_ToBytes(IoOperation_Send_m1_IoArgs_m1_Controls *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_port_pin),sizeof(ths->m1_port_pin) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_cmd_arg),sizeof(ths->m2_cmd_arg) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoOperation_Send_m1_IoArgs_ToBytes(IoOperation_Send_m1_IoArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_Controls_Length),sizeof(ths->m1_Controls_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_Controls_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_Controls[i].ToBytes( &(ths->m1_Controls[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_delay),sizeof(ths->m2_delay) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoOperation_Send_ToBytes(IoOperation_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_IoArgs_Length),sizeof(ths->m1_IoArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_IoArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_IoArgs[i].ToBytes( &(ths->m1_IoArgs[i])   ));
    }
    return list;
}
BytesBufferNode * IoOperation_Return_ToBytes(IoOperation_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_value),sizeof(ths->m2_value) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoBlink_Send_ToBytes(IoBlink_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_port),sizeof(ths->m1_port) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_pin),sizeof(ths->m2_pin) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_state),sizeof(ths->m3_state) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_frequence),sizeof(ths->m4_frequence) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IoBlink_Return_ToBytes(IoBlink_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->errCode),sizeof(ths->errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartChangeBode_Send_ToBytes(UartChangeBode_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_uartIdx),sizeof(ths->m1_uartIdx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_bode),sizeof(ths->m2_bode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * UartChangeBode_Return_ToBytes(UartChangeBode_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->errCode),sizeof(ths->errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorMultiRotate_Send_m1_MoveArgs_ToBytes(ServoMotorMultiRotate_Send_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_dir),sizeof(ths->m2_dir) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_acce),sizeof(ths->m4_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_delayms),sizeof(ths->m5_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_isSendSyncSigle),sizeof(ths->m6_isSendSyncSigle) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorMultiRotate_Send_ToBytes(ServoMotorMultiRotate_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    return list;
}
BytesBufferNode * ServoMotorMultiRotate_Return_ToBytes(ServoMotorMultiRotate_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateShakeDetect_Send_ToBytes(ServoMotorRotateShakeDetect_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_time),sizeof(ths->m4_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isForward),sizeof(ths->m5_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_time),sizeof(ths->m6_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_frequency),sizeof(ths->m7_frequency) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_amplitude),sizeof(ths->m8_amplitude) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_acce),sizeof(ths->m9_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_speed),sizeof(ths->m10_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m11_isForward),sizeof(ths->m11_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateShakeDetect_Return_ToBytes(ServoMotorRotateShakeDetect_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateShakeRotateDetect_Send_ToBytes(ServoMotorRotateShakeRotateDetect_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_time),sizeof(ths->m4_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isForward),sizeof(ths->m5_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_time),sizeof(ths->m6_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_frequency),sizeof(ths->m7_frequency) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_amplitude),sizeof(ths->m8_amplitude) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_acce),sizeof(ths->m9_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_speed),sizeof(ths->m10_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m11_isForward),sizeof(ths->m11_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m12_time),sizeof(ths->m12_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m13_acce),sizeof(ths->m13_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m14_speed),sizeof(ths->m14_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m15_isForward),sizeof(ths->m15_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateShakeRotateDetect_Return_ToBytes(ServoMotorRotateShakeRotateDetect_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateTwoDirOnTime_Send_ToBytes(ServoMotorRotateTwoDirOnTime_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isForward),sizeof(ths->m4_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_time),sizeof(ths->m5_time) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateTwoDirOnTime_Return_ToBytes(ServoMotorRotateTwoDirOnTime_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorStatetGet_Send_ToBytes(ServoMotorStatetGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorStatetGet_Return_ToBytes(ServoMotorStatetGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isZeroTriggered),sizeof(ths->m2_isZeroTriggered) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_currentCordinate),sizeof(ths->m3_currentCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_currentSpeed),sizeof(ths->m4_currentSpeed) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateStart_Send_ToBytes(ServoMotorRotateStart_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_isForward),sizeof(ths->m4_isForward) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isSendSyncSigle),sizeof(ths->m5_isSendSyncSigle) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateStart_Return_ToBytes(ServoMotorRotateStart_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateStop_Send_ToBytes(ServoMotorRotateStop_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorRotateStop_Return_ToBytes(ServoMotorRotateStop_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorReset_Send_ToBytes(ServoMotorReset_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorReset_Return_ToBytes(ServoMotorReset_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorAutoCalibratio_Send_ToBytes(ServoMotorAutoCalibratio_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorAutoCalibratio_Return_ToBytes(ServoMotorAutoCalibratio_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorShake_Send_ToBytes(ServoMotorShake_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_time),sizeof(ths->m2_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_frequency),sizeof(ths->m3_frequency) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_amplitude),sizeof(ths->m4_amplitude) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorShake_Return_ToBytes(ServoMotorShake_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_maxCordinate),sizeof(ths->m1_maxCordinate) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_errCode),sizeof(ths->m2_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorBaseCtrl_Send_ToBytes(ServoMotorBaseCtrl_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_option),sizeof(ths->m2_option) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorBaseCtrl_Return_ToBytes(ServoMotorBaseCtrl_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorMove_Send_ToBytes(ServoMotorMove_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_steps),sizeof(ths->m2_steps) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_acce),sizeof(ths->m4_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isAbsolute),sizeof(ths->m5_isAbsolute) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * ServoMotorMove_Return_ToBytes(ServoMotorMove_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * RGBSensorGet_Send_ToBytes(RGBSensorGet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * RGBSensorGet_Return_ToBytes(RGBSensorGet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_R),sizeof(ths->m2_R) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_G),sizeof(ths->m3_G) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_B),sizeof(ths->m4_B) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_C),sizeof(ths->m5_C) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SerialDataReport_Send_ToBytes(SerialDataReport_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_data_Length),sizeof(ths->m1_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m1_data), ths->m1_data_Length *sizeof(ths->m1_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SerialDataReport_Return_ToBytes(SerialDataReport_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AddressBroadcastSend_Send_ToBytes(AddressBroadcastSend_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_mode),sizeof(ths->m1_mode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_address),sizeof(ths->m2_address) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_count),sizeof(ths->m3_count) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_addrRestrict_Length),sizeof(ths->m4_addrRestrict_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m4_addrRestrict), ths->m4_addrRestrict_Length *sizeof(ths->m4_addrRestrict[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AddressBroadcastSend_Return_ToBytes(AddressBroadcastSend_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AddressBroadcastReceive_Send_ToBytes(AddressBroadcastReceive_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_address),sizeof(ths->m1_address) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * AddressBroadcastReceive_Return_ToBytes(AddressBroadcastReceive_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * StartCheckPress_Send_ToBytes(StartCheckPress_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_address),sizeof(ths->m1_address) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * StartCheckPress_Return_ToBytes(StartCheckPress_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EndCheckPress_Send_ToBytes(EndCheckPress_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_address),sizeof(ths->m1_address) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * EndCheckPress_Return_ToBytes(EndCheckPress_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * Blending_Send_ToBytes(Blending_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_address),sizeof(ths->m1_address) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_speed),sizeof(ths->m2_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_time),sizeof(ths->m3_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_dir),sizeof(ths->m4_dir) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * Blending_Return_ToBytes(Blending_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * GetEPOSType_Send_ToBytes(GetEPOSType_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * GetEPOSType_Return_ToBytes(GetEPOSType_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_motorType),sizeof(ths->m1_motorType) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_errCode),sizeof(ths->m2_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SetEPOSType_Send_ToBytes(SetEPOSType_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_typeId),sizeof(ths->m2_typeId) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SetEPOSType_Return_ToBytes(SetEPOSType_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DACSet_Send_ToBytes(DACSet_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_value),sizeof(ths->m1_value) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DACSet_Return_ToBytes(DACSet_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LightCtrl_Send_ToBytes(LightCtrl_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->idx),sizeof(ths->idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->value),sizeof(ths->value) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LightCtrl_Return_ToBytes(LightCtrl_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->errCode),sizeof(ths->errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotorMoveSeq_Send_m1_MoveArgs_ToBytes(DCMotorMoveSeq_Send_m1_MoveArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_delayms),sizeof(ths->m6_delayms) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_isBackZero),sizeof(ths->m2_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotorMoveSeq_Send_ToBytes(DCMotorMoveSeq_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_MoveArgs_Length),sizeof(ths->m1_MoveArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_MoveArgs[i].ToBytes( &(ths->m1_MoveArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_WaitSyncTime),sizeof(ths->m2_WaitSyncTime) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotorMoveSeq_Return_ToBytes(DCMotorMoveSeq_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotorMoveTmc_Send_ToBytes(DCMotorMoveTmc_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_idx),sizeof(ths->m1_idx) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_acce),sizeof(ths->m2_acce) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_speed),sizeof(ths->m3_speed) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_distance),sizeof(ths->m4_distance) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_isRelative),sizeof(ths->m5_isRelative) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m6_isBackZero),sizeof(ths->m6_isBackZero) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m7_currentLevel),sizeof(ths->m7_currentLevel) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m8_subdivision),sizeof(ths->m8_subdivision) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m9_backzero_position),sizeof(ths->m9_backzero_position) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m10_writeSyncFlag),sizeof(ths->m10_writeSyncFlag) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * DCMotorMoveTmc_Return_ToBytes(DCMotorMoveTmc_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureRead_Send_ToBytes(PressureRead_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_id),sizeof(ths->m1_id) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureRead_Return_ToBytes(PressureRead_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_pressure1),sizeof(ths->m1_pressure1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_pressure2),sizeof(ths->m2_pressure2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_errCode),sizeof(ths->m3_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LittleMotorCtrl_Send_ToBytes(LittleMotorCtrl_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_flag),sizeof(ths->m1_flag) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_period),sizeof(ths->m2_period) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_dutyCycle),sizeof(ths->m3_dutyCycle) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LittleMotorCtrl_Return_ToBytes(LittleMotorCtrl_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * CongestionDetection_Send_ToBytes(CongestionDetection_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_status),sizeof(ths->m1_status) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_threshold),sizeof(ths->m2_threshold) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * CongestionDetection_Return_ToBytes(CongestionDetection_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_status),sizeof(ths->m1_status) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_ad_value),sizeof(ths->m2_ad_value) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_errCode),sizeof(ths->m3_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LaserCtrol_Send_ToBytes(LaserCtrol_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_status),sizeof(ths->m1_status) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * LaserCtrol_Return_ToBytes(LaserCtrol_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_errCode),sizeof(ths->m1_errCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadConfig_Send_ToBytes(NFL_ReadConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadConfig_Return_ToBytes(NFL_ReadConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset0),sizeof(ths->offset0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt0),sizeof(ths->volt0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay0),sizeof(ths->delay0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset1),sizeof(ths->offset1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt1),sizeof(ths->volt1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay1),sizeof(ths->delay1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset2),sizeof(ths->offset2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt2),sizeof(ths->volt2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay2),sizeof(ths->delay2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset3),sizeof(ths->offset3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt3),sizeof(ths->volt3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay3),sizeof(ths->delay3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset4),sizeof(ths->offset4) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt4),sizeof(ths->volt4) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay4),sizeof(ths->delay4) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_WriteConfig_Send_ToBytes(NFL_WriteConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset0),sizeof(ths->offset0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt0),sizeof(ths->volt0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay0),sizeof(ths->delay0) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset1),sizeof(ths->offset1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt1),sizeof(ths->volt1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay1),sizeof(ths->delay1) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset2),sizeof(ths->offset2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt2),sizeof(ths->volt2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay2),sizeof(ths->delay2) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset3),sizeof(ths->offset3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt3),sizeof(ths->volt3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay3),sizeof(ths->delay3) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->offset4),sizeof(ths->offset4) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->volt4),sizeof(ths->volt4) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->delay4),sizeof(ths->delay4) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_WriteConfig_Return_ToBytes(NFL_WriteConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadDataStatus_Send_ToBytes(NFL_ReadDataStatus_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->wait_time_ms),sizeof(ths->wait_time_ms) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadDataStatus_Return_ToBytes(NFL_ReadDataStatus_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data_status),sizeof(ths->data_status) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadData_Send_ToBytes(NFL_ReadData_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->channel),sizeof(ths->channel) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadData_Return_ToBytes(NFL_ReadData_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data_Length),sizeof(ths->data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->data), ths->data_Length *sizeof(ths->data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_DataCollectStart_Send_ToBytes(NFL_DataCollectStart_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_DataCollectStart_Return_ToBytes(NFL_DataCollectStart_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_DataCollectStop_Send_ToBytes(NFL_DataCollectStop_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_DataCollectStop_Return_ToBytes(NFL_DataCollectStop_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ThresholdConfig_Send_ToBytes(NFL_ThresholdConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->threshold),sizeof(ths->threshold) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ThresholdConfig_Return_ToBytes(NFL_ThresholdConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_StandbyConfig_Send_ToBytes(NFL_StandbyConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_StandbyConfig_Return_ToBytes(NFL_StandbyConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ActivateConfig_Send_ToBytes(NFL_ActivateConfig_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ActivateConfig_Return_ToBytes(NFL_ActivateConfig_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadTemperature_Send_ToBytes(NFL_ReadTemperature_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data),sizeof(ths->data) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_ReadTemperature_Return_ToBytes(NFL_ReadTemperature_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->temperature),sizeof(ths->temperature) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_SendEvent_Send_ToBytes(NFL_SendEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->length),sizeof(ths->length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->data_Length),sizeof(ths->data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->data), ths->data_Length *sizeof(ths->data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * NFL_SendEvent_Return_ToBytes(NFL_SendEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PowerCtrl_Send_ToBytes(PowerCtrl_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->PcPower),sizeof(ths->PcPower) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->MotorPower),sizeof(ths->MotorPower) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->TemperaturePower),sizeof(ths->TemperaturePower) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->DataCollectionPower),sizeof(ths->DataCollectionPower) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PowerCtrl_Return_ToBytes(PowerCtrl_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureListen_Send_ToBytes(PressureListen_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_id_flag),sizeof(ths->m1_id_flag) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_diff_threthold),sizeof(ths->m2_diff_threthold) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_listen_time_span),sizeof(ths->m3_listen_time_span) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_error_range),sizeof(ths->m4_error_range) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m5_listen_time),sizeof(ths->m5_listen_time) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * PressureListen_Return_ToBytes(PressureListen_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_error_code),sizeof(ths->m1_error_code) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_continue_time),sizeof(ths->m2_continue_time) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_end_pressure),sizeof(ths->m3_end_pressure) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_pressure_data_Length),sizeof(ths->m4_pressure_data_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)(ths->m4_pressure_data), ths->m4_pressure_data_Length *sizeof(ths->m4_pressure_data[0]) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SendErrorCode_Send_ToBytes(SendErrorCode_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * SendErrorCode_Return_ToBytes(SendErrorCode_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->cmd),sizeof(ths->cmd) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->ErrorCode),sizeof(ths->ErrorCode) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IOCtrlByEvent_Send_m1_PortArgs_ToBytes(IOCtrlByEvent_Send_m1_PortArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_state),sizeof(ths->m1_state) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_delay),sizeof(ths->m2_delay) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IOCtrlByEvent_Send_m2_PortArgs_ToBytes(IOCtrlByEvent_Send_m2_PortArgs *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_state),sizeof(ths->m1_state) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_delay),sizeof(ths->m2_delay) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IOCtrlByEvent_Send_ToBytes(IOCtrlByEvent_Send *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_PortArgs_Length),sizeof(ths->m1_PortArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m1_PortArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m1_PortArgs[i].ToBytes( &(ths->m1_PortArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m2_PortArgs_Length),sizeof(ths->m2_PortArgs_Length) );
        BytesBufferNode_Append(&list,temp);
    }
    for(int i =0;i<ths->m2_PortArgs_Length;i++){
        BytesBufferNode_Append(&list,ths->m2_PortArgs[i].ToBytes( &(ths->m2_PortArgs[i])   ));
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m3_id_event_threshold),sizeof(ths->m3_id_event_threshold) );
        BytesBufferNode_Append(&list,temp);
    }
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m4_timeout),sizeof(ths->m4_timeout) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
BytesBufferNode * IOCtrlByEvent_Return_ToBytes(IOCtrlByEvent_Return *ths ) 
{
    BytesBufferNode * list = NULL ;
    {
        BytesBufferNode * temp = New_BytesBufferNode((u8*)&(ths->m1_error_code),sizeof(ths->m1_error_code) );
        BytesBufferNode_Append(&list,temp);
    }
    return list;
}
int Dsp_DHCP_Send_GetFromBytes(Dsp_DHCP_Send *ths,u8* buff,int offset ) 
{
    ths->m1_uid_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_uid_Length));
    ths->m1_uid = (u8*)(buff + offset );
    offset += (ths->m1_uid_Length*sizeof(ths->m1_uid[0])  );
    return offset;
}
int Dsp_DHCP_Return_GetFromBytes(Dsp_DHCP_Return *ths,u8* buff,int offset ) 
{
    ths->m1_uid_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_uid_Length));
    ths->m1_uid = (u8*)(buff + offset );
    offset += (ths->m1_uid_Length*sizeof(ths->m1_uid[0])  );
    ths->m2_localId = buff[offset];
    offset += (sizeof(ths->m2_localId));
    return offset;
}
int Ping_Send_GetFromBytes(Ping_Send *ths,u8* buff,int offset ) 
{
    ths->m1_test1 = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_test1));
    ths->m2_test2 = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_test2));
    ths->m3_test3 = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_test3));
    ths->m4_test4 = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_test4));
    return offset;
}
int Ping_Return_GetFromBytes(Ping_Return *ths,u8* buff,int offset ) 
{
    ths->m1_uniqueId_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_uniqueId_Length));
    ths->m1_uniqueId = (u8*)(buff + offset );
    offset += (ths->m1_uniqueId_Length*sizeof(ths->m1_uniqueId[0])  );
    return offset;
}
int FilePush_Send_GetFromBytes(FilePush_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cmd = buff[offset];
    offset += (sizeof(ths->m1_cmd));
    ths->m2_fileLen = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_fileLen));
    ths->m3_fileContent_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_fileContent_Length));
    ths->m3_fileContent = (u8*)(buff + offset );
    offset += (ths->m3_fileContent_Length*sizeof(ths->m3_fileContent[0])  );
    return offset;
}
int FilePush_Return_GetFromBytes(FilePush_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int InstrumentStateReport_Send_GetFromBytes(InstrumentStateReport_Send *ths,u8* buff,int offset ) 
{
    ths->m1_uniqueId_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_uniqueId_Length));
    ths->m1_uniqueId = (u8*)(buff + offset );
    offset += (ths->m1_uniqueId_Length*sizeof(ths->m1_uniqueId[0])  );
    return offset;
}
int InstrumentStateReport_Return_GetFromBytes(InstrumentStateReport_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ProtoTest_Send_GetFromBytes(ProtoTest_Send *ths,u8* buff,int offset ) 
{
    ths->m1_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data_Length));
    ths->m1_data = (u8*)(buff + offset );
    offset += (ths->m1_data_Length*sizeof(ths->m1_data[0])  );
    ths->m2_data_crc = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_data_crc));
    return offset;
}
int ProtoTest_Return_GetFromBytes(ProtoTest_Return *ths,u8* buff,int offset ) 
{
    ths->m1_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data_Length));
    ths->m1_data = (u8*)(buff + offset );
    offset += (ths->m1_data_Length*sizeof(ths->m1_data[0])  );
    ths->m2_data_crc = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_data_crc));
    return offset;
}
int McuErrorToPC_Send_GetFromBytes(McuErrorToPC_Send *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_info = (char * )buff;
    offset += ( strlen(ths->m2_info) + 1 );
    ths->m3_reserved = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_reserved));
    ths->m4_reserved = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_reserved));
    ths->m5_reserved = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_reserved));
    return offset;
}
int McuErrorToPC_Return_GetFromBytes(McuErrorToPC_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int AutoAdapter_Send_GetFromBytes(AutoAdapter_Send *ths,u8* buff,int offset ) 
{
    ths->m1_localID = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_localID));
    return offset;
}
int AutoAdapter_Return_GetFromBytes(AutoAdapter_Return *ths,u8* buff,int offset ) 
{
    ths->m1_writeState = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_writeState));
    return offset;
}
int MotorMultiMove_Send_m1_MoveArgs_GetFromBytes(MotorMultiMove_Send_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_delayms));
    return offset;
}
int MotorMultiMove_Send_GetFromBytes(MotorMultiMove_Send *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = MotorMultiMove_Send_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    ths->m2_isBackZero = buff[offset];
    offset += (sizeof(ths->m2_isBackZero));
    return offset;
}
int MotorMultiMove_Return_GetFromBytes(MotorMultiMove_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMove_Send_GetFromBytes(MotorMove_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_isBackZero = buff[offset];
    offset += (sizeof(ths->m6_isBackZero));
    return offset;
}
int MotorMove_Return_GetFromBytes(MotorMove_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorStatetGet_Send_GetFromBytes(MotorStatetGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorStatetGet_Return_GetFromBytes(MotorStatetGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_isZeroTriggered = buff[offset];
    offset += (sizeof(ths->m1_isZeroTriggered));
    ths->m2_currentCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_currentCordinate));
    ths->m3_lostSteps = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_lostSteps));
    ths->m4_isBusy = buff[offset];
    offset += (sizeof(ths->m4_isBusy));
    return offset;
}
int MotorRotateStart_Send_GetFromBytes(MotorRotateStart_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_isForward = buff[offset];
    offset += (sizeof(ths->m4_isForward));
    return offset;
}
int MotorRotateStart_Return_GetFromBytes(MotorRotateStart_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorRotateStop_Send_GetFromBytes(MotorRotateStop_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorRotateStop_Return_GetFromBytes(MotorRotateStop_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorRotate_Send_GetFromBytes(MotorRotate_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    return offset;
}
int MotorRotate_Return_GetFromBytes(MotorRotate_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorReset_Send_GetFromBytes(MotorReset_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    return offset;
}
int MotorReset_Return_GetFromBytes(MotorReset_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int SensorValueOperation_Send_GetFromBytes(SensorValueOperation_Send *ths,u8* buff,int offset ) 
{
    ths->m1_reg = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_reg));
    ths->m2_op = buff[offset];
    offset += (sizeof(ths->m2_op));
    ths->m3_value = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_value));
    ths->m4_t_delay_1 = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_t_delay_1));
    ths->m5_t_delay_2 = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_t_delay_2));
    return offset;
}
int SensorValueOperation_Return_GetFromBytes(SensorValueOperation_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_value = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_value));
    ths->m3_airpres = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_airpres));
    ths->m4_basepres = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_basepres));
    ths->m5_actpres = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_actpres));
    return offset;
}
int ADAutoCaliStart_Send_GetFromBytes(ADAutoCaliStart_Send *ths,u8* buff,int offset ) 
{
    ths->m1_scan_times_offset = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_scan_times_offset));
    ths->m2_f2f = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_f2f));
    ths->m3_f2d = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_f2d));
    ths->m4_d2f = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_d2f));
    ths->m5_f2rc = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_f2rc));
    ths->m6_refer_width_times = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_refer_width_times));
    return offset;
}
int ADAutoCaliStart_Return_GetFromBytes(ADAutoCaliStart_Return *ths,u8* buff,int offset ) 
{
    ths->m1_z2f = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_z2f));
    ths->m2_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_errCode));
    return offset;
}
int TemperatureGet_Send_GetFromBytes(TemperatureGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    return offset;
}
int TemperatureGet_Return_GetFromBytes(TemperatureGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_state = buff[offset];
    offset += (sizeof(ths->m1_state));
    ths->m2_current_temp = ReadF32(buff + offset);
    offset += (sizeof(ths->m2_current_temp));
    return offset;
}
int TemperatureOpen_Send_GetFromBytes(TemperatureOpen_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    ths->m2_targetTemp = ReadF32(buff + offset);
    offset += (sizeof(ths->m2_targetTemp));
    ths->m3_p = ReadF32(buff + offset);
    offset += (sizeof(ths->m3_p));
    ths->m4_i = ReadF32(buff + offset);
    offset += (sizeof(ths->m4_i));
    ths->m5_d = ReadF32(buff + offset);
    offset += (sizeof(ths->m5_d));
    ths->m6_offset = ReadF32(buff + offset);
    offset += (sizeof(ths->m6_offset));
    ths->m7_from = ReadF32(buff + offset);
    offset += (sizeof(ths->m7_from));
    ths->m8_to = ReadF32(buff + offset);
    offset += (sizeof(ths->m8_to));
    return offset;
}
int TemperatureOpen_Return_GetFromBytes(TemperatureOpen_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int TemperatureClose_Send_GetFromBytes(TemperatureClose_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    return offset;
}
int TemperatureClose_Return_GetFromBytes(TemperatureClose_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int BarCodeGet_Send_GetFromBytes(BarCodeGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int BarCodeGet_Return_GetFromBytes(BarCodeGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_value = (char * )buff;
    offset += ( strlen(ths->m2_value) + 1 );
    return offset;
}
int FirmWareConfig_Send_GetFromBytes(FirmWareConfig_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_cmd));
    ths->m2_version = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_version));
    return offset;
}
int FirmWareConfig_Return_GetFromBytes(FirmWareConfig_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int FirmWareTransmitFile_Send_GetFromBytes(FirmWareTransmitFile_Send *ths,u8* buff,int offset ) 
{
    ths->m1_datas_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_datas_Length));
    ths->m1_datas = (u8*)(buff + offset );
    offset += (ths->m1_datas_Length*sizeof(ths->m1_datas[0])  );
    ths->m2_crc = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_crc));
    ths->m3_offset = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_offset));
    return offset;
}
int FirmWareTransmitFile_Return_GetFromBytes(FirmWareTransmitFile_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int McuReset_Send_GetFromBytes(McuReset_Send *ths,u8* buff,int offset ) 
{
    ths->m1_reserved = buff[offset];
    offset += (sizeof(ths->m1_reserved));
    return offset;
}
int McuReset_Return_GetFromBytes(McuReset_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int FirmWareVersionGet_Send_GetFromBytes(FirmWareVersionGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_reserved = buff[offset];
    offset += (sizeof(ths->m1_reserved));
    return offset;
}
int FirmWareVersionGet_Return_GetFromBytes(FirmWareVersionGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_value = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_value));
    return offset;
}
int MotorConfigSet_Send_GetFromBytes(MotorConfigSet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_zeroCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_zeroCordinate));
    ths->m3_maxCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_maxCordinate));
    ths->m4_lostEndureContinue = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_lostEndureContinue));
    ths->m5_lostEndureStop = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_lostEndureStop));
    ths->m6_zeroTrigVol = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_zeroTrigVol));
    ths->m7_dirVol = ReadU32(buff + offset);
    offset += (sizeof(ths->m7_dirVol));
    ths->m8_isRotate = buff[offset];
    offset += (sizeof(ths->m8_isRotate));
    ths->m9_enableVol = buff[offset];
    offset += (sizeof(ths->m9_enableVol));
    ths->m10_halfVol = buff[offset];
    offset += (sizeof(ths->m10_halfVol));
    return offset;
}
int MotorConfigSet_Return_GetFromBytes(MotorConfigSet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMoveEvent_Send_GetFromBytes(MotorMoveEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_eventId = buff[offset];
    offset += (sizeof(ths->m6_eventId));
    return offset;
}
int MotorMoveEvent_Return_GetFromBytes(MotorMoveEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_trigger_pos = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_trigger_pos));
    return offset;
}
int McuIoStateChangeEvent_Send_GetFromBytes(McuIoStateChangeEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_port_pin_state_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_port_pin_state_Length));
    ths->m1_port_pin_state = (u16*)(buff + offset );
    offset += (ths->m1_port_pin_state_Length*sizeof(ths->m1_port_pin_state[0])  );
    return offset;
}
int McuIoStateChangeEvent_Return_GetFromBytes(McuIoStateChangeEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int SampleUnitSensorGet_Send_GetFromBytes(SampleUnitSensorGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_port = (char * )buff;
    offset += ( strlen(ths->m1_port) + 1 );
    ths->m2_pin = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_pin));
    return offset;
}
int SampleUnitSensorGet_Return_GetFromBytes(SampleUnitSensorGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int AirPumpOpen_Send_GetFromBytes(AirPumpOpen_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    ths->m2_pw = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_pw));
    return offset;
}
int AirPumpOpen_Return_GetFromBytes(AirPumpOpen_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int AirPumpClose_Send_GetFromBytes(AirPumpClose_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    return offset;
}
int AirPumpClose_Return_GetFromBytes(AirPumpClose_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}

int ADSampleRead_Send_GetFromBytes(ADSampleRead_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channel = buff[offset];
    offset += (sizeof(ths->m1_channel));
    ths->m2_amp = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_amp));
    return offset;
}
int ADSampleRead_Return_GetFromBytes(ADSampleRead_Return *ths,u8* buff,int offset ) 
{
    ths->m1_val = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_val));
    return offset;
}
/*
这里实现 数据接口 的GetFromBytes成员函数
*/

int DiskInstalledRed_Send_GetFromBytes(DiskInstalledRed_Send *ths,u8* buff,int offset)
{
    ths->m1_cnt = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_cnt));
		ths->m2_time_span_ms = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_time_span_ms));
		ths->m3_channel = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_channel));

    return offset;
}
int DiskInstalledRed_Return_GetFromBytes(DiskInstalledRed_Return *ths,u8* buff,int offset) 
{
		ths->m1_errCode = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_errCode));
	
    ths->m2_maxval = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_maxval));
	  ths->m3_minval = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_minval));
	
    return offset;
}


int MotorMoveGPIOEvent_Send_GetFromBytes(MotorMoveGPIOEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_state_port_pin = ReadU16(buff + offset);
    offset += (sizeof(ths->m6_state_port_pin));
    return offset;
}
int MotorMoveGPIOEvent_Return_GetFromBytes(MotorMoveGPIOEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_trigger_pos = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_trigger_pos));
    return offset;
}
int MotorConfigSetNew_Send_GetFromBytes(MotorConfigSetNew_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx_zero_dir_isR_en_hf = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_idx_zero_dir_isR_en_hf));
    ths->m2_maxCordinate = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_maxCordinate));
    ths->m3_zeroCordinate = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_zeroCordinate));
    ths->m4_lostEndureContinue = ReadU16(buff + offset);
    offset += (sizeof(ths->m4_lostEndureContinue));
    ths->m5_lostEndureStop = ReadU16(buff + offset);
    offset += (sizeof(ths->m5_lostEndureStop));
    return offset;
}
int MotorConfigSetNew_Return_GetFromBytes(MotorConfigSetNew_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ExecutesCtrl_Send_GetFromBytes(ExecutesCtrl_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelId = buff[offset];
    offset += (sizeof(ths->m1_channelId));
    ths->m2_isOpen = buff[offset];
    offset += (sizeof(ths->m2_isOpen));
    return offset;
}
int ExecutesCtrl_Return_GetFromBytes(ExecutesCtrl_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ADDetectConfigSet_Send_GetFromBytes(ADDetectConfigSet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_amp_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_amp_Length));
    ths->m1_amp = (u16*)(buff + offset );
    offset += (ths->m1_amp_Length*sizeof(ths->m1_amp[0])  );
    ths->m2_filterCnt = buff[offset];
    offset += (sizeof(ths->m2_filterCnt));
    ths->m3_light_offset = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_light_offset));
    ths->m4_channel_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_channel_Length));
    ths->m4_channel = (u16*)(buff + offset );
    offset += (ths->m4_channel_Length*sizeof(ths->m4_channel[0])  );
    ths->m5_light_motor_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_light_motor_speed));
    return offset;
}
int ADDetectConfigSet_Return_GetFromBytes(ADDetectConfigSet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ADDataRead_Send_GetFromBytes(ADDataRead_Send *ths,u8* buff,int offset ) 
{
    ths->m1_filterId = buff[offset];
    offset += (sizeof(ths->m1_filterId));
    ths->m2_method = buff[offset];
    offset += (sizeof(ths->m2_method));
    ths->m3_aver_cnt = buff[offset];
    offset += (sizeof(ths->m3_aver_cnt));
    ths->m4_channel = buff[offset];
    offset += (sizeof(ths->m4_channel));
    ths->m5_detect_circle_cnt = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_detect_circle_cnt));
    ths->m6_z2f = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_z2f));
	
	ths->m7_time_per_degree = ReadU32(buff + offset);
    offset += (sizeof(ths->m7_time_per_degree));
	
    return offset;
}
int ADDataRead_Return_GetFromBytes(ADDataRead_Return *ths,u8* buff,int offset ) 
{
    ths->m1_val_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_val_Length));
    ths->m1_val = (u16*)(buff + offset );
    offset += (ths->m1_val_Length*sizeof(ths->m1_val[0])  );
    return offset;
}
int ADDataSeriesSend_Send_GetFromBytes(ADDataSeriesSend_Send *ths,u8* buff,int offset ) 
{
    ths->m1_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data_Length));
    ths->m1_data = (u16*)(buff + offset );
    offset += (ths->m1_data_Length*sizeof(ths->m1_data[0])  );
    ths->m2_current_detect_circle = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_current_detect_circle));
    return offset;
}
int ADDataSeriesSend_Return_GetFromBytes(ADDataSeriesSend_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int UartTransmit_Send_GetFromBytes(UartTransmit_Send *ths,u8* buff,int offset ) 
{
    ths->m1_uart_id = buff[offset];
    offset += (sizeof(ths->m1_uart_id));
    ths->m2_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_data_Length));
    ths->m2_data = (u8*)(buff + offset );
    offset += (ths->m2_data_Length*sizeof(ths->m2_data[0])  );
    return offset;
}
int UartTransmit_Return_GetFromBytes(UartTransmit_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int UartReceiveEvent_Send_GetFromBytes(UartReceiveEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_uart_id = buff[offset];
    offset += (sizeof(ths->m1_uart_id));
    ths->m2_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_data_Length));
    ths->m2_data = (u8*)(buff + offset );
    offset += (ths->m2_data_Length*sizeof(ths->m2_data[0])  );
    return offset;
}
int UartReceiveEvent_Return_GetFromBytes(UartReceiveEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_GetFromBytes(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    return offset;
}
int SeqMotorMultiMove_Send_m1_Sequence_GetFromBytes(SeqMotorMultiMove_Send_m1_Sequence *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    ths->m2_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_delayms));
    return offset;
}
int SeqMotorMultiMove_Send_GetFromBytes(SeqMotorMultiMove_Send *ths,u8* buff,int offset ) 
{
    ths->m1_Sequence_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_Sequence_Length));
    ths->m1_Sequence = malloc(sizeof(ths->m1_Sequence[0])*(ths->m1_Sequence_Length));
    PTR_ASSERT(ths->m1_Sequence);
    for(int i =0;i<ths->m1_Sequence_Length;i++){
        offset = SeqMotorMultiMove_Send_m1_Sequence_GetFromBytes( &(ths->m1_Sequence[i]) ,buff,offset );
    }
    return offset;
}
int SeqMotorMultiMove_Return_GetFromBytes(SeqMotorMultiMove_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_errIdx = buff[offset];
    offset += (sizeof(ths->m2_errIdx));
    return offset;
}
int PressureConfig_Send_GetFromBytes(PressureConfig_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cmd = buff[offset];
    offset += (sizeof(ths->m1_cmd));
    ths->m2_averageCnt = buff[offset];
    offset += (sizeof(ths->m2_averageCnt));
    ths->m3_tick = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_tick));
    ths->m4_maxSampleCnt = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_maxSampleCnt));
    ths->m5_threshold = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_threshold));
    return offset;
}
int PressureConfig_Return_GetFromBytes(PressureConfig_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int PressureDataGet_Send_GetFromBytes(PressureDataGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cmd = buff[offset];
    offset += (sizeof(ths->m1_cmd));
    ths->m2_data_offset = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_data_offset));
    return offset;
}
int PressureDataGet_Return_GetFromBytes(PressureDataGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_remainLen = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_remainLen));
    ths->m3_pressData_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_pressData_Length));
    ths->m3_pressData = (u16*)(buff + offset );
    offset += (ths->m3_pressData_Length*sizeof(ths->m3_pressData[0])  );
    return offset;
}
int MotorAbort_Send_GetFromBytes(MotorAbort_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx_mask = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_idx_mask));
    return offset;
}
int MotorAbort_Return_GetFromBytes(MotorAbort_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int PressureEvent_Send_GetFromBytes(PressureEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_eventId = buff[offset];
    offset += (sizeof(ths->m1_eventId));
    return offset;
}
int PressureEvent_Return_GetFromBytes(PressureEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMoveSeq_Send_m1_MoveArgs_GetFromBytes(MotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_delayms));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m2_isBackZero = buff[offset];
    offset += (sizeof(ths->m2_isBackZero));
    return offset;
}
int MotorMoveSeq_Send_GetFromBytes(MotorMoveSeq_Send *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = MotorMoveSeq_Send_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    return offset;
}
int MotorMoveSeq_Return_GetFromBytes(MotorMoveSeq_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int FirmWareReadyEvent_Send_GetFromBytes(FirmWareReadyEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_reserved = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_reserved));
    return offset;
}
int FirmWareReadyEvent_Return_GetFromBytes(FirmWareReadyEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorConfigGet_Send_GetFromBytes(MotorConfigGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorConfigGet_Return_GetFromBytes(MotorConfigGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_zeroCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_zeroCordinate));
    ths->m3_maxCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_maxCordinate));
    ths->m4_lostEndureContinue = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_lostEndureContinue));
    ths->m5_lostEndureStop = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_lostEndureStop));
    ths->m6_zeroTrigVol = buff[offset];
    offset += (sizeof(ths->m6_zeroTrigVol));
    ths->m7_dirVol = buff[offset];
    offset += (sizeof(ths->m7_dirVol));
    ths->m8_isRotate = buff[offset];
    offset += (sizeof(ths->m8_isRotate));
    ths->m9_enableVol = buff[offset];
    offset += (sizeof(ths->m9_enableVol));
    ths->m10_halfVol = buff[offset];
    offset += (sizeof(ths->m10_halfVol));
    return offset;
}
int PWM_Set_Send_GetFromBytes(PWM_Set_Send *ths,u8* buff,int offset ) 
{
    ths->m1_time_channle = buff[offset];
    offset += (sizeof(ths->m1_time_channle));
    ths->m2_period = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_period));
    ths->m3_dutyCycle = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_dutyCycle));
    return offset;
}
int PWM_Set_Return_GetFromBytes(PWM_Set_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int TurnTable_Move_Send_GetFromBytes(TurnTable_Move_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cali_location = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_cali_location));
    ths->m2_idx = buff[offset];
    offset += (sizeof(ths->m2_idx));
    ths->m3_steps = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_steps));
    ths->m4_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_speed));
    ths->m5_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_acce));
    return offset;
}
int TurnTable_Move_Return_GetFromBytes(TurnTable_Move_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_filter_id = buff[offset];
    offset += (sizeof(ths->m2_filter_id));
    ths->m3_ad_data = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_ad_data));
    return offset;
}
int EncodeTurnTableFindZero_Send_GetFromBytes(EncodeTurnTableFindZero_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_cali_location = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_cali_location));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_acce));
    return offset;
}
int EncodeTurnTableFindZero_Return_GetFromBytes(EncodeTurnTableFindZero_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int EncodeTurnTableRotateStart_Send_GetFromBytes(EncodeTurnTableRotateStart_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_cali_location = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_cali_location));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_acce));
    return offset;
}
int EncodeTurnTableRotateStart_Return_GetFromBytes(EncodeTurnTableRotateStart_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int EncodeTurnTableRotateStop_Send_GetFromBytes(EncodeTurnTableRotateStop_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_dest_id = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_dest_id));
    return offset;
}
int EncodeTurnTableRotateStop_Return_GetFromBytes(EncodeTurnTableRotateStop_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int EncodeTurnTableStateGet_Send_GetFromBytes(EncodeTurnTableStateGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int EncodeTurnTableStateGet_Return_GetFromBytes(EncodeTurnTableStateGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_isZeroTriggered = buff[offset];
    offset += (sizeof(ths->m1_isZeroTriggered));
    ths->m2_is_busy = buff[offset];
    offset += (sizeof(ths->m2_is_busy));
    ths->m3_current_id = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_current_id));
    return offset;
}
int DCAdRead_Send_GetFromBytes(DCAdRead_Send *ths,u8* buff,int offset ) 
{
    ths->m1_cmd = buff[offset];
    offset += (sizeof(ths->m1_cmd));
    ths->m2_channel = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_channel));
    return offset;
}
int DCAdRead_Return_GetFromBytes(DCAdRead_Return *ths,u8* buff,int offset ) 
{
    ths->m1_remainCnt = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_remainCnt));
    ths->m2_adValue_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_adValue_Length));
    ths->m2_adValue = (u16*)(buff + offset );
    offset += (ths->m2_adValue_Length*sizeof(ths->m2_adValue[0])  );
    return offset;
}
int JSAdRead_Send_GetFromBytes(JSAdRead_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channel_id = buff[offset];
    offset += (sizeof(ths->m1_channel_id));
    ths->m2_time_span = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_time_span));
    ths->m3_cnt_read = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_cnt_read));
    ths->m4_dac_data = ReadU16(buff + offset);
    offset += (sizeof(ths->m4_dac_data));
    return offset;
}
int JSAdRead_Return_GetFromBytes(JSAdRead_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMoveSyncLevel_Send_GetFromBytes(MotorMoveSyncLevel_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_steps = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_steps));
    ths->m3_sync_steps = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_sync_steps));
    ths->m4_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_speed));
    ths->m5_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_acce));
    return offset;
}
int MotorMoveSyncLevel_Return_GetFromBytes(MotorMoveSyncLevel_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int DCMotor_Send_GetFromBytes(DCMotor_Send *ths,u8* buff,int offset ) 
{
    ths->m1_Idx = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_Idx));
    ths->m2_isOpen = buff[offset];
    offset += (sizeof(ths->m2_isOpen));
    return offset;
}
int DCMotor_Return_GetFromBytes(DCMotor_Return *ths,u8* buff,int offset ) 
{
    ths->errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->errCode));
    return offset;
}
int MotorCupIDSet_Send_GetFromBytes(MotorCupIDSet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_CupID = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_CupID));
    ths->m2_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_speed));
    ths->m3_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_acce));
    return offset;
}
int MotorCupIDSet_Return_GetFromBytes(MotorCupIDSet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorCupIDInfoGet_Send_GetFromBytes(MotorCupIDInfoGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorCupIDInfoGet_Return_GetFromBytes(MotorCupIDInfoGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_errCode));
    ths->m3_turntableStatus = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_turntableStatus));
    ths->m4_nowCupID = ReadU16(buff + offset);
    offset += (sizeof(ths->m4_nowCupID));
    ths->m5_isinit = buff[offset];
    offset += (sizeof(ths->m5_isinit));
    return offset;
}
int JSAdDataGet_Send_GetFromBytes(JSAdDataGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_channelIdx = buff[offset];
    offset += (sizeof(ths->m1_channelIdx));
    ths->m2_timeSpan = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_timeSpan));
    ths->m3_dataCnt = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_dataCnt));
    return offset;
}
int JSAdDataGet_Return_GetFromBytes(JSAdDataGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data_Length));
    ths->m1_data = (u16*)(buff + offset );
    offset += (ths->m1_data_Length*sizeof(ths->m1_data[0])  );
    return offset;
}
int MotorMultiMoveTmc_Send_m1_MoveArgs_GetFromBytes(MotorMultiMoveTmc_Send_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_delayms));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m8_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m8_subdivision));
    return offset;
}
int MotorMultiMoveTmc_Send_GetFromBytes(MotorMultiMoveTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = MotorMultiMoveTmc_Send_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    ths->m2_isBackZero = buff[offset];
    offset += (sizeof(ths->m2_isBackZero));
    return offset;
}
int MotorMultiMoveTmc_Return_GetFromBytes(MotorMultiMoveTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMoveTmc_Send_GetFromBytes(MotorMoveTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_isBackZero = buff[offset];
    offset += (sizeof(ths->m6_isBackZero));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m8_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m8_subdivision));
    ths->m9_backzero_position = ReadU16(buff + offset);
    offset += (sizeof(ths->m9_backzero_position));
    return offset;
}
int MotorMoveTmc_Return_GetFromBytes(MotorMoveTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorStatetGetTmc_Send_GetFromBytes(MotorStatetGetTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorStatetGetTmc_Return_GetFromBytes(MotorStatetGetTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_isZeroTriggered = buff[offset];
    offset += (sizeof(ths->m1_isZeroTriggered));
    ths->m2_currentCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_currentCordinate));
    ths->m3_lostSteps = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_lostSteps));
    ths->m4_isBusy = buff[offset];
    offset += (sizeof(ths->m4_isBusy));
    ths->m5_currentLevel = buff[offset];
    offset += (sizeof(ths->m5_currentLevel));
    ths->m6_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m6_subdivision));
    return offset;
}
int MotorRotateStartTmc_Send_GetFromBytes(MotorRotateStartTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_isForward = buff[offset];
    offset += (sizeof(ths->m4_isForward));
    ths->m5_currentLevel = buff[offset];
    offset += (sizeof(ths->m5_currentLevel));
    ths->m6_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m6_subdivision));
    return offset;
}
int MotorRotateStartTmc_Return_GetFromBytes(MotorRotateStartTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorRotateStopTmc_Send_GetFromBytes(MotorRotateStopTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int MotorRotateStopTmc_Return_GetFromBytes(MotorRotateStopTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorRotateTmc_Send_GetFromBytes(MotorRotateTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_currentLevel = buff[offset];
    offset += (sizeof(ths->m6_currentLevel));
    ths->m7_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m7_subdivision));
    return offset;
}
int MotorRotateTmc_Return_GetFromBytes(MotorRotateTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorResetTmc_Send_GetFromBytes(MotorResetTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_currentLevel = buff[offset];
    offset += (sizeof(ths->m4_currentLevel));
    ths->m5_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m5_subdivision));
    return offset;
}
int MotorResetTmc_Return_GetFromBytes(MotorResetTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorConfigSetTmc_Send_GetFromBytes(MotorConfigSetTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_zeroCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_zeroCordinate));
    ths->m3_maxCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_maxCordinate));
    ths->m4_lostEndureContinue = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_lostEndureContinue));
    ths->m5_lostEndureStop = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_lostEndureStop));
    ths->m6_zeroTrigVol = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_zeroTrigVol));
    ths->m7_dirVol = ReadU32(buff + offset);
    offset += (sizeof(ths->m7_dirVol));
    ths->m8_isRotate = buff[offset];
    offset += (sizeof(ths->m8_isRotate));
    ths->m9_enableVol = buff[offset];
    offset += (sizeof(ths->m9_enableVol));
    ths->m10_halfVol = buff[offset];
    offset += (sizeof(ths->m10_halfVol));
    ths->m11_currentLevel = buff[offset];
    offset += (sizeof(ths->m11_currentLevel));
    ths->m12_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m12_subdivision));
    ths->m13_holdLevel = buff[offset];
    offset += (sizeof(ths->m13_holdLevel));
    return offset;
}
int MotorConfigSetTmc_Return_GetFromBytes(MotorConfigSetTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int MotorMoveGPIOEventTmc_Send_GetFromBytes(MotorMoveGPIOEventTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_state_port_pin = ReadU16(buff + offset);
    offset += (sizeof(ths->m6_state_port_pin));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m8_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m8_subdivision));
    ths->m9_isdetect = ReadU16(buff + offset);
    offset += (sizeof(ths->m9_isdetect));
    ths->m10_movedelay = ReadU16(buff + offset);
    offset += (sizeof(ths->m10_movedelay));
    return offset;
}
int MotorMoveGPIOEventTmc_Return_GetFromBytes(MotorMoveGPIOEventTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_trigger_pos = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_trigger_pos));
    ths->m3_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_distance));
    return offset;
}
int MotorRotateShake_Send_GetFromBytes(MotorRotateShake_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_dir = buff[offset];
    offset += (sizeof(ths->m4_dir));
    ths->m5_currentLevel = buff[offset];
    offset += (sizeof(ths->m5_currentLevel));
    ths->m6_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m6_subdivision));
    ths->m7_step = ReadU32(buff + offset);
    offset += (sizeof(ths->m7_step));
    return offset;
}
int MotorRotateShake_Return_GetFromBytes(MotorRotateShake_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int LiquidLevelResultGet_Send_GetFromBytes(LiquidLevelResultGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_data = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data));
    return offset;
}
int LiquidLevelResultGet_Return_GetFromBytes(LiquidLevelResultGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_trigger_pos = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_trigger_pos));
    return offset;
}
int MotorDetectLiquidLevel_Send_GetFromBytes(MotorDetectLiquidLevel_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m8_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m8_subdivision));
    ths->m10_idx = buff[offset];
    offset += (sizeof(ths->m10_idx));
    ths->m11_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m11_acce));
    ths->m12_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m12_speed));
    ths->m13_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m13_distance));
    ths->m14_isRelative = buff[offset];
    offset += (sizeof(ths->m14_isRelative));
    ths->m15_currentLevel = buff[offset];
    offset += (sizeof(ths->m15_currentLevel));
    ths->m16_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m16_subdivision));
    return offset;
}
int MotorDetectLiquidLevel_Return_GetFromBytes(MotorDetectLiquidLevel_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m3_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_distance));
    return offset;
}
int RFIDValueOperation_Send_GetFromBytes(RFIDValueOperation_Send *ths,u8* buff,int offset ) 
{
    ths->m1_op = buff[offset];
    offset += (sizeof(ths->m1_op));
    ths->data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->data_Length));
    ths->data = (u8*)(buff + offset );
    offset += (ths->data_Length*sizeof(ths->data[0])  );
    return offset;
}
int RFIDValueOperation_Return_GetFromBytes(RFIDValueOperation_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->data_Length));
    ths->data = (u8*)(buff + offset );
    offset += (ths->data_Length*sizeof(ths->data[0])  );
    return offset;
}
int IoSimple_Send_GetFromBytes(IoSimple_Send *ths,u8* buff,int offset ) 
{
    ths->m1_port_pin_arg = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_port_pin_arg));
    return offset;
}
int IoSimple_Return_GetFromBytes(IoSimple_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode_value = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode_value));
    return offset;
}
int IoOperation_Send_m1_IoArgs_m1_Controls_GetFromBytes(IoOperation_Send_m1_IoArgs_m1_Controls *ths,u8* buff,int offset ) 
{
    ths->m1_port_pin = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_port_pin));
    ths->m2_cmd_arg = buff[offset];
    offset += (sizeof(ths->m2_cmd_arg));
    return offset;
}
int IoOperation_Send_m1_IoArgs_GetFromBytes(IoOperation_Send_m1_IoArgs *ths,u8* buff,int offset ) 
{
    ths->m1_Controls_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_Controls_Length));
    ths->m1_Controls = malloc(sizeof(ths->m1_Controls[0])*(ths->m1_Controls_Length));
    PTR_ASSERT(ths->m1_Controls);
    for(int i =0;i<ths->m1_Controls_Length;i++){
        offset = IoOperation_Send_m1_IoArgs_m1_Controls_GetFromBytes( &(ths->m1_Controls[i]) ,buff,offset );
    }
    ths->m2_delay = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_delay));
    return offset;
}
int IoOperation_Send_GetFromBytes(IoOperation_Send *ths,u8* buff,int offset ) 
{
    ths->m1_IoArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_IoArgs_Length));
    ths->m1_IoArgs = malloc(sizeof(ths->m1_IoArgs[0])*(ths->m1_IoArgs_Length));
    PTR_ASSERT(ths->m1_IoArgs);
    for(int i =0;i<ths->m1_IoArgs_Length;i++){
        offset = IoOperation_Send_m1_IoArgs_GetFromBytes( &(ths->m1_IoArgs[i]) ,buff,offset );
    }
    return offset;
}
int IoOperation_Return_GetFromBytes(IoOperation_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_value = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_value));
    return offset;
}
int IoBlink_Send_GetFromBytes(IoBlink_Send *ths,u8* buff,int offset ) 
{
    ths->m1_port = buff[offset];
    offset += (sizeof(ths->m1_port));
    ths->m2_pin = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_pin));
    ths->m3_state = buff[offset];
    offset += (sizeof(ths->m3_state));
    ths->m4_frequence = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_frequence));
    return offset;
}
int IoBlink_Return_GetFromBytes(IoBlink_Return *ths,u8* buff,int offset ) 
{
    ths->errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->errCode));
    return offset;
}
int UartChangeBode_Send_GetFromBytes(UartChangeBode_Send *ths,u8* buff,int offset ) 
{
    ths->m1_uartIdx = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_uartIdx));
    ths->m2_bode = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_bode));
    return offset;
}
int UartChangeBode_Return_GetFromBytes(UartChangeBode_Return *ths,u8* buff,int offset ) 
{
    ths->errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->errCode));
    return offset;
}
int ServoMotorMultiRotate_Send_m1_MoveArgs_GetFromBytes(ServoMotorMultiRotate_Send_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_dir = buff[offset];
    offset += (sizeof(ths->m2_dir));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_acce));
    ths->m5_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_delayms));
    ths->m6_isSendSyncSigle = buff[offset];
    offset += (sizeof(ths->m6_isSendSyncSigle));
    return offset;
}
int ServoMotorMultiRotate_Send_GetFromBytes(ServoMotorMultiRotate_Send *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = ServoMotorMultiRotate_Send_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    return offset;
}
int ServoMotorMultiRotate_Return_GetFromBytes(ServoMotorMultiRotate_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorRotateShakeDetect_Send_GetFromBytes(ServoMotorRotateShakeDetect_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_time));
    ths->m5_isForward = buff[offset];
    offset += (sizeof(ths->m5_isForward));
    ths->m6_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_time));
    ths->m7_frequency = buff[offset];
    offset += (sizeof(ths->m7_frequency));
    ths->m8_amplitude = ReadU32(buff + offset);
    offset += (sizeof(ths->m8_amplitude));
    ths->m9_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m9_acce));
    ths->m10_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m10_speed));
    ths->m11_isForward = buff[offset];
    offset += (sizeof(ths->m11_isForward));
    return offset;
}
int ServoMotorRotateShakeDetect_Return_GetFromBytes(ServoMotorRotateShakeDetect_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorRotateShakeRotateDetect_Send_GetFromBytes(ServoMotorRotateShakeRotateDetect_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_time));
    ths->m5_isForward = buff[offset];
    offset += (sizeof(ths->m5_isForward));
    ths->m6_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_time));
    ths->m7_frequency = buff[offset];
    offset += (sizeof(ths->m7_frequency));
    ths->m8_amplitude = ReadU32(buff + offset);
    offset += (sizeof(ths->m8_amplitude));
    ths->m9_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m9_acce));
    ths->m10_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m10_speed));
    ths->m11_isForward = buff[offset];
    offset += (sizeof(ths->m11_isForward));
    ths->m12_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m12_time));
    ths->m13_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m13_acce));
    ths->m14_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m14_speed));
    ths->m15_isForward = buff[offset];
    offset += (sizeof(ths->m15_isForward));
    return offset;
}
int ServoMotorRotateShakeRotateDetect_Return_GetFromBytes(ServoMotorRotateShakeRotateDetect_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorRotateTwoDirOnTime_Send_GetFromBytes(ServoMotorRotateTwoDirOnTime_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_isForward = buff[offset];
    offset += (sizeof(ths->m4_isForward));
    ths->m5_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_time));
    return offset;
}
int ServoMotorRotateTwoDirOnTime_Return_GetFromBytes(ServoMotorRotateTwoDirOnTime_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorStatetGet_Send_GetFromBytes(ServoMotorStatetGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int ServoMotorStatetGet_Return_GetFromBytes(ServoMotorStatetGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_isZeroTriggered = buff[offset];
    offset += (sizeof(ths->m2_isZeroTriggered));
    ths->m3_currentCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_currentCordinate));
    ths->m4_currentSpeed = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_currentSpeed));
    return offset;
}
int ServoMotorRotateStart_Send_GetFromBytes(ServoMotorRotateStart_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_isForward = buff[offset];
    offset += (sizeof(ths->m4_isForward));
    ths->m5_isSendSyncSigle = buff[offset];
    offset += (sizeof(ths->m5_isSendSyncSigle));
    return offset;
}
int ServoMotorRotateStart_Return_GetFromBytes(ServoMotorRotateStart_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorRotateStop_Send_GetFromBytes(ServoMotorRotateStop_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int ServoMotorRotateStop_Return_GetFromBytes(ServoMotorRotateStop_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorReset_Send_GetFromBytes(ServoMotorReset_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    return offset;
}
int ServoMotorReset_Return_GetFromBytes(ServoMotorReset_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorAutoCalibratio_Send_GetFromBytes(ServoMotorAutoCalibratio_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int ServoMotorAutoCalibratio_Return_GetFromBytes(ServoMotorAutoCalibratio_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorShake_Send_GetFromBytes(ServoMotorShake_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_time));
    ths->m3_frequency = buff[offset];
    offset += (sizeof(ths->m3_frequency));
    ths->m4_amplitude = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_amplitude));
    return offset;
}
int ServoMotorShake_Return_GetFromBytes(ServoMotorShake_Return *ths,u8* buff,int offset ) 
{
    ths->m1_maxCordinate = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_maxCordinate));
    ths->m2_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_errCode));
    return offset;
}
int ServoMotorBaseCtrl_Send_GetFromBytes(ServoMotorBaseCtrl_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_option = buff[offset];
    offset += (sizeof(ths->m2_option));
    return offset;
}
int ServoMotorBaseCtrl_Return_GetFromBytes(ServoMotorBaseCtrl_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int ServoMotorMove_Send_GetFromBytes(ServoMotorMove_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_steps = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_steps));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_acce));
    ths->m5_isAbsolute = buff[offset];
    offset += (sizeof(ths->m5_isAbsolute));
    return offset;
}
int ServoMotorMove_Return_GetFromBytes(ServoMotorMove_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int RGBSensorGet_Send_GetFromBytes(RGBSensorGet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int RGBSensorGet_Return_GetFromBytes(RGBSensorGet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    ths->m2_R = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_R));
    ths->m3_G = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_G));
    ths->m4_B = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_B));
    ths->m5_C = ReadU32(buff + offset);
    offset += (sizeof(ths->m5_C));
    return offset;
}
int SerialDataReport_Send_GetFromBytes(SerialDataReport_Send *ths,u8* buff,int offset ) 
{
    ths->m1_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_data_Length));
    ths->m1_data = (u8*)(buff + offset );
    offset += (ths->m1_data_Length*sizeof(ths->m1_data[0])  );
    return offset;
}
int SerialDataReport_Return_GetFromBytes(SerialDataReport_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int AddressBroadcastSend_Send_GetFromBytes(AddressBroadcastSend_Send *ths,u8* buff,int offset ) 
{
    ths->m1_mode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_mode));
    ths->m2_address = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_address));
    ths->m3_count = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_count));
    ths->m4_addrRestrict_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_addrRestrict_Length));
    ths->m4_addrRestrict = (u8*)(buff + offset );
    offset += (ths->m4_addrRestrict_Length*sizeof(ths->m4_addrRestrict[0])  );
    return offset;
}
int AddressBroadcastSend_Return_GetFromBytes(AddressBroadcastSend_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int AddressBroadcastReceive_Send_GetFromBytes(AddressBroadcastReceive_Send *ths,u8* buff,int offset ) 
{
    ths->m1_address = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_address));
    return offset;
}
int AddressBroadcastReceive_Return_GetFromBytes(AddressBroadcastReceive_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int StartCheckPress_Send_GetFromBytes(StartCheckPress_Send *ths,u8* buff,int offset ) 
{
    ths->m1_address = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_address));
    return offset;
}
int StartCheckPress_Return_GetFromBytes(StartCheckPress_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int EndCheckPress_Send_GetFromBytes(EndCheckPress_Send *ths,u8* buff,int offset ) 
{
    ths->m1_address = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_address));
    return offset;
}
int EndCheckPress_Return_GetFromBytes(EndCheckPress_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int Blending_Send_GetFromBytes(Blending_Send *ths,u8* buff,int offset ) 
{
    ths->m1_address = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_address));
    ths->m2_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_speed));
    ths->m3_time = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_time));
    ths->m4_dir = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_dir));
    return offset;
}
int Blending_Return_GetFromBytes(Blending_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int GetEPOSType_Send_GetFromBytes(GetEPOSType_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_idx));
    return offset;
}
int GetEPOSType_Return_GetFromBytes(GetEPOSType_Return *ths,u8* buff,int offset ) 
{
    ths->m1_motorType = buff[offset];
    offset += (sizeof(ths->m1_motorType));
    ths->m2_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_errCode));
    return offset;
}
int SetEPOSType_Send_GetFromBytes(SetEPOSType_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_idx));
    ths->m2_typeId = buff[offset];
    offset += (sizeof(ths->m2_typeId));
    return offset;
}
int SetEPOSType_Return_GetFromBytes(SetEPOSType_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int DACSet_Send_GetFromBytes(DACSet_Send *ths,u8* buff,int offset ) 
{
    ths->m1_value = ReadU16(buff + offset);
    offset += (sizeof(ths->m1_value));
    return offset;
}
int DACSet_Return_GetFromBytes(DACSet_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int LightCtrl_Send_GetFromBytes(LightCtrl_Send *ths,u8* buff,int offset ) 
{
    ths->idx = buff[offset];
    offset += (sizeof(ths->idx));
    ths->value = ReadU32(buff + offset);
    offset += (sizeof(ths->value));
    return offset;
}
int LightCtrl_Return_GetFromBytes(LightCtrl_Return *ths,u8* buff,int offset ) 
{
    ths->errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->errCode));
    return offset;
}
int DCMotorMoveSeq_Send_m1_MoveArgs_GetFromBytes(DCMotorMoveSeq_Send_m1_MoveArgs *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_delayms = ReadU32(buff + offset);
    offset += (sizeof(ths->m6_delayms));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m2_isBackZero = buff[offset];
    offset += (sizeof(ths->m2_isBackZero));
    return offset;
}
int DCMotorMoveSeq_Send_GetFromBytes(DCMotorMoveSeq_Send *ths,u8* buff,int offset ) 
{
    ths->m1_MoveArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_MoveArgs_Length));
    ths->m1_MoveArgs = malloc(sizeof(ths->m1_MoveArgs[0])*(ths->m1_MoveArgs_Length));
    PTR_ASSERT(ths->m1_MoveArgs);
    for(int i =0;i<ths->m1_MoveArgs_Length;i++){
        offset = DCMotorMoveSeq_Send_m1_MoveArgs_GetFromBytes( &(ths->m1_MoveArgs[i]) ,buff,offset );
    }
    ths->m2_WaitSyncTime = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_WaitSyncTime));
    return offset;
}
int DCMotorMoveSeq_Return_GetFromBytes(DCMotorMoveSeq_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int DCMotorMoveTmc_Send_GetFromBytes(DCMotorMoveTmc_Send *ths,u8* buff,int offset ) 
{
    ths->m1_idx = buff[offset];
    offset += (sizeof(ths->m1_idx));
    ths->m2_acce = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_acce));
    ths->m3_speed = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_speed));
    ths->m4_distance = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_distance));
    ths->m5_isRelative = buff[offset];
    offset += (sizeof(ths->m5_isRelative));
    ths->m6_isBackZero = buff[offset];
    offset += (sizeof(ths->m6_isBackZero));
    ths->m7_currentLevel = buff[offset];
    offset += (sizeof(ths->m7_currentLevel));
    ths->m8_subdivision = ReadU16(buff + offset);
    offset += (sizeof(ths->m8_subdivision));
    ths->m9_backzero_position = ReadU16(buff + offset);
    offset += (sizeof(ths->m9_backzero_position));
    ths->m10_writeSyncFlag = buff[offset];
    offset += (sizeof(ths->m10_writeSyncFlag));
    return offset;
}
int DCMotorMoveTmc_Return_GetFromBytes(DCMotorMoveTmc_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int PressureRead_Send_GetFromBytes(PressureRead_Send *ths,u8* buff,int offset ) 
{
    ths->m1_id = buff[offset];
    offset += (sizeof(ths->m1_id));
    return offset;
}
int PressureRead_Return_GetFromBytes(PressureRead_Return *ths,u8* buff,int offset ) 
{
    ths->m1_pressure1 = ReadF32(buff + offset);
    offset += (sizeof(ths->m1_pressure1));
    ths->m2_pressure2 = ReadF32(buff + offset);
    offset += (sizeof(ths->m2_pressure2));
    ths->m3_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_errCode));
    return offset;
}
int LittleMotorCtrl_Send_GetFromBytes(LittleMotorCtrl_Send *ths,u8* buff,int offset ) 
{
    ths->m1_flag = buff[offset];
    offset += (sizeof(ths->m1_flag));
    ths->m2_period = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_period));
    ths->m3_dutyCycle = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_dutyCycle));
    return offset;
}
int LittleMotorCtrl_Return_GetFromBytes(LittleMotorCtrl_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int CongestionDetection_Send_GetFromBytes(CongestionDetection_Send *ths,u8* buff,int offset ) 
{
    ths->m1_status = buff[offset];
    offset += (sizeof(ths->m1_status));
    ths->m2_threshold = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_threshold));
    return offset;
}
int CongestionDetection_Return_GetFromBytes(CongestionDetection_Return *ths,u8* buff,int offset ) 
{
    ths->m1_status = buff[offset];
    offset += (sizeof(ths->m1_status));
    ths->m2_ad_value = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_ad_value));
    ths->m3_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_errCode));
    return offset;
}
int LaserCtrol_Send_GetFromBytes(LaserCtrol_Send *ths,u8* buff,int offset ) 
{
    ths->m1_status = buff[offset];
    offset += (sizeof(ths->m1_status));
    return offset;
}
int LaserCtrol_Return_GetFromBytes(LaserCtrol_Return *ths,u8* buff,int offset ) 
{
    ths->m1_errCode = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_errCode));
    return offset;
}
int NFL_ReadConfig_Send_GetFromBytes(NFL_ReadConfig_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_ReadConfig_Return_GetFromBytes(NFL_ReadConfig_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    ths->offset0 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset0));
    ths->volt0 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt0));
    ths->delay0 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay0));
    ths->offset1 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset1));
    ths->volt1 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt1));
    ths->delay1 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay1));
    ths->offset2 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset2));
    ths->volt2 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt2));
    ths->delay2 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay2));
    ths->offset3 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset3));
    ths->volt3 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt3));
    ths->delay3 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay3));
    ths->offset4 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset4));
    ths->volt4 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt4));
    ths->delay4 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay4));
    return offset;
}
int NFL_WriteConfig_Send_GetFromBytes(NFL_WriteConfig_Send *ths,u8* buff,int offset ) 
{
    ths->offset0 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset0));
    ths->volt0 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt0));
    ths->delay0 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay0));
    ths->offset1 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset1));
    ths->volt1 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt1));
    ths->delay1 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay1));
    ths->offset2 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset2));
    ths->volt2 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt2));
    ths->delay2 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay2));
    ths->offset3 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset3));
    ths->volt3 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt3));
    ths->delay3 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay3));
    ths->offset4 = ReadF32(buff + offset);
    offset += (sizeof(ths->offset4));
    ths->volt4 = ReadF32(buff + offset);
    offset += (sizeof(ths->volt4));
    ths->delay4 = ReadU16(buff + offset);
    offset += (sizeof(ths->delay4));
    return offset;
}
int NFL_WriteConfig_Return_GetFromBytes(NFL_WriteConfig_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_ReadDataStatus_Send_GetFromBytes(NFL_ReadDataStatus_Send *ths,u8* buff,int offset ) 
{
    ths->wait_time_ms = ReadU32(buff + offset);
    offset += (sizeof(ths->wait_time_ms));
    return offset;
}
int NFL_ReadDataStatus_Return_GetFromBytes(NFL_ReadDataStatus_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    ths->data_status = ReadU32(buff + offset);
    offset += (sizeof(ths->data_status));
    return offset;
}
int NFL_ReadData_Send_GetFromBytes(NFL_ReadData_Send *ths,u8* buff,int offset ) 
{
    ths->channel = ReadU32(buff + offset);
    offset += (sizeof(ths->channel));
    return offset;
}
int NFL_ReadData_Return_GetFromBytes(NFL_ReadData_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    ths->data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->data_Length));
    ths->data = (u8*)(buff + offset );
    offset += (ths->data_Length*sizeof(ths->data[0])  );
    return offset;
}
int NFL_DataCollectStart_Send_GetFromBytes(NFL_DataCollectStart_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_DataCollectStart_Return_GetFromBytes(NFL_DataCollectStart_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_DataCollectStop_Send_GetFromBytes(NFL_DataCollectStop_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_DataCollectStop_Return_GetFromBytes(NFL_DataCollectStop_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_ThresholdConfig_Send_GetFromBytes(NFL_ThresholdConfig_Send *ths,u8* buff,int offset ) 
{
    ths->threshold = ReadU16(buff + offset);
    offset += (sizeof(ths->threshold));
    return offset;
}
int NFL_ThresholdConfig_Return_GetFromBytes(NFL_ThresholdConfig_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_StandbyConfig_Send_GetFromBytes(NFL_StandbyConfig_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_StandbyConfig_Return_GetFromBytes(NFL_StandbyConfig_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_ActivateConfig_Send_GetFromBytes(NFL_ActivateConfig_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_ActivateConfig_Return_GetFromBytes(NFL_ActivateConfig_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int NFL_ReadTemperature_Send_GetFromBytes(NFL_ReadTemperature_Send *ths,u8* buff,int offset ) 
{
    ths->data = ReadU32(buff + offset);
    offset += (sizeof(ths->data));
    return offset;
}
int NFL_ReadTemperature_Return_GetFromBytes(NFL_ReadTemperature_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    ths->temperature = ReadF32(buff + offset);
    offset += (sizeof(ths->temperature));
    return offset;
}
int NFL_SendEvent_Send_GetFromBytes(NFL_SendEvent_Send *ths,u8* buff,int offset ) 
{
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    ths->length = ReadU32(buff + offset);
    offset += (sizeof(ths->length));
    ths->data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->data_Length));
    ths->data = (u8*)(buff + offset );
    offset += (ths->data_Length*sizeof(ths->data[0])  );
    return offset;
}
int NFL_SendEvent_Return_GetFromBytes(NFL_SendEvent_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int PowerCtrl_Send_GetFromBytes(PowerCtrl_Send *ths,u8* buff,int offset ) 
{
    ths->PcPower = ReadU16(buff + offset);
    offset += (sizeof(ths->PcPower));
    ths->MotorPower = ReadU16(buff + offset);
    offset += (sizeof(ths->MotorPower));
    ths->TemperaturePower = ReadU16(buff + offset);
    offset += (sizeof(ths->TemperaturePower));
    ths->DataCollectionPower = ReadU16(buff + offset);
    offset += (sizeof(ths->DataCollectionPower));
    return offset;
}
int PowerCtrl_Return_GetFromBytes(PowerCtrl_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int PressureListen_Send_GetFromBytes(PressureListen_Send *ths,u8* buff,int offset ) 
{
    ths->m1_id_flag = buff[offset];
    offset += (sizeof(ths->m1_id_flag));
    ths->m2_diff_threthold = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_diff_threthold));
    ths->m3_listen_time_span = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_listen_time_span));
    ths->m4_error_range = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_error_range));
    ths->m5_listen_time = ReadU16(buff + offset);
    offset += (sizeof(ths->m5_listen_time));
    return offset;
}
int PressureListen_Return_GetFromBytes(PressureListen_Return *ths,u8* buff,int offset ) 
{
    ths->m1_error_code = buff[offset];
    offset += (sizeof(ths->m1_error_code));
    ths->m2_continue_time = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_continue_time));
    ths->m3_end_pressure = ReadU16(buff + offset);
    offset += (sizeof(ths->m3_end_pressure));
    ths->m4_pressure_data_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_pressure_data_Length));
    ths->m4_pressure_data = (u16*)(buff + offset );
    offset += (ths->m4_pressure_data_Length*sizeof(ths->m4_pressure_data[0])  );
    return offset;
}
int SendErrorCode_Send_GetFromBytes(SendErrorCode_Send *ths,u8* buff,int offset ) 
{
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
int SendErrorCode_Return_GetFromBytes(SendErrorCode_Return *ths,u8* buff,int offset ) 
{
    ths->cmd = ReadU32(buff + offset);
    offset += (sizeof(ths->cmd));
    ths->ErrorCode = ReadU32(buff + offset);
    offset += (sizeof(ths->ErrorCode));
    return offset;
}
PressureListen_Send* New_PressureListen_Send(void)
{
    PressureListen_Send*rt = malloc( sizeof( PressureListen_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureListen_Send) ) ;
    rt->ToBytes = &PressureListen_Send_ToBytes;
    rt->GetFromBytes = &PressureListen_Send_GetFromBytes;
    return rt;
}
PressureListen_Return* New_PressureListen_Return(void)
{
    PressureListen_Return*rt = malloc( sizeof( PressureListen_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureListen_Return) ) ;
    rt->ToBytes = &PressureListen_Return_ToBytes;
    rt->GetFromBytes = &PressureListen_Return_GetFromBytes;
    return rt;
}
int IOCtrlByEvent_Send_m1_PortArgs_GetFromBytes(IOCtrlByEvent_Send_m1_PortArgs *ths,u8* buff,int offset ) 
{
    ths->m1_state = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_state));
    ths->m2_delay = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_delay));
    return offset;
}
int IOCtrlByEvent_Send_m2_PortArgs_GetFromBytes(IOCtrlByEvent_Send_m2_PortArgs *ths,u8* buff,int offset ) 
{
    ths->m1_state = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_state));
    ths->m2_delay = ReadU16(buff + offset);
    offset += (sizeof(ths->m2_delay));
    return offset;
}
int IOCtrlByEvent_Send_GetFromBytes(IOCtrlByEvent_Send *ths,u8* buff,int offset ) 
{
    ths->m1_PortArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m1_PortArgs_Length));
    ths->m1_PortArgs = malloc(sizeof(ths->m1_PortArgs[0])*(ths->m1_PortArgs_Length));
    PTR_ASSERT(ths->m1_PortArgs);
    for(int i =0;i<ths->m1_PortArgs_Length;i++){
        offset = IOCtrlByEvent_Send_m1_PortArgs_GetFromBytes( &(ths->m1_PortArgs[i]) ,buff,offset );
    }
    ths->m2_PortArgs_Length = ReadU32(buff + offset);
    offset += (sizeof(ths->m2_PortArgs_Length));
    ths->m2_PortArgs = malloc(sizeof(ths->m2_PortArgs[0])*(ths->m2_PortArgs_Length));
    PTR_ASSERT(ths->m2_PortArgs);
    for(int i =0;i<ths->m2_PortArgs_Length;i++){
        offset = IOCtrlByEvent_Send_m2_PortArgs_GetFromBytes( &(ths->m2_PortArgs[i]) ,buff,offset );
    }
    ths->m3_id_event_threshold = ReadU32(buff + offset);
    offset += (sizeof(ths->m3_id_event_threshold));
    ths->m4_timeout = ReadU32(buff + offset);
    offset += (sizeof(ths->m4_timeout));
    return offset;
}
int IOCtrlByEvent_Return_GetFromBytes(IOCtrlByEvent_Return *ths,u8* buff,int offset ) 
{
    ths->m1_error_code = buff[offset];
    offset += (sizeof(ths->m1_error_code));
    return offset;
}
Dsp_DHCP_Send* New_Dsp_DHCP_Send(void)
{
    Dsp_DHCP_Send*rt = malloc( sizeof( Dsp_DHCP_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Dsp_DHCP_Send) ) ;
    rt->ToBytes = &Dsp_DHCP_Send_ToBytes;
    rt->GetFromBytes = &Dsp_DHCP_Send_GetFromBytes;
    return rt;
}
Dsp_DHCP_Return* New_Dsp_DHCP_Return(void)
{
    Dsp_DHCP_Return*rt = malloc( sizeof( Dsp_DHCP_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Dsp_DHCP_Return) ) ;
    rt->ToBytes = &Dsp_DHCP_Return_ToBytes;
    rt->GetFromBytes = &Dsp_DHCP_Return_GetFromBytes;
    return rt;
}
Ping_Send* New_Ping_Send(void)
{
    Ping_Send*rt = malloc( sizeof( Ping_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Ping_Send) ) ;
    rt->ToBytes = &Ping_Send_ToBytes;
    rt->GetFromBytes = &Ping_Send_GetFromBytes;
    return rt;
}
Ping_Return* New_Ping_Return(void)
{
    Ping_Return*rt = malloc( sizeof( Ping_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Ping_Return) ) ;
    rt->ToBytes = &Ping_Return_ToBytes;
    rt->GetFromBytes = &Ping_Return_GetFromBytes;
    return rt;
}
FilePush_Send* New_FilePush_Send(void)
{
    FilePush_Send*rt = malloc( sizeof( FilePush_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FilePush_Send) ) ;
    rt->ToBytes = &FilePush_Send_ToBytes;
    rt->GetFromBytes = &FilePush_Send_GetFromBytes;
    return rt;
}
FilePush_Return* New_FilePush_Return(void)
{
    FilePush_Return*rt = malloc( sizeof( FilePush_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FilePush_Return) ) ;
    rt->ToBytes = &FilePush_Return_ToBytes;
    rt->GetFromBytes = &FilePush_Return_GetFromBytes;
    return rt;
}
InstrumentStateReport_Send* New_InstrumentStateReport_Send(void)
{
    InstrumentStateReport_Send*rt = malloc( sizeof( InstrumentStateReport_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( InstrumentStateReport_Send) ) ;
    rt->ToBytes = &InstrumentStateReport_Send_ToBytes;
    rt->GetFromBytes = &InstrumentStateReport_Send_GetFromBytes;
    return rt;
}
InstrumentStateReport_Return* New_InstrumentStateReport_Return(void)
{
    InstrumentStateReport_Return*rt = malloc( sizeof( InstrumentStateReport_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( InstrumentStateReport_Return) ) ;
    rt->ToBytes = &InstrumentStateReport_Return_ToBytes;
    rt->GetFromBytes = &InstrumentStateReport_Return_GetFromBytes;
    return rt;
}
ProtoTest_Send* New_ProtoTest_Send(void)
{
    ProtoTest_Send*rt = malloc( sizeof( ProtoTest_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ProtoTest_Send) ) ;
    rt->ToBytes = &ProtoTest_Send_ToBytes;
    rt->GetFromBytes = &ProtoTest_Send_GetFromBytes;
    return rt;
}
ProtoTest_Return* New_ProtoTest_Return(void)
{
    ProtoTest_Return*rt = malloc( sizeof( ProtoTest_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ProtoTest_Return) ) ;
    rt->ToBytes = &ProtoTest_Return_ToBytes;
    rt->GetFromBytes = &ProtoTest_Return_GetFromBytes;
    return rt;
}
McuErrorToPC_Send* New_McuErrorToPC_Send(void)
{
    McuErrorToPC_Send*rt = malloc( sizeof( McuErrorToPC_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuErrorToPC_Send) ) ;
    rt->ToBytes = &McuErrorToPC_Send_ToBytes;
    rt->GetFromBytes = &McuErrorToPC_Send_GetFromBytes;
    return rt;
}
McuErrorToPC_Return* New_McuErrorToPC_Return(void)
{
    McuErrorToPC_Return*rt = malloc( sizeof( McuErrorToPC_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuErrorToPC_Return) ) ;
    rt->ToBytes = &McuErrorToPC_Return_ToBytes;
    rt->GetFromBytes = &McuErrorToPC_Return_GetFromBytes;
    return rt;
}
AutoAdapter_Send* New_AutoAdapter_Send(void)
{
    AutoAdapter_Send*rt = malloc( sizeof( AutoAdapter_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AutoAdapter_Send) ) ;
    rt->ToBytes = &AutoAdapter_Send_ToBytes;
    rt->GetFromBytes = &AutoAdapter_Send_GetFromBytes;
    return rt;
}
AutoAdapter_Return* New_AutoAdapter_Return(void)
{
    AutoAdapter_Return*rt = malloc( sizeof( AutoAdapter_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AutoAdapter_Return) ) ;
    rt->ToBytes = &AutoAdapter_Return_ToBytes;
    rt->GetFromBytes = &AutoAdapter_Return_GetFromBytes;
    return rt;
}
MotorMultiMove_Send_m1_MoveArgs* New_MotorMultiMove_Send_m1_MoveArgs(void)
{
    MotorMultiMove_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMultiMove_Send_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Send_m1_MoveArgs) ) ;
    rt->ToBytes = &MotorMultiMove_Send_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &MotorMultiMove_Send_m1_MoveArgs_GetFromBytes;
    return rt;
}
MotorMultiMove_Send* New_MotorMultiMove_Send(void)
{
    MotorMultiMove_Send*rt = malloc( sizeof( MotorMultiMove_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Send) ) ;
    rt->ToBytes = &MotorMultiMove_Send_ToBytes;
    rt->GetFromBytes = &MotorMultiMove_Send_GetFromBytes;
    return rt;
}
MotorMultiMove_Return* New_MotorMultiMove_Return(void)
{
    MotorMultiMove_Return*rt = malloc( sizeof( MotorMultiMove_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Return) ) ;
    rt->ToBytes = &MotorMultiMove_Return_ToBytes;
    rt->GetFromBytes = &MotorMultiMove_Return_GetFromBytes;
    return rt;
}
MotorMove_Send* New_MotorMove_Send(void)
{
    MotorMove_Send*rt = malloc( sizeof( MotorMove_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMove_Send) ) ;
    rt->ToBytes = &MotorMove_Send_ToBytes;
    rt->GetFromBytes = &MotorMove_Send_GetFromBytes;
    return rt;
}
MotorMove_Return* New_MotorMove_Return(void)
{
    MotorMove_Return*rt = malloc( sizeof( MotorMove_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMove_Return) ) ;
    rt->ToBytes = &MotorMove_Return_ToBytes;
    rt->GetFromBytes = &MotorMove_Return_GetFromBytes;
    return rt;
}
MotorStatetGet_Send* New_MotorStatetGet_Send(void)
{
    MotorStatetGet_Send*rt = malloc( sizeof( MotorStatetGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGet_Send) ) ;
    rt->ToBytes = &MotorStatetGet_Send_ToBytes;
    rt->GetFromBytes = &MotorStatetGet_Send_GetFromBytes;
    return rt;
}
MotorStatetGet_Return* New_MotorStatetGet_Return(void)
{
    MotorStatetGet_Return*rt = malloc( sizeof( MotorStatetGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGet_Return) ) ;
    rt->ToBytes = &MotorStatetGet_Return_ToBytes;
    rt->GetFromBytes = &MotorStatetGet_Return_GetFromBytes;
    return rt;
}
MotorRotateStart_Send* New_MotorRotateStart_Send(void)
{
    MotorRotateStart_Send*rt = malloc( sizeof( MotorRotateStart_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStart_Send) ) ;
    rt->ToBytes = &MotorRotateStart_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateStart_Send_GetFromBytes;
    return rt;
}
MotorRotateStart_Return* New_MotorRotateStart_Return(void)
{
    MotorRotateStart_Return*rt = malloc( sizeof( MotorRotateStart_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStart_Return) ) ;
    rt->ToBytes = &MotorRotateStart_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateStart_Return_GetFromBytes;
    return rt;
}
MotorRotateStop_Send* New_MotorRotateStop_Send(void)
{
    MotorRotateStop_Send*rt = malloc( sizeof( MotorRotateStop_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStop_Send) ) ;
    rt->ToBytes = &MotorRotateStop_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateStop_Send_GetFromBytes;
    return rt;
}
MotorRotateStop_Return* New_MotorRotateStop_Return(void)
{
    MotorRotateStop_Return*rt = malloc( sizeof( MotorRotateStop_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStop_Return) ) ;
    rt->ToBytes = &MotorRotateStop_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateStop_Return_GetFromBytes;
    return rt;
}
MotorRotate_Send* New_MotorRotate_Send(void)
{
    MotorRotate_Send*rt = malloc( sizeof( MotorRotate_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotate_Send) ) ;
    rt->ToBytes = &MotorRotate_Send_ToBytes;
    rt->GetFromBytes = &MotorRotate_Send_GetFromBytes;
    return rt;
}
MotorRotate_Return* New_MotorRotate_Return(void)
{
    MotorRotate_Return*rt = malloc( sizeof( MotorRotate_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotate_Return) ) ;
    rt->ToBytes = &MotorRotate_Return_ToBytes;
    rt->GetFromBytes = &MotorRotate_Return_GetFromBytes;
    return rt;
}
MotorReset_Send* New_MotorReset_Send(void)
{
    MotorReset_Send*rt = malloc( sizeof( MotorReset_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorReset_Send) ) ;
    rt->ToBytes = &MotorReset_Send_ToBytes;
    rt->GetFromBytes = &MotorReset_Send_GetFromBytes;
    return rt;
}
MotorReset_Return* New_MotorReset_Return(void)
{
    MotorReset_Return*rt = malloc( sizeof( MotorReset_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorReset_Return) ) ;
    rt->ToBytes = &MotorReset_Return_ToBytes;
    rt->GetFromBytes = &MotorReset_Return_GetFromBytes;
    return rt;
}
SensorValueOperation_Send* New_SensorValueOperation_Send(void)
{
    SensorValueOperation_Send*rt = malloc( sizeof( SensorValueOperation_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SensorValueOperation_Send) ) ;
    rt->ToBytes = &SensorValueOperation_Send_ToBytes;
    rt->GetFromBytes = &SensorValueOperation_Send_GetFromBytes;
    return rt;
}
SensorValueOperation_Return* New_SensorValueOperation_Return(void)
{
    SensorValueOperation_Return*rt = malloc( sizeof( SensorValueOperation_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SensorValueOperation_Return) ) ;
    rt->ToBytes = &SensorValueOperation_Return_ToBytes;
    rt->GetFromBytes = &SensorValueOperation_Return_GetFromBytes;
    return rt;
}
ADAutoCaliStart_Send* New_ADAutoCaliStart_Send(void)
{
    ADAutoCaliStart_Send*rt = malloc( sizeof( ADAutoCaliStart_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADAutoCaliStart_Send) ) ;
    rt->ToBytes = &ADAutoCaliStart_Send_ToBytes;
    rt->GetFromBytes = &ADAutoCaliStart_Send_GetFromBytes;
    return rt;
}
ADAutoCaliStart_Return* New_ADAutoCaliStart_Return(void)
{
    ADAutoCaliStart_Return*rt = malloc( sizeof( ADAutoCaliStart_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADAutoCaliStart_Return) ) ;
    rt->ToBytes = &ADAutoCaliStart_Return_ToBytes;
    rt->GetFromBytes = &ADAutoCaliStart_Return_GetFromBytes;
    return rt;
}
TemperatureGet_Send* New_TemperatureGet_Send(void)
{
    TemperatureGet_Send*rt = malloc( sizeof( TemperatureGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureGet_Send) ) ;
    rt->ToBytes = &TemperatureGet_Send_ToBytes;
    rt->GetFromBytes = &TemperatureGet_Send_GetFromBytes;
    return rt;
}
TemperatureGet_Return* New_TemperatureGet_Return(void)
{
    TemperatureGet_Return*rt = malloc( sizeof( TemperatureGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureGet_Return) ) ;
    rt->ToBytes = &TemperatureGet_Return_ToBytes;
    rt->GetFromBytes = &TemperatureGet_Return_GetFromBytes;
    return rt;
}
TemperatureOpen_Send* New_TemperatureOpen_Send(void)
{
    TemperatureOpen_Send*rt = malloc( sizeof( TemperatureOpen_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureOpen_Send) ) ;
    rt->ToBytes = &TemperatureOpen_Send_ToBytes;
    rt->GetFromBytes = &TemperatureOpen_Send_GetFromBytes;
    return rt;
}
TemperatureOpen_Return* New_TemperatureOpen_Return(void)
{
    TemperatureOpen_Return*rt = malloc( sizeof( TemperatureOpen_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureOpen_Return) ) ;
    rt->ToBytes = &TemperatureOpen_Return_ToBytes;
    rt->GetFromBytes = &TemperatureOpen_Return_GetFromBytes;
    return rt;
}
TemperatureClose_Send* New_TemperatureClose_Send(void)
{
    TemperatureClose_Send*rt = malloc( sizeof( TemperatureClose_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureClose_Send) ) ;
    rt->ToBytes = &TemperatureClose_Send_ToBytes;
    rt->GetFromBytes = &TemperatureClose_Send_GetFromBytes;
    return rt;
}
TemperatureClose_Return* New_TemperatureClose_Return(void)
{
    TemperatureClose_Return*rt = malloc( sizeof( TemperatureClose_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureClose_Return) ) ;
    rt->ToBytes = &TemperatureClose_Return_ToBytes;
    rt->GetFromBytes = &TemperatureClose_Return_GetFromBytes;
    return rt;
}
BarCodeGet_Send* New_BarCodeGet_Send(void)
{
    BarCodeGet_Send*rt = malloc( sizeof( BarCodeGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( BarCodeGet_Send) ) ;
    rt->ToBytes = &BarCodeGet_Send_ToBytes;
    rt->GetFromBytes = &BarCodeGet_Send_GetFromBytes;
    return rt;
}
BarCodeGet_Return* New_BarCodeGet_Return(void)
{
    BarCodeGet_Return*rt = malloc( sizeof( BarCodeGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( BarCodeGet_Return) ) ;
    rt->ToBytes = &BarCodeGet_Return_ToBytes;
    rt->GetFromBytes = &BarCodeGet_Return_GetFromBytes;
    return rt;
}
FirmWareConfig_Send* New_FirmWareConfig_Send(void)
{
    FirmWareConfig_Send*rt = malloc( sizeof( FirmWareConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareConfig_Send) ) ;
    rt->ToBytes = &FirmWareConfig_Send_ToBytes;
    rt->GetFromBytes = &FirmWareConfig_Send_GetFromBytes;
    return rt;
}
FirmWareConfig_Return* New_FirmWareConfig_Return(void)
{
    FirmWareConfig_Return*rt = malloc( sizeof( FirmWareConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareConfig_Return) ) ;
    rt->ToBytes = &FirmWareConfig_Return_ToBytes;
    rt->GetFromBytes = &FirmWareConfig_Return_GetFromBytes;
    return rt;
}
FirmWareTransmitFile_Send* New_FirmWareTransmitFile_Send(void)
{
    FirmWareTransmitFile_Send*rt = malloc( sizeof( FirmWareTransmitFile_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareTransmitFile_Send) ) ;
    rt->ToBytes = &FirmWareTransmitFile_Send_ToBytes;
    rt->GetFromBytes = &FirmWareTransmitFile_Send_GetFromBytes;
    return rt;
}
FirmWareTransmitFile_Return* New_FirmWareTransmitFile_Return(void)
{
    FirmWareTransmitFile_Return*rt = malloc( sizeof( FirmWareTransmitFile_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareTransmitFile_Return) ) ;
    rt->ToBytes = &FirmWareTransmitFile_Return_ToBytes;
    rt->GetFromBytes = &FirmWareTransmitFile_Return_GetFromBytes;
    return rt;
}
McuReset_Send* New_McuReset_Send(void)
{
    McuReset_Send*rt = malloc( sizeof( McuReset_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuReset_Send) ) ;
    rt->ToBytes = &McuReset_Send_ToBytes;
    rt->GetFromBytes = &McuReset_Send_GetFromBytes;
    return rt;
}
McuReset_Return* New_McuReset_Return(void)
{
    McuReset_Return*rt = malloc( sizeof( McuReset_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuReset_Return) ) ;
    rt->ToBytes = &McuReset_Return_ToBytes;
    rt->GetFromBytes = &McuReset_Return_GetFromBytes;
    return rt;
}
FirmWareVersionGet_Send* New_FirmWareVersionGet_Send(void)
{
    FirmWareVersionGet_Send*rt = malloc( sizeof( FirmWareVersionGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareVersionGet_Send) ) ;
    rt->ToBytes = &FirmWareVersionGet_Send_ToBytes;
    rt->GetFromBytes = &FirmWareVersionGet_Send_GetFromBytes;
    return rt;
}
FirmWareVersionGet_Return* New_FirmWareVersionGet_Return(void)
{
    FirmWareVersionGet_Return*rt = malloc( sizeof( FirmWareVersionGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareVersionGet_Return) ) ;
    rt->ToBytes = &FirmWareVersionGet_Return_ToBytes;
    rt->GetFromBytes = &FirmWareVersionGet_Return_GetFromBytes;
    return rt;
}
MotorConfigSet_Send* New_MotorConfigSet_Send(void)
{
    MotorConfigSet_Send*rt = malloc( sizeof( MotorConfigSet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSet_Send) ) ;
    rt->ToBytes = &MotorConfigSet_Send_ToBytes;
    rt->GetFromBytes = &MotorConfigSet_Send_GetFromBytes;
    return rt;
}
MotorConfigSet_Return* New_MotorConfigSet_Return(void)
{
    MotorConfigSet_Return*rt = malloc( sizeof( MotorConfigSet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSet_Return) ) ;
    rt->ToBytes = &MotorConfigSet_Return_ToBytes;
    rt->GetFromBytes = &MotorConfigSet_Return_GetFromBytes;
    return rt;
}
MotorMoveEvent_Send* New_MotorMoveEvent_Send(void)
{
    MotorMoveEvent_Send*rt = malloc( sizeof( MotorMoveEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveEvent_Send) ) ;
    rt->ToBytes = &MotorMoveEvent_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveEvent_Send_GetFromBytes;
    return rt;
}
MotorMoveEvent_Return* New_MotorMoveEvent_Return(void)
{
    MotorMoveEvent_Return*rt = malloc( sizeof( MotorMoveEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveEvent_Return) ) ;
    rt->ToBytes = &MotorMoveEvent_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveEvent_Return_GetFromBytes;
    return rt;
}
McuIoStateChangeEvent_Send* New_McuIoStateChangeEvent_Send(void)
{
    McuIoStateChangeEvent_Send*rt = malloc( sizeof( McuIoStateChangeEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuIoStateChangeEvent_Send) ) ;
    rt->ToBytes = &McuIoStateChangeEvent_Send_ToBytes;
    rt->GetFromBytes = &McuIoStateChangeEvent_Send_GetFromBytes;
    return rt;
}
McuIoStateChangeEvent_Return* New_McuIoStateChangeEvent_Return(void)
{
    McuIoStateChangeEvent_Return*rt = malloc( sizeof( McuIoStateChangeEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuIoStateChangeEvent_Return) ) ;
    rt->ToBytes = &McuIoStateChangeEvent_Return_ToBytes;
    rt->GetFromBytes = &McuIoStateChangeEvent_Return_GetFromBytes;
    return rt;
}
SampleUnitSensorGet_Send* New_SampleUnitSensorGet_Send(void)
{
    SampleUnitSensorGet_Send*rt = malloc( sizeof( SampleUnitSensorGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SampleUnitSensorGet_Send) ) ;
    rt->ToBytes = &SampleUnitSensorGet_Send_ToBytes;
    rt->GetFromBytes = &SampleUnitSensorGet_Send_GetFromBytes;
    return rt;
}
SampleUnitSensorGet_Return* New_SampleUnitSensorGet_Return(void)
{
    SampleUnitSensorGet_Return*rt = malloc( sizeof( SampleUnitSensorGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SampleUnitSensorGet_Return) ) ;
    rt->ToBytes = &SampleUnitSensorGet_Return_ToBytes;
    rt->GetFromBytes = &SampleUnitSensorGet_Return_GetFromBytes;
    return rt;
}
AirPumpOpen_Send* New_AirPumpOpen_Send(void)
{
    AirPumpOpen_Send*rt = malloc( sizeof( AirPumpOpen_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpOpen_Send) ) ;
    rt->ToBytes = &AirPumpOpen_Send_ToBytes;
    rt->GetFromBytes = &AirPumpOpen_Send_GetFromBytes;
    return rt;
}
AirPumpOpen_Return* New_AirPumpOpen_Return(void)
{
    AirPumpOpen_Return*rt = malloc( sizeof( AirPumpOpen_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpOpen_Return) ) ;
    rt->ToBytes = &AirPumpOpen_Return_ToBytes;
    rt->GetFromBytes = &AirPumpOpen_Return_GetFromBytes;
    return rt;
}
AirPumpClose_Send* New_AirPumpClose_Send(void)
{
    AirPumpClose_Send*rt = malloc( sizeof( AirPumpClose_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpClose_Send) ) ;
    rt->ToBytes = &AirPumpClose_Send_ToBytes;
    rt->GetFromBytes = &AirPumpClose_Send_GetFromBytes;
    return rt;
}
AirPumpClose_Return* New_AirPumpClose_Return(void)
{
    AirPumpClose_Return*rt = malloc( sizeof( AirPumpClose_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpClose_Return) ) ;
    rt->ToBytes = &AirPumpClose_Return_ToBytes;
    rt->GetFromBytes = &AirPumpClose_Return_GetFromBytes;
    return rt;
}


ADSampleRead_Send* New_ADSampleRead_Send(void)
{
    ADSampleRead_Send*rt = malloc( sizeof( ADSampleRead_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADSampleRead_Send) ) ;
    rt->ToBytes = &ADSampleRead_Send_ToBytes;
    rt->GetFromBytes = &ADSampleRead_Send_GetFromBytes;
    return rt;
}
ADSampleRead_Return* New_ADSampleRead_Return(void)
{
    ADSampleRead_Return*rt = malloc( sizeof( ADSampleRead_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADSampleRead_Return) ) ;
    rt->ToBytes = &ADSampleRead_Return_ToBytes;
    rt->GetFromBytes = &ADSampleRead_Return_GetFromBytes;
    return rt;
}

/*
这里实现 数据接口 的构造函数
*/

DiskInstalledRed_Send* New_DiskInstalledRed_Send(void)
{
	  DiskInstalledRed_Send*rt = malloc( sizeof( DiskInstalledRed_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DiskInstalledRed_Send) ) ;
    rt->ToBytes = &DiskInstalledRed_Send_ToBytes;
    rt->GetFromBytes = &DiskInstalledRed_Send_GetFromBytes;
    return rt;
}
DiskInstalledRed_Return* New_DiskInstalledRed_Return(void)
{
	  DiskInstalledRed_Return*rt = malloc( sizeof( DiskInstalledRed_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DiskInstalledRed_Return) ) ;
    rt->ToBytes = &DiskInstalledRed_Return_ToBytes;
    rt->GetFromBytes = &DiskInstalledRed_Return_GetFromBytes;
    return rt;
}

MotorMoveGPIOEvent_Send* New_MotorMoveGPIOEvent_Send(void)
{
    MotorMoveGPIOEvent_Send*rt = malloc( sizeof( MotorMoveGPIOEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEvent_Send) ) ;
    rt->ToBytes = &MotorMoveGPIOEvent_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveGPIOEvent_Send_GetFromBytes;
    return rt;
}
MotorMoveGPIOEvent_Return* New_MotorMoveGPIOEvent_Return(void)
{
    MotorMoveGPIOEvent_Return*rt = malloc( sizeof( MotorMoveGPIOEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEvent_Return) ) ;
    rt->ToBytes = &MotorMoveGPIOEvent_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveGPIOEvent_Return_GetFromBytes;
    return rt;
}
MotorConfigSetNew_Send* New_MotorConfigSetNew_Send(void)
{
    MotorConfigSetNew_Send*rt = malloc( sizeof( MotorConfigSetNew_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetNew_Send) ) ;
    rt->ToBytes = &MotorConfigSetNew_Send_ToBytes;
    rt->GetFromBytes = &MotorConfigSetNew_Send_GetFromBytes;
    return rt;
}
MotorConfigSetNew_Return* New_MotorConfigSetNew_Return(void)
{
    MotorConfigSetNew_Return*rt = malloc( sizeof( MotorConfigSetNew_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetNew_Return) ) ;
    rt->ToBytes = &MotorConfigSetNew_Return_ToBytes;
    rt->GetFromBytes = &MotorConfigSetNew_Return_GetFromBytes;
    return rt;
}
ExecutesCtrl_Send* New_ExecutesCtrl_Send(void)
{
    ExecutesCtrl_Send*rt = malloc( sizeof( ExecutesCtrl_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ExecutesCtrl_Send) ) ;
    rt->ToBytes = &ExecutesCtrl_Send_ToBytes;
    rt->GetFromBytes = &ExecutesCtrl_Send_GetFromBytes;
    return rt;
}
ExecutesCtrl_Return* New_ExecutesCtrl_Return(void)
{
    ExecutesCtrl_Return*rt = malloc( sizeof( ExecutesCtrl_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ExecutesCtrl_Return) ) ;
    rt->ToBytes = &ExecutesCtrl_Return_ToBytes;
    rt->GetFromBytes = &ExecutesCtrl_Return_GetFromBytes;
    return rt;
}
ADDetectConfigSet_Send* New_ADDetectConfigSet_Send(void)
{
    ADDetectConfigSet_Send*rt = malloc( sizeof( ADDetectConfigSet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDetectConfigSet_Send) ) ;
    rt->ToBytes = &ADDetectConfigSet_Send_ToBytes;
    rt->GetFromBytes = &ADDetectConfigSet_Send_GetFromBytes;
    return rt;
}
ADDetectConfigSet_Return* New_ADDetectConfigSet_Return(void)
{
    ADDetectConfigSet_Return*rt = malloc( sizeof( ADDetectConfigSet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDetectConfigSet_Return) ) ;
    rt->ToBytes = &ADDetectConfigSet_Return_ToBytes;
    rt->GetFromBytes = &ADDetectConfigSet_Return_GetFromBytes;
    return rt;
}
ADDataRead_Send* New_ADDataRead_Send(void)
{
    ADDataRead_Send*rt = malloc( sizeof( ADDataRead_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataRead_Send) ) ;
    rt->ToBytes = &ADDataRead_Send_ToBytes;
    rt->GetFromBytes = &ADDataRead_Send_GetFromBytes;
    return rt;
}
ADDataRead_Return* New_ADDataRead_Return(void)
{
    ADDataRead_Return*rt = malloc( sizeof( ADDataRead_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataRead_Return) ) ;
    rt->ToBytes = &ADDataRead_Return_ToBytes;
    rt->GetFromBytes = &ADDataRead_Return_GetFromBytes;
    return rt;
}
ADDataSeriesSend_Send* New_ADDataSeriesSend_Send(void)
{
    ADDataSeriesSend_Send*rt = malloc( sizeof( ADDataSeriesSend_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataSeriesSend_Send) ) ;
    rt->ToBytes = &ADDataSeriesSend_Send_ToBytes;
    rt->GetFromBytes = &ADDataSeriesSend_Send_GetFromBytes;
    return rt;
}
ADDataSeriesSend_Return* New_ADDataSeriesSend_Return(void)
{
    ADDataSeriesSend_Return*rt = malloc( sizeof( ADDataSeriesSend_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataSeriesSend_Return) ) ;
    rt->ToBytes = &ADDataSeriesSend_Return_ToBytes;
    rt->GetFromBytes = &ADDataSeriesSend_Return_GetFromBytes;
    return rt;
}
UartTransmit_Send* New_UartTransmit_Send(void)
{
    UartTransmit_Send*rt = malloc( sizeof( UartTransmit_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartTransmit_Send) ) ;
    rt->ToBytes = &UartTransmit_Send_ToBytes;
    rt->GetFromBytes = &UartTransmit_Send_GetFromBytes;
    return rt;
}
UartTransmit_Return* New_UartTransmit_Return(void)
{
    UartTransmit_Return*rt = malloc( sizeof( UartTransmit_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartTransmit_Return) ) ;
    rt->ToBytes = &UartTransmit_Return_ToBytes;
    rt->GetFromBytes = &UartTransmit_Return_GetFromBytes;
    return rt;
}
UartReceiveEvent_Send* New_UartReceiveEvent_Send(void)
{
    UartReceiveEvent_Send*rt = malloc( sizeof( UartReceiveEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartReceiveEvent_Send) ) ;
    rt->ToBytes = &UartReceiveEvent_Send_ToBytes;
    rt->GetFromBytes = &UartReceiveEvent_Send_GetFromBytes;
    return rt;
}
UartReceiveEvent_Return* New_UartReceiveEvent_Return(void)
{
    UartReceiveEvent_Return*rt = malloc( sizeof( UartReceiveEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartReceiveEvent_Return) ) ;
    rt->ToBytes = &UartReceiveEvent_Return_ToBytes;
    rt->GetFromBytes = &UartReceiveEvent_Return_GetFromBytes;
    return rt;
}
SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* New_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs(void)
{
    SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs*rt = malloc( sizeof( SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs) ) ;
    rt->ToBytes = &SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_GetFromBytes;
    return rt;
}
SeqMotorMultiMove_Send_m1_Sequence* New_SeqMotorMultiMove_Send_m1_Sequence(void)
{
    SeqMotorMultiMove_Send_m1_Sequence*rt = malloc( sizeof( SeqMotorMultiMove_Send_m1_Sequence) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send_m1_Sequence) ) ;
    rt->ToBytes = &SeqMotorMultiMove_Send_m1_Sequence_ToBytes;
    rt->GetFromBytes = &SeqMotorMultiMove_Send_m1_Sequence_GetFromBytes;
    return rt;
}
SeqMotorMultiMove_Send* New_SeqMotorMultiMove_Send(void)
{
    SeqMotorMultiMove_Send*rt = malloc( sizeof( SeqMotorMultiMove_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send) ) ;
    rt->ToBytes = &SeqMotorMultiMove_Send_ToBytes;
    rt->GetFromBytes = &SeqMotorMultiMove_Send_GetFromBytes;
    return rt;
}
SeqMotorMultiMove_Return* New_SeqMotorMultiMove_Return(void)
{
    SeqMotorMultiMove_Return*rt = malloc( sizeof( SeqMotorMultiMove_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Return) ) ;
    rt->ToBytes = &SeqMotorMultiMove_Return_ToBytes;
    rt->GetFromBytes = &SeqMotorMultiMove_Return_GetFromBytes;
    return rt;
}
PressureConfig_Send* New_PressureConfig_Send(void)
{
    PressureConfig_Send*rt = malloc( sizeof( PressureConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureConfig_Send) ) ;
    rt->ToBytes = &PressureConfig_Send_ToBytes;
    rt->GetFromBytes = &PressureConfig_Send_GetFromBytes;
    return rt;
}
PressureConfig_Return* New_PressureConfig_Return(void)
{
    PressureConfig_Return*rt = malloc( sizeof( PressureConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureConfig_Return) ) ;
    rt->ToBytes = &PressureConfig_Return_ToBytes;
    rt->GetFromBytes = &PressureConfig_Return_GetFromBytes;
    return rt;
}
PressureDataGet_Send* New_PressureDataGet_Send(void)
{
    PressureDataGet_Send*rt = malloc( sizeof( PressureDataGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureDataGet_Send) ) ;
    rt->ToBytes = &PressureDataGet_Send_ToBytes;
    rt->GetFromBytes = &PressureDataGet_Send_GetFromBytes;
    return rt;
}
PressureDataGet_Return* New_PressureDataGet_Return(void)
{
    PressureDataGet_Return*rt = malloc( sizeof( PressureDataGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureDataGet_Return) ) ;
    rt->ToBytes = &PressureDataGet_Return_ToBytes;
    rt->GetFromBytes = &PressureDataGet_Return_GetFromBytes;
    return rt;
}
MotorAbort_Send* New_MotorAbort_Send(void)
{
    MotorAbort_Send*rt = malloc( sizeof( MotorAbort_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorAbort_Send) ) ;
    rt->ToBytes = &MotorAbort_Send_ToBytes;
    rt->GetFromBytes = &MotorAbort_Send_GetFromBytes;
    return rt;
}
MotorAbort_Return* New_MotorAbort_Return(void)
{
    MotorAbort_Return*rt = malloc( sizeof( MotorAbort_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorAbort_Return) ) ;
    rt->ToBytes = &MotorAbort_Return_ToBytes;
    rt->GetFromBytes = &MotorAbort_Return_GetFromBytes;
    return rt;
}
PressureEvent_Send* New_PressureEvent_Send(void)
{
    PressureEvent_Send*rt = malloc( sizeof( PressureEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureEvent_Send) ) ;
    rt->ToBytes = &PressureEvent_Send_ToBytes;
    rt->GetFromBytes = &PressureEvent_Send_GetFromBytes;
    return rt;
}
PressureEvent_Return* New_PressureEvent_Return(void)
{
    PressureEvent_Return*rt = malloc( sizeof( PressureEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureEvent_Return) ) ;
    rt->ToBytes = &PressureEvent_Return_ToBytes;
    rt->GetFromBytes = &PressureEvent_Return_GetFromBytes;
    return rt;
}
MotorMoveSeq_Send_m1_MoveArgs* New_MotorMoveSeq_Send_m1_MoveArgs(void)
{
    MotorMoveSeq_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMoveSeq_Send_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Send_m1_MoveArgs) ) ;
    rt->ToBytes = &MotorMoveSeq_Send_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &MotorMoveSeq_Send_m1_MoveArgs_GetFromBytes;
    return rt;
}
MotorMoveSeq_Send* New_MotorMoveSeq_Send(void)
{
    MotorMoveSeq_Send*rt = malloc( sizeof( MotorMoveSeq_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Send) ) ;
    rt->ToBytes = &MotorMoveSeq_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveSeq_Send_GetFromBytes;
    return rt;
}
MotorMoveSeq_Return* New_MotorMoveSeq_Return(void)
{
    MotorMoveSeq_Return*rt = malloc( sizeof( MotorMoveSeq_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Return) ) ;
    rt->ToBytes = &MotorMoveSeq_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveSeq_Return_GetFromBytes;
    return rt;
}
FirmWareReadyEvent_Send* New_FirmWareReadyEvent_Send(void)
{
    FirmWareReadyEvent_Send*rt = malloc( sizeof( FirmWareReadyEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareReadyEvent_Send) ) ;
    rt->ToBytes = &FirmWareReadyEvent_Send_ToBytes;
    rt->GetFromBytes = &FirmWareReadyEvent_Send_GetFromBytes;
    return rt;
}
FirmWareReadyEvent_Return* New_FirmWareReadyEvent_Return(void)
{
    FirmWareReadyEvent_Return*rt = malloc( sizeof( FirmWareReadyEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareReadyEvent_Return) ) ;
    rt->ToBytes = &FirmWareReadyEvent_Return_ToBytes;
    rt->GetFromBytes = &FirmWareReadyEvent_Return_GetFromBytes;
    return rt;
}
MotorConfigGet_Send* New_MotorConfigGet_Send(void)
{
    MotorConfigGet_Send*rt = malloc( sizeof( MotorConfigGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigGet_Send) ) ;
    rt->ToBytes = &MotorConfigGet_Send_ToBytes;
    rt->GetFromBytes = &MotorConfigGet_Send_GetFromBytes;
    return rt;
}
MotorConfigGet_Return* New_MotorConfigGet_Return(void)
{
    MotorConfigGet_Return*rt = malloc( sizeof( MotorConfigGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigGet_Return) ) ;
    rt->ToBytes = &MotorConfigGet_Return_ToBytes;
    rt->GetFromBytes = &MotorConfigGet_Return_GetFromBytes;
    return rt;
}
PWM_Set_Send* New_PWM_Set_Send(void)
{
    PWM_Set_Send*rt = malloc( sizeof( PWM_Set_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PWM_Set_Send) ) ;
    rt->ToBytes = &PWM_Set_Send_ToBytes;
    rt->GetFromBytes = &PWM_Set_Send_GetFromBytes;
    return rt;
}
PWM_Set_Return* New_PWM_Set_Return(void)
{
    PWM_Set_Return*rt = malloc( sizeof( PWM_Set_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PWM_Set_Return) ) ;
    rt->ToBytes = &PWM_Set_Return_ToBytes;
    rt->GetFromBytes = &PWM_Set_Return_GetFromBytes;
    return rt;
}
TurnTable_Move_Send* New_TurnTable_Move_Send(void)
{
    TurnTable_Move_Send*rt = malloc( sizeof( TurnTable_Move_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TurnTable_Move_Send) ) ;
    rt->ToBytes = &TurnTable_Move_Send_ToBytes;
    rt->GetFromBytes = &TurnTable_Move_Send_GetFromBytes;
    return rt;
}
TurnTable_Move_Return* New_TurnTable_Move_Return(void)
{
    TurnTable_Move_Return*rt = malloc( sizeof( TurnTable_Move_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TurnTable_Move_Return) ) ;
    rt->ToBytes = &TurnTable_Move_Return_ToBytes;
    rt->GetFromBytes = &TurnTable_Move_Return_GetFromBytes;
    return rt;
}
EncodeTurnTableFindZero_Send* New_EncodeTurnTableFindZero_Send(void)
{
    EncodeTurnTableFindZero_Send*rt = malloc( sizeof( EncodeTurnTableFindZero_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableFindZero_Send) ) ;
    rt->ToBytes = &EncodeTurnTableFindZero_Send_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableFindZero_Send_GetFromBytes;
    return rt;
}
EncodeTurnTableFindZero_Return* New_EncodeTurnTableFindZero_Return(void)
{
    EncodeTurnTableFindZero_Return*rt = malloc( sizeof( EncodeTurnTableFindZero_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableFindZero_Return) ) ;
    rt->ToBytes = &EncodeTurnTableFindZero_Return_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableFindZero_Return_GetFromBytes;
    return rt;
}
EncodeTurnTableRotateStart_Send* New_EncodeTurnTableRotateStart_Send(void)
{
    EncodeTurnTableRotateStart_Send*rt = malloc( sizeof( EncodeTurnTableRotateStart_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStart_Send) ) ;
    rt->ToBytes = &EncodeTurnTableRotateStart_Send_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableRotateStart_Send_GetFromBytes;
    return rt;
}
EncodeTurnTableRotateStart_Return* New_EncodeTurnTableRotateStart_Return(void)
{
    EncodeTurnTableRotateStart_Return*rt = malloc( sizeof( EncodeTurnTableRotateStart_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStart_Return) ) ;
    rt->ToBytes = &EncodeTurnTableRotateStart_Return_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableRotateStart_Return_GetFromBytes;
    return rt;
}
EncodeTurnTableRotateStop_Send* New_EncodeTurnTableRotateStop_Send(void)
{
    EncodeTurnTableRotateStop_Send*rt = malloc( sizeof( EncodeTurnTableRotateStop_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStop_Send) ) ;
    rt->ToBytes = &EncodeTurnTableRotateStop_Send_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableRotateStop_Send_GetFromBytes;
    return rt;
}
EncodeTurnTableRotateStop_Return* New_EncodeTurnTableRotateStop_Return(void)
{
    EncodeTurnTableRotateStop_Return*rt = malloc( sizeof( EncodeTurnTableRotateStop_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStop_Return) ) ;
    rt->ToBytes = &EncodeTurnTableRotateStop_Return_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableRotateStop_Return_GetFromBytes;
    return rt;
}
EncodeTurnTableStateGet_Send* New_EncodeTurnTableStateGet_Send(void)
{
    EncodeTurnTableStateGet_Send*rt = malloc( sizeof( EncodeTurnTableStateGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableStateGet_Send) ) ;
    rt->ToBytes = &EncodeTurnTableStateGet_Send_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableStateGet_Send_GetFromBytes;
    return rt;
}
EncodeTurnTableStateGet_Return* New_EncodeTurnTableStateGet_Return(void)
{
    EncodeTurnTableStateGet_Return*rt = malloc( sizeof( EncodeTurnTableStateGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableStateGet_Return) ) ;
    rt->ToBytes = &EncodeTurnTableStateGet_Return_ToBytes;
    rt->GetFromBytes = &EncodeTurnTableStateGet_Return_GetFromBytes;
    return rt;
}
DCAdRead_Send* New_DCAdRead_Send(void)
{
    DCAdRead_Send*rt = malloc( sizeof( DCAdRead_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCAdRead_Send) ) ;
    rt->ToBytes = &DCAdRead_Send_ToBytes;
    rt->GetFromBytes = &DCAdRead_Send_GetFromBytes;
    return rt;
}
DCAdRead_Return* New_DCAdRead_Return(void)
{
    DCAdRead_Return*rt = malloc( sizeof( DCAdRead_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCAdRead_Return) ) ;
    rt->ToBytes = &DCAdRead_Return_ToBytes;
    rt->GetFromBytes = &DCAdRead_Return_GetFromBytes;
    return rt;
}
JSAdRead_Send* New_JSAdRead_Send(void)
{
    JSAdRead_Send*rt = malloc( sizeof( JSAdRead_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdRead_Send) ) ;
    rt->ToBytes = &JSAdRead_Send_ToBytes;
    rt->GetFromBytes = &JSAdRead_Send_GetFromBytes;
    return rt;
}
JSAdRead_Return* New_JSAdRead_Return(void)
{
    JSAdRead_Return*rt = malloc( sizeof( JSAdRead_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdRead_Return) ) ;
    rt->ToBytes = &JSAdRead_Return_ToBytes;
    rt->GetFromBytes = &JSAdRead_Return_GetFromBytes;
    return rt;
}
MotorMoveSyncLevel_Send* New_MotorMoveSyncLevel_Send(void)
{
    MotorMoveSyncLevel_Send*rt = malloc( sizeof( MotorMoveSyncLevel_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSyncLevel_Send) ) ;
    rt->ToBytes = &MotorMoveSyncLevel_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveSyncLevel_Send_GetFromBytes;
    return rt;
}
MotorMoveSyncLevel_Return* New_MotorMoveSyncLevel_Return(void)
{
    MotorMoveSyncLevel_Return*rt = malloc( sizeof( MotorMoveSyncLevel_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSyncLevel_Return) ) ;
    rt->ToBytes = &MotorMoveSyncLevel_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveSyncLevel_Return_GetFromBytes;
    return rt;
}
DCMotor_Send* New_DCMotor_Send(void)
{
    DCMotor_Send*rt = malloc( sizeof( DCMotor_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotor_Send) ) ;
    rt->ToBytes = &DCMotor_Send_ToBytes;
    rt->GetFromBytes = &DCMotor_Send_GetFromBytes;
    return rt;
}
DCMotor_Return* New_DCMotor_Return(void)
{
    DCMotor_Return*rt = malloc( sizeof( DCMotor_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotor_Return) ) ;
    rt->ToBytes = &DCMotor_Return_ToBytes;
    rt->GetFromBytes = &DCMotor_Return_GetFromBytes;
    return rt;
}
MotorCupIDSet_Send* New_MotorCupIDSet_Send(void)
{
    MotorCupIDSet_Send*rt = malloc( sizeof( MotorCupIDSet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDSet_Send) ) ;
    rt->ToBytes = &MotorCupIDSet_Send_ToBytes;
    rt->GetFromBytes = &MotorCupIDSet_Send_GetFromBytes;
    return rt;
}
MotorCupIDSet_Return* New_MotorCupIDSet_Return(void)
{
    MotorCupIDSet_Return*rt = malloc( sizeof( MotorCupIDSet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDSet_Return) ) ;
    rt->ToBytes = &MotorCupIDSet_Return_ToBytes;
    rt->GetFromBytes = &MotorCupIDSet_Return_GetFromBytes;
    return rt;
}
MotorCupIDInfoGet_Send* New_MotorCupIDInfoGet_Send(void)
{
    MotorCupIDInfoGet_Send*rt = malloc( sizeof( MotorCupIDInfoGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDInfoGet_Send) ) ;
    rt->ToBytes = &MotorCupIDInfoGet_Send_ToBytes;
    rt->GetFromBytes = &MotorCupIDInfoGet_Send_GetFromBytes;
    return rt;
}
MotorCupIDInfoGet_Return* New_MotorCupIDInfoGet_Return(void)
{
    MotorCupIDInfoGet_Return*rt = malloc( sizeof( MotorCupIDInfoGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDInfoGet_Return) ) ;
    rt->ToBytes = &MotorCupIDInfoGet_Return_ToBytes;
    rt->GetFromBytes = &MotorCupIDInfoGet_Return_GetFromBytes;
    return rt;
}
JSAdDataGet_Send* New_JSAdDataGet_Send(void)
{
    JSAdDataGet_Send*rt = malloc( sizeof( JSAdDataGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdDataGet_Send) ) ;
    rt->ToBytes = &JSAdDataGet_Send_ToBytes;
    rt->GetFromBytes = &JSAdDataGet_Send_GetFromBytes;
    return rt;
}
JSAdDataGet_Return* New_JSAdDataGet_Return(void)
{
    JSAdDataGet_Return*rt = malloc( sizeof( JSAdDataGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdDataGet_Return) ) ;
    rt->ToBytes = &JSAdDataGet_Return_ToBytes;
    rt->GetFromBytes = &JSAdDataGet_Return_GetFromBytes;
    return rt;
}
MotorMultiMoveTmc_Send_m1_MoveArgs* New_MotorMultiMoveTmc_Send_m1_MoveArgs(void)
{
    MotorMultiMoveTmc_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMultiMoveTmc_Send_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Send_m1_MoveArgs) ) ;
    rt->ToBytes = &MotorMultiMoveTmc_Send_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &MotorMultiMoveTmc_Send_m1_MoveArgs_GetFromBytes;
    return rt;
}
MotorMultiMoveTmc_Send* New_MotorMultiMoveTmc_Send(void)
{
    MotorMultiMoveTmc_Send*rt = malloc( sizeof( MotorMultiMoveTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Send) ) ;
    rt->ToBytes = &MotorMultiMoveTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorMultiMoveTmc_Send_GetFromBytes;
    return rt;
}
MotorMultiMoveTmc_Return* New_MotorMultiMoveTmc_Return(void)
{
    MotorMultiMoveTmc_Return*rt = malloc( sizeof( MotorMultiMoveTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Return) ) ;
    rt->ToBytes = &MotorMultiMoveTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorMultiMoveTmc_Return_GetFromBytes;
    return rt;
}
MotorMoveTmc_Send* New_MotorMoveTmc_Send(void)
{
    MotorMoveTmc_Send*rt = malloc( sizeof( MotorMoveTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveTmc_Send) ) ;
    rt->ToBytes = &MotorMoveTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveTmc_Send_GetFromBytes;
    return rt;
}
MotorMoveTmc_Return* New_MotorMoveTmc_Return(void)
{
    MotorMoveTmc_Return*rt = malloc( sizeof( MotorMoveTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveTmc_Return) ) ;
    rt->ToBytes = &MotorMoveTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveTmc_Return_GetFromBytes;
    return rt;
}
MotorStatetGetTmc_Send* New_MotorStatetGetTmc_Send(void)
{
    MotorStatetGetTmc_Send*rt = malloc( sizeof( MotorStatetGetTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGetTmc_Send) ) ;
    rt->ToBytes = &MotorStatetGetTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorStatetGetTmc_Send_GetFromBytes;
    return rt;
}
MotorStatetGetTmc_Return* New_MotorStatetGetTmc_Return(void)
{
    MotorStatetGetTmc_Return*rt = malloc( sizeof( MotorStatetGetTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGetTmc_Return) ) ;
    rt->ToBytes = &MotorStatetGetTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorStatetGetTmc_Return_GetFromBytes;
    return rt;
}
MotorRotateStartTmc_Send* New_MotorRotateStartTmc_Send(void)
{
    MotorRotateStartTmc_Send*rt = malloc( sizeof( MotorRotateStartTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStartTmc_Send) ) ;
    rt->ToBytes = &MotorRotateStartTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateStartTmc_Send_GetFromBytes;
    return rt;
}
MotorRotateStartTmc_Return* New_MotorRotateStartTmc_Return(void)
{
    MotorRotateStartTmc_Return*rt = malloc( sizeof( MotorRotateStartTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStartTmc_Return) ) ;
    rt->ToBytes = &MotorRotateStartTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateStartTmc_Return_GetFromBytes;
    return rt;
}
MotorRotateStopTmc_Send* New_MotorRotateStopTmc_Send(void)
{
    MotorRotateStopTmc_Send*rt = malloc( sizeof( MotorRotateStopTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStopTmc_Send) ) ;
    rt->ToBytes = &MotorRotateStopTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateStopTmc_Send_GetFromBytes;
    return rt;
}
MotorRotateStopTmc_Return* New_MotorRotateStopTmc_Return(void)
{
    MotorRotateStopTmc_Return*rt = malloc( sizeof( MotorRotateStopTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStopTmc_Return) ) ;
    rt->ToBytes = &MotorRotateStopTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateStopTmc_Return_GetFromBytes;
    return rt;
}
MotorRotateTmc_Send* New_MotorRotateTmc_Send(void)
{
    MotorRotateTmc_Send*rt = malloc( sizeof( MotorRotateTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateTmc_Send) ) ;
    rt->ToBytes = &MotorRotateTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateTmc_Send_GetFromBytes;
    return rt;
}
MotorRotateTmc_Return* New_MotorRotateTmc_Return(void)
{
    MotorRotateTmc_Return*rt = malloc( sizeof( MotorRotateTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateTmc_Return) ) ;
    rt->ToBytes = &MotorRotateTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateTmc_Return_GetFromBytes;
    return rt;
}
MotorResetTmc_Send* New_MotorResetTmc_Send(void)
{
    MotorResetTmc_Send*rt = malloc( sizeof( MotorResetTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorResetTmc_Send) ) ;
    rt->ToBytes = &MotorResetTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorResetTmc_Send_GetFromBytes;
    return rt;
}
MotorResetTmc_Return* New_MotorResetTmc_Return(void)
{
    MotorResetTmc_Return*rt = malloc( sizeof( MotorResetTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorResetTmc_Return) ) ;
    rt->ToBytes = &MotorResetTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorResetTmc_Return_GetFromBytes;
    return rt;
}
MotorConfigSetTmc_Send* New_MotorConfigSetTmc_Send(void)
{
    MotorConfigSetTmc_Send*rt = malloc( sizeof( MotorConfigSetTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetTmc_Send) ) ;
    rt->ToBytes = &MotorConfigSetTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorConfigSetTmc_Send_GetFromBytes;
    return rt;
}
MotorConfigSetTmc_Return* New_MotorConfigSetTmc_Return(void)
{
    MotorConfigSetTmc_Return*rt = malloc( sizeof( MotorConfigSetTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetTmc_Return) ) ;
    rt->ToBytes = &MotorConfigSetTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorConfigSetTmc_Return_GetFromBytes;
    return rt;
}
MotorMoveGPIOEventTmc_Send* New_MotorMoveGPIOEventTmc_Send(void)
{
    MotorMoveGPIOEventTmc_Send*rt = malloc( sizeof( MotorMoveGPIOEventTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEventTmc_Send) ) ;
    rt->ToBytes = &MotorMoveGPIOEventTmc_Send_ToBytes;
    rt->GetFromBytes = &MotorMoveGPIOEventTmc_Send_GetFromBytes;
    return rt;
}
MotorMoveGPIOEventTmc_Return* New_MotorMoveGPIOEventTmc_Return(void)
{
    MotorMoveGPIOEventTmc_Return*rt = malloc( sizeof( MotorMoveGPIOEventTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEventTmc_Return) ) ;
    rt->ToBytes = &MotorMoveGPIOEventTmc_Return_ToBytes;
    rt->GetFromBytes = &MotorMoveGPIOEventTmc_Return_GetFromBytes;
    return rt;
}
MotorRotateShake_Send* New_MotorRotateShake_Send(void)
{
    MotorRotateShake_Send*rt = malloc( sizeof( MotorRotateShake_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateShake_Send) ) ;
    rt->ToBytes = &MotorRotateShake_Send_ToBytes;
    rt->GetFromBytes = &MotorRotateShake_Send_GetFromBytes;
    return rt;
}
MotorRotateShake_Return* New_MotorRotateShake_Return(void)
{
    MotorRotateShake_Return*rt = malloc( sizeof( MotorRotateShake_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateShake_Return) ) ;
    rt->ToBytes = &MotorRotateShake_Return_ToBytes;
    rt->GetFromBytes = &MotorRotateShake_Return_GetFromBytes;
    return rt;
}
LiquidLevelResultGet_Send* New_LiquidLevelResultGet_Send(void)
{
    LiquidLevelResultGet_Send*rt = malloc( sizeof( LiquidLevelResultGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LiquidLevelResultGet_Send) ) ;
    rt->ToBytes = &LiquidLevelResultGet_Send_ToBytes;
    rt->GetFromBytes = &LiquidLevelResultGet_Send_GetFromBytes;
    return rt;
}
LiquidLevelResultGet_Return* New_LiquidLevelResultGet_Return(void)
{
    LiquidLevelResultGet_Return*rt = malloc( sizeof( LiquidLevelResultGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LiquidLevelResultGet_Return) ) ;
    rt->ToBytes = &LiquidLevelResultGet_Return_ToBytes;
    rt->GetFromBytes = &LiquidLevelResultGet_Return_GetFromBytes;
    return rt;
}
MotorDetectLiquidLevel_Send* New_MotorDetectLiquidLevel_Send(void)
{
    MotorDetectLiquidLevel_Send*rt = malloc( sizeof( MotorDetectLiquidLevel_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorDetectLiquidLevel_Send) ) ;
    rt->ToBytes = &MotorDetectLiquidLevel_Send_ToBytes;
    rt->GetFromBytes = &MotorDetectLiquidLevel_Send_GetFromBytes;
    return rt;
}
MotorDetectLiquidLevel_Return* New_MotorDetectLiquidLevel_Return(void)
{
    MotorDetectLiquidLevel_Return*rt = malloc( sizeof( MotorDetectLiquidLevel_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorDetectLiquidLevel_Return) ) ;
    rt->ToBytes = &MotorDetectLiquidLevel_Return_ToBytes;
    rt->GetFromBytes = &MotorDetectLiquidLevel_Return_GetFromBytes;
    return rt;
}
RFIDValueOperation_Send* New_RFIDValueOperation_Send(void)
{
    RFIDValueOperation_Send*rt = malloc( sizeof( RFIDValueOperation_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RFIDValueOperation_Send) ) ;
    rt->ToBytes = &RFIDValueOperation_Send_ToBytes;
    rt->GetFromBytes = &RFIDValueOperation_Send_GetFromBytes;
    return rt;
}
RFIDValueOperation_Return* New_RFIDValueOperation_Return(void)
{
    RFIDValueOperation_Return*rt = malloc( sizeof( RFIDValueOperation_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RFIDValueOperation_Return) ) ;
    rt->ToBytes = &RFIDValueOperation_Return_ToBytes;
    rt->GetFromBytes = &RFIDValueOperation_Return_GetFromBytes;
    return rt;
}
IoSimple_Send* New_IoSimple_Send(void)
{
    IoSimple_Send*rt = malloc( sizeof( IoSimple_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoSimple_Send) ) ;
    rt->ToBytes = &IoSimple_Send_ToBytes;
    rt->GetFromBytes = &IoSimple_Send_GetFromBytes;
    return rt;
}
IoSimple_Return* New_IoSimple_Return(void)
{
    IoSimple_Return*rt = malloc( sizeof( IoSimple_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoSimple_Return) ) ;
    rt->ToBytes = &IoSimple_Return_ToBytes;
    rt->GetFromBytes = &IoSimple_Return_GetFromBytes;
    return rt;
}
IoOperation_Send_m1_IoArgs_m1_Controls* New_IoOperation_Send_m1_IoArgs_m1_Controls(void)
{
    IoOperation_Send_m1_IoArgs_m1_Controls*rt = malloc( sizeof( IoOperation_Send_m1_IoArgs_m1_Controls) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send_m1_IoArgs_m1_Controls) ) ;
    rt->ToBytes = &IoOperation_Send_m1_IoArgs_m1_Controls_ToBytes;
    rt->GetFromBytes = &IoOperation_Send_m1_IoArgs_m1_Controls_GetFromBytes;
    return rt;
}
IoOperation_Send_m1_IoArgs* New_IoOperation_Send_m1_IoArgs(void)
{
    IoOperation_Send_m1_IoArgs*rt = malloc( sizeof( IoOperation_Send_m1_IoArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send_m1_IoArgs) ) ;
    rt->ToBytes = &IoOperation_Send_m1_IoArgs_ToBytes;
    rt->GetFromBytes = &IoOperation_Send_m1_IoArgs_GetFromBytes;
    return rt;
}
IoOperation_Send* New_IoOperation_Send(void)
{
    IoOperation_Send*rt = malloc( sizeof( IoOperation_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send) ) ;
    rt->ToBytes = &IoOperation_Send_ToBytes;
    rt->GetFromBytes = &IoOperation_Send_GetFromBytes;
    return rt;
}
IoOperation_Return* New_IoOperation_Return(void)
{
    IoOperation_Return*rt = malloc( sizeof( IoOperation_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Return) ) ;
    rt->ToBytes = &IoOperation_Return_ToBytes;
    rt->GetFromBytes = &IoOperation_Return_GetFromBytes;
    return rt;
}
IoBlink_Send* New_IoBlink_Send(void)
{
    IoBlink_Send*rt = malloc( sizeof( IoBlink_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoBlink_Send) ) ;
    rt->ToBytes = &IoBlink_Send_ToBytes;
    rt->GetFromBytes = &IoBlink_Send_GetFromBytes;
    return rt;
}
IoBlink_Return* New_IoBlink_Return(void)
{
    IoBlink_Return*rt = malloc( sizeof( IoBlink_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoBlink_Return) ) ;
    rt->ToBytes = &IoBlink_Return_ToBytes;
    rt->GetFromBytes = &IoBlink_Return_GetFromBytes;
    return rt;
}
UartChangeBode_Send* New_UartChangeBode_Send(void)
{
    UartChangeBode_Send*rt = malloc( sizeof( UartChangeBode_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartChangeBode_Send) ) ;
    rt->ToBytes = &UartChangeBode_Send_ToBytes;
    rt->GetFromBytes = &UartChangeBode_Send_GetFromBytes;
    return rt;
}
UartChangeBode_Return* New_UartChangeBode_Return(void)
{
    UartChangeBode_Return*rt = malloc( sizeof( UartChangeBode_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartChangeBode_Return) ) ;
    rt->ToBytes = &UartChangeBode_Return_ToBytes;
    rt->GetFromBytes = &UartChangeBode_Return_GetFromBytes;
    return rt;
}
ServoMotorMultiRotate_Send_m1_MoveArgs* New_ServoMotorMultiRotate_Send_m1_MoveArgs(void)
{
    ServoMotorMultiRotate_Send_m1_MoveArgs*rt = malloc( sizeof( ServoMotorMultiRotate_Send_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Send_m1_MoveArgs) ) ;
    rt->ToBytes = &ServoMotorMultiRotate_Send_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &ServoMotorMultiRotate_Send_m1_MoveArgs_GetFromBytes;
    return rt;
}
ServoMotorMultiRotate_Send* New_ServoMotorMultiRotate_Send(void)
{
    ServoMotorMultiRotate_Send*rt = malloc( sizeof( ServoMotorMultiRotate_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Send) ) ;
    rt->ToBytes = &ServoMotorMultiRotate_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorMultiRotate_Send_GetFromBytes;
    return rt;
}
ServoMotorMultiRotate_Return* New_ServoMotorMultiRotate_Return(void)
{
    ServoMotorMultiRotate_Return*rt = malloc( sizeof( ServoMotorMultiRotate_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Return) ) ;
    rt->ToBytes = &ServoMotorMultiRotate_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorMultiRotate_Return_GetFromBytes;
    return rt;
}
ServoMotorRotateShakeDetect_Send* New_ServoMotorRotateShakeDetect_Send(void)
{
    ServoMotorRotateShakeDetect_Send*rt = malloc( sizeof( ServoMotorRotateShakeDetect_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeDetect_Send) ) ;
    rt->ToBytes = &ServoMotorRotateShakeDetect_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateShakeDetect_Send_GetFromBytes;
    return rt;
}
ServoMotorRotateShakeDetect_Return* New_ServoMotorRotateShakeDetect_Return(void)
{
    ServoMotorRotateShakeDetect_Return*rt = malloc( sizeof( ServoMotorRotateShakeDetect_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeDetect_Return) ) ;
    rt->ToBytes = &ServoMotorRotateShakeDetect_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateShakeDetect_Return_GetFromBytes;
    return rt;
}
ServoMotorRotateShakeRotateDetect_Send* New_ServoMotorRotateShakeRotateDetect_Send(void)
{
    ServoMotorRotateShakeRotateDetect_Send*rt = malloc( sizeof( ServoMotorRotateShakeRotateDetect_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeRotateDetect_Send) ) ;
    rt->ToBytes = &ServoMotorRotateShakeRotateDetect_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateShakeRotateDetect_Send_GetFromBytes;
    return rt;
}
ServoMotorRotateShakeRotateDetect_Return* New_ServoMotorRotateShakeRotateDetect_Return(void)
{
    ServoMotorRotateShakeRotateDetect_Return*rt = malloc( sizeof( ServoMotorRotateShakeRotateDetect_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeRotateDetect_Return) ) ;
    rt->ToBytes = &ServoMotorRotateShakeRotateDetect_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateShakeRotateDetect_Return_GetFromBytes;
    return rt;
}
ServoMotorRotateTwoDirOnTime_Send* New_ServoMotorRotateTwoDirOnTime_Send(void)
{
    ServoMotorRotateTwoDirOnTime_Send*rt = malloc( sizeof( ServoMotorRotateTwoDirOnTime_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateTwoDirOnTime_Send) ) ;
    rt->ToBytes = &ServoMotorRotateTwoDirOnTime_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateTwoDirOnTime_Send_GetFromBytes;
    return rt;
}
ServoMotorRotateTwoDirOnTime_Return* New_ServoMotorRotateTwoDirOnTime_Return(void)
{
    ServoMotorRotateTwoDirOnTime_Return*rt = malloc( sizeof( ServoMotorRotateTwoDirOnTime_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateTwoDirOnTime_Return) ) ;
    rt->ToBytes = &ServoMotorRotateTwoDirOnTime_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateTwoDirOnTime_Return_GetFromBytes;
    return rt;
}
ServoMotorStatetGet_Send* New_ServoMotorStatetGet_Send(void)
{
    ServoMotorStatetGet_Send*rt = malloc( sizeof( ServoMotorStatetGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorStatetGet_Send) ) ;
    rt->ToBytes = &ServoMotorStatetGet_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorStatetGet_Send_GetFromBytes;
    return rt;
}
ServoMotorStatetGet_Return* New_ServoMotorStatetGet_Return(void)
{
    ServoMotorStatetGet_Return*rt = malloc( sizeof( ServoMotorStatetGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorStatetGet_Return) ) ;
    rt->ToBytes = &ServoMotorStatetGet_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorStatetGet_Return_GetFromBytes;
    return rt;
}
ServoMotorRotateStart_Send* New_ServoMotorRotateStart_Send(void)
{
    ServoMotorRotateStart_Send*rt = malloc( sizeof( ServoMotorRotateStart_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStart_Send) ) ;
    rt->ToBytes = &ServoMotorRotateStart_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateStart_Send_GetFromBytes;
    return rt;
}
ServoMotorRotateStart_Return* New_ServoMotorRotateStart_Return(void)
{
    ServoMotorRotateStart_Return*rt = malloc( sizeof( ServoMotorRotateStart_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStart_Return) ) ;
    rt->ToBytes = &ServoMotorRotateStart_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateStart_Return_GetFromBytes;
    return rt;
}
ServoMotorRotateStop_Send* New_ServoMotorRotateStop_Send(void)
{
    ServoMotorRotateStop_Send*rt = malloc( sizeof( ServoMotorRotateStop_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStop_Send) ) ;
    rt->ToBytes = &ServoMotorRotateStop_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateStop_Send_GetFromBytes;
    return rt;
}
ServoMotorRotateStop_Return* New_ServoMotorRotateStop_Return(void)
{
    ServoMotorRotateStop_Return*rt = malloc( sizeof( ServoMotorRotateStop_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStop_Return) ) ;
    rt->ToBytes = &ServoMotorRotateStop_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorRotateStop_Return_GetFromBytes;
    return rt;
}
ServoMotorReset_Send* New_ServoMotorReset_Send(void)
{
    ServoMotorReset_Send*rt = malloc( sizeof( ServoMotorReset_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorReset_Send) ) ;
    rt->ToBytes = &ServoMotorReset_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorReset_Send_GetFromBytes;
    return rt;
}
ServoMotorReset_Return* New_ServoMotorReset_Return(void)
{
    ServoMotorReset_Return*rt = malloc( sizeof( ServoMotorReset_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorReset_Return) ) ;
    rt->ToBytes = &ServoMotorReset_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorReset_Return_GetFromBytes;
    return rt;
}
ServoMotorAutoCalibratio_Send* New_ServoMotorAutoCalibratio_Send(void)
{
    ServoMotorAutoCalibratio_Send*rt = malloc( sizeof( ServoMotorAutoCalibratio_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorAutoCalibratio_Send) ) ;
    rt->ToBytes = &ServoMotorAutoCalibratio_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorAutoCalibratio_Send_GetFromBytes;
    return rt;
}
ServoMotorAutoCalibratio_Return* New_ServoMotorAutoCalibratio_Return(void)
{
    ServoMotorAutoCalibratio_Return*rt = malloc( sizeof( ServoMotorAutoCalibratio_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorAutoCalibratio_Return) ) ;
    rt->ToBytes = &ServoMotorAutoCalibratio_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorAutoCalibratio_Return_GetFromBytes;
    return rt;
}
ServoMotorShake_Send* New_ServoMotorShake_Send(void)
{
    ServoMotorShake_Send*rt = malloc( sizeof( ServoMotorShake_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorShake_Send) ) ;
    rt->ToBytes = &ServoMotorShake_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorShake_Send_GetFromBytes;
    return rt;
}
ServoMotorShake_Return* New_ServoMotorShake_Return(void)
{
    ServoMotorShake_Return*rt = malloc( sizeof( ServoMotorShake_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorShake_Return) ) ;
    rt->ToBytes = &ServoMotorShake_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorShake_Return_GetFromBytes;
    return rt;
}
ServoMotorBaseCtrl_Send* New_ServoMotorBaseCtrl_Send(void)
{
    ServoMotorBaseCtrl_Send*rt = malloc( sizeof( ServoMotorBaseCtrl_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorBaseCtrl_Send) ) ;
    rt->ToBytes = &ServoMotorBaseCtrl_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorBaseCtrl_Send_GetFromBytes;
    return rt;
}
ServoMotorBaseCtrl_Return* New_ServoMotorBaseCtrl_Return(void)
{
    ServoMotorBaseCtrl_Return*rt = malloc( sizeof( ServoMotorBaseCtrl_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorBaseCtrl_Return) ) ;
    rt->ToBytes = &ServoMotorBaseCtrl_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorBaseCtrl_Return_GetFromBytes;
    return rt;
}
ServoMotorMove_Send* New_ServoMotorMove_Send(void)
{
    ServoMotorMove_Send*rt = malloc( sizeof( ServoMotorMove_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMove_Send) ) ;
    rt->ToBytes = &ServoMotorMove_Send_ToBytes;
    rt->GetFromBytes = &ServoMotorMove_Send_GetFromBytes;
    return rt;
}
ServoMotorMove_Return* New_ServoMotorMove_Return(void)
{
    ServoMotorMove_Return*rt = malloc( sizeof( ServoMotorMove_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMove_Return) ) ;
    rt->ToBytes = &ServoMotorMove_Return_ToBytes;
    rt->GetFromBytes = &ServoMotorMove_Return_GetFromBytes;
    return rt;
}
RGBSensorGet_Send* New_RGBSensorGet_Send(void)
{
    RGBSensorGet_Send*rt = malloc( sizeof( RGBSensorGet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RGBSensorGet_Send) ) ;
    rt->ToBytes = &RGBSensorGet_Send_ToBytes;
    rt->GetFromBytes = &RGBSensorGet_Send_GetFromBytes;
    return rt;
}
RGBSensorGet_Return* New_RGBSensorGet_Return(void)
{
    RGBSensorGet_Return*rt = malloc( sizeof( RGBSensorGet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RGBSensorGet_Return) ) ;
    rt->ToBytes = &RGBSensorGet_Return_ToBytes;
    rt->GetFromBytes = &RGBSensorGet_Return_GetFromBytes;
    return rt;
}
SerialDataReport_Send* New_SerialDataReport_Send(void)
{
    SerialDataReport_Send*rt = malloc( sizeof( SerialDataReport_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SerialDataReport_Send) ) ;
    rt->ToBytes = &SerialDataReport_Send_ToBytes;
    rt->GetFromBytes = &SerialDataReport_Send_GetFromBytes;
    return rt;
}
SerialDataReport_Return* New_SerialDataReport_Return(void)
{
    SerialDataReport_Return*rt = malloc( sizeof( SerialDataReport_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SerialDataReport_Return) ) ;
    rt->ToBytes = &SerialDataReport_Return_ToBytes;
    rt->GetFromBytes = &SerialDataReport_Return_GetFromBytes;
    return rt;
}
AddressBroadcastSend_Send* New_AddressBroadcastSend_Send(void)
{
    AddressBroadcastSend_Send*rt = malloc( sizeof( AddressBroadcastSend_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastSend_Send) ) ;
    rt->ToBytes = &AddressBroadcastSend_Send_ToBytes;
    rt->GetFromBytes = &AddressBroadcastSend_Send_GetFromBytes;
    return rt;
}
AddressBroadcastSend_Return* New_AddressBroadcastSend_Return(void)
{
    AddressBroadcastSend_Return*rt = malloc( sizeof( AddressBroadcastSend_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastSend_Return) ) ;
    rt->ToBytes = &AddressBroadcastSend_Return_ToBytes;
    rt->GetFromBytes = &AddressBroadcastSend_Return_GetFromBytes;
    return rt;
}
AddressBroadcastReceive_Send* New_AddressBroadcastReceive_Send(void)
{
    AddressBroadcastReceive_Send*rt = malloc( sizeof( AddressBroadcastReceive_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastReceive_Send) ) ;
    rt->ToBytes = &AddressBroadcastReceive_Send_ToBytes;
    rt->GetFromBytes = &AddressBroadcastReceive_Send_GetFromBytes;
    return rt;
}
AddressBroadcastReceive_Return* New_AddressBroadcastReceive_Return(void)
{
    AddressBroadcastReceive_Return*rt = malloc( sizeof( AddressBroadcastReceive_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastReceive_Return) ) ;
    rt->ToBytes = &AddressBroadcastReceive_Return_ToBytes;
    rt->GetFromBytes = &AddressBroadcastReceive_Return_GetFromBytes;
    return rt;
}
StartCheckPress_Send* New_StartCheckPress_Send(void)
{
    StartCheckPress_Send*rt = malloc( sizeof( StartCheckPress_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( StartCheckPress_Send) ) ;
    rt->ToBytes = &StartCheckPress_Send_ToBytes;
    rt->GetFromBytes = &StartCheckPress_Send_GetFromBytes;
    return rt;
}
StartCheckPress_Return* New_StartCheckPress_Return(void)
{
    StartCheckPress_Return*rt = malloc( sizeof( StartCheckPress_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( StartCheckPress_Return) ) ;
    rt->ToBytes = &StartCheckPress_Return_ToBytes;
    rt->GetFromBytes = &StartCheckPress_Return_GetFromBytes;
    return rt;
}
EndCheckPress_Send* New_EndCheckPress_Send(void)
{
    EndCheckPress_Send*rt = malloc( sizeof( EndCheckPress_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EndCheckPress_Send) ) ;
    rt->ToBytes = &EndCheckPress_Send_ToBytes;
    rt->GetFromBytes = &EndCheckPress_Send_GetFromBytes;
    return rt;
}
EndCheckPress_Return* New_EndCheckPress_Return(void)
{
    EndCheckPress_Return*rt = malloc( sizeof( EndCheckPress_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EndCheckPress_Return) ) ;
    rt->ToBytes = &EndCheckPress_Return_ToBytes;
    rt->GetFromBytes = &EndCheckPress_Return_GetFromBytes;
    return rt;
}
Blending_Send* New_Blending_Send(void)
{
    Blending_Send*rt = malloc( sizeof( Blending_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Blending_Send) ) ;
    rt->ToBytes = &Blending_Send_ToBytes;
    rt->GetFromBytes = &Blending_Send_GetFromBytes;
    return rt;
}
Blending_Return* New_Blending_Return(void)
{
    Blending_Return*rt = malloc( sizeof( Blending_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Blending_Return) ) ;
    rt->ToBytes = &Blending_Return_ToBytes;
    rt->GetFromBytes = &Blending_Return_GetFromBytes;
    return rt;
}
GetEPOSType_Send* New_GetEPOSType_Send(void)
{
    GetEPOSType_Send*rt = malloc( sizeof( GetEPOSType_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( GetEPOSType_Send) ) ;
    rt->ToBytes = &GetEPOSType_Send_ToBytes;
    rt->GetFromBytes = &GetEPOSType_Send_GetFromBytes;
    return rt;
}
GetEPOSType_Return* New_GetEPOSType_Return(void)
{
    GetEPOSType_Return*rt = malloc( sizeof( GetEPOSType_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( GetEPOSType_Return) ) ;
    rt->ToBytes = &GetEPOSType_Return_ToBytes;
    rt->GetFromBytes = &GetEPOSType_Return_GetFromBytes;
    return rt;
}
SetEPOSType_Send* New_SetEPOSType_Send(void)
{
    SetEPOSType_Send*rt = malloc( sizeof( SetEPOSType_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SetEPOSType_Send) ) ;
    rt->ToBytes = &SetEPOSType_Send_ToBytes;
    rt->GetFromBytes = &SetEPOSType_Send_GetFromBytes;
    return rt;
}
SetEPOSType_Return* New_SetEPOSType_Return(void)
{
    SetEPOSType_Return*rt = malloc( sizeof( SetEPOSType_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SetEPOSType_Return) ) ;
    rt->ToBytes = &SetEPOSType_Return_ToBytes;
    rt->GetFromBytes = &SetEPOSType_Return_GetFromBytes;
    return rt;
}
DACSet_Send* New_DACSet_Send(void)
{
    DACSet_Send*rt = malloc( sizeof( DACSet_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DACSet_Send) ) ;
    rt->ToBytes = &DACSet_Send_ToBytes;
    rt->GetFromBytes = &DACSet_Send_GetFromBytes;
    return rt;
}
DACSet_Return* New_DACSet_Return(void)
{
    DACSet_Return*rt = malloc( sizeof( DACSet_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DACSet_Return) ) ;
    rt->ToBytes = &DACSet_Return_ToBytes;
    rt->GetFromBytes = &DACSet_Return_GetFromBytes;
    return rt;
}
LightCtrl_Send* New_LightCtrl_Send(void)
{
    LightCtrl_Send*rt = malloc( sizeof( LightCtrl_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LightCtrl_Send) ) ;
    rt->ToBytes = &LightCtrl_Send_ToBytes;
    rt->GetFromBytes = &LightCtrl_Send_GetFromBytes;
    return rt;
}
LightCtrl_Return* New_LightCtrl_Return(void)
{
    LightCtrl_Return*rt = malloc( sizeof( LightCtrl_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LightCtrl_Return) ) ;
    rt->ToBytes = &LightCtrl_Return_ToBytes;
    rt->GetFromBytes = &LightCtrl_Return_GetFromBytes;
    return rt;
}
DCMotorMoveSeq_Send_m1_MoveArgs* New_DCMotorMoveSeq_Send_m1_MoveArgs(void)
{
    DCMotorMoveSeq_Send_m1_MoveArgs*rt = malloc( sizeof( DCMotorMoveSeq_Send_m1_MoveArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Send_m1_MoveArgs) ) ;
    rt->ToBytes = &DCMotorMoveSeq_Send_m1_MoveArgs_ToBytes;
    rt->GetFromBytes = &DCMotorMoveSeq_Send_m1_MoveArgs_GetFromBytes;
    return rt;
}
DCMotorMoveSeq_Send* New_DCMotorMoveSeq_Send(void)
{
    DCMotorMoveSeq_Send*rt = malloc( sizeof( DCMotorMoveSeq_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Send) ) ;
    rt->ToBytes = &DCMotorMoveSeq_Send_ToBytes;
    rt->GetFromBytes = &DCMotorMoveSeq_Send_GetFromBytes;
    return rt;
}
DCMotorMoveSeq_Return* New_DCMotorMoveSeq_Return(void)
{
    DCMotorMoveSeq_Return*rt = malloc( sizeof( DCMotorMoveSeq_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Return) ) ;
    rt->ToBytes = &DCMotorMoveSeq_Return_ToBytes;
    rt->GetFromBytes = &DCMotorMoveSeq_Return_GetFromBytes;
    return rt;
}
DCMotorMoveTmc_Send* New_DCMotorMoveTmc_Send(void)
{
    DCMotorMoveTmc_Send*rt = malloc( sizeof( DCMotorMoveTmc_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveTmc_Send) ) ;
    rt->ToBytes = &DCMotorMoveTmc_Send_ToBytes;
    rt->GetFromBytes = &DCMotorMoveTmc_Send_GetFromBytes;
    return rt;
}
DCMotorMoveTmc_Return* New_DCMotorMoveTmc_Return(void)
{
    DCMotorMoveTmc_Return*rt = malloc( sizeof( DCMotorMoveTmc_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveTmc_Return) ) ;
    rt->ToBytes = &DCMotorMoveTmc_Return_ToBytes;
    rt->GetFromBytes = &DCMotorMoveTmc_Return_GetFromBytes;
    return rt;
}
PressureRead_Send* New_PressureRead_Send(void)
{
    PressureRead_Send*rt = malloc( sizeof( PressureRead_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureRead_Send) ) ;
    rt->ToBytes = &PressureRead_Send_ToBytes;
    rt->GetFromBytes = &PressureRead_Send_GetFromBytes;
    return rt;
}
PressureRead_Return* New_PressureRead_Return(void)
{
    PressureRead_Return*rt = malloc( sizeof( PressureRead_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureRead_Return) ) ;
    rt->ToBytes = &PressureRead_Return_ToBytes;
    rt->GetFromBytes = &PressureRead_Return_GetFromBytes;
    return rt;
}
LittleMotorCtrl_Send* New_LittleMotorCtrl_Send(void)
{
    LittleMotorCtrl_Send*rt = malloc( sizeof( LittleMotorCtrl_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LittleMotorCtrl_Send) ) ;
    rt->ToBytes = &LittleMotorCtrl_Send_ToBytes;
    rt->GetFromBytes = &LittleMotorCtrl_Send_GetFromBytes;
    return rt;
}
LittleMotorCtrl_Return* New_LittleMotorCtrl_Return(void)
{
    LittleMotorCtrl_Return*rt = malloc( sizeof( LittleMotorCtrl_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LittleMotorCtrl_Return) ) ;
    rt->ToBytes = &LittleMotorCtrl_Return_ToBytes;
    rt->GetFromBytes = &LittleMotorCtrl_Return_GetFromBytes;
    return rt;
}
CongestionDetection_Send* New_CongestionDetection_Send(void)
{
    CongestionDetection_Send*rt = malloc( sizeof( CongestionDetection_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( CongestionDetection_Send) ) ;
    rt->ToBytes = &CongestionDetection_Send_ToBytes;
    rt->GetFromBytes = &CongestionDetection_Send_GetFromBytes;
    return rt;
}
CongestionDetection_Return* New_CongestionDetection_Return(void)
{
    CongestionDetection_Return*rt = malloc( sizeof( CongestionDetection_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( CongestionDetection_Return) ) ;
    rt->ToBytes = &CongestionDetection_Return_ToBytes;
    rt->GetFromBytes = &CongestionDetection_Return_GetFromBytes;
    return rt;
}
LaserCtrol_Send* New_LaserCtrol_Send(void)
{
    LaserCtrol_Send*rt = malloc( sizeof( LaserCtrol_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LaserCtrol_Send) ) ;
    rt->ToBytes = &LaserCtrol_Send_ToBytes;
    rt->GetFromBytes = &LaserCtrol_Send_GetFromBytes;
    return rt;
}
LaserCtrol_Return* New_LaserCtrol_Return(void)
{
    LaserCtrol_Return*rt = malloc( sizeof( LaserCtrol_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LaserCtrol_Return) ) ;
    rt->ToBytes = &LaserCtrol_Return_ToBytes;
    rt->GetFromBytes = &LaserCtrol_Return_GetFromBytes;
    return rt;
}
NFL_ReadConfig_Send* New_NFL_ReadConfig_Send(void)
{
    NFL_ReadConfig_Send*rt = malloc( sizeof( NFL_ReadConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadConfig_Send) ) ;
    rt->ToBytes = &NFL_ReadConfig_Send_ToBytes;
    rt->GetFromBytes = &NFL_ReadConfig_Send_GetFromBytes;
    return rt;
}
NFL_ReadConfig_Return* New_NFL_ReadConfig_Return(void)
{
    NFL_ReadConfig_Return*rt = malloc( sizeof( NFL_ReadConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadConfig_Return) ) ;
    rt->ToBytes = &NFL_ReadConfig_Return_ToBytes;
    rt->GetFromBytes = &NFL_ReadConfig_Return_GetFromBytes;
    return rt;
}
NFL_WriteConfig_Send* New_NFL_WriteConfig_Send(void)
{
    NFL_WriteConfig_Send*rt = malloc( sizeof( NFL_WriteConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_WriteConfig_Send) ) ;
    rt->ToBytes = &NFL_WriteConfig_Send_ToBytes;
    rt->GetFromBytes = &NFL_WriteConfig_Send_GetFromBytes;
    return rt;
}
NFL_WriteConfig_Return* New_NFL_WriteConfig_Return(void)
{
    NFL_WriteConfig_Return*rt = malloc( sizeof( NFL_WriteConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_WriteConfig_Return) ) ;
    rt->ToBytes = &NFL_WriteConfig_Return_ToBytes;
    rt->GetFromBytes = &NFL_WriteConfig_Return_GetFromBytes;
    return rt;
}
NFL_ReadDataStatus_Send* New_NFL_ReadDataStatus_Send(void)
{
    NFL_ReadDataStatus_Send*rt = malloc( sizeof( NFL_ReadDataStatus_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadDataStatus_Send) ) ;
    rt->ToBytes = &NFL_ReadDataStatus_Send_ToBytes;
    rt->GetFromBytes = &NFL_ReadDataStatus_Send_GetFromBytes;
    return rt;
}
NFL_ReadDataStatus_Return* New_NFL_ReadDataStatus_Return(void)
{
    NFL_ReadDataStatus_Return*rt = malloc( sizeof( NFL_ReadDataStatus_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadDataStatus_Return) ) ;
    rt->ToBytes = &NFL_ReadDataStatus_Return_ToBytes;
    rt->GetFromBytes = &NFL_ReadDataStatus_Return_GetFromBytes;
    return rt;
}
NFL_ReadData_Send* New_NFL_ReadData_Send(void)
{
    NFL_ReadData_Send*rt = malloc( sizeof( NFL_ReadData_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadData_Send) ) ;
    rt->ToBytes = &NFL_ReadData_Send_ToBytes;
    rt->GetFromBytes = &NFL_ReadData_Send_GetFromBytes;
    return rt;
}
NFL_ReadData_Return* New_NFL_ReadData_Return(void)
{
    NFL_ReadData_Return*rt = malloc( sizeof( NFL_ReadData_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadData_Return) ) ;
    rt->ToBytes = &NFL_ReadData_Return_ToBytes;
    rt->GetFromBytes = &NFL_ReadData_Return_GetFromBytes;
    return rt;
}
NFL_DataCollectStart_Send* New_NFL_DataCollectStart_Send(void)
{
    NFL_DataCollectStart_Send*rt = malloc( sizeof( NFL_DataCollectStart_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStart_Send) ) ;
    rt->ToBytes = &NFL_DataCollectStart_Send_ToBytes;
    rt->GetFromBytes = &NFL_DataCollectStart_Send_GetFromBytes;
    return rt;
}
NFL_DataCollectStart_Return* New_NFL_DataCollectStart_Return(void)
{
    NFL_DataCollectStart_Return*rt = malloc( sizeof( NFL_DataCollectStart_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStart_Return) ) ;
    rt->ToBytes = &NFL_DataCollectStart_Return_ToBytes;
    rt->GetFromBytes = &NFL_DataCollectStart_Return_GetFromBytes;
    return rt;
}
NFL_DataCollectStop_Send* New_NFL_DataCollectStop_Send(void)
{
    NFL_DataCollectStop_Send*rt = malloc( sizeof( NFL_DataCollectStop_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStop_Send) ) ;
    rt->ToBytes = &NFL_DataCollectStop_Send_ToBytes;
    rt->GetFromBytes = &NFL_DataCollectStop_Send_GetFromBytes;
    return rt;
}
NFL_DataCollectStop_Return* New_NFL_DataCollectStop_Return(void)
{
    NFL_DataCollectStop_Return*rt = malloc( sizeof( NFL_DataCollectStop_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStop_Return) ) ;
    rt->ToBytes = &NFL_DataCollectStop_Return_ToBytes;
    rt->GetFromBytes = &NFL_DataCollectStop_Return_GetFromBytes;
    return rt;
}
NFL_ThresholdConfig_Send* New_NFL_ThresholdConfig_Send(void)
{
    NFL_ThresholdConfig_Send*rt = malloc( sizeof( NFL_ThresholdConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ThresholdConfig_Send) ) ;
    rt->ToBytes = &NFL_ThresholdConfig_Send_ToBytes;
    rt->GetFromBytes = &NFL_ThresholdConfig_Send_GetFromBytes;
    return rt;
}
NFL_ThresholdConfig_Return* New_NFL_ThresholdConfig_Return(void)
{
    NFL_ThresholdConfig_Return*rt = malloc( sizeof( NFL_ThresholdConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ThresholdConfig_Return) ) ;
    rt->ToBytes = &NFL_ThresholdConfig_Return_ToBytes;
    rt->GetFromBytes = &NFL_ThresholdConfig_Return_GetFromBytes;
    return rt;
}
NFL_StandbyConfig_Send* New_NFL_StandbyConfig_Send(void)
{
    NFL_StandbyConfig_Send*rt = malloc( sizeof( NFL_StandbyConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_StandbyConfig_Send) ) ;
    rt->ToBytes = &NFL_StandbyConfig_Send_ToBytes;
    rt->GetFromBytes = &NFL_StandbyConfig_Send_GetFromBytes;
    return rt;
}
NFL_StandbyConfig_Return* New_NFL_StandbyConfig_Return(void)
{
    NFL_StandbyConfig_Return*rt = malloc( sizeof( NFL_StandbyConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_StandbyConfig_Return) ) ;
    rt->ToBytes = &NFL_StandbyConfig_Return_ToBytes;
    rt->GetFromBytes = &NFL_StandbyConfig_Return_GetFromBytes;
    return rt;
}
NFL_ActivateConfig_Send* New_NFL_ActivateConfig_Send(void)
{
    NFL_ActivateConfig_Send*rt = malloc( sizeof( NFL_ActivateConfig_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ActivateConfig_Send) ) ;
    rt->ToBytes = &NFL_ActivateConfig_Send_ToBytes;
    rt->GetFromBytes = &NFL_ActivateConfig_Send_GetFromBytes;
    return rt;
}
NFL_ActivateConfig_Return* New_NFL_ActivateConfig_Return(void)
{
    NFL_ActivateConfig_Return*rt = malloc( sizeof( NFL_ActivateConfig_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ActivateConfig_Return) ) ;
    rt->ToBytes = &NFL_ActivateConfig_Return_ToBytes;
    rt->GetFromBytes = &NFL_ActivateConfig_Return_GetFromBytes;
    return rt;
}
NFL_ReadTemperature_Send* New_NFL_ReadTemperature_Send(void)
{
    NFL_ReadTemperature_Send*rt = malloc( sizeof( NFL_ReadTemperature_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadTemperature_Send) ) ;
    rt->ToBytes = &NFL_ReadTemperature_Send_ToBytes;
    rt->GetFromBytes = &NFL_ReadTemperature_Send_GetFromBytes;
    return rt;
}
NFL_ReadTemperature_Return* New_NFL_ReadTemperature_Return(void)
{
    NFL_ReadTemperature_Return*rt = malloc( sizeof( NFL_ReadTemperature_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadTemperature_Return) ) ;
    rt->ToBytes = &NFL_ReadTemperature_Return_ToBytes;
    rt->GetFromBytes = &NFL_ReadTemperature_Return_GetFromBytes;
    return rt;
}
NFL_SendEvent_Send* New_NFL_SendEvent_Send(void)
{
    NFL_SendEvent_Send*rt = malloc( sizeof( NFL_SendEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_SendEvent_Send) ) ;
    rt->ToBytes = &NFL_SendEvent_Send_ToBytes;
    rt->GetFromBytes = &NFL_SendEvent_Send_GetFromBytes;
    return rt;
}
NFL_SendEvent_Return* New_NFL_SendEvent_Return(void)
{
    NFL_SendEvent_Return*rt = malloc( sizeof( NFL_SendEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_SendEvent_Return) ) ;
    rt->ToBytes = &NFL_SendEvent_Return_ToBytes;
    rt->GetFromBytes = &NFL_SendEvent_Return_GetFromBytes;
    return rt;
}
PowerCtrl_Send* New_PowerCtrl_Send(void)
{
    PowerCtrl_Send*rt = malloc( sizeof( PowerCtrl_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PowerCtrl_Send) ) ;
    rt->ToBytes = &PowerCtrl_Send_ToBytes;
    rt->GetFromBytes = &PowerCtrl_Send_GetFromBytes;
    return rt;
}
PowerCtrl_Return* New_PowerCtrl_Return(void)
{
    PowerCtrl_Return*rt = malloc( sizeof( PowerCtrl_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PowerCtrl_Return) ) ;
    rt->ToBytes = &PowerCtrl_Return_ToBytes;
    rt->GetFromBytes = &PowerCtrl_Return_GetFromBytes;
    return rt;
}
SendErrorCode_Send* New_SendErrorCode_Send(void)
{
    SendErrorCode_Send*rt = malloc( sizeof( SendErrorCode_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SendErrorCode_Send) ) ;
    rt->ToBytes = &SendErrorCode_Send_ToBytes;
    rt->GetFromBytes = &SendErrorCode_Send_GetFromBytes;
    return rt;
}
SendErrorCode_Return* New_SendErrorCode_Return(void)
{
    SendErrorCode_Return*rt = malloc( sizeof( SendErrorCode_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SendErrorCode_Return) ) ;
    rt->ToBytes = &SendErrorCode_Return_ToBytes;
    rt->GetFromBytes = &SendErrorCode_Return_GetFromBytes;
    return rt;
}
IOCtrlByEvent_Send_m1_PortArgs* New_IOCtrlByEvent_Send_m1_PortArgs(void)
{
    IOCtrlByEvent_Send_m1_PortArgs*rt = malloc( sizeof( IOCtrlByEvent_Send_m1_PortArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send_m1_PortArgs) ) ;
    rt->ToBytes = &IOCtrlByEvent_Send_m1_PortArgs_ToBytes;
    rt->GetFromBytes = &IOCtrlByEvent_Send_m1_PortArgs_GetFromBytes;
    return rt;
}
IOCtrlByEvent_Send_m2_PortArgs* New_IOCtrlByEvent_Send_m2_PortArgs(void)
{
    IOCtrlByEvent_Send_m2_PortArgs*rt = malloc( sizeof( IOCtrlByEvent_Send_m2_PortArgs) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send_m2_PortArgs) ) ;
    rt->ToBytes = &IOCtrlByEvent_Send_m2_PortArgs_ToBytes;
    rt->GetFromBytes = &IOCtrlByEvent_Send_m2_PortArgs_GetFromBytes;
    return rt;
}
IOCtrlByEvent_Send* New_IOCtrlByEvent_Send(void)
{
    IOCtrlByEvent_Send*rt = malloc( sizeof( IOCtrlByEvent_Send) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send) ) ;
    rt->ToBytes = &IOCtrlByEvent_Send_ToBytes;
    rt->GetFromBytes = &IOCtrlByEvent_Send_GetFromBytes;
    return rt;
}
IOCtrlByEvent_Return* New_IOCtrlByEvent_Return(void)
{
    IOCtrlByEvent_Return*rt = malloc( sizeof( IOCtrlByEvent_Return) ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Return) ) ;
    rt->ToBytes = &IOCtrlByEvent_Return_ToBytes;
    rt->GetFromBytes = &IOCtrlByEvent_Return_GetFromBytes;
    return rt;
}
Dsp_DHCP_Send* New_Dsp_DHCP_Send_Array(int cnt)
{
    Dsp_DHCP_Send*rt = malloc( sizeof( Dsp_DHCP_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Dsp_DHCP_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Dsp_DHCP_Send_ToBytes;
        rt[i].GetFromBytes = &Dsp_DHCP_Send_GetFromBytes;
    }
    return rt;
}
Dsp_DHCP_Return* New_Dsp_DHCP_Return_Array(int cnt)
{
    Dsp_DHCP_Return*rt = malloc( sizeof( Dsp_DHCP_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Dsp_DHCP_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Dsp_DHCP_Return_ToBytes;
        rt[i].GetFromBytes = &Dsp_DHCP_Return_GetFromBytes;
    }
    return rt;
}
Ping_Send* New_Ping_Send_Array(int cnt)
{
    Ping_Send*rt = malloc( sizeof( Ping_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Ping_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Ping_Send_ToBytes;
        rt[i].GetFromBytes = &Ping_Send_GetFromBytes;
    }
    return rt;
}
Ping_Return* New_Ping_Return_Array(int cnt)
{
    Ping_Return*rt = malloc( sizeof( Ping_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Ping_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Ping_Return_ToBytes;
        rt[i].GetFromBytes = &Ping_Return_GetFromBytes;
    }
    return rt;
}
FilePush_Send* New_FilePush_Send_Array(int cnt)
{
    FilePush_Send*rt = malloc( sizeof( FilePush_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FilePush_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FilePush_Send_ToBytes;
        rt[i].GetFromBytes = &FilePush_Send_GetFromBytes;
    }
    return rt;
}
FilePush_Return* New_FilePush_Return_Array(int cnt)
{
    FilePush_Return*rt = malloc( sizeof( FilePush_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FilePush_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FilePush_Return_ToBytes;
        rt[i].GetFromBytes = &FilePush_Return_GetFromBytes;
    }
    return rt;
}
InstrumentStateReport_Send* New_InstrumentStateReport_Send_Array(int cnt)
{
    InstrumentStateReport_Send*rt = malloc( sizeof( InstrumentStateReport_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( InstrumentStateReport_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &InstrumentStateReport_Send_ToBytes;
        rt[i].GetFromBytes = &InstrumentStateReport_Send_GetFromBytes;
    }
    return rt;
}
InstrumentStateReport_Return* New_InstrumentStateReport_Return_Array(int cnt)
{
    InstrumentStateReport_Return*rt = malloc( sizeof( InstrumentStateReport_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( InstrumentStateReport_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &InstrumentStateReport_Return_ToBytes;
        rt[i].GetFromBytes = &InstrumentStateReport_Return_GetFromBytes;
    }
    return rt;
}
ProtoTest_Send* New_ProtoTest_Send_Array(int cnt)
{
    ProtoTest_Send*rt = malloc( sizeof( ProtoTest_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ProtoTest_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ProtoTest_Send_ToBytes;
        rt[i].GetFromBytes = &ProtoTest_Send_GetFromBytes;
    }
    return rt;
}
ProtoTest_Return* New_ProtoTest_Return_Array(int cnt)
{
    ProtoTest_Return*rt = malloc( sizeof( ProtoTest_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ProtoTest_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ProtoTest_Return_ToBytes;
        rt[i].GetFromBytes = &ProtoTest_Return_GetFromBytes;
    }
    return rt;
}
McuErrorToPC_Send* New_McuErrorToPC_Send_Array(int cnt)
{
    McuErrorToPC_Send*rt = malloc( sizeof( McuErrorToPC_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuErrorToPC_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuErrorToPC_Send_ToBytes;
        rt[i].GetFromBytes = &McuErrorToPC_Send_GetFromBytes;
    }
    return rt;
}
McuErrorToPC_Return* New_McuErrorToPC_Return_Array(int cnt)
{
    McuErrorToPC_Return*rt = malloc( sizeof( McuErrorToPC_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuErrorToPC_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuErrorToPC_Return_ToBytes;
        rt[i].GetFromBytes = &McuErrorToPC_Return_GetFromBytes;
    }
    return rt;
}
AutoAdapter_Send* New_AutoAdapter_Send_Array(int cnt)
{
    AutoAdapter_Send*rt = malloc( sizeof( AutoAdapter_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AutoAdapter_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AutoAdapter_Send_ToBytes;
        rt[i].GetFromBytes = &AutoAdapter_Send_GetFromBytes;
    }
    return rt;
}
AutoAdapter_Return* New_AutoAdapter_Return_Array(int cnt)
{
    AutoAdapter_Return*rt = malloc( sizeof( AutoAdapter_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AutoAdapter_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AutoAdapter_Return_ToBytes;
        rt[i].GetFromBytes = &AutoAdapter_Return_GetFromBytes;
    }
    return rt;
}
MotorMultiMove_Send_m1_MoveArgs* New_MotorMultiMove_Send_m1_MoveArgs_Array(int cnt)
{
    MotorMultiMove_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMultiMove_Send_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Send_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMove_Send_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMove_Send_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
MotorMultiMove_Send* New_MotorMultiMove_Send_Array(int cnt)
{
    MotorMultiMove_Send*rt = malloc( sizeof( MotorMultiMove_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMove_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMove_Send_GetFromBytes;
    }
    return rt;
}
MotorMultiMove_Return* New_MotorMultiMove_Return_Array(int cnt)
{
    MotorMultiMove_Return*rt = malloc( sizeof( MotorMultiMove_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMove_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMove_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMove_Return_GetFromBytes;
    }
    return rt;
}
MotorMove_Send* New_MotorMove_Send_Array(int cnt)
{
    MotorMove_Send*rt = malloc( sizeof( MotorMove_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMove_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMove_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMove_Send_GetFromBytes;
    }
    return rt;
}
MotorMove_Return* New_MotorMove_Return_Array(int cnt)
{
    MotorMove_Return*rt = malloc( sizeof( MotorMove_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMove_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMove_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMove_Return_GetFromBytes;
    }
    return rt;
}
MotorStatetGet_Send* New_MotorStatetGet_Send_Array(int cnt)
{
    MotorStatetGet_Send*rt = malloc( sizeof( MotorStatetGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorStatetGet_Send_ToBytes;
        rt[i].GetFromBytes = &MotorStatetGet_Send_GetFromBytes;
    }
    return rt;
}
MotorStatetGet_Return* New_MotorStatetGet_Return_Array(int cnt)
{
    MotorStatetGet_Return*rt = malloc( sizeof( MotorStatetGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorStatetGet_Return_ToBytes;
        rt[i].GetFromBytes = &MotorStatetGet_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateStart_Send* New_MotorRotateStart_Send_Array(int cnt)
{
    MotorRotateStart_Send*rt = malloc( sizeof( MotorRotateStart_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStart_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStart_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStart_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateStart_Return* New_MotorRotateStart_Return_Array(int cnt)
{
    MotorRotateStart_Return*rt = malloc( sizeof( MotorRotateStart_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStart_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStart_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStart_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateStop_Send* New_MotorRotateStop_Send_Array(int cnt)
{
    MotorRotateStop_Send*rt = malloc( sizeof( MotorRotateStop_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStop_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStop_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStop_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateStop_Return* New_MotorRotateStop_Return_Array(int cnt)
{
    MotorRotateStop_Return*rt = malloc( sizeof( MotorRotateStop_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStop_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStop_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStop_Return_GetFromBytes;
    }
    return rt;
}
MotorRotate_Send* New_MotorRotate_Send_Array(int cnt)
{
    MotorRotate_Send*rt = malloc( sizeof( MotorRotate_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotate_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotate_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotate_Send_GetFromBytes;
    }
    return rt;
}
MotorRotate_Return* New_MotorRotate_Return_Array(int cnt)
{
    MotorRotate_Return*rt = malloc( sizeof( MotorRotate_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotate_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotate_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotate_Return_GetFromBytes;
    }
    return rt;
}
MotorReset_Send* New_MotorReset_Send_Array(int cnt)
{
    MotorReset_Send*rt = malloc( sizeof( MotorReset_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorReset_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorReset_Send_ToBytes;
        rt[i].GetFromBytes = &MotorReset_Send_GetFromBytes;
    }
    return rt;
}
MotorReset_Return* New_MotorReset_Return_Array(int cnt)
{
    MotorReset_Return*rt = malloc( sizeof( MotorReset_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorReset_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorReset_Return_ToBytes;
        rt[i].GetFromBytes = &MotorReset_Return_GetFromBytes;
    }
    return rt;
}
SensorValueOperation_Send* New_SensorValueOperation_Send_Array(int cnt)
{
    SensorValueOperation_Send*rt = malloc( sizeof( SensorValueOperation_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SensorValueOperation_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SensorValueOperation_Send_ToBytes;
        rt[i].GetFromBytes = &SensorValueOperation_Send_GetFromBytes;
    }
    return rt;
}
SensorValueOperation_Return* New_SensorValueOperation_Return_Array(int cnt)
{
    SensorValueOperation_Return*rt = malloc( sizeof( SensorValueOperation_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SensorValueOperation_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SensorValueOperation_Return_ToBytes;
        rt[i].GetFromBytes = &SensorValueOperation_Return_GetFromBytes;
    }
    return rt;
}
ADAutoCaliStart_Send* New_ADAutoCaliStart_Send_Array(int cnt)
{
    ADAutoCaliStart_Send*rt = malloc( sizeof( ADAutoCaliStart_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADAutoCaliStart_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADAutoCaliStart_Send_ToBytes;
        rt[i].GetFromBytes = &ADAutoCaliStart_Send_GetFromBytes;
    }
    return rt;
}
ADAutoCaliStart_Return* New_ADAutoCaliStart_Return_Array(int cnt)
{
    ADAutoCaliStart_Return*rt = malloc( sizeof( ADAutoCaliStart_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADAutoCaliStart_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADAutoCaliStart_Return_ToBytes;
        rt[i].GetFromBytes = &ADAutoCaliStart_Return_GetFromBytes;
    }
    return rt;
}
TemperatureGet_Send* New_TemperatureGet_Send_Array(int cnt)
{
    TemperatureGet_Send*rt = malloc( sizeof( TemperatureGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureGet_Send_ToBytes;
        rt[i].GetFromBytes = &TemperatureGet_Send_GetFromBytes;
    }
    return rt;
}
TemperatureGet_Return* New_TemperatureGet_Return_Array(int cnt)
{
    TemperatureGet_Return*rt = malloc( sizeof( TemperatureGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureGet_Return_ToBytes;
        rt[i].GetFromBytes = &TemperatureGet_Return_GetFromBytes;
    }
    return rt;
}
TemperatureOpen_Send* New_TemperatureOpen_Send_Array(int cnt)
{
    TemperatureOpen_Send*rt = malloc( sizeof( TemperatureOpen_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureOpen_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureOpen_Send_ToBytes;
        rt[i].GetFromBytes = &TemperatureOpen_Send_GetFromBytes;
    }
    return rt;
}
TemperatureOpen_Return* New_TemperatureOpen_Return_Array(int cnt)
{
    TemperatureOpen_Return*rt = malloc( sizeof( TemperatureOpen_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureOpen_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureOpen_Return_ToBytes;
        rt[i].GetFromBytes = &TemperatureOpen_Return_GetFromBytes;
    }
    return rt;
}
TemperatureClose_Send* New_TemperatureClose_Send_Array(int cnt)
{
    TemperatureClose_Send*rt = malloc( sizeof( TemperatureClose_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureClose_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureClose_Send_ToBytes;
        rt[i].GetFromBytes = &TemperatureClose_Send_GetFromBytes;
    }
    return rt;
}
TemperatureClose_Return* New_TemperatureClose_Return_Array(int cnt)
{
    TemperatureClose_Return*rt = malloc( sizeof( TemperatureClose_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TemperatureClose_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TemperatureClose_Return_ToBytes;
        rt[i].GetFromBytes = &TemperatureClose_Return_GetFromBytes;
    }
    return rt;
}
BarCodeGet_Send* New_BarCodeGet_Send_Array(int cnt)
{
    BarCodeGet_Send*rt = malloc( sizeof( BarCodeGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( BarCodeGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &BarCodeGet_Send_ToBytes;
        rt[i].GetFromBytes = &BarCodeGet_Send_GetFromBytes;
    }
    return rt;
}
BarCodeGet_Return* New_BarCodeGet_Return_Array(int cnt)
{
    BarCodeGet_Return*rt = malloc( sizeof( BarCodeGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( BarCodeGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &BarCodeGet_Return_ToBytes;
        rt[i].GetFromBytes = &BarCodeGet_Return_GetFromBytes;
    }
    return rt;
}
FirmWareConfig_Send* New_FirmWareConfig_Send_Array(int cnt)
{
    FirmWareConfig_Send*rt = malloc( sizeof( FirmWareConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareConfig_Send_ToBytes;
        rt[i].GetFromBytes = &FirmWareConfig_Send_GetFromBytes;
    }
    return rt;
}
FirmWareConfig_Return* New_FirmWareConfig_Return_Array(int cnt)
{
    FirmWareConfig_Return*rt = malloc( sizeof( FirmWareConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareConfig_Return_ToBytes;
        rt[i].GetFromBytes = &FirmWareConfig_Return_GetFromBytes;
    }
    return rt;
}
FirmWareTransmitFile_Send* New_FirmWareTransmitFile_Send_Array(int cnt)
{
    FirmWareTransmitFile_Send*rt = malloc( sizeof( FirmWareTransmitFile_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareTransmitFile_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareTransmitFile_Send_ToBytes;
        rt[i].GetFromBytes = &FirmWareTransmitFile_Send_GetFromBytes;
    }
    return rt;
}
FirmWareTransmitFile_Return* New_FirmWareTransmitFile_Return_Array(int cnt)
{
    FirmWareTransmitFile_Return*rt = malloc( sizeof( FirmWareTransmitFile_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareTransmitFile_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareTransmitFile_Return_ToBytes;
        rt[i].GetFromBytes = &FirmWareTransmitFile_Return_GetFromBytes;
    }
    return rt;
}
McuReset_Send* New_McuReset_Send_Array(int cnt)
{
    McuReset_Send*rt = malloc( sizeof( McuReset_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuReset_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuReset_Send_ToBytes;
        rt[i].GetFromBytes = &McuReset_Send_GetFromBytes;
    }
    return rt;
}
McuReset_Return* New_McuReset_Return_Array(int cnt)
{
    McuReset_Return*rt = malloc( sizeof( McuReset_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuReset_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuReset_Return_ToBytes;
        rt[i].GetFromBytes = &McuReset_Return_GetFromBytes;
    }
    return rt;
}
FirmWareVersionGet_Send* New_FirmWareVersionGet_Send_Array(int cnt)
{
    FirmWareVersionGet_Send*rt = malloc( sizeof( FirmWareVersionGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareVersionGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareVersionGet_Send_ToBytes;
        rt[i].GetFromBytes = &FirmWareVersionGet_Send_GetFromBytes;
    }
    return rt;
}
FirmWareVersionGet_Return* New_FirmWareVersionGet_Return_Array(int cnt)
{
    FirmWareVersionGet_Return*rt = malloc( sizeof( FirmWareVersionGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareVersionGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareVersionGet_Return_ToBytes;
        rt[i].GetFromBytes = &FirmWareVersionGet_Return_GetFromBytes;
    }
    return rt;
}
MotorConfigSet_Send* New_MotorConfigSet_Send_Array(int cnt)
{
    MotorConfigSet_Send*rt = malloc( sizeof( MotorConfigSet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSet_Send_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSet_Send_GetFromBytes;
    }
    return rt;
}
MotorConfigSet_Return* New_MotorConfigSet_Return_Array(int cnt)
{
    MotorConfigSet_Return*rt = malloc( sizeof( MotorConfigSet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSet_Return_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSet_Return_GetFromBytes;
    }
    return rt;
}
MotorMoveEvent_Send* New_MotorMoveEvent_Send_Array(int cnt)
{
    MotorMoveEvent_Send*rt = malloc( sizeof( MotorMoveEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveEvent_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveEvent_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveEvent_Return* New_MotorMoveEvent_Return_Array(int cnt)
{
    MotorMoveEvent_Return*rt = malloc( sizeof( MotorMoveEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveEvent_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveEvent_Return_GetFromBytes;
    }
    return rt;
}
McuIoStateChangeEvent_Send* New_McuIoStateChangeEvent_Send_Array(int cnt)
{
    McuIoStateChangeEvent_Send*rt = malloc( sizeof( McuIoStateChangeEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuIoStateChangeEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuIoStateChangeEvent_Send_ToBytes;
        rt[i].GetFromBytes = &McuIoStateChangeEvent_Send_GetFromBytes;
    }
    return rt;
}
McuIoStateChangeEvent_Return* New_McuIoStateChangeEvent_Return_Array(int cnt)
{
    McuIoStateChangeEvent_Return*rt = malloc( sizeof( McuIoStateChangeEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( McuIoStateChangeEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &McuIoStateChangeEvent_Return_ToBytes;
        rt[i].GetFromBytes = &McuIoStateChangeEvent_Return_GetFromBytes;
    }
    return rt;
}
SampleUnitSensorGet_Send* New_SampleUnitSensorGet_Send_Array(int cnt)
{
    SampleUnitSensorGet_Send*rt = malloc( sizeof( SampleUnitSensorGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SampleUnitSensorGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SampleUnitSensorGet_Send_ToBytes;
        rt[i].GetFromBytes = &SampleUnitSensorGet_Send_GetFromBytes;
    }
    return rt;
}
SampleUnitSensorGet_Return* New_SampleUnitSensorGet_Return_Array(int cnt)
{
    SampleUnitSensorGet_Return*rt = malloc( sizeof( SampleUnitSensorGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SampleUnitSensorGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SampleUnitSensorGet_Return_ToBytes;
        rt[i].GetFromBytes = &SampleUnitSensorGet_Return_GetFromBytes;
    }
    return rt;
}
AirPumpOpen_Send* New_AirPumpOpen_Send_Array(int cnt)
{
    AirPumpOpen_Send*rt = malloc( sizeof( AirPumpOpen_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpOpen_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AirPumpOpen_Send_ToBytes;
        rt[i].GetFromBytes = &AirPumpOpen_Send_GetFromBytes;
    }
    return rt;
}
AirPumpOpen_Return* New_AirPumpOpen_Return_Array(int cnt)
{
    AirPumpOpen_Return*rt = malloc( sizeof( AirPumpOpen_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpOpen_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AirPumpOpen_Return_ToBytes;
        rt[i].GetFromBytes = &AirPumpOpen_Return_GetFromBytes;
    }
    return rt;
}
AirPumpClose_Send* New_AirPumpClose_Send_Array(int cnt)
{
    AirPumpClose_Send*rt = malloc( sizeof( AirPumpClose_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpClose_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AirPumpClose_Send_ToBytes;
        rt[i].GetFromBytes = &AirPumpClose_Send_GetFromBytes;
    }
    return rt;
}
AirPumpClose_Return* New_AirPumpClose_Return_Array(int cnt)
{
    AirPumpClose_Return*rt = malloc( sizeof( AirPumpClose_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AirPumpClose_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AirPumpClose_Return_ToBytes;
        rt[i].GetFromBytes = &AirPumpClose_Return_GetFromBytes;
    }
    return rt;
}

ADSampleRead_Send* New_ADSampleRead_Send_Array(int cnt)
{
    ADSampleRead_Send*rt = malloc( sizeof( ADSampleRead_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADSampleRead_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADSampleRead_Send_ToBytes;
        rt[i].GetFromBytes = &ADSampleRead_Send_GetFromBytes;
    }
    return rt;
}
ADSampleRead_Return* New_ADSampleRead_Return_Array(int cnt)
{
    ADSampleRead_Return*rt = malloc( sizeof( ADSampleRead_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADSampleRead_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADSampleRead_Return_ToBytes;
        rt[i].GetFromBytes = &ADSampleRead_Return_GetFromBytes;
    }
    return rt;
}

/*
这里实现 数据接口 构造数组函数
*/
DiskInstalledRed_Send* New_DiskInstalledRed_Send_Array(int cnt)
{
	  DiskInstalledRed_Send*rt = malloc( sizeof( DiskInstalledRed_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DiskInstalledRed_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DiskInstalledRed_Send_ToBytes;
        rt[i].GetFromBytes = &DiskInstalledRed_Send_GetFromBytes;
    }
    return rt;
}
DiskInstalledRed_Return* New_DiskInstalledRed_Return_Array(int cnt)
{
	  DiskInstalledRed_Return*rt = malloc( sizeof( DiskInstalledRed_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DiskInstalledRed_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DiskInstalledRed_Return_ToBytes;
        rt[i].GetFromBytes = &DiskInstalledRed_Return_GetFromBytes;
    }
    return rt;
}

MotorMoveGPIOEvent_Send* New_MotorMoveGPIOEvent_Send_Array(int cnt)
{
    MotorMoveGPIOEvent_Send*rt = malloc( sizeof( MotorMoveGPIOEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveGPIOEvent_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveGPIOEvent_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveGPIOEvent_Return* New_MotorMoveGPIOEvent_Return_Array(int cnt)
{
    MotorMoveGPIOEvent_Return*rt = malloc( sizeof( MotorMoveGPIOEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveGPIOEvent_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveGPIOEvent_Return_GetFromBytes;
    }
    return rt;
}
MotorConfigSetNew_Send* New_MotorConfigSetNew_Send_Array(int cnt)
{
    MotorConfigSetNew_Send*rt = malloc( sizeof( MotorConfigSetNew_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetNew_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSetNew_Send_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSetNew_Send_GetFromBytes;
    }
    return rt;
}
MotorConfigSetNew_Return* New_MotorConfigSetNew_Return_Array(int cnt)
{
    MotorConfigSetNew_Return*rt = malloc( sizeof( MotorConfigSetNew_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetNew_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSetNew_Return_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSetNew_Return_GetFromBytes;
    }
    return rt;
}
ExecutesCtrl_Send* New_ExecutesCtrl_Send_Array(int cnt)
{
    ExecutesCtrl_Send*rt = malloc( sizeof( ExecutesCtrl_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ExecutesCtrl_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ExecutesCtrl_Send_ToBytes;
        rt[i].GetFromBytes = &ExecutesCtrl_Send_GetFromBytes;
    }
    return rt;
}
ExecutesCtrl_Return* New_ExecutesCtrl_Return_Array(int cnt)
{
    ExecutesCtrl_Return*rt = malloc( sizeof( ExecutesCtrl_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ExecutesCtrl_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ExecutesCtrl_Return_ToBytes;
        rt[i].GetFromBytes = &ExecutesCtrl_Return_GetFromBytes;
    }
    return rt;
}
ADDetectConfigSet_Send* New_ADDetectConfigSet_Send_Array(int cnt)
{
    ADDetectConfigSet_Send*rt = malloc( sizeof( ADDetectConfigSet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDetectConfigSet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDetectConfigSet_Send_ToBytes;
        rt[i].GetFromBytes = &ADDetectConfigSet_Send_GetFromBytes;
    }
    return rt;
}
ADDetectConfigSet_Return* New_ADDetectConfigSet_Return_Array(int cnt)
{
    ADDetectConfigSet_Return*rt = malloc( sizeof( ADDetectConfigSet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDetectConfigSet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDetectConfigSet_Return_ToBytes;
        rt[i].GetFromBytes = &ADDetectConfigSet_Return_GetFromBytes;
    }
    return rt;
}
ADDataRead_Send* New_ADDataRead_Send_Array(int cnt)
{
    ADDataRead_Send*rt = malloc( sizeof( ADDataRead_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataRead_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDataRead_Send_ToBytes;
        rt[i].GetFromBytes = &ADDataRead_Send_GetFromBytes;
    }
    return rt;
}
ADDataRead_Return* New_ADDataRead_Return_Array(int cnt)
{
    ADDataRead_Return*rt = malloc( sizeof( ADDataRead_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataRead_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDataRead_Return_ToBytes;
        rt[i].GetFromBytes = &ADDataRead_Return_GetFromBytes;
    }
    return rt;
}
ADDataSeriesSend_Send* New_ADDataSeriesSend_Send_Array(int cnt)
{
    ADDataSeriesSend_Send*rt = malloc( sizeof( ADDataSeriesSend_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataSeriesSend_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDataSeriesSend_Send_ToBytes;
        rt[i].GetFromBytes = &ADDataSeriesSend_Send_GetFromBytes;
    }
    return rt;
}
ADDataSeriesSend_Return* New_ADDataSeriesSend_Return_Array(int cnt)
{
    ADDataSeriesSend_Return*rt = malloc( sizeof( ADDataSeriesSend_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ADDataSeriesSend_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ADDataSeriesSend_Return_ToBytes;
        rt[i].GetFromBytes = &ADDataSeriesSend_Return_GetFromBytes;
    }
    return rt;
}
UartTransmit_Send* New_UartTransmit_Send_Array(int cnt)
{
    UartTransmit_Send*rt = malloc( sizeof( UartTransmit_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartTransmit_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartTransmit_Send_ToBytes;
        rt[i].GetFromBytes = &UartTransmit_Send_GetFromBytes;
    }
    return rt;
}
UartTransmit_Return* New_UartTransmit_Return_Array(int cnt)
{
    UartTransmit_Return*rt = malloc( sizeof( UartTransmit_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartTransmit_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartTransmit_Return_ToBytes;
        rt[i].GetFromBytes = &UartTransmit_Return_GetFromBytes;
    }
    return rt;
}
UartReceiveEvent_Send* New_UartReceiveEvent_Send_Array(int cnt)
{
    UartReceiveEvent_Send*rt = malloc( sizeof( UartReceiveEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartReceiveEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartReceiveEvent_Send_ToBytes;
        rt[i].GetFromBytes = &UartReceiveEvent_Send_GetFromBytes;
    }
    return rt;
}
UartReceiveEvent_Return* New_UartReceiveEvent_Return_Array(int cnt)
{
    UartReceiveEvent_Return*rt = malloc( sizeof( UartReceiveEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartReceiveEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartReceiveEvent_Return_ToBytes;
        rt[i].GetFromBytes = &UartReceiveEvent_Return_GetFromBytes;
    }
    return rt;
}
SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* New_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_Array(int cnt)
{
    SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs*rt = malloc( sizeof( SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
SeqMotorMultiMove_Send_m1_Sequence* New_SeqMotorMultiMove_Send_m1_Sequence_Array(int cnt)
{
    SeqMotorMultiMove_Send_m1_Sequence*rt = malloc( sizeof( SeqMotorMultiMove_Send_m1_Sequence)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send_m1_Sequence)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SeqMotorMultiMove_Send_m1_Sequence_ToBytes;
        rt[i].GetFromBytes = &SeqMotorMultiMove_Send_m1_Sequence_GetFromBytes;
    }
    return rt;
}
SeqMotorMultiMove_Send* New_SeqMotorMultiMove_Send_Array(int cnt)
{
    SeqMotorMultiMove_Send*rt = malloc( sizeof( SeqMotorMultiMove_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SeqMotorMultiMove_Send_ToBytes;
        rt[i].GetFromBytes = &SeqMotorMultiMove_Send_GetFromBytes;
    }
    return rt;
}
SeqMotorMultiMove_Return* New_SeqMotorMultiMove_Return_Array(int cnt)
{
    SeqMotorMultiMove_Return*rt = malloc( sizeof( SeqMotorMultiMove_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SeqMotorMultiMove_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SeqMotorMultiMove_Return_ToBytes;
        rt[i].GetFromBytes = &SeqMotorMultiMove_Return_GetFromBytes;
    }
    return rt;
}
PressureConfig_Send* New_PressureConfig_Send_Array(int cnt)
{
    PressureConfig_Send*rt = malloc( sizeof( PressureConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureConfig_Send_ToBytes;
        rt[i].GetFromBytes = &PressureConfig_Send_GetFromBytes;
    }
    return rt;
}
PressureConfig_Return* New_PressureConfig_Return_Array(int cnt)
{
    PressureConfig_Return*rt = malloc( sizeof( PressureConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureConfig_Return_ToBytes;
        rt[i].GetFromBytes = &PressureConfig_Return_GetFromBytes;
    }
    return rt;
}
PressureDataGet_Send* New_PressureDataGet_Send_Array(int cnt)
{
    PressureDataGet_Send*rt = malloc( sizeof( PressureDataGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureDataGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureDataGet_Send_ToBytes;
        rt[i].GetFromBytes = &PressureDataGet_Send_GetFromBytes;
    }
    return rt;
}
PressureDataGet_Return* New_PressureDataGet_Return_Array(int cnt)
{
    PressureDataGet_Return*rt = malloc( sizeof( PressureDataGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureDataGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureDataGet_Return_ToBytes;
        rt[i].GetFromBytes = &PressureDataGet_Return_GetFromBytes;
    }
    return rt;
}
MotorAbort_Send* New_MotorAbort_Send_Array(int cnt)
{
    MotorAbort_Send*rt = malloc( sizeof( MotorAbort_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorAbort_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorAbort_Send_ToBytes;
        rt[i].GetFromBytes = &MotorAbort_Send_GetFromBytes;
    }
    return rt;
}
MotorAbort_Return* New_MotorAbort_Return_Array(int cnt)
{
    MotorAbort_Return*rt = malloc( sizeof( MotorAbort_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorAbort_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorAbort_Return_ToBytes;
        rt[i].GetFromBytes = &MotorAbort_Return_GetFromBytes;
    }
    return rt;
}
PressureEvent_Send* New_PressureEvent_Send_Array(int cnt)
{
    PressureEvent_Send*rt = malloc( sizeof( PressureEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureEvent_Send_ToBytes;
        rt[i].GetFromBytes = &PressureEvent_Send_GetFromBytes;
    }
    return rt;
}
PressureEvent_Return* New_PressureEvent_Return_Array(int cnt)
{
    PressureEvent_Return*rt = malloc( sizeof( PressureEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureEvent_Return_ToBytes;
        rt[i].GetFromBytes = &PressureEvent_Return_GetFromBytes;
    }
    return rt;
}
MotorMoveSeq_Send_m1_MoveArgs* New_MotorMoveSeq_Send_m1_MoveArgs_Array(int cnt)
{
    MotorMoveSeq_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMoveSeq_Send_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Send_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveSeq_Send_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &MotorMoveSeq_Send_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
MotorMoveSeq_Send* New_MotorMoveSeq_Send_Array(int cnt)
{
    MotorMoveSeq_Send*rt = malloc( sizeof( MotorMoveSeq_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveSeq_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveSeq_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveSeq_Return* New_MotorMoveSeq_Return_Array(int cnt)
{
    MotorMoveSeq_Return*rt = malloc( sizeof( MotorMoveSeq_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSeq_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveSeq_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveSeq_Return_GetFromBytes;
    }
    return rt;
}
FirmWareReadyEvent_Send* New_FirmWareReadyEvent_Send_Array(int cnt)
{
    FirmWareReadyEvent_Send*rt = malloc( sizeof( FirmWareReadyEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareReadyEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareReadyEvent_Send_ToBytes;
        rt[i].GetFromBytes = &FirmWareReadyEvent_Send_GetFromBytes;
    }
    return rt;
}
FirmWareReadyEvent_Return* New_FirmWareReadyEvent_Return_Array(int cnt)
{
    FirmWareReadyEvent_Return*rt = malloc( sizeof( FirmWareReadyEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( FirmWareReadyEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &FirmWareReadyEvent_Return_ToBytes;
        rt[i].GetFromBytes = &FirmWareReadyEvent_Return_GetFromBytes;
    }
    return rt;
}
MotorConfigGet_Send* New_MotorConfigGet_Send_Array(int cnt)
{
    MotorConfigGet_Send*rt = malloc( sizeof( MotorConfigGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigGet_Send_ToBytes;
        rt[i].GetFromBytes = &MotorConfigGet_Send_GetFromBytes;
    }
    return rt;
}
MotorConfigGet_Return* New_MotorConfigGet_Return_Array(int cnt)
{
    MotorConfigGet_Return*rt = malloc( sizeof( MotorConfigGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigGet_Return_ToBytes;
        rt[i].GetFromBytes = &MotorConfigGet_Return_GetFromBytes;
    }
    return rt;
}
PWM_Set_Send* New_PWM_Set_Send_Array(int cnt)
{
    PWM_Set_Send*rt = malloc( sizeof( PWM_Set_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PWM_Set_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PWM_Set_Send_ToBytes;
        rt[i].GetFromBytes = &PWM_Set_Send_GetFromBytes;
    }
    return rt;
}
PWM_Set_Return* New_PWM_Set_Return_Array(int cnt)
{
    PWM_Set_Return*rt = malloc( sizeof( PWM_Set_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PWM_Set_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PWM_Set_Return_ToBytes;
        rt[i].GetFromBytes = &PWM_Set_Return_GetFromBytes;
    }
    return rt;
}
TurnTable_Move_Send* New_TurnTable_Move_Send_Array(int cnt)
{
    TurnTable_Move_Send*rt = malloc( sizeof( TurnTable_Move_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TurnTable_Move_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TurnTable_Move_Send_ToBytes;
        rt[i].GetFromBytes = &TurnTable_Move_Send_GetFromBytes;
    }
    return rt;
}
TurnTable_Move_Return* New_TurnTable_Move_Return_Array(int cnt)
{
    TurnTable_Move_Return*rt = malloc( sizeof( TurnTable_Move_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( TurnTable_Move_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &TurnTable_Move_Return_ToBytes;
        rt[i].GetFromBytes = &TurnTable_Move_Return_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableFindZero_Send* New_EncodeTurnTableFindZero_Send_Array(int cnt)
{
    EncodeTurnTableFindZero_Send*rt = malloc( sizeof( EncodeTurnTableFindZero_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableFindZero_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableFindZero_Send_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableFindZero_Send_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableFindZero_Return* New_EncodeTurnTableFindZero_Return_Array(int cnt)
{
    EncodeTurnTableFindZero_Return*rt = malloc( sizeof( EncodeTurnTableFindZero_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableFindZero_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableFindZero_Return_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableFindZero_Return_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableRotateStart_Send* New_EncodeTurnTableRotateStart_Send_Array(int cnt)
{
    EncodeTurnTableRotateStart_Send*rt = malloc( sizeof( EncodeTurnTableRotateStart_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStart_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableRotateStart_Send_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableRotateStart_Send_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableRotateStart_Return* New_EncodeTurnTableRotateStart_Return_Array(int cnt)
{
    EncodeTurnTableRotateStart_Return*rt = malloc( sizeof( EncodeTurnTableRotateStart_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStart_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableRotateStart_Return_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableRotateStart_Return_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableRotateStop_Send* New_EncodeTurnTableRotateStop_Send_Array(int cnt)
{
    EncodeTurnTableRotateStop_Send*rt = malloc( sizeof( EncodeTurnTableRotateStop_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStop_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableRotateStop_Send_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableRotateStop_Send_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableRotateStop_Return* New_EncodeTurnTableRotateStop_Return_Array(int cnt)
{
    EncodeTurnTableRotateStop_Return*rt = malloc( sizeof( EncodeTurnTableRotateStop_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableRotateStop_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableRotateStop_Return_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableRotateStop_Return_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableStateGet_Send* New_EncodeTurnTableStateGet_Send_Array(int cnt)
{
    EncodeTurnTableStateGet_Send*rt = malloc( sizeof( EncodeTurnTableStateGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableStateGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableStateGet_Send_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableStateGet_Send_GetFromBytes;
    }
    return rt;
}
EncodeTurnTableStateGet_Return* New_EncodeTurnTableStateGet_Return_Array(int cnt)
{
    EncodeTurnTableStateGet_Return*rt = malloc( sizeof( EncodeTurnTableStateGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EncodeTurnTableStateGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EncodeTurnTableStateGet_Return_ToBytes;
        rt[i].GetFromBytes = &EncodeTurnTableStateGet_Return_GetFromBytes;
    }
    return rt;
}
DCAdRead_Send* New_DCAdRead_Send_Array(int cnt)
{
    DCAdRead_Send*rt = malloc( sizeof( DCAdRead_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCAdRead_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCAdRead_Send_ToBytes;
        rt[i].GetFromBytes = &DCAdRead_Send_GetFromBytes;
    }
    return rt;
}
DCAdRead_Return* New_DCAdRead_Return_Array(int cnt)
{
    DCAdRead_Return*rt = malloc( sizeof( DCAdRead_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCAdRead_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCAdRead_Return_ToBytes;
        rt[i].GetFromBytes = &DCAdRead_Return_GetFromBytes;
    }
    return rt;
}
JSAdRead_Send* New_JSAdRead_Send_Array(int cnt)
{
    JSAdRead_Send*rt = malloc( sizeof( JSAdRead_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdRead_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &JSAdRead_Send_ToBytes;
        rt[i].GetFromBytes = &JSAdRead_Send_GetFromBytes;
    }
    return rt;
}
JSAdRead_Return* New_JSAdRead_Return_Array(int cnt)
{
    JSAdRead_Return*rt = malloc( sizeof( JSAdRead_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdRead_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &JSAdRead_Return_ToBytes;
        rt[i].GetFromBytes = &JSAdRead_Return_GetFromBytes;
    }
    return rt;
}
MotorMoveSyncLevel_Send* New_MotorMoveSyncLevel_Send_Array(int cnt)
{
    MotorMoveSyncLevel_Send*rt = malloc( sizeof( MotorMoveSyncLevel_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSyncLevel_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveSyncLevel_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveSyncLevel_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveSyncLevel_Return* New_MotorMoveSyncLevel_Return_Array(int cnt)
{
    MotorMoveSyncLevel_Return*rt = malloc( sizeof( MotorMoveSyncLevel_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveSyncLevel_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveSyncLevel_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveSyncLevel_Return_GetFromBytes;
    }
    return rt;
}
DCMotor_Send* New_DCMotor_Send_Array(int cnt)
{
    DCMotor_Send*rt = malloc( sizeof( DCMotor_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotor_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotor_Send_ToBytes;
        rt[i].GetFromBytes = &DCMotor_Send_GetFromBytes;
    }
    return rt;
}
DCMotor_Return* New_DCMotor_Return_Array(int cnt)
{
    DCMotor_Return*rt = malloc( sizeof( DCMotor_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotor_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotor_Return_ToBytes;
        rt[i].GetFromBytes = &DCMotor_Return_GetFromBytes;
    }
    return rt;
}
MotorCupIDSet_Send* New_MotorCupIDSet_Send_Array(int cnt)
{
    MotorCupIDSet_Send*rt = malloc( sizeof( MotorCupIDSet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDSet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorCupIDSet_Send_ToBytes;
        rt[i].GetFromBytes = &MotorCupIDSet_Send_GetFromBytes;
    }
    return rt;
}
MotorCupIDSet_Return* New_MotorCupIDSet_Return_Array(int cnt)
{
    MotorCupIDSet_Return*rt = malloc( sizeof( MotorCupIDSet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDSet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorCupIDSet_Return_ToBytes;
        rt[i].GetFromBytes = &MotorCupIDSet_Return_GetFromBytes;
    }
    return rt;
}
MotorCupIDInfoGet_Send* New_MotorCupIDInfoGet_Send_Array(int cnt)
{
    MotorCupIDInfoGet_Send*rt = malloc( sizeof( MotorCupIDInfoGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDInfoGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorCupIDInfoGet_Send_ToBytes;
        rt[i].GetFromBytes = &MotorCupIDInfoGet_Send_GetFromBytes;
    }
    return rt;
}
MotorCupIDInfoGet_Return* New_MotorCupIDInfoGet_Return_Array(int cnt)
{
    MotorCupIDInfoGet_Return*rt = malloc( sizeof( MotorCupIDInfoGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorCupIDInfoGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorCupIDInfoGet_Return_ToBytes;
        rt[i].GetFromBytes = &MotorCupIDInfoGet_Return_GetFromBytes;
    }
    return rt;
}
JSAdDataGet_Send* New_JSAdDataGet_Send_Array(int cnt)
{
    JSAdDataGet_Send*rt = malloc( sizeof( JSAdDataGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdDataGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &JSAdDataGet_Send_ToBytes;
        rt[i].GetFromBytes = &JSAdDataGet_Send_GetFromBytes;
    }
    return rt;
}
JSAdDataGet_Return* New_JSAdDataGet_Return_Array(int cnt)
{
    JSAdDataGet_Return*rt = malloc( sizeof( JSAdDataGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( JSAdDataGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &JSAdDataGet_Return_ToBytes;
        rt[i].GetFromBytes = &JSAdDataGet_Return_GetFromBytes;
    }
    return rt;
}
MotorMultiMoveTmc_Send_m1_MoveArgs* New_MotorMultiMoveTmc_Send_m1_MoveArgs_Array(int cnt)
{
    MotorMultiMoveTmc_Send_m1_MoveArgs*rt = malloc( sizeof( MotorMultiMoveTmc_Send_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Send_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMoveTmc_Send_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMoveTmc_Send_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
MotorMultiMoveTmc_Send* New_MotorMultiMoveTmc_Send_Array(int cnt)
{
    MotorMultiMoveTmc_Send*rt = malloc( sizeof( MotorMultiMoveTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMoveTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMoveTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorMultiMoveTmc_Return* New_MotorMultiMoveTmc_Return_Array(int cnt)
{
    MotorMultiMoveTmc_Return*rt = malloc( sizeof( MotorMultiMoveTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMultiMoveTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMultiMoveTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMultiMoveTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorMoveTmc_Send* New_MotorMoveTmc_Send_Array(int cnt)
{
    MotorMoveTmc_Send*rt = malloc( sizeof( MotorMoveTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveTmc_Return* New_MotorMoveTmc_Return_Array(int cnt)
{
    MotorMoveTmc_Return*rt = malloc( sizeof( MotorMoveTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorStatetGetTmc_Send* New_MotorStatetGetTmc_Send_Array(int cnt)
{
    MotorStatetGetTmc_Send*rt = malloc( sizeof( MotorStatetGetTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGetTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorStatetGetTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorStatetGetTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorStatetGetTmc_Return* New_MotorStatetGetTmc_Return_Array(int cnt)
{
    MotorStatetGetTmc_Return*rt = malloc( sizeof( MotorStatetGetTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorStatetGetTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorStatetGetTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorStatetGetTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateStartTmc_Send* New_MotorRotateStartTmc_Send_Array(int cnt)
{
    MotorRotateStartTmc_Send*rt = malloc( sizeof( MotorRotateStartTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStartTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStartTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStartTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateStartTmc_Return* New_MotorRotateStartTmc_Return_Array(int cnt)
{
    MotorRotateStartTmc_Return*rt = malloc( sizeof( MotorRotateStartTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStartTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStartTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStartTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateStopTmc_Send* New_MotorRotateStopTmc_Send_Array(int cnt)
{
    MotorRotateStopTmc_Send*rt = malloc( sizeof( MotorRotateStopTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStopTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStopTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStopTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateStopTmc_Return* New_MotorRotateStopTmc_Return_Array(int cnt)
{
    MotorRotateStopTmc_Return*rt = malloc( sizeof( MotorRotateStopTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateStopTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateStopTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateStopTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateTmc_Send* New_MotorRotateTmc_Send_Array(int cnt)
{
    MotorRotateTmc_Send*rt = malloc( sizeof( MotorRotateTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateTmc_Return* New_MotorRotateTmc_Return_Array(int cnt)
{
    MotorRotateTmc_Return*rt = malloc( sizeof( MotorRotateTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorResetTmc_Send* New_MotorResetTmc_Send_Array(int cnt)
{
    MotorResetTmc_Send*rt = malloc( sizeof( MotorResetTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorResetTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorResetTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorResetTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorResetTmc_Return* New_MotorResetTmc_Return_Array(int cnt)
{
    MotorResetTmc_Return*rt = malloc( sizeof( MotorResetTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorResetTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorResetTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorResetTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorConfigSetTmc_Send* New_MotorConfigSetTmc_Send_Array(int cnt)
{
    MotorConfigSetTmc_Send*rt = malloc( sizeof( MotorConfigSetTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSetTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSetTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorConfigSetTmc_Return* New_MotorConfigSetTmc_Return_Array(int cnt)
{
    MotorConfigSetTmc_Return*rt = malloc( sizeof( MotorConfigSetTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorConfigSetTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorConfigSetTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorConfigSetTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorMoveGPIOEventTmc_Send* New_MotorMoveGPIOEventTmc_Send_Array(int cnt)
{
    MotorMoveGPIOEventTmc_Send*rt = malloc( sizeof( MotorMoveGPIOEventTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEventTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveGPIOEventTmc_Send_ToBytes;
        rt[i].GetFromBytes = &MotorMoveGPIOEventTmc_Send_GetFromBytes;
    }
    return rt;
}
MotorMoveGPIOEventTmc_Return* New_MotorMoveGPIOEventTmc_Return_Array(int cnt)
{
    MotorMoveGPIOEventTmc_Return*rt = malloc( sizeof( MotorMoveGPIOEventTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorMoveGPIOEventTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorMoveGPIOEventTmc_Return_ToBytes;
        rt[i].GetFromBytes = &MotorMoveGPIOEventTmc_Return_GetFromBytes;
    }
    return rt;
}
MotorRotateShake_Send* New_MotorRotateShake_Send_Array(int cnt)
{
    MotorRotateShake_Send*rt = malloc( sizeof( MotorRotateShake_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateShake_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateShake_Send_ToBytes;
        rt[i].GetFromBytes = &MotorRotateShake_Send_GetFromBytes;
    }
    return rt;
}
MotorRotateShake_Return* New_MotorRotateShake_Return_Array(int cnt)
{
    MotorRotateShake_Return*rt = malloc( sizeof( MotorRotateShake_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorRotateShake_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorRotateShake_Return_ToBytes;
        rt[i].GetFromBytes = &MotorRotateShake_Return_GetFromBytes;
    }
    return rt;
}
LiquidLevelResultGet_Send* New_LiquidLevelResultGet_Send_Array(int cnt)
{
    LiquidLevelResultGet_Send*rt = malloc( sizeof( LiquidLevelResultGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LiquidLevelResultGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LiquidLevelResultGet_Send_ToBytes;
        rt[i].GetFromBytes = &LiquidLevelResultGet_Send_GetFromBytes;
    }
    return rt;
}
LiquidLevelResultGet_Return* New_LiquidLevelResultGet_Return_Array(int cnt)
{
    LiquidLevelResultGet_Return*rt = malloc( sizeof( LiquidLevelResultGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LiquidLevelResultGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LiquidLevelResultGet_Return_ToBytes;
        rt[i].GetFromBytes = &LiquidLevelResultGet_Return_GetFromBytes;
    }
    return rt;
}
MotorDetectLiquidLevel_Send* New_MotorDetectLiquidLevel_Send_Array(int cnt)
{
    MotorDetectLiquidLevel_Send*rt = malloc( sizeof( MotorDetectLiquidLevel_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorDetectLiquidLevel_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorDetectLiquidLevel_Send_ToBytes;
        rt[i].GetFromBytes = &MotorDetectLiquidLevel_Send_GetFromBytes;
    }
    return rt;
}
MotorDetectLiquidLevel_Return* New_MotorDetectLiquidLevel_Return_Array(int cnt)
{
    MotorDetectLiquidLevel_Return*rt = malloc( sizeof( MotorDetectLiquidLevel_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( MotorDetectLiquidLevel_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &MotorDetectLiquidLevel_Return_ToBytes;
        rt[i].GetFromBytes = &MotorDetectLiquidLevel_Return_GetFromBytes;
    }
    return rt;
}
RFIDValueOperation_Send* New_RFIDValueOperation_Send_Array(int cnt)
{
    RFIDValueOperation_Send*rt = malloc( sizeof( RFIDValueOperation_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RFIDValueOperation_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &RFIDValueOperation_Send_ToBytes;
        rt[i].GetFromBytes = &RFIDValueOperation_Send_GetFromBytes;
    }
    return rt;
}
RFIDValueOperation_Return* New_RFIDValueOperation_Return_Array(int cnt)
{
    RFIDValueOperation_Return*rt = malloc( sizeof( RFIDValueOperation_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RFIDValueOperation_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &RFIDValueOperation_Return_ToBytes;
        rt[i].GetFromBytes = &RFIDValueOperation_Return_GetFromBytes;
    }
    return rt;
}
IoSimple_Send* New_IoSimple_Send_Array(int cnt)
{
    IoSimple_Send*rt = malloc( sizeof( IoSimple_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoSimple_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoSimple_Send_ToBytes;
        rt[i].GetFromBytes = &IoSimple_Send_GetFromBytes;
    }
    return rt;
}
IoSimple_Return* New_IoSimple_Return_Array(int cnt)
{
    IoSimple_Return*rt = malloc( sizeof( IoSimple_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoSimple_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoSimple_Return_ToBytes;
        rt[i].GetFromBytes = &IoSimple_Return_GetFromBytes;
    }
    return rt;
}
IoOperation_Send_m1_IoArgs_m1_Controls* New_IoOperation_Send_m1_IoArgs_m1_Controls_Array(int cnt)
{
    IoOperation_Send_m1_IoArgs_m1_Controls*rt = malloc( sizeof( IoOperation_Send_m1_IoArgs_m1_Controls)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send_m1_IoArgs_m1_Controls)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoOperation_Send_m1_IoArgs_m1_Controls_ToBytes;
        rt[i].GetFromBytes = &IoOperation_Send_m1_IoArgs_m1_Controls_GetFromBytes;
    }
    return rt;
}
IoOperation_Send_m1_IoArgs* New_IoOperation_Send_m1_IoArgs_Array(int cnt)
{
    IoOperation_Send_m1_IoArgs*rt = malloc( sizeof( IoOperation_Send_m1_IoArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send_m1_IoArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoOperation_Send_m1_IoArgs_ToBytes;
        rt[i].GetFromBytes = &IoOperation_Send_m1_IoArgs_GetFromBytes;
    }
    return rt;
}
IoOperation_Send* New_IoOperation_Send_Array(int cnt)
{
    IoOperation_Send*rt = malloc( sizeof( IoOperation_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoOperation_Send_ToBytes;
        rt[i].GetFromBytes = &IoOperation_Send_GetFromBytes;
    }
    return rt;
}
IoOperation_Return* New_IoOperation_Return_Array(int cnt)
{
    IoOperation_Return*rt = malloc( sizeof( IoOperation_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoOperation_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoOperation_Return_ToBytes;
        rt[i].GetFromBytes = &IoOperation_Return_GetFromBytes;
    }
    return rt;
}
IoBlink_Send* New_IoBlink_Send_Array(int cnt)
{
    IoBlink_Send*rt = malloc( sizeof( IoBlink_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoBlink_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoBlink_Send_ToBytes;
        rt[i].GetFromBytes = &IoBlink_Send_GetFromBytes;
    }
    return rt;
}
IoBlink_Return* New_IoBlink_Return_Array(int cnt)
{
    IoBlink_Return*rt = malloc( sizeof( IoBlink_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IoBlink_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IoBlink_Return_ToBytes;
        rt[i].GetFromBytes = &IoBlink_Return_GetFromBytes;
    }
    return rt;
}
UartChangeBode_Send* New_UartChangeBode_Send_Array(int cnt)
{
    UartChangeBode_Send*rt = malloc( sizeof( UartChangeBode_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartChangeBode_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartChangeBode_Send_ToBytes;
        rt[i].GetFromBytes = &UartChangeBode_Send_GetFromBytes;
    }
    return rt;
}
UartChangeBode_Return* New_UartChangeBode_Return_Array(int cnt)
{
    UartChangeBode_Return*rt = malloc( sizeof( UartChangeBode_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( UartChangeBode_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &UartChangeBode_Return_ToBytes;
        rt[i].GetFromBytes = &UartChangeBode_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorMultiRotate_Send_m1_MoveArgs* New_ServoMotorMultiRotate_Send_m1_MoveArgs_Array(int cnt)
{
    ServoMotorMultiRotate_Send_m1_MoveArgs*rt = malloc( sizeof( ServoMotorMultiRotate_Send_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Send_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorMultiRotate_Send_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &ServoMotorMultiRotate_Send_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
ServoMotorMultiRotate_Send* New_ServoMotorMultiRotate_Send_Array(int cnt)
{
    ServoMotorMultiRotate_Send*rt = malloc( sizeof( ServoMotorMultiRotate_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorMultiRotate_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorMultiRotate_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorMultiRotate_Return* New_ServoMotorMultiRotate_Return_Array(int cnt)
{
    ServoMotorMultiRotate_Return*rt = malloc( sizeof( ServoMotorMultiRotate_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMultiRotate_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorMultiRotate_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorMultiRotate_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateShakeDetect_Send* New_ServoMotorRotateShakeDetect_Send_Array(int cnt)
{
    ServoMotorRotateShakeDetect_Send*rt = malloc( sizeof( ServoMotorRotateShakeDetect_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeDetect_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateShakeDetect_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateShakeDetect_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateShakeDetect_Return* New_ServoMotorRotateShakeDetect_Return_Array(int cnt)
{
    ServoMotorRotateShakeDetect_Return*rt = malloc( sizeof( ServoMotorRotateShakeDetect_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeDetect_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateShakeDetect_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateShakeDetect_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateShakeRotateDetect_Send* New_ServoMotorRotateShakeRotateDetect_Send_Array(int cnt)
{
    ServoMotorRotateShakeRotateDetect_Send*rt = malloc( sizeof( ServoMotorRotateShakeRotateDetect_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeRotateDetect_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateShakeRotateDetect_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateShakeRotateDetect_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateShakeRotateDetect_Return* New_ServoMotorRotateShakeRotateDetect_Return_Array(int cnt)
{
    ServoMotorRotateShakeRotateDetect_Return*rt = malloc( sizeof( ServoMotorRotateShakeRotateDetect_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateShakeRotateDetect_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateShakeRotateDetect_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateShakeRotateDetect_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateTwoDirOnTime_Send* New_ServoMotorRotateTwoDirOnTime_Send_Array(int cnt)
{
    ServoMotorRotateTwoDirOnTime_Send*rt = malloc( sizeof( ServoMotorRotateTwoDirOnTime_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateTwoDirOnTime_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateTwoDirOnTime_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateTwoDirOnTime_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateTwoDirOnTime_Return* New_ServoMotorRotateTwoDirOnTime_Return_Array(int cnt)
{
    ServoMotorRotateTwoDirOnTime_Return*rt = malloc( sizeof( ServoMotorRotateTwoDirOnTime_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateTwoDirOnTime_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateTwoDirOnTime_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateTwoDirOnTime_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorStatetGet_Send* New_ServoMotorStatetGet_Send_Array(int cnt)
{
    ServoMotorStatetGet_Send*rt = malloc( sizeof( ServoMotorStatetGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorStatetGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorStatetGet_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorStatetGet_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorStatetGet_Return* New_ServoMotorStatetGet_Return_Array(int cnt)
{
    ServoMotorStatetGet_Return*rt = malloc( sizeof( ServoMotorStatetGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorStatetGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorStatetGet_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorStatetGet_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateStart_Send* New_ServoMotorRotateStart_Send_Array(int cnt)
{
    ServoMotorRotateStart_Send*rt = malloc( sizeof( ServoMotorRotateStart_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStart_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateStart_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateStart_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateStart_Return* New_ServoMotorRotateStart_Return_Array(int cnt)
{
    ServoMotorRotateStart_Return*rt = malloc( sizeof( ServoMotorRotateStart_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStart_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateStart_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateStart_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateStop_Send* New_ServoMotorRotateStop_Send_Array(int cnt)
{
    ServoMotorRotateStop_Send*rt = malloc( sizeof( ServoMotorRotateStop_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStop_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateStop_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateStop_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorRotateStop_Return* New_ServoMotorRotateStop_Return_Array(int cnt)
{
    ServoMotorRotateStop_Return*rt = malloc( sizeof( ServoMotorRotateStop_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorRotateStop_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorRotateStop_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorRotateStop_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorReset_Send* New_ServoMotorReset_Send_Array(int cnt)
{
    ServoMotorReset_Send*rt = malloc( sizeof( ServoMotorReset_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorReset_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorReset_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorReset_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorReset_Return* New_ServoMotorReset_Return_Array(int cnt)
{
    ServoMotorReset_Return*rt = malloc( sizeof( ServoMotorReset_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorReset_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorReset_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorReset_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorAutoCalibratio_Send* New_ServoMotorAutoCalibratio_Send_Array(int cnt)
{
    ServoMotorAutoCalibratio_Send*rt = malloc( sizeof( ServoMotorAutoCalibratio_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorAutoCalibratio_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorAutoCalibratio_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorAutoCalibratio_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorAutoCalibratio_Return* New_ServoMotorAutoCalibratio_Return_Array(int cnt)
{
    ServoMotorAutoCalibratio_Return*rt = malloc( sizeof( ServoMotorAutoCalibratio_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorAutoCalibratio_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorAutoCalibratio_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorAutoCalibratio_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorShake_Send* New_ServoMotorShake_Send_Array(int cnt)
{
    ServoMotorShake_Send*rt = malloc( sizeof( ServoMotorShake_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorShake_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorShake_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorShake_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorShake_Return* New_ServoMotorShake_Return_Array(int cnt)
{
    ServoMotorShake_Return*rt = malloc( sizeof( ServoMotorShake_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorShake_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorShake_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorShake_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorBaseCtrl_Send* New_ServoMotorBaseCtrl_Send_Array(int cnt)
{
    ServoMotorBaseCtrl_Send*rt = malloc( sizeof( ServoMotorBaseCtrl_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorBaseCtrl_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorBaseCtrl_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorBaseCtrl_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorBaseCtrl_Return* New_ServoMotorBaseCtrl_Return_Array(int cnt)
{
    ServoMotorBaseCtrl_Return*rt = malloc( sizeof( ServoMotorBaseCtrl_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorBaseCtrl_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorBaseCtrl_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorBaseCtrl_Return_GetFromBytes;
    }
    return rt;
}
ServoMotorMove_Send* New_ServoMotorMove_Send_Array(int cnt)
{
    ServoMotorMove_Send*rt = malloc( sizeof( ServoMotorMove_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMove_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorMove_Send_ToBytes;
        rt[i].GetFromBytes = &ServoMotorMove_Send_GetFromBytes;
    }
    return rt;
}
ServoMotorMove_Return* New_ServoMotorMove_Return_Array(int cnt)
{
    ServoMotorMove_Return*rt = malloc( sizeof( ServoMotorMove_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( ServoMotorMove_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &ServoMotorMove_Return_ToBytes;
        rt[i].GetFromBytes = &ServoMotorMove_Return_GetFromBytes;
    }
    return rt;
}
RGBSensorGet_Send* New_RGBSensorGet_Send_Array(int cnt)
{
    RGBSensorGet_Send*rt = malloc( sizeof( RGBSensorGet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RGBSensorGet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &RGBSensorGet_Send_ToBytes;
        rt[i].GetFromBytes = &RGBSensorGet_Send_GetFromBytes;
    }
    return rt;
}
RGBSensorGet_Return* New_RGBSensorGet_Return_Array(int cnt)
{
    RGBSensorGet_Return*rt = malloc( sizeof( RGBSensorGet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( RGBSensorGet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &RGBSensorGet_Return_ToBytes;
        rt[i].GetFromBytes = &RGBSensorGet_Return_GetFromBytes;
    }
    return rt;
}
SerialDataReport_Send* New_SerialDataReport_Send_Array(int cnt)
{
    SerialDataReport_Send*rt = malloc( sizeof( SerialDataReport_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SerialDataReport_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SerialDataReport_Send_ToBytes;
        rt[i].GetFromBytes = &SerialDataReport_Send_GetFromBytes;
    }
    return rt;
}
SerialDataReport_Return* New_SerialDataReport_Return_Array(int cnt)
{
    SerialDataReport_Return*rt = malloc( sizeof( SerialDataReport_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SerialDataReport_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SerialDataReport_Return_ToBytes;
        rt[i].GetFromBytes = &SerialDataReport_Return_GetFromBytes;
    }
    return rt;
}
AddressBroadcastSend_Send* New_AddressBroadcastSend_Send_Array(int cnt)
{
    AddressBroadcastSend_Send*rt = malloc( sizeof( AddressBroadcastSend_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastSend_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AddressBroadcastSend_Send_ToBytes;
        rt[i].GetFromBytes = &AddressBroadcastSend_Send_GetFromBytes;
    }
    return rt;
}
AddressBroadcastSend_Return* New_AddressBroadcastSend_Return_Array(int cnt)
{
    AddressBroadcastSend_Return*rt = malloc( sizeof( AddressBroadcastSend_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastSend_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AddressBroadcastSend_Return_ToBytes;
        rt[i].GetFromBytes = &AddressBroadcastSend_Return_GetFromBytes;
    }
    return rt;
}
AddressBroadcastReceive_Send* New_AddressBroadcastReceive_Send_Array(int cnt)
{
    AddressBroadcastReceive_Send*rt = malloc( sizeof( AddressBroadcastReceive_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastReceive_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AddressBroadcastReceive_Send_ToBytes;
        rt[i].GetFromBytes = &AddressBroadcastReceive_Send_GetFromBytes;
    }
    return rt;
}
AddressBroadcastReceive_Return* New_AddressBroadcastReceive_Return_Array(int cnt)
{
    AddressBroadcastReceive_Return*rt = malloc( sizeof( AddressBroadcastReceive_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( AddressBroadcastReceive_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &AddressBroadcastReceive_Return_ToBytes;
        rt[i].GetFromBytes = &AddressBroadcastReceive_Return_GetFromBytes;
    }
    return rt;
}
StartCheckPress_Send* New_StartCheckPress_Send_Array(int cnt)
{
    StartCheckPress_Send*rt = malloc( sizeof( StartCheckPress_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( StartCheckPress_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &StartCheckPress_Send_ToBytes;
        rt[i].GetFromBytes = &StartCheckPress_Send_GetFromBytes;
    }
    return rt;
}
StartCheckPress_Return* New_StartCheckPress_Return_Array(int cnt)
{
    StartCheckPress_Return*rt = malloc( sizeof( StartCheckPress_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( StartCheckPress_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &StartCheckPress_Return_ToBytes;
        rt[i].GetFromBytes = &StartCheckPress_Return_GetFromBytes;
    }
    return rt;
}
EndCheckPress_Send* New_EndCheckPress_Send_Array(int cnt)
{
    EndCheckPress_Send*rt = malloc( sizeof( EndCheckPress_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EndCheckPress_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EndCheckPress_Send_ToBytes;
        rt[i].GetFromBytes = &EndCheckPress_Send_GetFromBytes;
    }
    return rt;
}
EndCheckPress_Return* New_EndCheckPress_Return_Array(int cnt)
{
    EndCheckPress_Return*rt = malloc( sizeof( EndCheckPress_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( EndCheckPress_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &EndCheckPress_Return_ToBytes;
        rt[i].GetFromBytes = &EndCheckPress_Return_GetFromBytes;
    }
    return rt;
}
Blending_Send* New_Blending_Send_Array(int cnt)
{
    Blending_Send*rt = malloc( sizeof( Blending_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Blending_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Blending_Send_ToBytes;
        rt[i].GetFromBytes = &Blending_Send_GetFromBytes;
    }
    return rt;
}
Blending_Return* New_Blending_Return_Array(int cnt)
{
    Blending_Return*rt = malloc( sizeof( Blending_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( Blending_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &Blending_Return_ToBytes;
        rt[i].GetFromBytes = &Blending_Return_GetFromBytes;
    }
    return rt;
}
GetEPOSType_Send* New_GetEPOSType_Send_Array(int cnt)
{
    GetEPOSType_Send*rt = malloc( sizeof( GetEPOSType_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( GetEPOSType_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &GetEPOSType_Send_ToBytes;
        rt[i].GetFromBytes = &GetEPOSType_Send_GetFromBytes;
    }
    return rt;
}
GetEPOSType_Return* New_GetEPOSType_Return_Array(int cnt)
{
    GetEPOSType_Return*rt = malloc( sizeof( GetEPOSType_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( GetEPOSType_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &GetEPOSType_Return_ToBytes;
        rt[i].GetFromBytes = &GetEPOSType_Return_GetFromBytes;
    }
    return rt;
}
SetEPOSType_Send* New_SetEPOSType_Send_Array(int cnt)
{
    SetEPOSType_Send*rt = malloc( sizeof( SetEPOSType_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SetEPOSType_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SetEPOSType_Send_ToBytes;
        rt[i].GetFromBytes = &SetEPOSType_Send_GetFromBytes;
    }
    return rt;
}
SetEPOSType_Return* New_SetEPOSType_Return_Array(int cnt)
{
    SetEPOSType_Return*rt = malloc( sizeof( SetEPOSType_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SetEPOSType_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SetEPOSType_Return_ToBytes;
        rt[i].GetFromBytes = &SetEPOSType_Return_GetFromBytes;
    }
    return rt;
}
DACSet_Send* New_DACSet_Send_Array(int cnt)
{
    DACSet_Send*rt = malloc( sizeof( DACSet_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DACSet_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DACSet_Send_ToBytes;
        rt[i].GetFromBytes = &DACSet_Send_GetFromBytes;
    }
    return rt;
}
DACSet_Return* New_DACSet_Return_Array(int cnt)
{
    DACSet_Return*rt = malloc( sizeof( DACSet_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DACSet_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DACSet_Return_ToBytes;
        rt[i].GetFromBytes = &DACSet_Return_GetFromBytes;
    }
    return rt;
}
LightCtrl_Send* New_LightCtrl_Send_Array(int cnt)
{
    LightCtrl_Send*rt = malloc( sizeof( LightCtrl_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LightCtrl_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LightCtrl_Send_ToBytes;
        rt[i].GetFromBytes = &LightCtrl_Send_GetFromBytes;
    }
    return rt;
}
LightCtrl_Return* New_LightCtrl_Return_Array(int cnt)
{
    LightCtrl_Return*rt = malloc( sizeof( LightCtrl_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LightCtrl_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LightCtrl_Return_ToBytes;
        rt[i].GetFromBytes = &LightCtrl_Return_GetFromBytes;
    }
    return rt;
}
DCMotorMoveSeq_Send_m1_MoveArgs* New_DCMotorMoveSeq_Send_m1_MoveArgs_Array(int cnt)
{
    DCMotorMoveSeq_Send_m1_MoveArgs*rt = malloc( sizeof( DCMotorMoveSeq_Send_m1_MoveArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Send_m1_MoveArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotorMoveSeq_Send_m1_MoveArgs_ToBytes;
        rt[i].GetFromBytes = &DCMotorMoveSeq_Send_m1_MoveArgs_GetFromBytes;
    }
    return rt;
}
DCMotorMoveSeq_Send* New_DCMotorMoveSeq_Send_Array(int cnt)
{
    DCMotorMoveSeq_Send*rt = malloc( sizeof( DCMotorMoveSeq_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotorMoveSeq_Send_ToBytes;
        rt[i].GetFromBytes = &DCMotorMoveSeq_Send_GetFromBytes;
    }
    return rt;
}
DCMotorMoveSeq_Return* New_DCMotorMoveSeq_Return_Array(int cnt)
{
    DCMotorMoveSeq_Return*rt = malloc( sizeof( DCMotorMoveSeq_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveSeq_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotorMoveSeq_Return_ToBytes;
        rt[i].GetFromBytes = &DCMotorMoveSeq_Return_GetFromBytes;
    }
    return rt;
}
DCMotorMoveTmc_Send* New_DCMotorMoveTmc_Send_Array(int cnt)
{
    DCMotorMoveTmc_Send*rt = malloc( sizeof( DCMotorMoveTmc_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveTmc_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotorMoveTmc_Send_ToBytes;
        rt[i].GetFromBytes = &DCMotorMoveTmc_Send_GetFromBytes;
    }
    return rt;
}
DCMotorMoveTmc_Return* New_DCMotorMoveTmc_Return_Array(int cnt)
{
    DCMotorMoveTmc_Return*rt = malloc( sizeof( DCMotorMoveTmc_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( DCMotorMoveTmc_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &DCMotorMoveTmc_Return_ToBytes;
        rt[i].GetFromBytes = &DCMotorMoveTmc_Return_GetFromBytes;
    }
    return rt;
}
PressureRead_Send* New_PressureRead_Send_Array(int cnt)
{
    PressureRead_Send*rt = malloc( sizeof( PressureRead_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureRead_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureRead_Send_ToBytes;
        rt[i].GetFromBytes = &PressureRead_Send_GetFromBytes;
    }
    return rt;
}
PressureRead_Return* New_PressureRead_Return_Array(int cnt)
{
    PressureRead_Return*rt = malloc( sizeof( PressureRead_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureRead_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureRead_Return_ToBytes;
        rt[i].GetFromBytes = &PressureRead_Return_GetFromBytes;
    }
    return rt;
}
LittleMotorCtrl_Send* New_LittleMotorCtrl_Send_Array(int cnt)
{
    LittleMotorCtrl_Send*rt = malloc( sizeof( LittleMotorCtrl_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LittleMotorCtrl_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LittleMotorCtrl_Send_ToBytes;
        rt[i].GetFromBytes = &LittleMotorCtrl_Send_GetFromBytes;
    }
    return rt;
}
LittleMotorCtrl_Return* New_LittleMotorCtrl_Return_Array(int cnt)
{
    LittleMotorCtrl_Return*rt = malloc( sizeof( LittleMotorCtrl_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LittleMotorCtrl_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LittleMotorCtrl_Return_ToBytes;
        rt[i].GetFromBytes = &LittleMotorCtrl_Return_GetFromBytes;
    }
    return rt;
}
CongestionDetection_Send* New_CongestionDetection_Send_Array(int cnt)
{
    CongestionDetection_Send*rt = malloc( sizeof( CongestionDetection_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( CongestionDetection_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &CongestionDetection_Send_ToBytes;
        rt[i].GetFromBytes = &CongestionDetection_Send_GetFromBytes;
    }
    return rt;
}
CongestionDetection_Return* New_CongestionDetection_Return_Array(int cnt)
{
    CongestionDetection_Return*rt = malloc( sizeof( CongestionDetection_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( CongestionDetection_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &CongestionDetection_Return_ToBytes;
        rt[i].GetFromBytes = &CongestionDetection_Return_GetFromBytes;
    }
    return rt;
}
LaserCtrol_Send* New_LaserCtrol_Send_Array(int cnt)
{
    LaserCtrol_Send*rt = malloc( sizeof( LaserCtrol_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LaserCtrol_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LaserCtrol_Send_ToBytes;
        rt[i].GetFromBytes = &LaserCtrol_Send_GetFromBytes;
    }
    return rt;
}
LaserCtrol_Return* New_LaserCtrol_Return_Array(int cnt)
{
    LaserCtrol_Return*rt = malloc( sizeof( LaserCtrol_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( LaserCtrol_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &LaserCtrol_Return_ToBytes;
        rt[i].GetFromBytes = &LaserCtrol_Return_GetFromBytes;
    }
    return rt;
}
NFL_ReadConfig_Send* New_NFL_ReadConfig_Send_Array(int cnt)
{
    NFL_ReadConfig_Send*rt = malloc( sizeof( NFL_ReadConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadConfig_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadConfig_Send_GetFromBytes;
    }
    return rt;
}
NFL_ReadConfig_Return* New_NFL_ReadConfig_Return_Array(int cnt)
{
    NFL_ReadConfig_Return*rt = malloc( sizeof( NFL_ReadConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadConfig_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadConfig_Return_GetFromBytes;
    }
    return rt;
}
NFL_WriteConfig_Send* New_NFL_WriteConfig_Send_Array(int cnt)
{
    NFL_WriteConfig_Send*rt = malloc( sizeof( NFL_WriteConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_WriteConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_WriteConfig_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_WriteConfig_Send_GetFromBytes;
    }
    return rt;
}
NFL_WriteConfig_Return* New_NFL_WriteConfig_Return_Array(int cnt)
{
    NFL_WriteConfig_Return*rt = malloc( sizeof( NFL_WriteConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_WriteConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_WriteConfig_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_WriteConfig_Return_GetFromBytes;
    }
    return rt;
}
NFL_ReadDataStatus_Send* New_NFL_ReadDataStatus_Send_Array(int cnt)
{
    NFL_ReadDataStatus_Send*rt = malloc( sizeof( NFL_ReadDataStatus_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadDataStatus_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadDataStatus_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadDataStatus_Send_GetFromBytes;
    }
    return rt;
}
NFL_ReadDataStatus_Return* New_NFL_ReadDataStatus_Return_Array(int cnt)
{
    NFL_ReadDataStatus_Return*rt = malloc( sizeof( NFL_ReadDataStatus_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadDataStatus_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadDataStatus_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadDataStatus_Return_GetFromBytes;
    }
    return rt;
}
NFL_ReadData_Send* New_NFL_ReadData_Send_Array(int cnt)
{
    NFL_ReadData_Send*rt = malloc( sizeof( NFL_ReadData_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadData_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadData_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadData_Send_GetFromBytes;
    }
    return rt;
}
NFL_ReadData_Return* New_NFL_ReadData_Return_Array(int cnt)
{
    NFL_ReadData_Return*rt = malloc( sizeof( NFL_ReadData_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadData_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadData_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadData_Return_GetFromBytes;
    }
    return rt;
}
NFL_DataCollectStart_Send* New_NFL_DataCollectStart_Send_Array(int cnt)
{
    NFL_DataCollectStart_Send*rt = malloc( sizeof( NFL_DataCollectStart_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStart_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_DataCollectStart_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_DataCollectStart_Send_GetFromBytes;
    }
    return rt;
}
NFL_DataCollectStart_Return* New_NFL_DataCollectStart_Return_Array(int cnt)
{
    NFL_DataCollectStart_Return*rt = malloc( sizeof( NFL_DataCollectStart_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStart_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_DataCollectStart_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_DataCollectStart_Return_GetFromBytes;
    }
    return rt;
}
NFL_DataCollectStop_Send* New_NFL_DataCollectStop_Send_Array(int cnt)
{
    NFL_DataCollectStop_Send*rt = malloc( sizeof( NFL_DataCollectStop_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStop_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_DataCollectStop_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_DataCollectStop_Send_GetFromBytes;
    }
    return rt;
}
NFL_DataCollectStop_Return* New_NFL_DataCollectStop_Return_Array(int cnt)
{
    NFL_DataCollectStop_Return*rt = malloc( sizeof( NFL_DataCollectStop_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_DataCollectStop_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_DataCollectStop_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_DataCollectStop_Return_GetFromBytes;
    }
    return rt;
}
NFL_ThresholdConfig_Send* New_NFL_ThresholdConfig_Send_Array(int cnt)
{
    NFL_ThresholdConfig_Send*rt = malloc( sizeof( NFL_ThresholdConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ThresholdConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ThresholdConfig_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ThresholdConfig_Send_GetFromBytes;
    }
    return rt;
}
NFL_ThresholdConfig_Return* New_NFL_ThresholdConfig_Return_Array(int cnt)
{
    NFL_ThresholdConfig_Return*rt = malloc( sizeof( NFL_ThresholdConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ThresholdConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ThresholdConfig_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ThresholdConfig_Return_GetFromBytes;
    }
    return rt;
}
NFL_StandbyConfig_Send* New_NFL_StandbyConfig_Send_Array(int cnt)
{
    NFL_StandbyConfig_Send*rt = malloc( sizeof( NFL_StandbyConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_StandbyConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_StandbyConfig_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_StandbyConfig_Send_GetFromBytes;
    }
    return rt;
}
NFL_StandbyConfig_Return* New_NFL_StandbyConfig_Return_Array(int cnt)
{
    NFL_StandbyConfig_Return*rt = malloc( sizeof( NFL_StandbyConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_StandbyConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_StandbyConfig_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_StandbyConfig_Return_GetFromBytes;
    }
    return rt;
}
NFL_ActivateConfig_Send* New_NFL_ActivateConfig_Send_Array(int cnt)
{
    NFL_ActivateConfig_Send*rt = malloc( sizeof( NFL_ActivateConfig_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ActivateConfig_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ActivateConfig_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ActivateConfig_Send_GetFromBytes;
    }
    return rt;
}
NFL_ActivateConfig_Return* New_NFL_ActivateConfig_Return_Array(int cnt)
{
    NFL_ActivateConfig_Return*rt = malloc( sizeof( NFL_ActivateConfig_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ActivateConfig_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ActivateConfig_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ActivateConfig_Return_GetFromBytes;
    }
    return rt;
}
NFL_ReadTemperature_Send* New_NFL_ReadTemperature_Send_Array(int cnt)
{
    NFL_ReadTemperature_Send*rt = malloc( sizeof( NFL_ReadTemperature_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadTemperature_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadTemperature_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadTemperature_Send_GetFromBytes;
    }
    return rt;
}
NFL_ReadTemperature_Return* New_NFL_ReadTemperature_Return_Array(int cnt)
{
    NFL_ReadTemperature_Return*rt = malloc( sizeof( NFL_ReadTemperature_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_ReadTemperature_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_ReadTemperature_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_ReadTemperature_Return_GetFromBytes;
    }
    return rt;
}
NFL_SendEvent_Send* New_NFL_SendEvent_Send_Array(int cnt)
{
    NFL_SendEvent_Send*rt = malloc( sizeof( NFL_SendEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_SendEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_SendEvent_Send_ToBytes;
        rt[i].GetFromBytes = &NFL_SendEvent_Send_GetFromBytes;
    }
    return rt;
}
NFL_SendEvent_Return* New_NFL_SendEvent_Return_Array(int cnt)
{
    NFL_SendEvent_Return*rt = malloc( sizeof( NFL_SendEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( NFL_SendEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &NFL_SendEvent_Return_ToBytes;
        rt[i].GetFromBytes = &NFL_SendEvent_Return_GetFromBytes;
    }
    return rt;
}
PowerCtrl_Send* New_PowerCtrl_Send_Array(int cnt)
{
    PowerCtrl_Send*rt = malloc( sizeof( PowerCtrl_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PowerCtrl_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PowerCtrl_Send_ToBytes;
        rt[i].GetFromBytes = &PowerCtrl_Send_GetFromBytes;
    }
    return rt;
}
PowerCtrl_Return* New_PowerCtrl_Return_Array(int cnt)
{
    PowerCtrl_Return*rt = malloc( sizeof( PowerCtrl_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PowerCtrl_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PowerCtrl_Return_ToBytes;
        rt[i].GetFromBytes = &PowerCtrl_Return_GetFromBytes;
    }
    return rt;
}
PressureListen_Send* New_PressureListen_Send_Array(int cnt)
{
    PressureListen_Send*rt = malloc( sizeof( PressureListen_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureListen_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureListen_Send_ToBytes;
        rt[i].GetFromBytes = &PressureListen_Send_GetFromBytes;
    }
    return rt;
}
PressureListen_Return* New_PressureListen_Return_Array(int cnt)
{
    PressureListen_Return*rt = malloc( sizeof( PressureListen_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( PressureListen_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &PressureListen_Return_ToBytes;
        rt[i].GetFromBytes = &PressureListen_Return_GetFromBytes;
    }
    return rt;
}
SendErrorCode_Send* New_SendErrorCode_Send_Array(int cnt)
{
    SendErrorCode_Send*rt = malloc( sizeof( SendErrorCode_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SendErrorCode_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SendErrorCode_Send_ToBytes;
        rt[i].GetFromBytes = &SendErrorCode_Send_GetFromBytes;
    }
    return rt;
}
SendErrorCode_Return* New_SendErrorCode_Return_Array(int cnt)
{
    SendErrorCode_Return*rt = malloc( sizeof( SendErrorCode_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( SendErrorCode_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &SendErrorCode_Return_ToBytes;
        rt[i].GetFromBytes = &SendErrorCode_Return_GetFromBytes;
    }
    return rt;
}
IOCtrlByEvent_Send_m1_PortArgs* New_IOCtrlByEvent_Send_m1_PortArgs_Array(int cnt)
{
    IOCtrlByEvent_Send_m1_PortArgs*rt = malloc( sizeof( IOCtrlByEvent_Send_m1_PortArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send_m1_PortArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IOCtrlByEvent_Send_m1_PortArgs_ToBytes;
        rt[i].GetFromBytes = &IOCtrlByEvent_Send_m1_PortArgs_GetFromBytes;
    }
    return rt;
}
IOCtrlByEvent_Send_m2_PortArgs* New_IOCtrlByEvent_Send_m2_PortArgs_Array(int cnt)
{
    IOCtrlByEvent_Send_m2_PortArgs*rt = malloc( sizeof( IOCtrlByEvent_Send_m2_PortArgs)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send_m2_PortArgs)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IOCtrlByEvent_Send_m2_PortArgs_ToBytes;
        rt[i].GetFromBytes = &IOCtrlByEvent_Send_m2_PortArgs_GetFromBytes;
    }
    return rt;
}
IOCtrlByEvent_Send* New_IOCtrlByEvent_Send_Array(int cnt)
{
    IOCtrlByEvent_Send*rt = malloc( sizeof( IOCtrlByEvent_Send)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Send)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IOCtrlByEvent_Send_ToBytes;
        rt[i].GetFromBytes = &IOCtrlByEvent_Send_GetFromBytes;
    }
    return rt;
}
IOCtrlByEvent_Return* New_IOCtrlByEvent_Return_Array(int cnt)
{
    IOCtrlByEvent_Return*rt = malloc( sizeof( IOCtrlByEvent_Return)*cnt ) ;
    PTR_ASSERT(rt);
    memset(rt,0, sizeof( IOCtrlByEvent_Return)*cnt ) ;
    for(int i =0;i<cnt;i++){
        rt[i].ToBytes = &IOCtrlByEvent_Return_ToBytes;
        rt[i].GetFromBytes = &IOCtrlByEvent_Return_GetFromBytes;
    }
    return rt;
}
void Delete_Dsp_DHCP_Send(Dsp_DHCP_Send* ths)
{
    free(ths) ;
}
void Delete_Dsp_DHCP_Return(Dsp_DHCP_Return* ths)
{
    free(ths) ;
}
void Delete_Ping_Send(Ping_Send* ths)
{
    free(ths) ;
}
void Delete_Ping_Return(Ping_Return* ths)
{
    free(ths) ;
}
void Delete_FilePush_Send(FilePush_Send* ths)
{
    free(ths) ;
}
void Delete_FilePush_Return(FilePush_Return* ths)
{
    free(ths) ;
}
void Delete_InstrumentStateReport_Send(InstrumentStateReport_Send* ths)
{
    free(ths) ;
}
void Delete_InstrumentStateReport_Return(InstrumentStateReport_Return* ths)
{
    free(ths) ;
}
void Delete_ProtoTest_Send(ProtoTest_Send* ths)
{
    free(ths) ;
}
void Delete_ProtoTest_Return(ProtoTest_Return* ths)
{
    free(ths) ;
}
void Delete_McuErrorToPC_Send(McuErrorToPC_Send* ths)
{
    free(ths) ;
}
void Delete_McuErrorToPC_Return(McuErrorToPC_Return* ths)
{
    free(ths) ;
}
void Delete_AutoAdapter_Send(AutoAdapter_Send* ths)
{
    free(ths) ;
}
void Delete_AutoAdapter_Return(AutoAdapter_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMove_Send_m1_MoveArgs(MotorMultiMove_Send_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMove_Send(MotorMultiMove_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMove_Return(MotorMultiMove_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMove_Send(MotorMove_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMove_Return(MotorMove_Return* ths)
{
    free(ths) ;
}
void Delete_MotorStatetGet_Send(MotorStatetGet_Send* ths)
{
    free(ths) ;
}
void Delete_MotorStatetGet_Return(MotorStatetGet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStart_Send(MotorRotateStart_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStart_Return(MotorRotateStart_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStop_Send(MotorRotateStop_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStop_Return(MotorRotateStop_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotate_Send(MotorRotate_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotate_Return(MotorRotate_Return* ths)
{
    free(ths) ;
}
void Delete_MotorReset_Send(MotorReset_Send* ths)
{
    free(ths) ;
}
void Delete_MotorReset_Return(MotorReset_Return* ths)
{
    free(ths) ;
}
void Delete_SensorValueOperation_Send(SensorValueOperation_Send* ths)
{
    free(ths) ;
}
void Delete_SensorValueOperation_Return(SensorValueOperation_Return* ths)
{
    free(ths) ;
}
void Delete_ADAutoCaliStart_Send(ADAutoCaliStart_Send* ths)
{
    free(ths) ;
}
void Delete_ADAutoCaliStart_Return(ADAutoCaliStart_Return* ths)
{
    free(ths) ;
}
void Delete_TemperatureGet_Send(TemperatureGet_Send* ths)
{
    free(ths) ;
}
void Delete_TemperatureGet_Return(TemperatureGet_Return* ths)
{
    free(ths) ;
}
void Delete_TemperatureOpen_Send(TemperatureOpen_Send* ths)
{
    free(ths) ;
}
void Delete_TemperatureOpen_Return(TemperatureOpen_Return* ths)
{
    free(ths) ;
}
void Delete_TemperatureClose_Send(TemperatureClose_Send* ths)
{
    free(ths) ;
}
void Delete_TemperatureClose_Return(TemperatureClose_Return* ths)
{
    free(ths) ;
}
void Delete_BarCodeGet_Send(BarCodeGet_Send* ths)
{
    free(ths) ;
}
void Delete_BarCodeGet_Return(BarCodeGet_Return* ths)
{
    free(ths) ;
}
void Delete_FirmWareConfig_Send(FirmWareConfig_Send* ths)
{
    free(ths) ;
}
void Delete_FirmWareConfig_Return(FirmWareConfig_Return* ths)
{
    free(ths) ;
}
void Delete_FirmWareTransmitFile_Send(FirmWareTransmitFile_Send* ths)
{
    free(ths) ;
}
void Delete_FirmWareTransmitFile_Return(FirmWareTransmitFile_Return* ths)
{
    free(ths) ;
}
void Delete_McuReset_Send(McuReset_Send* ths)
{
    free(ths) ;
}
void Delete_McuReset_Return(McuReset_Return* ths)
{
    free(ths) ;
}
void Delete_FirmWareVersionGet_Send(FirmWareVersionGet_Send* ths)
{
    free(ths) ;
}
void Delete_FirmWareVersionGet_Return(FirmWareVersionGet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSet_Send(MotorConfigSet_Send* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSet_Return(MotorConfigSet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMoveEvent_Send(MotorMoveEvent_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveEvent_Return(MotorMoveEvent_Return* ths)
{
    free(ths) ;
}
void Delete_McuIoStateChangeEvent_Send(McuIoStateChangeEvent_Send* ths)
{
    free(ths) ;
}
void Delete_McuIoStateChangeEvent_Return(McuIoStateChangeEvent_Return* ths)
{
    free(ths) ;
}
void Delete_SampleUnitSensorGet_Send(SampleUnitSensorGet_Send* ths)
{
    free(ths) ;
}
void Delete_SampleUnitSensorGet_Return(SampleUnitSensorGet_Return* ths)
{
    free(ths) ;
}
void Delete_AirPumpOpen_Send(AirPumpOpen_Send* ths)
{
    free(ths) ;
}
void Delete_AirPumpOpen_Return(AirPumpOpen_Return* ths)
{
    free(ths) ;
}
void Delete_AirPumpClose_Send(AirPumpClose_Send* ths)
{
    free(ths) ;
}
void Delete_AirPumpClose_Return(AirPumpClose_Return* ths)
{
    free(ths) ;
}
void Delete_ADSampleRead_Send(ADSampleRead_Send* ths)
{
    free(ths) ;
}
void Delete_ADSampleRead_Return(ADSampleRead_Return* ths)
{
    free(ths) ;
}
/*
 这里 实现 数据接口 析构函数 仅能释放单个数据接口 未实现接口数组的释放
*/

void Delete_DiskInstalledRed_Send(DiskInstalledRed_Send* ths)
{
		free(ths) ;
}
void Delete_DiskInstalledRed_Return(DiskInstalledRed_Return* ths)
{
		free(ths) ;
}


void Delete_MotorMoveGPIOEvent_Send(MotorMoveGPIOEvent_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveGPIOEvent_Return(MotorMoveGPIOEvent_Return* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSetNew_Send(MotorConfigSetNew_Send* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSetNew_Return(MotorConfigSetNew_Return* ths)
{
    free(ths) ;
}
void Delete_ExecutesCtrl_Send(ExecutesCtrl_Send* ths)
{
    free(ths) ;
}
void Delete_ExecutesCtrl_Return(ExecutesCtrl_Return* ths)
{
    free(ths) ;
}
void Delete_ADDetectConfigSet_Send(ADDetectConfigSet_Send* ths)
{
    free(ths) ;
}
void Delete_ADDetectConfigSet_Return(ADDetectConfigSet_Return* ths)
{
    free(ths) ;
}
void Delete_ADDataRead_Send(ADDataRead_Send* ths)
{
    free(ths) ;
}
void Delete_ADDataRead_Return(ADDataRead_Return* ths)
{
    free(ths) ;
}
void Delete_ADDataSeriesSend_Send(ADDataSeriesSend_Send* ths)
{
    free(ths) ;
}
void Delete_ADDataSeriesSend_Return(ADDataSeriesSend_Return* ths)
{
    free(ths) ;
}
void Delete_UartTransmit_Send(UartTransmit_Send* ths)
{
    free(ths) ;
}
void Delete_UartTransmit_Return(UartTransmit_Return* ths)
{
    free(ths) ;
}
void Delete_UartReceiveEvent_Send(UartReceiveEvent_Send* ths)
{
    free(ths) ;
}
void Delete_UartReceiveEvent_Return(UartReceiveEvent_Return* ths)
{
    free(ths) ;
}
void Delete_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs(SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_SeqMotorMultiMove_Send_m1_Sequence(SeqMotorMultiMove_Send_m1_Sequence* ths)
{
    free(ths) ;
}
void Delete_SeqMotorMultiMove_Send(SeqMotorMultiMove_Send* ths)
{
    free(ths) ;
}
void Delete_SeqMotorMultiMove_Return(SeqMotorMultiMove_Return* ths)
{
    free(ths) ;
}
void Delete_PressureConfig_Send(PressureConfig_Send* ths)
{
    free(ths) ;
}
void Delete_PressureConfig_Return(PressureConfig_Return* ths)
{
    free(ths) ;
}
void Delete_PressureDataGet_Send(PressureDataGet_Send* ths)
{
    free(ths) ;
}
void Delete_PressureDataGet_Return(PressureDataGet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorAbort_Send(MotorAbort_Send* ths)
{
    free(ths) ;
}
void Delete_MotorAbort_Return(MotorAbort_Return* ths)
{
    free(ths) ;
}
void Delete_PressureEvent_Send(PressureEvent_Send* ths)
{
    free(ths) ;
}
void Delete_PressureEvent_Return(PressureEvent_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMoveSeq_Send_m1_MoveArgs(MotorMoveSeq_Send_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_MotorMoveSeq_Send(MotorMoveSeq_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveSeq_Return(MotorMoveSeq_Return* ths)
{
    free(ths) ;
}
void Delete_FirmWareReadyEvent_Send(FirmWareReadyEvent_Send* ths)
{
    free(ths) ;
}
void Delete_FirmWareReadyEvent_Return(FirmWareReadyEvent_Return* ths)
{
    free(ths) ;
}
void Delete_MotorConfigGet_Send(MotorConfigGet_Send* ths)
{
    free(ths) ;
}
void Delete_MotorConfigGet_Return(MotorConfigGet_Return* ths)
{
    free(ths) ;
}
void Delete_PWM_Set_Send(PWM_Set_Send* ths)
{
    free(ths) ;
}
void Delete_PWM_Set_Return(PWM_Set_Return* ths)
{
    free(ths) ;
}
void Delete_TurnTable_Move_Send(TurnTable_Move_Send* ths)
{
    free(ths) ;
}
void Delete_TurnTable_Move_Return(TurnTable_Move_Return* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableFindZero_Send(EncodeTurnTableFindZero_Send* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableFindZero_Return(EncodeTurnTableFindZero_Return* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableRotateStart_Send(EncodeTurnTableRotateStart_Send* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableRotateStart_Return(EncodeTurnTableRotateStart_Return* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableRotateStop_Send(EncodeTurnTableRotateStop_Send* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableRotateStop_Return(EncodeTurnTableRotateStop_Return* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableStateGet_Send(EncodeTurnTableStateGet_Send* ths)
{
    free(ths) ;
}
void Delete_EncodeTurnTableStateGet_Return(EncodeTurnTableStateGet_Return* ths)
{
    free(ths) ;
}
void Delete_DCAdRead_Send(DCAdRead_Send* ths)
{
    free(ths) ;
}
void Delete_DCAdRead_Return(DCAdRead_Return* ths)
{
    free(ths) ;
}
void Delete_JSAdRead_Send(JSAdRead_Send* ths)
{
    free(ths) ;
}
void Delete_JSAdRead_Return(JSAdRead_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMoveSyncLevel_Send(MotorMoveSyncLevel_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveSyncLevel_Return(MotorMoveSyncLevel_Return* ths)
{
    free(ths) ;
}
void Delete_DCMotor_Send(DCMotor_Send* ths)
{
    free(ths) ;
}
void Delete_DCMotor_Return(DCMotor_Return* ths)
{
    free(ths) ;
}
void Delete_MotorCupIDSet_Send(MotorCupIDSet_Send* ths)
{
    free(ths) ;
}
void Delete_MotorCupIDSet_Return(MotorCupIDSet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorCupIDInfoGet_Send(MotorCupIDInfoGet_Send* ths)
{
    free(ths) ;
}
void Delete_MotorCupIDInfoGet_Return(MotorCupIDInfoGet_Return* ths)
{
    free(ths) ;
}
void Delete_JSAdDataGet_Send(JSAdDataGet_Send* ths)
{
    free(ths) ;
}
void Delete_JSAdDataGet_Return(JSAdDataGet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMoveTmc_Send_m1_MoveArgs(MotorMultiMoveTmc_Send_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMoveTmc_Send(MotorMultiMoveTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMultiMoveTmc_Return(MotorMultiMoveTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMoveTmc_Send(MotorMoveTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveTmc_Return(MotorMoveTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorStatetGetTmc_Send(MotorStatetGetTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorStatetGetTmc_Return(MotorStatetGetTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStartTmc_Send(MotorRotateStartTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStartTmc_Return(MotorRotateStartTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStopTmc_Send(MotorRotateStopTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateStopTmc_Return(MotorRotateStopTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateTmc_Send(MotorRotateTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateTmc_Return(MotorRotateTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorResetTmc_Send(MotorResetTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorResetTmc_Return(MotorResetTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSetTmc_Send(MotorConfigSetTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorConfigSetTmc_Return(MotorConfigSetTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorMoveGPIOEventTmc_Send(MotorMoveGPIOEventTmc_Send* ths)
{
    free(ths) ;
}
void Delete_MotorMoveGPIOEventTmc_Return(MotorMoveGPIOEventTmc_Return* ths)
{
    free(ths) ;
}
void Delete_MotorRotateShake_Send(MotorRotateShake_Send* ths)
{
    free(ths) ;
}
void Delete_MotorRotateShake_Return(MotorRotateShake_Return* ths)
{
    free(ths) ;
}
void Delete_LiquidLevelResultGet_Send(LiquidLevelResultGet_Send* ths)
{
    free(ths) ;
}
void Delete_LiquidLevelResultGet_Return(LiquidLevelResultGet_Return* ths)
{
    free(ths) ;
}
void Delete_MotorDetectLiquidLevel_Send(MotorDetectLiquidLevel_Send* ths)
{
    free(ths) ;
}
void Delete_MotorDetectLiquidLevel_Return(MotorDetectLiquidLevel_Return* ths)
{
    free(ths) ;
}
void Delete_RFIDValueOperation_Send(RFIDValueOperation_Send* ths)
{
    free(ths) ;
}
void Delete_RFIDValueOperation_Return(RFIDValueOperation_Return* ths)
{
    free(ths) ;
}
void Delete_IoSimple_Send(IoSimple_Send* ths)
{
    free(ths) ;
}
void Delete_IoSimple_Return(IoSimple_Return* ths)
{
    free(ths) ;
}
void Delete_IoOperation_Send_m1_IoArgs_m1_Controls(IoOperation_Send_m1_IoArgs_m1_Controls* ths)
{
    free(ths) ;
}
void Delete_IoOperation_Send_m1_IoArgs(IoOperation_Send_m1_IoArgs* ths)
{
    free(ths) ;
}
void Delete_IoOperation_Send(IoOperation_Send* ths)
{
    free(ths) ;
}
void Delete_IoOperation_Return(IoOperation_Return* ths)
{
    free(ths) ;
}
void Delete_IoBlink_Send(IoBlink_Send* ths)
{
    free(ths) ;
}
void Delete_IoBlink_Return(IoBlink_Return* ths)
{
    free(ths) ;
}
void Delete_UartChangeBode_Send(UartChangeBode_Send* ths)
{
    free(ths) ;
}
void Delete_UartChangeBode_Return(UartChangeBode_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorMultiRotate_Send_m1_MoveArgs(ServoMotorMultiRotate_Send_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_ServoMotorMultiRotate_Send(ServoMotorMultiRotate_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorMultiRotate_Return(ServoMotorMultiRotate_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateShakeDetect_Send(ServoMotorRotateShakeDetect_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateShakeDetect_Return(ServoMotorRotateShakeDetect_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateShakeRotateDetect_Send(ServoMotorRotateShakeRotateDetect_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateShakeRotateDetect_Return(ServoMotorRotateShakeRotateDetect_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateTwoDirOnTime_Send(ServoMotorRotateTwoDirOnTime_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateTwoDirOnTime_Return(ServoMotorRotateTwoDirOnTime_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorStatetGet_Send(ServoMotorStatetGet_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorStatetGet_Return(ServoMotorStatetGet_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateStart_Send(ServoMotorRotateStart_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateStart_Return(ServoMotorRotateStart_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateStop_Send(ServoMotorRotateStop_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorRotateStop_Return(ServoMotorRotateStop_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorReset_Send(ServoMotorReset_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorReset_Return(ServoMotorReset_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorAutoCalibratio_Send(ServoMotorAutoCalibratio_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorAutoCalibratio_Return(ServoMotorAutoCalibratio_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorShake_Send(ServoMotorShake_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorShake_Return(ServoMotorShake_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorBaseCtrl_Send(ServoMotorBaseCtrl_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorBaseCtrl_Return(ServoMotorBaseCtrl_Return* ths)
{
    free(ths) ;
}
void Delete_ServoMotorMove_Send(ServoMotorMove_Send* ths)
{
    free(ths) ;
}
void Delete_ServoMotorMove_Return(ServoMotorMove_Return* ths)
{
    free(ths) ;
}
void Delete_RGBSensorGet_Send(RGBSensorGet_Send* ths)
{
    free(ths) ;
}
void Delete_RGBSensorGet_Return(RGBSensorGet_Return* ths)
{
    free(ths) ;
}
void Delete_SerialDataReport_Send(SerialDataReport_Send* ths)
{
    free(ths) ;
}
void Delete_SerialDataReport_Return(SerialDataReport_Return* ths)
{
    free(ths) ;
}
void Delete_AddressBroadcastSend_Send(AddressBroadcastSend_Send* ths)
{
    free(ths) ;
}
void Delete_AddressBroadcastSend_Return(AddressBroadcastSend_Return* ths)
{
    free(ths) ;
}
void Delete_AddressBroadcastReceive_Send(AddressBroadcastReceive_Send* ths)
{
    free(ths) ;
}
void Delete_AddressBroadcastReceive_Return(AddressBroadcastReceive_Return* ths)
{
    free(ths) ;
}
void Delete_StartCheckPress_Send(StartCheckPress_Send* ths)
{
    free(ths) ;
}
void Delete_StartCheckPress_Return(StartCheckPress_Return* ths)
{
    free(ths) ;
}
void Delete_EndCheckPress_Send(EndCheckPress_Send* ths)
{
    free(ths) ;
}
void Delete_EndCheckPress_Return(EndCheckPress_Return* ths)
{
    free(ths) ;
}
void Delete_Blending_Send(Blending_Send* ths)
{
    free(ths) ;
}
void Delete_Blending_Return(Blending_Return* ths)
{
    free(ths) ;
}
void Delete_GetEPOSType_Send(GetEPOSType_Send* ths)
{
    free(ths) ;
}
void Delete_GetEPOSType_Return(GetEPOSType_Return* ths)
{
    free(ths) ;
}
void Delete_SetEPOSType_Send(SetEPOSType_Send* ths)
{
    free(ths) ;
}
void Delete_SetEPOSType_Return(SetEPOSType_Return* ths)
{
    free(ths) ;
}
void Delete_DACSet_Send(DACSet_Send* ths)
{
    free(ths) ;
}
void Delete_DACSet_Return(DACSet_Return* ths)
{
    free(ths) ;
}
void Delete_LightCtrl_Send(LightCtrl_Send* ths)
{
    free(ths) ;
}
void Delete_LightCtrl_Return(LightCtrl_Return* ths)
{
    free(ths) ;
}
void Delete_DCMotorMoveSeq_Send_m1_MoveArgs(DCMotorMoveSeq_Send_m1_MoveArgs* ths)
{
    free(ths) ;
}
void Delete_DCMotorMoveSeq_Send(DCMotorMoveSeq_Send* ths)
{
    free(ths) ;
}
void Delete_DCMotorMoveSeq_Return(DCMotorMoveSeq_Return* ths)
{
    free(ths) ;
}
void Delete_DCMotorMoveTmc_Send(DCMotorMoveTmc_Send* ths)
{
    free(ths) ;
}
void Delete_DCMotorMoveTmc_Return(DCMotorMoveTmc_Return* ths)
{
    free(ths) ;
}
void Delete_PressureRead_Send(PressureRead_Send* ths)
{
    free(ths) ;
}
void Delete_PressureRead_Return(PressureRead_Return* ths)
{
    free(ths) ;
}
void Delete_LittleMotorCtrl_Send(LittleMotorCtrl_Send* ths)
{
    free(ths) ;
}
void Delete_LittleMotorCtrl_Return(LittleMotorCtrl_Return* ths)
{
    free(ths) ;
}
void Delete_CongestionDetection_Send(CongestionDetection_Send* ths)
{
    free(ths) ;
}
void Delete_CongestionDetection_Return(CongestionDetection_Return* ths)
{
    free(ths) ;
}
void Delete_LaserCtrol_Send(LaserCtrol_Send* ths)
{
    free(ths) ;
}
void Delete_LaserCtrol_Return(LaserCtrol_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadConfig_Send(NFL_ReadConfig_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadConfig_Return(NFL_ReadConfig_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_WriteConfig_Send(NFL_WriteConfig_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_WriteConfig_Return(NFL_WriteConfig_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadDataStatus_Send(NFL_ReadDataStatus_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadDataStatus_Return(NFL_ReadDataStatus_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadData_Send(NFL_ReadData_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadData_Return(NFL_ReadData_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_DataCollectStart_Send(NFL_DataCollectStart_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_DataCollectStart_Return(NFL_DataCollectStart_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_DataCollectStop_Send(NFL_DataCollectStop_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_DataCollectStop_Return(NFL_DataCollectStop_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ThresholdConfig_Send(NFL_ThresholdConfig_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ThresholdConfig_Return(NFL_ThresholdConfig_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_StandbyConfig_Send(NFL_StandbyConfig_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_StandbyConfig_Return(NFL_StandbyConfig_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ActivateConfig_Send(NFL_ActivateConfig_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ActivateConfig_Return(NFL_ActivateConfig_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadTemperature_Send(NFL_ReadTemperature_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_ReadTemperature_Return(NFL_ReadTemperature_Return* ths)
{
    free(ths) ;
}
void Delete_NFL_SendEvent_Send(NFL_SendEvent_Send* ths)
{
    free(ths) ;
}
void Delete_NFL_SendEvent_Return(NFL_SendEvent_Return* ths)
{
    free(ths) ;
}
void Delete_PowerCtrl_Send(PowerCtrl_Send* ths)
{
    free(ths) ;
}
void Delete_PowerCtrl_Return(PowerCtrl_Return* ths)
{
    free(ths) ;
}
void Delete_PressureListen_Send(PressureListen_Send* ths)
{
    free(ths) ;
}
void Delete_PressureListen_Return(PressureListen_Return* ths)
{
    free(ths) ;
}
void Delete_SendErrorCode_Send(SendErrorCode_Send* ths)
{
    free(ths) ;
}
void Delete_SendErrorCode_Return(SendErrorCode_Return* ths)
{
    free(ths) ;
}
void Delete_IOCtrlByEvent_Send_m1_PortArgs(IOCtrlByEvent_Send_m1_PortArgs* ths)
{
    free(ths) ;
}
void Delete_IOCtrlByEvent_Send_m2_PortArgs(IOCtrlByEvent_Send_m2_PortArgs* ths)
{
    free(ths) ;
}
void Delete_IOCtrlByEvent_Send(IOCtrlByEvent_Send* ths)
{
    free(ths) ;
}
void Delete_IOCtrlByEvent_Return(IOCtrlByEvent_Return* ths)
{
    free(ths) ;
}
