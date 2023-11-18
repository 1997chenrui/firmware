#include "EPOS4Driver.h"
#include <stdlib.h>
#include "os_implement.h"
#include "errCode.h"

#define MaxLenFrame 15
#define USART_BUFF_LEN   20

extern void MX_USART2_UART_Init(void);
extern UART_HandleTypeDef huart2;

__align(4) uint8_t UART2_RECEIVE_DATA[USART_BUFF_LEN];//作为UASRT DMA的接收地址

typedef struct sUARTRERECEIVE
{
	uint8_t  data[8];
	bus_os_sem 	data_sem;
}UARTRERECEIVE;

static UARTRERECEIVE UARTTxReceive; //串口接收

static uint8_t UART_ReadWrite(uint8_t *pdata,uint8_t len,int32_t *pRecData)
{
	uint8_t result=UART_COMMUNICATION_TIMEOUT;
	uint8_t stick=0;
	uint32_t err=0;    	    
	for(char i=0;i<8;i++) UARTTxReceive.data[i]=0;
	do
	{					
		stick++;			
		if(HAL_UART_Transmit_DMA(&huart2,pdata,len) != HAL_OK) 
		{
			MX_USART2_UART_Init();
			printf("uart err  init.....\r\n");
		}
		else
		{
			err=bus_os_pend_sem(&UARTTxReceive.data_sem ,50);
			if(err == 0)
			{		
				result=0; //接收正常
				*pRecData=(UARTTxReceive.data[3] << 24 | UARTTxReceive.data[2] << 16 | UARTTxReceive.data[1] << 8 | UARTTxReceive.data[0]);
				break;
			}
			else  //没有读取到返回的数据  说明读定失败
			{
				printf("uart err / angin send %d\r\n",stick);
			}
		}
	}while(stick <= 20); 
	
	return result;
}

static void MYDMA_Enable(UART_HandleTypeDef *huart,uint16_t ndtr)
{
	huart->hdmarx->Instance->CR=~(1<<0); 
	while(huart->hdmarx->Instance->CR&0X1); 
	huart->hdmarx->Instance->NDTR=ndtr;
	huart->hdmarx->Instance->CR |= 1<<0;
}

void UartCallback(UART_HandleTypeDef *huart)
{  		
	if(huart->pRxBuffPtr[0] == 0x90 && huart->pRxBuffPtr[1] == 0x02)
	{
		for(uint8_t i=8,j=0;i<USART_BUFF_LEN-huart->hdmarx->Instance->NDTR-2;j++)
		{
			if(huart->pRxBuffPtr[i] == 0x90 && huart->pRxBuffPtr[i+1] == 0x90)
			{
				UARTTxReceive.data[j]=0x90;			
				i+=2;
			}		
			else
			{
				UARTTxReceive.data[j]=huart->pRxBuffPtr[i];			
				i+=1;											
			}
		}					 
		bus_os_post_sem(&UARTTxReceive.data_sem);						
	}						
	for(uint8_t i=0;i<USART_BUFF_LEN;i++) huart->pRxBuffPtr[i]=0;		   			
}

void USART2_IRQHandler(void)
{	    	
	HAL_UART_IRQHandler(&huart2);
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!=RESET))
	{    										
		UartCallback(&huart2);
		MYDMA_Enable(&huart2,USART_BUFF_LEN);
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);				
	}			
	huart2.Instance->SR;
	huart2.Instance->DR; 
}

static uint8_t ReadWriteToUart(uint8_t *pdata,uint8_t len,int32_t *pRecData)
{
	uint8_t err=0;
	err=UART_ReadWrite(pdata,len,pRecData);	
	if(err == UART_COMMUNICATION_TIMEOUT)
	{
		printf("%s\r\n","UART OS_ERR_TIMEOUT");
	}	
	return err;
}


static uint8_t lowByte(uint16_t ch)
{
	return ch&0xff;
}

static uint8_t highByte(uint16_t ch)
{
	return (ch&0xff00) >>8;
}

