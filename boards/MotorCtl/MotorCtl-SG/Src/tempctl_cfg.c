#include "tempctl_cfg.h"
#include "ad5245.h"
#include "stm32f4xx_hal.h"


#include "../../drivers/ct7112c.h"
#include "../../drivers/soft_iic.h"
#include "stdio.h"

#include "ad5304.h"

#include "ms1112.h"

/************************************************************************

					CT7112 配置
**********************************************************************/
static SoftIIc SoftIICInstance;

static void SoftIIC_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,(GPIO_PinState)v);
}

static void SoftIIC_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,(GPIO_PinState)v);
}

static bool SoftIIC_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5);
}

static void SoftIIC_delayUs(volatile uint32_t us)
{
    uint32_t delay_i = 0,delay_j = 0;
	for(delay_j = 0; delay_j< us; delay_j++)
		for(delay_i = 0;delay_i<39;delay_i++);
}

static void SoftIIC_CFG_Init(void)
{
    SoftIICInstance.setSclOut=NULL;
    SoftIICInstance.setSdaOut=NULL;
    SoftIICInstance.setSdaIn=NULL;
	
    SoftIICInstance.writeScl=SoftIIC_writeScl_0;
    SoftIICInstance.writeSda=SoftIIC_writeSda_0;
    SoftIICInstance.readSda=SoftIIC_readSda_0;
    SoftIICInstance.delay=SoftIIC_delayUs;
    SoftIICInstance.ack_timeout=500;//应答超时时间 
	

}

CT7112 CT7112Instances[3];

static void CT7112_startWr(void)
{
    Soft_iic_stop(&SoftIICInstance);
    Soft_iic_start(&SoftIICInstance);   
}

static void CT7112_stopWr(void)
{
    Soft_iic_stop(&SoftIICInstance);
}



