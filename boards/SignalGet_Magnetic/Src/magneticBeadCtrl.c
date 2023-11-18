#include "magneticBeadCtrl.h"
#include "magneticBead_cfg.h"
#include "os_implement.h"
#include "stdio.h"
#include "kalmanFilter.h"

#define  					TASK_MAGNETICBEAD_PRIO                   8u
#define  					TASK_MAGNETICBEAD_STK_SIZE                     512u
static  bus_os_thread   	TASK_MAGNETICBEAD_TCB;
static  u32  				TASK_MAGNETICBEAD_STK[TASK_MAGNETICBEAD_STK_SIZE];

MagneticBeadTask MagneticBeadTask_Static;
static void MagneticBead_Thread(void *pdata);

static KalmanFilter magneticBeadKalmanFilter[MAGNETICBEAD_NUM];

void MagneticBeadTask_Init(void)
{
	for(uint8_t i=0;i<MAGNETICBEAD_NUM;i++)
	{
		KalmanFilterInit(&magneticBeadKalmanFilter[i],1000,0.001,0.01);
	}
//	bus_create_thread(&TASK_MAGNETICBEAD_TCB, 
//						MagneticBead_Thread,
//						TASK_MAGNETICBEAD_STK,
//						TASK_MAGNETICBEAD_STK_SIZE,
//						TASK_MAGNETICBEAD_PRIO,
//						NULL);

}




bool StartMBCtrl(MagneticBeadCtrl *mb,uint16_t time)
{
	if(mb->isStart==false)
	{
		mb->captureCount=0;//清空捕获计数
		mb->ctrlCount=0;	//清空控制计数	
		mb->overflowCount=0;//清空溢出次数计数
		
		mb->timeCount=time;
		mb->isStart=true;
		return true;
	}
	else
	{
		return false;
	}
}

void UpdateMagneticBeadCaptureCount(uint32_t *count)
{
	
	for(uint8_t i=0;i<MagneticBeadTask_Static.magneticBeadCount;i++)
	{
		uint8_t overflowNum;
		uint32_t overflowCount;
		uint8_t captureCount;
		if(MagneticBeadTask_Static.magneticBeadList[i].isStart==true)
		{		
			overflowNum=MagneticBeadTask_Static.magneticBeadList[i].overflowNum;
			overflowCount=MagneticBeadTask_Static.magneticBeadList[i].overflowCount;
			captureCount=MagneticBeadTask_Static.magneticBeadList[i].captureCount;
			
			//第一次不进行kalman
			if(captureCount==0&&MagneticBeadTask_Static.magneticBeadList[i].captureValueCount==0)
			{
				MagneticBeadTask_Static.magneticBeadList[i].captureValue[captureCount]=count[i]+(overflowNum*overflowCount);	
			}
			else
			{
				MagneticBeadTask_Static.magneticBeadList[i].captureValue[captureCount]=(uint32_t)KalmanFilterCalculate(
						&magneticBeadKalmanFilter[i],
						count[i]+(overflowNum*overflowCount)
					);
			}
//			if(i==1)
//			{
//				printf("%d\r\n",MagneticBeadTask_Static.magneticBeadList[i].captureValue[captureCount]);
//			}
				
			if(++captureCount>=10)
			{
				captureCount=0;
				MagneticBeadTask_Static.magneticBeadList[i].captureValueCount++;
		
				/*
				上传数据 格式为
				磁珠ID+数据上报计数+10个数据
				*/
				
			}
			
		}
		else
		{
			//说明任务结束了 但是还有结束数据没有上传
			if(MagneticBeadTask_Static.magneticBeadList[i].captureValueCount!=0)
			{
				//说明停止的时候 还有不足十个数据每上传
//				if(MagneticBeadTask_Static.magneticBeadList[i].captureCount!=0)
//				{
//					for(uint8_t j=MagneticBeadTask_Static.magneticBeadList[i].captureCount;j<10;j++)
//						MagneticBeadTask_Static.magneticBeadList[i].captureValue[j]=0;
//					
//					MagneticBeadTask_Static.magneticBeadList[i].captureValueCount++;
//					//上传数据
//					
//				}
				
				for(uint8_t j=0;j<10;j++)
					MagneticBeadTask_Static.magneticBeadList[i].captureValue[j]=0xFEFEFEFE;
				
				MagneticBeadTask_Static.magneticBeadList[i].captureValueCount++;
				
				//上传结束数据
				
				MagneticBeadTask_Static.magneticBeadList[i].captureValueCount=0;
			}
		}
		
	}
	
}



void UpdateMagneticBeadCtrl(void)
{
	for(uint8_t i=0;i<MagneticBeadTask_Static.magneticBeadCount;i++)
	{
		if(MagneticBeadTask_Static.magneticBeadList[i].isStart==true)
		{
			if(MagneticBeadTask_Static.magneticBeadList[i].ctrlCount<=
				MagneticBeadTask_Static.magneticBeadList[i].ctrlPulse)
			{	//打开电磁铁
				MagneticBeadTask_Static.magneticBeadList[i].electromagnetCtrl(true);
			}
			else
			{	//关闭电磁铁
				MagneticBeadTask_Static.magneticBeadList[i].electromagnetCtrl(false);
			}
			
			//更新 控制参数
			MagneticBeadTask_Static.magneticBeadList[i].ctrlCount=
			(	
				(MagneticBeadTask_Static.magneticBeadList[i].ctrlCount+1)%
				MagneticBeadTask_Static.magneticBeadList[i].ctrlCycle
			);
		
		}
		else
		{
			MagneticBeadTask_Static.magneticBeadList[i].ctrlCount=0;
			MagneticBeadTask_Static.magneticBeadList[i].electromagnetCtrl(false);
		}
	}	
}


void UpdateMagneticBeadTaskTimeCtrl(void)
{
	for(uint8_t i=0;i<MagneticBeadTask_Static.magneticBeadCount;i++)
	{
		if(MagneticBeadTask_Static.magneticBeadList[i].isStart==true)
		{
			if(MagneticBeadTask_Static.magneticBeadList[i].timeCount)
			{
				MagneticBeadTask_Static.magneticBeadList[i].timeCount--;
			}
			else
			{
				MagneticBeadTask_Static.magneticBeadList[i].isStart=false;
			}
		}
	}
}
	
static void MagneticBead_Thread(void *pdata)
{

	for(;;)
	{
		
		bus_os_sleep(10);
	}
}



