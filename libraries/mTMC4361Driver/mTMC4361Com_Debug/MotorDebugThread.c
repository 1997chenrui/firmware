#include "MotorDebugThread.h"
#include <stdio.h>
#include "splite.h"
#include <stdlib.h>
#include "usart1.h"
#include "outputdata.h"
#include "mTMC4361Device.h"

extern MOTOR motors[];

MOTOR_DEBUG_THRAD MotorDebug_Thread;


void MOTOR_THRAD_DEBUG_CREATE()
{
	bus_create_thread(&MotorDebug_Thread.tcb,
					MotorDebug_ThreadMain,
					MotorDebug_Thread.stack, 
					DEBUG_THREAD_STACK_SIZE, 
					DEBUG_THREAD_PRIO,
					NULL);
	MotorDebug_Thread.Queue =OSQCreate(&MotorDebug_Thread.QueueTbl[0],MOTORTHRAD_QUEUE_SIZE);
	Usart1_Start();
}


char GetMotorCmd(void * data)//从串口获取命令码
{
	char *d=(char *)data;
	if(d[1] != 0x2f)
	{
		return (((d[0]&0x0f) *10) + (d[1] &0x0f));
	}
	else
	{
		return (d[0]&0x0f);
	}
}

char GetDataSize(char *pdata)
{
	char result=0;
	for(char i=0;i<USART_REC_LEN;i++)
	{	
		if(pdata[i] == '/') result++;
	}
	return result;
}

void MotorDebug_ThreadMain(void *p_arg)
{
	uint8_t err=0;
	p_arg=p_arg;
	void *result=NULL;
	while(1)
	{		 
		result=OSQPend(MotorDebug_Thread.Queue,10,&err);
		if(err != 0) 
		{
			Plot(mTMC4361DeviceGetSpeed(&motors[0]));				
		}
		else
		{
			MotorDebug_Thread.cmd=(enum uMOTORCMD)GetMotorCmd(result);	          		
			switch(MotorDebug_Thread.cmd)
			{
				case Rotator://旋转
				{
		//		int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
		//		if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
		//		printf("Thread Com Debug Rotator........\r\n");								
		//		printf("motor %d speed=%x\r\n",data[1],data[2]);
		//		printf("motor %d acc=%x\r\n",data[1],data[3]);
		//		printf("motor %d dir=%x\r\n",data[1],data[4]);
		//		mTMC5130DevicRotate(&motors[data[1]],data[2],data[3],data[4],data[5],data[6]);
		//		free(data);
				
				}break;
				case Stop://停止
				{
		//		int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
		//		if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
		//		printf("Thread Com Debug Stop........\r\n");	
		//		printf("motor %d speed=%x\r\n",data[1],data[2]);
		//		printf("motor %d acc=%x\r\n",data[1],data[3]);					
		//		mTMC5130DevicStop(&motors[data[1]],data[2]);
		//		free(data);		
				}break;			
				case MoveTO: //绝对位移	
				{
		//		int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
		//		if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
		//		printf("Thread Com Debug MoveTO........\r\n");	
		//		printf("motor %d speed=%x\r\n",data[1],data[2]);
		//		printf("motor %d acc=%x\r\n",data[1],data[3]);
		//		printf("motor %d position=%x\r\n",data[1],data[4]);
		//		printf("motor %d dir=%x\r\n",data[1],data[5]);								
		//		mTMC5130DevicMoveto(&motors[data[1]],data[2],data[3],data[4],data[5],data[6],data[7]);
		//		free(data);			
				}break;	
				case MoveBY://相对位移
				{
					int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
					if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
					printf("Thread Com Debug MoveBY........\r\n");	
					printf("motor %d speed=%x\r\n",data[1],data[2]);
					printf("motor %d acc=%x\r\n",data[1],data[3]);
					printf("motor %d position=%x\r\n",data[1],data[4]);
					printf("motor %d dir=%x\r\n",data[1],data[5]);	
					mTMC4361DevicMoveby(&motors[data[1]],data[2],data[3],data[4],data[5],data[6],data[7]);
					free(data);		
				}break;	
				case GetPostion://获取位置信息
				{
					int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
					if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
					printf("Thread Com Debug GetENCPostion %d........\r\n",data[1]);						
					printf("The ENC Postion=%d\r\n",mTMC4361DeviceGetENCPostion(&motors[data[1]],data[2]));
					free(data);		
				}break;
				case GetState:
				{
					int *data=(int *)malloc(GetDataSize((char *)result) *sizeof(int));
					if(StrToInt((char *)result,data) == 0) {printf("data error......\r\n");free(data);break;}
					printf("Thread Com Debug GetState %d........\r\n",data[1]);						
					printf("The Drive State=%d\r\n",mTMC4361DeviceGetState(&motors[data[1]]));
					free(data);		
				}break;
				default:break;					
			}
		}
	}
}

