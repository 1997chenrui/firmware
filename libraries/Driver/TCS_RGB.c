#include "TCS_RGB.h"
#include <stdio.h>
#include "TCB_RGB_Cfg.h"
#include "delayus.h"
#include <stdbool.h>
#define CMD_REG_SET(type,addr) 					((1<<7)|(type<<5)|addr)
#define ENABLE_REG_DATA(aien,wen,aen,pon)		((0x00<<5)|(aien<<4)|(wen<<3)|(0<<2)|(aen<<1)|pon)
#define CFG_REG_DATA(wlong)    					(wlong<<1)
static void IIC_SCL(u8 idx,u8 val)
{
	switch(idx)
	{
#if(RGB_CHANNEL_CNT >= 1)
		case 0:
			IIC_SCL0 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 2)
		case 1:
			IIC_SCL1 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 3)
		case 2:
			IIC_SCL2 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 4)
		case 3:
			IIC_SCL3 = val;
			break;
#elif(RGB_CHANNEL_CNT == 5)
		case 4:
			IIC_SCL4 = val;
			break;
#endif
		default:break;
	}
	
}
static void IIC_SDA(u8 idx,u8 val)
{
	switch(idx)
	{
#if(RGB_CHANNEL_CNT >= 1)
		case 0:
			IIC_SDA0 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 2)
		case 1:
			IIC_SDA1 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 3)
		case 2:
			IIC_SDA2 = val;
			break;
#elif(RGB_CHANNEL_CNT >= 4)
		case 3:
			IIC_SDA3 = val;
			break;
#elif(RGB_CHANNEL_CNT == 5)
		case 4:
			IIC_SDA4 = val;
			break;
