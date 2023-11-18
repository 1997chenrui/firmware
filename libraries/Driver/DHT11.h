#ifndef __DHT11_H 
#define __DHT11_H  
#include <stdint.h>
  
//IO方向设置 
// #define DHT11_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;} 
// #define DHT11_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;} 
////IO操作函数     
// #define DHT11_DQ_OUT Gout(11) //数据端口 PA0  
// #define DHT11_DQ_IN  Gin(11)  //数据端口 PA0  

typedef struct{
    void (*dir_io_in)();
    void (*dir_io_out)();

    void (*data_io_write)(uint8_t data);
    uint8_t (*data_io_read)();
}DHT11_Struct;

uint8_t DHT11_Init(DHT11_Struct * dht,
					void (*dir_io_in)(),
					void (*dir_io_out)(),
					void (*data_io_write)(uint8_t data),
					uint8_t (*data_io_read)());//初始化DHT11 
uint8_t DHT11_Read_Data(DHT11_Struct * dht,uint8_t *temp,uint8_t *humi);//读取温湿度 
uint8_t DHT11_Read_Byte(DHT11_Struct * dht);//读出一个字节 
uint8_t DHT11_Read_Bit(DHT11_Struct * dht);//读出一个位 
uint8_t DHT11_Check(DHT11_Struct * dht);//检测是否存在DHT11 
void DHT11_Rst(DHT11_Struct * dht);//复位DHT11     

#endif 
