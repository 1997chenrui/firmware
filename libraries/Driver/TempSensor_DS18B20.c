#include "TempSensor_DS18B20.h"
#include "delayus.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define	TEMP_INVALID						  -274.0f

#define TEMP_READ_PEND(ths)  {	bus_os_pend_sem(&ths->temp_read_mutex, 0)
#define TEMP_READ_POST(ths)	    bus_os_post_sem(&ths->temp_read_mutex); }

void DS18B20_Init(TempSensor_Struct *ths,void (*set)(uint8_t val),uint8_t (*get)())
{
    ths->current_temp = 0;
	ths->get_dq_state = get;
	ths->set_dq_state = set;
    bus_os_create_sem(&ths->temp_read_mutex, 1);
}

static uint8_t Reset_DS18B20(TempSensor_Struct *ths)  
{ 	
	ths->set_dq_state(1);
	delayUs(1);
	ths->set_dq_state(0); 
	delayUs(500);  //500us
	ths->set_dq_state(1); 
	delayUs(100);  //40us 
	return ths->get_dq_state();
}  
  
static void WriteByte_DS18B20(TempSensor_Struct *ths,uint8_t Dat)
{   
	for(uint8_t i=8;i>0;i--){  
		ths->set_dq_state(0);      
		delayUs(5);    //5us  
		if(Dat & 0x01)  
			ths->set_dq_state(1);
		else  
			ths->set_dq_state(0);  
		delayUs(55);    //65us  
		ths->set_dq_state(1); 
		delayUs(2);    //1us  
		Dat >>= 1;   
	}
}  
  
  
static uint8_t ReadByte_DS18B20(TempSensor_Struct *ths)  
{  
	uint8_t i,Dat;  
	ths->set_dq_state(1);  
	delayUs(5);  
	for(i=8;i>0;i--){  
		Dat >>= 1;  
		ths->set_dq_state(0);
		delayUs(3);   //5us  
		ths->set_dq_state(1);  
		delayUs(5);   //5us  
		if(ths->get_dq_state())  
			Dat|=0x80;  
		else  
			Dat&=0x7f;    
		delayUs(50);   //65us   
	}  
	return Dat;  
}  
 
//static void ReadRom_DS18B20(TempSensor_Struct *ths,uint8_t *Read_Addr)  
//{  
//	uint8_t i;
//	WriteByte_DS18B20(ths,ReadROM);  
//	for(i=8;i>0;i--)  
//	{  
//		*Read_Addr=ReadByte_DS18B20(ths);  
//		Read_Addr++;  
//	}  
//}  
  
  
static uint8_t CRC8_DS18B20(uint8_t *pBuf, uint8_t iSize) {
	uint8_t iCrc = 0;
	for (; iSize--; ) {
		u8 i;
		iCrc ^= *pBuf++;
		for (i = 0; i < 8; i++) {
			if (iCrc & 0x01)
				iCrc = (iCrc >> 1) ^ 0x8c;
			else
				iCrc >>= 1;
		}
	}
	return iCrc;
} 

static float ReadData_DS18B20(TempSensor_Struct *ths)  
{  
	uint8_t i;
	TempSensorRAM_TypeDef TempSensorRAM;

	float  Temperature;  
	
	//StartConvert
	if(Reset_DS18B20(ths))
	{
		printf("Reset_DS18B20 Error\r\n");
		return TEMP_INVALID;
	}
	delayUs(100);  //40us 
	WriteByte_DS18B20(ths,SkipROM);   
	WriteByte_DS18B20(ths,StartConvert);
	
	//Wait Convert
	bus_os_sleep(750);
	//read
	if(Reset_DS18B20(ths))
	{
		printf("18b20 reset err\r\n");
		return TEMP_INVALID;
	}
	delayUs(100);  //40us 
	WriteByte_DS18B20(ths,SkipROM);   
	WriteByte_DS18B20(ths,ReadScratchpad);
	for(i = 0; i < DS18B20_RAM_SIZE; i++)
	{
    	*((u8 *)(&TempSensorRAM) + i) = ReadByte_DS18B20(ths);
		//printf("%2x,",*((u8 *)(&TempSensorRAM) + i));
	}	

	if(CRC8_DS18B20((u8 *)(&TempSensorRAM), DS18B20_RAM_SIZE))
	{
		printf("18b20 crc err\r\n");
		return TEMP_INVALID;
	}
	
//	printf("%x %x\r\n",TempSensorRAM.TempHigh,TempSensorRAM.TempLow);
	if( 0xf8 == ( 0xf8 & TempSensorRAM.TempHigh ) )
        Temperature = (float)( ( (((TempSensorRAM.TempHigh & 0x0f) << 4) | ((TempSensorRAM.TempLow & 0xf0) >> 4)) - 0xff ) + (-(1-(TempSensorRAM.TempLow & 0x0f) * 0.0625 ))); //除16  把小数部分剔除
    else
        Temperature = (float)( ( ( ( TempSensorRAM.TempHigh & 0x07 ) << 4 ) | ( ( TempSensorRAM.TempLow & 0xf0 ) >> 4 ) ) + ( TempSensorRAM.TempLow & 0x0f ) * 0.0625 ); //除16  把小数部分剔除	
    
    ths->current_temp = Temperature;
    return Temperature; 
}  

static float GetTemp_DS18B20_try(TempSensor_Struct *ths, int time, int ms)
{
	float temp;
	uint8_t i;
	for(i=0; i<time; i++)
	{// 读time次，读到正确跳出
		temp = ReadData_DS18B20(ths); 
		if(temp != TEMP_INVALID)
			break;
		else
		{
			OSTimeDly(ms);
		}
	}
	if(temp != TEMP_INVALID)
		ths->current_temp = temp*10/10;//保留一位小数
	else
		ths->current_temp = TEMP_INVALID;
    return ths->current_temp;
}

float GetTemp_DS18B20(TempSensor_Struct *ths)
{
    ths->current_temp = GetTemp_DS18B20_try(ths, 3, 500);
    return ths->current_temp;
}


/**
*  中位值平均滤波
* @param pData:没有滤波的数据 
* @param nSize:数据大小 
* @return:滤波数值
*/
static float medianFilter(float* pData,int nSize)
{
    float max,min;
    float sum;
    if(nSize>2)
    {
        max = pData[0];
        min = max;
        sum = 0;
        for(int i=0;i<nSize;i++)
        {
            sum += pData[i];            
            if(pData[i]>max)
            {
                max = pData[i];   //一个循环之后max就是最大的值
            }

            if(pData[i]<min)
            {
                min = pData[i];   //一个循环之后min就是最小的值
            }
        }

        sum = sum-max-min;       //去掉最大的值和最小的值
        return sum/(nSize-2); //对N-2个数求平均值          
    }

    return TEMP_INVALID;
}

float GetTemp_DS18B20_median_filter(TempSensor_Struct *ths)
{
    float temp_array[3];         
    TEMP_READ_PEND(ths);
    for(int i = 0;i<3;i++)
    {
        temp_array[i] = GetTemp_DS18B20_try(ths,3,2);
    }
    TEMP_READ_POST(ths);
    ths->current_temp = medianFilter(temp_array, sizeof(temp_array)/sizeof(float));
    return ths->current_temp;
}

float GetTemp_Buffer(TempSensor_Struct *ths){
    return ths->current_temp;
}


