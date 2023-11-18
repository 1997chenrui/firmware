#include "EPOS4Driver.h"
#include <stdlib.h>
#include "os_implement.h"
#include "errCode.h"

#define MaxLenFrame 15

extern UART_HandleTypeDef huart3;

typedef struct
{
	bus_os_sem sem;
	unsigned char rindex;
	unsigned char isstart;
	unsigned char rxbuf[200];
	unsigned long lasttime;
}Uart_RecvTypeDef;

static Uart_RecvTypeDef UartRecv = {0};

void uart_init(USART_TypeDef *UARTx, unsigned int baudrate)
{
	__HAL_RCC_USART3_CLK_ENABLE();
	
	// CR2 Configuration
	UARTx->CR2 &= (unsigned short)(~(0x3000UL));
	UARTx->CR2 &= (unsigned short)(~(0x3UL << 12)); // 一个停止位
	
	// CR1 Configuration
	UARTx->CR1 &= (unsigned short)(~((unsigned short)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE)));
	UARTx->CR1 &= (unsigned short)(~(0x1UL << 12)); // data bit 8
	UARTx->CR1 &= (unsigned short)(~(0x1UL << 10)); // no parity
	
	// 波特率设置
	unsigned int integerdivider = 42000000UL; // APB2
	if(UARTx == USART1 || UARTx == USART6)
	{
		integerdivider = 84000000UL; // APB1
	}
	
	// Determine the integer part
	if((UARTx->CR1 & USART_CR1_OVER8) != 0)
	{
		// Integer part computing in case Oversampling mode is 8 Samples
		integerdivider = ((25UL * integerdivider) / (baudrate << 1));
	}
	else
	{
		// Integer part computing in case Oversampling mode is 16 Samples
		integerdivider = ((25UL * integerdivider) / (baudrate << 2));
	}
	
	baudrate = (integerdivider / 100) << 4;
	integerdivider = integerdivider - (100 * (baudrate >> 4));
	
	if((UARTx->CR1 & USART_CR1_OVER8) != 0)
	{
		baudrate |= (((integerdivider << 3) + 50) / 100) & 0x07U;
	}
	else
	{
		baudrate |= (((integerdivider << 4) + 50) / 100) & 0x0FU;
	}
	UARTx->BRR = (unsigned short)baudrate;
	
	UARTx->CR1 &=~(unsigned short)(0x1UL << 2); // 串口接收器禁能
	UARTx->CR1 &=~(unsigned short)(0x1UL << 5); // 串口接收中断禁能
	UARTx->CR1 |= (unsigned short)(0x1UL << 3); // 串口发送器使能
//	UARTx->CR1 |= (unsigned short)(0x1UL << 7); // 串口发送中断使能
	
	UARTx->CR1 |= (unsigned short)(0x1UL << 2); // 串口接收器使能
	UARTx->CR1 |= (unsigned short)(0x1UL << 5); // 串口接收中断使能
	UARTx->CR1 |= (unsigned short)(0x1UL << 13); // 串口使能
	
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

unsigned char uart_write_bytes(unsigned char *pbuf, unsigned int length)
{
	unsigned int timeout = 0;
	if(pbuf == NULL || length == 0)
	{
		return 1;
	}
	
	for(unsigned int i = 0; i < length; i++)
	{
		while((huart3.Instance->SR & 0x0080) == RESET) // 等待缓存区为空
		{
			if(++timeout > 0x1FFFF)
			{
				return 3;
			}
		}
		
		timeout = 0;
		huart3.Instance->DR = (unsigned short)pbuf[i] & 0x01FF;
		while((huart3.Instance->SR & 0x0040) == RESET) // 等待数据发送成功
		{
			if(++timeout > 0x1FFFF)
			{
				return 4;
			}
		}
	}
	
	return 0;
}