#endif
		default:break;
	}
	
}
static u8 READ_SDA(u8 idx)
{
	u8 data = 0;
	switch(idx)
	{
#if(RGB_CHANNEL_CNT >= 1)
		case 0:
			data = READ_SDA0;
			break;
#elif(RGB_CHANNEL_CNT >= 2)
		case 1:
			data = READ_SDA1;
			break;
#elif(RGB_CHANNEL_CNT >= 3)
		case 2:
			data = READ_SDA2;
			break;
#elif(RGB_CHANNEL_CNT >= 4)
		case 3:
			data = READ_SDA3;
			break;
#elif(RGB_CHANNEL_CNT == 5)
		case 4:
			data = READ_SDA4;
			break;
#endif
		default:break;
	}
	return data;
}
static void SDA_OUT(u8 idx)
{
	switch(idx)
	{
#if(RGB_CHANNEL_CNT >= 1)
		case 0:
			SDA_OUT0();
			break;
#elif(RGB_CHANNEL_CNT >= 2)
		case 1:
			SDA_OUT1();
			break;
#elif(RGB_CHANNEL_CNT >= 3)
		case 2:
			SDA_OUT2();
			break;
#elif(RGB_CHANNEL_CNT >= 4)
		case 3:
			SDA_OUT3();
			break;
#elif(RGB_CHANNEL_CNT == 5)
		case 4:
			SDA_OUT4();
			break;
#endif
		default:break;
	}
}
static void SDA_IN(u8 idx)
{
	switch(idx)
	{
#if(RGB_CHANNEL_CNT >= 1)
		case 0:
			SDA_IN0();
			break;
#elif(RGB_CHANNEL_CNT >= 2)
		case 1:
			SDA_IN1();
			break;
#elif(RGB_CHANNEL_CNT >= 3)
		case 2:
			SDA_IN2();
			break;
#elif(RGB_CHANNEL_CNT >= 4)
		case 3:
			SDA_IN3();
			break;
#elif(RGB_CHANNEL_CNT == 5)
		case 4:
			SDA_IN4();
			break;
#endif
		default:break;
	}
}
//产生IIC起始信号
static void IIC_Start(u8 idx)
{
	SDA_OUT(idx);     //sda线输出
	IIC_SDA(idx,1);	  	  
	IIC_SCL(idx,1);
	delayUs(4);
 	IIC_SDA(idx,0);//START:when CLK is high,DATA change form high to low 
	delayUs(4);
	IIC_SCL(idx,0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
static void IIC_Stop(u8 idx)
{
	SDA_OUT(idx);//sda线输出
	IIC_SCL(idx,0);
	IIC_SDA(idx,0);//STOP:when CLK is high DATA change form low to high
 	delayUs(4);
	IIC_SCL(idx,1); 
	IIC_SDA(idx,1);//发送I2C总线结束信号
	delayUs(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static u8 IIC_Wait_Ack(u8 idx)
{
	u8 ucErrTime=0;
	SDA_IN(idx);      //SDA设置为输入  
 	IIC_SDA(idx,1);delayUs(1);	   
 	IIC_SCL(idx,1);delayUs(1);

	while(READ_SDA(idx))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(idx);
			printf("error\r\n");
			return 1;
		}
	}
	IIC_SCL(idx,0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
static void IIC_Ack(u8 idx)
{
	IIC_SCL(idx,0);
	SDA_OUT(idx);
	IIC_SDA(idx,0);
	delayUs(2);
	IIC_SCL(idx,1);
	delayUs(2);
	IIC_SCL(idx,0);
}
//不产生ACK应答		    
static void IIC_NAck(u8 idx)
{
	IIC_SCL(idx,0);
	SDA_OUT(idx);
	IIC_SDA(idx,1);
	delayUs(2);
	IIC_SCL(idx,1);
	delayUs(2);
	IIC_SCL(idx,0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
static void IIC_Send_Byte(u8 idx,u8 txd)
{                        
    u8 t;   
	SDA_OUT(idx); 	    
    IIC_SCL(idx,0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA(idx,(txd&0x80)>>7);
        txd<<=1; 	  
		delayUs(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(idx,1);
		delayUs(2); 
		IIC_SCL(idx,0);	
		delayUs(2);
    }	 
	
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static u8 IIC_Read_Byte(u8 idx,unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(idx);//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(idx,0); 
        delayUs(2);
		IIC_SCL(idx,1);
        receive<<=1;
        if(READ_SDA(idx))receive++;   
		delayUs(1); 
    }					 
    if (!ack)
        IIC_NAck(idx);//发送nACK
    else
        IIC_Ack(idx); //发送ACK   
    return receive;
}

void RGBAddress_Get(u8 idx)
{
	for(u8 i = 0;i<255;i++)
	{
		IIC_Start(idx);
		IIC_Send_Byte(idx,i);
		if(IIC_Wait_Ack(idx)!=0)
		{
			printf("error=%d\r\n",i);
			IIC_Stop(idx);
			delayMs(10);
			continue;
		}
		else
		{
			printf("address = %d\r\n",i);
			IIC_Stop(idx);
			//break;
		}
	}
}
u8 RGBID_Read(u8 idx)
{
	u8 addr = CMD_REG_SET(0,RGB_ID_REG);
	u8 data = 0;
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	if(IIC_Wait_Ack(idx)!=0)
	{
		printf("RGBID_Read Ack Error\r\n");
	}
	IIC_Send_Byte(idx,addr);
	if(IIC_Wait_Ack(idx)!=0)
	{
		printf("RGBID_Read Ack Error\r\n");
	}

	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_READ);
	if(IIC_Wait_Ack(idx)!=0)
	{
		printf("RGBID_Read Ack Error\r\n");
	}
	data = IIC_Read_Byte(idx,0);
	printf("data = %d\r\n",data);
	IIC_Stop(idx);
	return data;
}
void RGBEnable_Set(u8 idx,u8 aidisen,u8 wen,u8 aen,u8 pon)
{
	u8 cmd = CMD_REG_SET(0,RGB_ENABLE_REG);
	u8 data = ENABLE_REG_DATA(aidisen,wen,aen,pon);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,data);
	IIC_Wait_Ack(idx);
	IIC_Stop(idx);
}
//Count = (256 ? ATIME) × 1024 
void IntegrationTime_Set(u8 idx,u8 data)
{
	u8 cmd = CMD_REG_SET(0,RGB_TIMING_REG);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,data);
	IIC_Wait_Ack(idx);
	IIC_Stop(idx);
	
}
//Count = (256 ? ATIME) × 1024 
void WaitTime_Set(u8 idx,u8 data)
{
	u8 cmd = CMD_REG_SET(0,RGB_WAITTIME_REG);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,data);
	IIC_Wait_Ack(idx);
	IIC_Stop(idx);
}
void WLONG_Set(u8 idx)
{
	u8 cmd = CMD_REG_SET(0,RGB_CFG_REG);
	u8 data = CFG_REG_DATA(WLONG_EN);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,data);
	IIC_Wait_Ack(idx);
	IIC_Stop(idx);
}
void AGAIN_Set(u8 idx,u8 agin)
{
	u8 cmd = CMD_REG_SET(0,RGB_CTRL_REG);
	u8 data = CFG_REG_DATA(agin);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,data);
	IIC_Wait_Ack(idx);
	IIC_Stop(idx);
}
u8 AINT_Status_Get(u8 idx)
{
	u8 cmd = CMD_REG_SET(0,RGB_STATUS_REG);
	u8 data = 0;
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_READ);
	IIC_Wait_Ack(idx);
	data = (IIC_Read_Byte(idx,0)>>4)&1;
	IIC_Stop(idx);
	return data;
}
u8 AVALID_Status_Get(u8 idx)
{
	u8 cmd = CMD_REG_SET(0,RGB_STATUS_REG);
	u8 data = 0;
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_READ);
	IIC_Wait_Ack(idx);
	data = IIC_Read_Byte(idx,0)&1;
	IIC_Stop(idx);
	return data;
}

void RGBDATA_Get(u8 idx,u16*cdata,u16* rdata,u16*gdata,u16* bdata)
{
	u8 cmd = CMD_REG_SET(1,RGB_CLEARDATA_L_REG);
	u16 data = 0;
 	RGBEnable_Set(idx,AIDISEN,WDISEN,AEN,PON);
	while(AVALID_Status_Get(idx)!=1){}
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_WRITE);
	IIC_Wait_Ack(idx);
	IIC_Send_Byte(idx,cmd);
	IIC_Wait_Ack(idx);
	IIC_Start(idx);
	IIC_Send_Byte(idx,SLAVE_ADDR_READ);
	IIC_Wait_Ack(idx);
	data = IIC_Read_Byte(idx,1);
	*cdata = data;
	data = IIC_Read_Byte(idx,1);
	data <<= 8;
	*cdata |= data;
		
	data = IIC_Read_Byte(idx,1);
	*rdata = data;
	data = IIC_Read_Byte(idx,1);
	data <<= 8;
	*rdata |= data;
		
	data = IIC_Read_Byte(idx,1);
	*gdata = data;
	data = IIC_Read_Byte(idx,1);
	data <<= 8;
	*gdata |= data;
	
	data = IIC_Read_Byte(idx,1);
	*bdata = data;
	data = IIC_Read_Byte(idx,0);
	data <<= 8;
	*bdata |= data;
	IIC_Stop(idx);
}

