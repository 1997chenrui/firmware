#include "DHT11.h" 
#include "delayus.h" 
#include <stdio.h>


//复位DHT11 
void DHT11_Rst(DHT11_Struct * dht)     
{                  
    // DHT11_IO_OUT();  //SET OUTPUT 
    dht->dir_io_out();
    dht->data_io_write(0);
    // DHT11_DQ_OUT=0;  //拉低DQ 
    delayMs(20);     //拉低至少18ms 
    // DHT11_DQ_OUT=1;  //DQ=1  
    dht->data_io_write(1);
    delayUs(30);      //主机拉高20~40us 
} 
//等待DHT11的回应 
//返回1:未检测到DHT11的存在 
//返回0:存在 
uint8_t DHT11_Check(DHT11_Struct * dht)      
{    
    uint8_t retry=0; 
    // DHT11_IO_IN();//SET INPUT   
    dht->dir_io_in();
    while (dht->data_io_read()&&retry<100)//DHT11会拉低40~80us 
    { 
        retry++; 
        delayUs(1); 
    };   
    if(retry>=100)return 1; 
    else retry=0; 
    while (!dht->data_io_read()&&retry<100)//DHT11拉低后会再次拉高40~80us 
    { 
        retry++; 
        delayUs(1); 
    }; 
        if(retry>=100)return 1;      
        return 0; 
    } 
// 从DHT11读取一个位 
// 先判断此时引脚的电平状态，（读数据时序从低电平开始）若不是低电平，等待变为低电平；若是低电平就一直循环等待，直到高电平出现，
// 高电平出现后延时40us，并读取延时后的电平状态，如果此时是高电平，则数据为1，否则为0
// 返回值：1/0 
uint8_t DHT11_Read_Bit(DHT11_Struct * dht)    
{ 
    uint8_t retry=0; 
    while(dht->data_io_read()&&retry<100)//等待变为低电平 
    { 
        retry++; 
        delayUs(1); 
    } 
    retry=0; 
    while(!dht->data_io_read()&&retry<100)//等待变高电平 
    { 
        retry++; 
        delayUs(1); 
    } 
    delayUs(40);//等待40us 
    if(dht->data_io_read())return 1; 
    else return 0;     
} 
//从DHT11读取一个字节 
//返回值：读到的数据 
uint8_t DHT11_Read_Byte(DHT11_Struct * dht)     
{         
    uint8_t i,dat; 
    dat=0; 
    for (i=0;i<8;i++)  
    { 
        dat<<=1;  
        dat|=DHT11_Read_Bit(dht); 
	}      
	return dat; 
} 

//从DHT11读取一次数据 
//temp:温度值(范围:0~50°) 
//humi:湿度值(范围:20%~90%) 
//返回值：0,正常;1,读取失败 
uint8_t DHT11_Read_Data(DHT11_Struct * dht,uint8_t *temp,uint8_t *humi)     
{         
  uint8_t buf[5]; 
    uint8_t i; 
    DHT11_Rst(dht); 
    if(DHT11_Check(dht)==0) 
    { 
        for(i=0;i<5;i++)//读取40位数据 
        { 
			buf[i]=DHT11_Read_Byte(dht); 
        } 
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]) 
        { 
            *humi=buf[0]; 
            *temp=buf[2]; 
        } 
    }else 
	{
		printf("DHT11 err\r\n");
		return 1;
	}
	 
    return 0;      
} 
//初始化DHT11的IO口 DQ 同时检测DHT11的存在 
//返回1:不存在 
//返回0:存在       
uint8_t DHT11_Init(DHT11_Struct * dht,
					void (*dir_io_in)(),
					void (*dir_io_out)(),
					void (*data_io_write)(uint8_t data),
					uint8_t (*data_io_read)()) 
{ 
    // RCC->APB2ENR|=1<<8;    //使能PORTG口时钟  
    // GPIOG->CRH&=0XFFFF0FFF;//PORTG.11 推挽输出 
    // GPIOG->CRH|=0X00003000; 
    // GPIOG->ODR|=1<<11;      //输出1
	dht->data_io_read = data_io_read;
	dht->data_io_write = data_io_write;
	dht->dir_io_in = dir_io_in;
	dht->dir_io_out = dir_io_out;
	
    DHT11_Rst(dht); 
    return DHT11_Check(dht); 
}