#define PACKET_LENGTH(a)	(((a)<<1)+6)
void uart_receive_event(unsigned char rxbyte)
{
	if(UartRecv.isstart != 0)
	{
		unsigned long curtime = OSTimeGet();
		if((curtime - UartRecv.lasttime) > 10)
		{
			UartRecv.rindex = 0;
		}
		UartRecv.lasttime = curtime;
		if(UartRecv.rindex == 0)
		{
			if(rxbyte == 0x90)
			{
				UartRecv.rxbuf[UartRecv.rindex++] = rxbyte;
			}
			else
			{
				// 干扰数据直接忽略
			}
		}
		else if(UartRecv.rindex == 1)
		{
			if(rxbyte == 0x02)
			{
				UartRecv.rxbuf[UartRecv.rindex++] = rxbyte;
			}
			else
			{
				UartRecv.rindex = 0;
			}
		}
		else
		{
			if(rxbyte == 0x90 && UartRecv.rxbuf[UartRecv.rindex-1] == 0x90)
			{
				// 直接忽略
			}
			else
			{
				UartRecv.rxbuf[UartRecv.rindex++] = rxbyte;
				if(UartRecv.rindex >= 6)
				{
					if(UartRecv.rindex >= PACKET_LENGTH(UartRecv.rxbuf[3]))
					{
						UartRecv.isstart = 0;
						bus_os_post_sem(&UartRecv.sem);
					}
				}
			}
		}
	}
	else
	{
		UartRecv.rindex = 0;
	}
}

static uint8_t UART_ReadWrite(uint8_t *pdata,uint8_t len,int32_t *pRecData)
{
	uint8_t retrycnt = 0;
	while(retrycnt++ < 5)
	{
//		printf("epos4 uart write[%d]:\r\n", len);
//		for(uint8_t i=0; i<len; ++i)
//		{
//			printf("%02X, ", pdata[i]);
//		}
//		printf("\r\n");
		if(uart_write_bytes(pdata, len) == 0)
		{
			UartRecv.rindex = 0;
			UartRecv.lasttime = OSTimeGet();
			UartRecv.isstart = 1;
			if(bus_os_pend_sem(&UartRecv.sem, 200) == 0)
			{
				*pRecData = 0;
				if(UartRecv.rxbuf[3] == 0x00)
				{
				}
				else if(UartRecv.rxbuf[3] == 0x01)
				{
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[5];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[4];
				}
				else if(UartRecv.rxbuf[3] < 0x04)
				{
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[7];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[6];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[5];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[4];
				}
				else
				{
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[11];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[10];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[9];
					*pRecData = (*pRecData << 8) + UartRecv.rxbuf[8];
				}
//				printf("epos4 uart recv[%d]:\r\n", *pRecData);
//				for(uint8_t i=0; i<UartRecv.rindex; ++i)
//				{
//					printf("%02X, ", UartRecv.rxbuf[i]);
//				}
//				printf("\r\n");
				return 0;
			}
			else  //没有读取到返回的数据  说明读失败
			{
				printf("uart recv wait timeout\r\n");
			}
		}
		else
		{
			printf("uart send failed[%d]\r\n", retrycnt);
		}
	}
	return UART_COMMUNICATION_TIMEOUT;
}

void USART3_IRQHandler(void)
{
	if(((0x1UL << 5) & huart3.Instance->CR1) != RESET && ((0x1UL << 5) & huart3.Instance->SR) != RESET)
	{
		uart_receive_event((unsigned char)(huart3.Instance->DR & (unsigned short)0x01FF));
	}
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

static uint16_t pDataCRC[64] = {0};
static uint8_t  TxEpos4data[128] = {0};
static uint8_t WriteReadFrame(uint8_t OpCode,uint16_t* pDataArray,uint8_t number,int32_t *pRecData)
{
	int16_t crc;
	uint8_t postion;

	pDataCRC[0] = OpCode | (number << 8);
	for (uint8_t i = 0; i < number; i++)
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
	if(bus_os_create_sem(&UartRecv.sem, 0) == 0)
	{
		uart_init(huart3.Instance, 115200UL);
		return 0;
	}
	return 1;
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
	uint32_t err=0;
	err+=EPOS4_WriteData(0x6040,0,0x08) ;
	err+=EPOS4_WriteData(0x6060,0,0x06) ;//设置工作模式
	err+=EPOS4_WriteData(0x607c,0,homeOffset);
	err+=EPOS4_WriteData(0x6099,1,500);
	err+=EPOS4_WriteData(0x6099,2,10);
	err+=EPOS4_WriteData(0x609A,0,1000);
	err+=EPOS4_WriteData(0x30b0,0,0);
	err+=EPOS4_WriteData(0x2080,0,currentTreshold);//currentTreshold 0x2080-00
	err+=EPOS4_WriteData(0x2081,0,homePosition);//homePosition 0x2081-00
	err+=EPOS4_WriteData(0x6098,0,34);
	err+=EPOS4_WriteData(0x6040,0,0x06) ;
	OSTimeDly(20);
	err+=EPOS4_WriteData(0x6040,0,0x0F) ;
	OSTimeDly(20);
	err+=EPOS4_WriteData(0x6040,0,0x1F) ;
	return (err!=0)? 1:0;
}