static uint16_t CT7112_readWord_0(uint8_t data)
{
	uint16_t buf;
	uint8_t msb,lsb;
	
	CT7112_startWr();
	
    if(Soft_iic_write(&SoftIICInstance,CT7112Instances[0].address_write)==Soft_iic_timeout)
    {
        printf("0 IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
    if(Soft_iic_write(&SoftIICInstance,data)==Soft_iic_timeout)
    {
        printf("0 IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	
	CT7112_startWr();
	
	 if(Soft_iic_write(&SoftIICInstance,CT7112Instances[0].address_read)==Soft_iic_timeout)
    {
        printf("0 IIC READ ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
	
	msb=Soft_iic_read(&SoftIICInstance,true);
	lsb=Soft_iic_read(&SoftIICInstance,false);
	
	CT7112_stopWr();
	
	buf=(msb<<8)|lsb;
	
	
	printf("buf=%x\r\n",buf);
	return buf; 
}

static uint16_t CT7112_readWord_1(uint8_t data)
{
	uint16_t buf;
	uint8_t msb,lsb;
    if(Soft_iic_write(&SoftIICInstance,CT7112Instances[1].address_write)==Soft_iic_timeout)
    {
        printf("1 IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
    if(Soft_iic_write(&SoftIICInstance,data)==Soft_iic_timeout)
    {
        printf("1 IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	CT7112_startWr();
	 if(Soft_iic_write(&SoftIICInstance,CT7112Instances[1].address_read)==Soft_iic_timeout)
    {
        printf("1 IIC READ ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
	
	msb=Soft_iic_read(&SoftIICInstance,true);
	lsb=Soft_iic_read(&SoftIICInstance,false);
	
	buf=(msb<<8)|lsb;
	
	return buf; 
}

static uint16_t CT7112_readWord_2(uint8_t data)
{
	uint16_t buf;
	uint8_t msb,lsb;
    if(Soft_iic_write(&SoftIICInstance,CT7112Instances[2].address_write)==Soft_iic_timeout)
    {
        printf("2 IIC WRITE ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
    if(Soft_iic_write(&SoftIICInstance,data)==Soft_iic_timeout)
    {
        printf("2 IIC WRITE DATA TIME OUT !!! \r\n");
        return false; 
    }
	CT7112_startWr();
	 if(Soft_iic_write(&SoftIICInstance,CT7112Instances[2].address_read)==Soft_iic_timeout)
    {
        printf("2 IIC READ ADDRESS TIME OUT !!! \r\n");
        return false;  
    }
	
	msb=Soft_iic_read(&SoftIICInstance,true);
	lsb=Soft_iic_read(&SoftIICInstance,false);
	
	buf=(msb<<8)|lsb;
	printf("buf=%x\r\n",buf);
	return buf; 
}

void CT7112_CFG_INIT(void)
{
    SoftIIC_CFG_Init();

    CT7112Instances[0].address_write=0x92;
    CT7112Instances[0].address_read=0x93;
    CT7112Instances[0].startWR=CT7112_startWr;
    CT7112Instances[0].stopWR=CT7112_stopWr;
    CT7112Instances[0].readWord=CT7112_readWord_0;
    CT7112Instances[0].writeWord=NULL;

   
    CT7112Instances[1].address_write=0x90;
    CT7112Instances[1].address_read=0x91;
    CT7112Instances[1].startWR=CT7112_startWr;
    CT7112Instances[1].stopWR=CT7112_stopWr;
    CT7112Instances[1].readWord=CT7112_readWord_1;
    CT7112Instances[1].writeWord=NULL;
	
	CT7112Instances[2].address_write=0x94;
    CT7112Instances[2].address_read=0x95;
    CT7112Instances[2].startWR=CT7112_startWr;
    CT7112Instances[2].stopWR=CT7112_stopWr;
    CT7112Instances[2].readWord=CT7112_readWord_2;
    CT7112Instances[2].writeWord=NULL;

}

void Ct7122_Test()
{
	for(uint8_t i=0;i<3;i++)
	{
		uint8_t i =0;
		printf("id=%d,temp=%f\r\n",i,CT7112Read_Temp(&CT7112Instances[i]));
	}
}

/************************************************************************

					AD5304 配置
**********************************************************************/
AD5304 AD5304Instances;

static void AD5304_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,(GPIO_PinState)v);
}

static void AD5304_writesync_0(bool v)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,(GPIO_PinState)v);
}

static void AD5304_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,(GPIO_PinState)v);
}

static bool AD5304_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1);
}

void AD5304_CFG_Init(void)
{
  
    AD5304Instances.writeScl=AD5304_writeScl_0;
    AD5304Instances.writeSda=AD5304_writeSda_0;
    AD5304Instances.readSda=AD5304_readSda_0;
    AD5304Instances.delay=SoftIIC_delayUs;
    AD5304Instances.writeSync= AD5304_writesync_0;
	

	Ad5304_WriteData(&AD5304Instances,0x00,0);
	Ad5304_WriteData(&AD5304Instances,0x01,0);
	Ad5304_WriteData(&AD5304Instances,0x02,0);
	Ad5304_WriteData(&AD5304Instances,0x03,0);
}

/************************************************************************

					MS1112 配置
**********************************************************************/
MS1112 MS1112Instances;

static SoftIIc MS1112_SoftIICInstance;

static void MS1112_SoftIIC_writeScl_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,(GPIO_PinState)v);
}

static void MS1112_SoftIIC_writeSda_0(bool v)
{
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,(GPIO_PinState)v);
}

static bool MS1112_SoftIIC_readSda_0(void)
{
    return (bool)HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4);
}

static void MS1112_startWr(void)
{
    Soft_iic_stop(&MS1112_SoftIICInstance);
    Soft_iic_start(&MS1112_SoftIICInstance);   
}

static void MS1112_stopWr(void)
{
    Soft_iic_stop(&MS1112_SoftIICInstance);
}

static bool MS1112_Write_Byte_0(uint8_t byte)
{
    if(Soft_iic_write(&MS1112_SoftIICInstance,byte)==Soft_iic_timeout)
    {
        return false;  
    }
	return true;  
}

static uint8_t MS1112_Read_Byte_0(bool n)
{
    return Soft_iic_read(&MS1112_SoftIICInstance,n);  
}

void MS1112_CFG_Init(void)
{
    MS1112_SoftIICInstance.setSclOut=NULL;
    MS1112_SoftIICInstance.setSdaOut=NULL;
    MS1112_SoftIICInstance.setSdaIn=NULL;
	
    MS1112_SoftIICInstance.writeScl=MS1112_SoftIIC_writeScl_0;
    MS1112_SoftIICInstance.writeSda=MS1112_SoftIIC_writeSda_0;
    MS1112_SoftIICInstance.readSda=MS1112_SoftIIC_readSda_0;
    MS1112_SoftIICInstance.delay=SoftIIC_delayUs;
    MS1112_SoftIICInstance.ack_timeout=500;//应答超时时间 
	
	
	MS1112Instances.address_read=0x91;
	MS1112Instances.address_write=0x90;
	
	MS1112Instances.startWR=MS1112_startWr;
	MS1112Instances.stopWR=MS1112_stopWr;
	
	MS1112Instances.readbyte=MS1112_Read_Byte_0;
	MS1112Instances.writebyte=MS1112_Write_Byte_0;

}

void MS1112_Test()
{
	uint16_t value;
	
	value=Ms1112_ReadLastValueAndStartNext(&MS1112Instances,0,0);
	OSTimeDly(200);
	value=Ms1112_ReadLastValueAndStartNext(&MS1112Instances,0,0);
	
	printf("ms1112Value_ch0=%d\r\n",value);
	
	OSTimeDly(200);
	value=Ms1112_ReadLastValueAndStartNext(&MS1112Instances,1,0);
	OSTimeDly(200);
	value=Ms1112_ReadLastValueAndStartNext(&MS1112Instances,1,0);
	
	printf("ms1112Value_ch1=%d\r\n",value);
	
	
}
static u8 target0_over(f32 current);
static void hd0_open(u8 open);
static void temp0_hd_init(void);
static f32 adjust0_to_code(f32 adjust);
static float read_temperature(u8 i);
static float read_buffer_temperature(u8 i);

//加热
Temp_thread_t temperature[TEMP_CTRL_CNT] = {
    {
         .is_start = 1,
         .lowest_power = 255,
         .pid_cfg.type = POSITION,
         .pid_cfg.target = 37,
         .pid_cfg.kp = 200,
         .pid_cfg.ki = 10,
         .pid_cfg.kd = 50,
         .pid_cfg.limit = 255,
         .pid_cfg.separate_i_range = 2
    }
};

                             
void temp_hd_init(void) {
    temp0_hd_init();
}


static void temp0_hd_init(void) {
    Temp_thread_t *self = &temperature[0];
    self->iic_id = ad5245_create(PB1, PB0);
    // 打开LM控制开关
    gpio_config(PE1, eGPIO_OUT_PP_UP, 0);
   
	
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust0_to_code;
    self->read_buffer_temperature = read_buffer_temperature;
    self->temperature_read = read_temperature;
    self->target_over = target0_over;
    self->hd_open = hd0_open;
}




/**
 * @brief 将pid输出的功率转换成实际电压，注：-adjust代表实际pid功率
 *
 * @param adjust pid输出的功率
 * @return code
 */
static f32 adjust0_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 2.855;
    f32 max_voltage = 14.465;
    //转化为电压 由电子提供公式
    f32 voltage = max_voltage * adjust/self->pid_cfg.limit;
    voltage = voltage < min_voltage ? min_voltage : (voltage > max_voltage ? max_voltage : voltage);
    //转化成code
    return 208.64 / (voltage - 1.63) - 0.256;
}

/**
 * @brief  关闭温控开关
 *
 * @return none
 */
static void hd0_open(u8 open){
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, open ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/**
 * @brief 当前温度是否超过阈值，加热时为 > ， 制冷时为 <
 * 
 * @param current 当前温度
 * @return u8 0 未超过 1 超过
 */
static u8 target0_over(f32 current){
    return current > temperature[0].pid->entity.cfg.target;
}

static float read_temperature(u8 i){
    return 0;
}

static float read_buffer_temperature(u8 i){
    return 0;
}
