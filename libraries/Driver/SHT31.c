#include "SHT31.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "delayus.h" 
#include <math.h>

/*软件延时函数*/
//void delay_user(uint16_t us)
//{
//	uint16_t delayus = 0;
//	delayus = us*28;
//	while(delayus--);
//}

/* 
  I2C 起始信号
	SCL高电平期间 ，SDA负跳变 1 -> 0
*/
void SHT31_S_Start(void)
{
	SHT_SCL_Set();
	SHT_SDA_Set();
	delayUs(2);
	SHT_SDA_Reset();
}

/* 
  I2C 终止信号
	SCL高电平期间  SDA正跳变  0 -> 1
*/
void SHT31_P_Stop(void)
{
	SHT_SDA_Reset();
	SHT_SCL_Set();
	delayUs(1);
	SHT_SDA_Set();

}

/*
 应答/非应答位发送
*/

void SHT31_ACK( uint8_t ack)
{
	SHT_SCL_Reset();  //拉低时钟线，允许数据变化
	delayUs(1);    //等待总线稳定

	if(ack == SHT_ACK)
	{
		SHT_SDA_Reset();
	}
	else
	{
		SHT_SDA_Set();
	}
	delayUs(1);    //等待总线稳定

	SHT_SCL_Set();     //信号传递
	delayUs(5);   
	SHT_SCL_Reset();    
	SHT_SDA_Set();     //释放数据总线

}

/*应答位检测*/

uint8_t SHT31_Test_ACK(void )
{
	uint8_t timeout = 10;

	SHT_SCL_Reset();
	delayUs(1);
	while((SHT_SDA_Read() == GPIO_PIN_SET))
	{
		timeout-- ;
		delayUs(1);
		if(timeout == 0)
		{
			SHT_SCL_Reset();
			return SHT_NACK ;
		}
		delayUs(30);
	
	}
	SHT_SCL_Set();
	delayUs(1);
	SHT_SCL_Reset();
	return SHT_ACK ;
		
}

/*发送 1 byte*/
void SHT31_SentByte(uint8_t TxData)
{
	uint8_t i ;
	for(i=0;i<8;i++)
	{
		SHT_SCL_Reset();
		if((TxData&0x80))
		{
			SHT_SDA_Set();
		}
		else
		{
			SHT_SDA_Reset();
		}
		delayUs(1);
		SHT_SCL_Set();
		TxData <<= 1;
		delayUs(1);
	}

	SHT_SCL_Reset();

}

/*接收1 byte 数据*/
uint8_t SHT31_ReadByte(void)
{
	uint8_t  RxData ,i ;

	SHT_SDA_Set();
	SHT_SCL_Reset();

	for(i= 0 ;i<8 ;i++)
	{
		delayUs(1);
		SHT_SCL_Set();
		
		RxData <<= 1;
		if((SHT_SDA_Read()==GPIO_PIN_SET))
		{
				RxData |= 0x01; 
		}
		else
		{
				RxData &= (~0x01);
		}
		SHT_SCL_Reset();
	}
	return RxData ;
}
//3涉及的通讯问题解决了，但是4涉及的通讯阻塞未解决，并且此问题也会引起探液失败，5和6属于原因不明的问题
uint8_t  Temp_MSB,Temp_LSB,Temp_CRC,Humi_MSB,Humi_LSB ,Humi_CRC;
	
//	float   temp, humi ;
uint8_t SHT31_Read_Data(uint8_t address,uint8_t *temp,uint8_t *humi)
{
	uint8_t T, H;
	uint16_t Temp, Humi ;
	SHT31_S_Start();
	SHT31_SentByte(((address<<1)|SHT_Write_Bit));  //写 传感器 
	SHT31_Test_ACK();

	SHT31_SentByte(SHT_Cmd_MSB);
	SHT31_Test_ACK();

	SHT31_SentByte(SHT_Cmd_LSB);
	SHT31_Test_ACK();

	SHT31_P_Stop();

	HAL_Delay(50);
	
	SHT31_S_Start();
	SHT31_SentByte(((address<<1)|SHT_Read_Bit));  //读 传感器 1
	if(SHT31_Test_ACK() == SHT_ACK )
	{
		Temp_MSB = SHT31_ReadByte();
		SHT31_ACK(SHT_ACK);
		Temp_LSB = SHT31_ReadByte();
		SHT31_ACK(SHT_ACK);
		Temp_CRC = SHT31_ReadByte();
		SHT31_ACK(SHT_ACK);

		Humi_MSB = SHT31_ReadByte();
		SHT31_ACK(SHT_ACK);
		Humi_LSB = SHT31_ReadByte();
		SHT31_ACK(SHT_ACK);
		Humi_CRC = SHT31_ReadByte();
		SHT31_ACK(SHT_NACK);
	}
	
	Temp = ((uint16_t)Temp_MSB<<8)|0x00;//Temp_LSB ;
	Humi = ((uint16_t)Humi_MSB<<8)|0x00;//Humi_LSB ;	
	/*数据转换,然后取整，四舍五入*/
	T = round((175.0f*((float)Temp/65535.0f) - 45.0f));
	H = round((100.0f*((float)Humi/65535.0f)));
	
	*temp = T;
	*humi = H;
	
	if(address==0x44)
	{
//		printf("\r\n");
//		printf("temp1 is %x   %d\r\n",*temp,*temp);
//		printf("humi1 is %x   %d\r\n",*humi,*humi);
	}
	else if(address==0x45)
	{
//		printf("\r\n");
//		printf("temp2 is %x   %d\r\n",*temp,*temp);
//		printf("humi2 is %x   %d\r\n",*humi,*humi);
	}
	SHT31_P_Stop();
	
	return 0;
}

//void SHT31_Read(uint8_t *temp,uint8_t *humi)
//{
//	SHT31_Read_Data(&ths,temp,humi);
//}