static uint16_t CalcFieldCRC(uint16_t* pDataArray, uint16_t numberOfints)
{
	uint16_t shifter, c;
	uint16_t carry;
	uint16_t crc = 0;
	while(numberOfints--)
	{
		shifter = 0x8000;          //Initialize BitX to Bit15 
		c = *pDataArray++;         //Copy next DataWord to c
		do
		{
			carry = crc & 0x8000;    //Check if Bit15 of CRC is set 
			crc <<= 1;               //CRC = CRC * 2   
			if(c & shifter) 
				crc++;   
			if(carry) 
				crc ^= 0x1021; //CRC = CRC XOR G(x), if carry is true 
			shifter >>= 1;           //Set BitX to next lower Bit, shifter = shifter/2
		 } while(shifter);
	}
	return crc;
}

static uint8_t WriteReadFrame(uint8_t OpCode,uint16_t* pDataArray,uint8_t number,int32_t *pRecData)
{
	int16_t crc;
	uint8_t postion;
	
	__align(4)  uint16_t pDataCRC[MaxLenFrame + 2];
	__align(4)  uint8_t  TxEpos4data[(MaxLenFrame) << 2]={0};//EPOS4发送数据缓冲区

	pDataCRC[0] = OpCode | (number << 8);	    
	for (int i = 0; i < number; i++)
	{
		pDataCRC[i + 1] = pDataArray[i];
	}
	pDataCRC[number + 1] = 0x0000;
	crc = CalcFieldCRC(pDataCRC, (uint16_t)(number + 2));

	for(uint8_t i = 0; i < sizeof(TxEpos4data); i++)
	{
		TxEpos4data[i] = 0x00;
	}

	postion = 0;
	TxEpos4data[postion++] = 0x90;
	TxEpos4data[postion++] = 0x02;
	TxEpos4data[postion++] = OpCode;
	if(OpCode == 0x90)
	{
		TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
	}
	TxEpos4data[postion++] = number; // 长度
	if(number == 0x90)
	{
		TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
	}
	for(uint8_t i = 0; i < number; i++)
	{
		TxEpos4data[postion++] = lowByte(pDataArray[i]);
		if(TxEpos4data[postion - 1] == 0x90)
		{
			TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
		}
				
		TxEpos4data[postion++] = highByte(pDataArray[i]);	
		if(TxEpos4data[postion - 1] == 0x90)
		{
			TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
		}		
	}

	TxEpos4data[postion++] = lowByte(crc);
	if(TxEpos4data[postion - 1] == 0x90)
	{
		TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
	}
	
	TxEpos4data[postion++] = highByte(crc);
	if(TxEpos4data[postion - 1] == 0x90)
	{
		TxEpos4data[postion++] = 0x90; // 0x90需要重发一次
	}
	
//	TxEpos4data[postion] = '\0'; // 添加结束符(但不发送结束符)
	
	// 打印发送数据
	for(int i = 0; i < postion; i++)
	{
		printf("%02x ", TxEpos4data[i]);
	}	
	printf("\r\n");
	
	return ReadWriteToUart(TxEpos4data, postion, pRecData);
}

static uint8_t WriteObject(uint16_t Index, uint8_t SubIndex,uint16_t* pArray,int32_t *pRecData)
{
	uint16_t data[4]={0};
	data[0]= 0x01 | (lowByte(Index)<<8);  // NodeId=1
	data[1]= highByte(Index)|(((uint16_t)SubIndex )<< 8);
	data[2]=pArray[0];
	data[3]=pArray[1];	

	return WriteReadFrame(0x68,data,(uint8_t)4,pRecData);
}

static uint8_t ReadObject(uint16_t Index, uint8_t SubIndex,int32_t *pRecData)
{
	uint16_t data[2]={0};
	data[0]= 0x01 | (lowByte(Index)<<8);
	data[1]= highByte(Index)|(((uint16_t)SubIndex )<< 8);
	
	return WriteReadFrame(0x60,data,(uint8_t)2,pRecData);
}


uint8_t UART_EnableUart()  //串口初始化
{	    
	bus_os_create_sem(&UARTTxReceive.data_sem,0);
	HAL_UART_Receive_DMA(&huart2, UART2_RECEIVE_DATA, USART_BUFF_LEN);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);	
	return 0;
}

uint8_t EPOS4_WriteData(uint16_t Index, uint8_t SubIndex,int32_t data)
{
	int32_t pRecData=0;
	uint16_t tmp[2]={0,0};
	tmp[0]=data>>0  & 0xffff;	
	tmp[1]=data>>16 & 0xffff;		
	
	return WriteObject(Index,SubIndex,tmp,&pRecData);
}


uint8_t EPOS4_ReadData(uint16_t Index, uint8_t SubIndex,int32_t *pRecData)
{
	return ReadObject(Index,SubIndex,pRecData);
}

uint8_t Move_By_Position_Mode(int Velocity,int Acceleration,int Deceleration,int targetPosition,uint8_t absolute,uint8_t immediately)   //位置位置下运动
{
	EPOS4_WriteData(0x6060,0,0x1) ;//设置工作模式
	EPOS4_WriteData(0x6081,0,Velocity) ;//设置速度
	EPOS4_WriteData(0x6083,0,Acceleration) ;//设置加速度
	EPOS4_WriteData(0x6084,0,Deceleration) ;//设置减速度
	EPOS4_WriteData(0x6085,0,10000) ;//设置快速停止减速度
	EPOS4_WriteData(0x6086,0,0);//设置电机类型
	EPOS4_WriteData(0x6040,0,0x0F);
	EPOS4_WriteData(0x607a,0,0);///在设置电机目标位置之前先清零  不然没有写进去会直接沿用上一次的位置信息
	EPOS4_WriteData(0x607a,0,targetPosition) ;//设置目标位置	
	if(absolute)
	{
		if(immediately)	EPOS4_WriteData(0x6040,0,0x3f);else EPOS4_WriteData(0x6040,0,0x1f);
	}
	else
	{
		if(immediately) EPOS4_WriteData(0x6040,0,0x7f);else EPOS4_WriteData(0x6040,0,0x5f);
	}
	OSTimeDly(15);
	EPOS4_WriteData(0x6040,0,0x0f);  //toggle new Position
	return 0;
}

uint8_t Move_By_Velocity_Mode(int Velocity,int Acceleration,int dir)   //速度模式下运动
{
	EPOS4_WriteData(0x6060,0,0x03) ;//设置工作模式
	EPOS4_WriteData(0x6083,0,Acceleration) ;//设置加速度
	EPOS4_WriteData(0x6084,0,Acceleration) ;//设置减速度	
	EPOS4_WriteData(0x6085,0,20000) ;//设置快速停止减速度	
	EPOS4_WriteData(0x6086,0,0) ;//设置电机类型	
	OSTimeDly(15);  
	EPOS4_WriteData(0x6040,0,0x0F) ;	
	EPOS4_WriteData(0x60ff,0,dir==0?(Velocity = -abs(Velocity)):(Velocity = abs(Velocity))) ;//设置目标速度
	OSTimeDly(15);	
	EPOS4_WriteData(0x6040,0,0x0f);  //toggle new Position
	return 0;
}

uint8_t Move_By_Homing_Mode(int homingAcceleration,int speedSwitch,int speedIndex,int homeOffset,int currentTreshold,int homePosition)  //在回零模式下运动
{
	EPOS4_WriteData(0x6040,0,0x08) ;
	EPOS4_WriteData(0x6060,0,0x06) ;//设置工作模式
	EPOS4_WriteData(0x607c,0,homeOffset);
	EPOS4_WriteData(0x6099,1,500);
	EPOS4_WriteData(0x6099,2,10);
	EPOS4_WriteData(0x609A,0,1000);
	EPOS4_WriteData(0x30b0,0,0);
	EPOS4_WriteData(0x2080,0,currentTreshold);//currentTreshold 0x2080-00	
	EPOS4_WriteData(0x2081,0,homePosition);//homePosition 0x2081-00
	EPOS4_WriteData(0x6098,0,34);
	EPOS4_WriteData(0x6040,0,0x06) ;
	OSTimeDly(20);
	EPOS4_WriteData(0x6040,0,0x0F) ;
	OSTimeDly(20);
	EPOS4_WriteData(0x6040,0,0x1F) ;
	return 0;
}




