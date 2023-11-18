#include "stm32f4xx.h"
#include "Soft_iic.h"
#include "delayus.h"


#define IIC_OBJECT_COUNT	((unsigned char)10) // IIC�������֧�ֶ������




// IIC���ԣ�
//       1��SDA���ϵ����ݱ�����SCL�ĸߵ�ƽ���ڱ����ȶ�
//       2��SDA�ߵĸ߻�͵�ƽ״ֻ̬����SCL �ߵ�ʱ���ź��ǵ͵�ƽʱ���ܸı�
//       3��IIC���ߵ�SDA��SCL�����ź���ͬʱ���ڸߵ�ƽʱ���涨Ϊ���ߵĿ���״̬
//       4����ʼ������SCL���Ǹߵ�ƽʱ SDA�ߴӸߵ�ƽ��͵�ƽ�л�
//       5��ֹͣ������SCL���Ǹߵ�ƽʱ SDA���ɵ͵�ƽ��ߵ�ƽ�л�
//       6������λ�����ʽ��MSB
//       7��ÿ���ֽں�����һ����Ӧλ


typedef struct _sIicDev
{
	unsigned int clk_pin; // IIC����ʱ������
	unsigned int sda_pin; // IIC������������
}sIicDev_t;


static sIicDev_t sIicDev[IIC_OBJECT_COUNT];

/********************************************************
 * �������ܣ�����IIC���߶���
 * ��    �Σ�clk_pin��IICʱ������
             sda_pin��IIC��������
 * �� �� ֵ��IIC���߶���NULL=IIC���߶����Ѿ��ﵽ����������GPIO��ʼ��ʧ��
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
IICHandle_t iic_object_create(unsigned int clk_pin, unsigned int sda_pin)
{
	static unsigned char iic_object_count = 0;
	
	if(iic_object_count >= IIC_OBJECT_COUNT)
	{
		return NULL; // IIC���߶����Ѿ��ﵽ������
	}
	
	sIicDev[iic_object_count].clk_pin = clk_pin;
	sIicDev[iic_object_count].sda_pin = sda_pin;
	
	// IIC���ߵ�SDA��SCL�����ź���ͬʱ���ڸߵ�ƽʱ���涨Ϊ���ߵĿ���״̬
	gpio_config(sIicDev[iic_object_count].clk_pin, eGPIO_OUT_PP_UP, 1); // SCL��ʼ��ƽΪ��
	gpio_config(sIicDev[iic_object_count].sda_pin, eGPIO_OUT_PP_UP, 1); // SDA��ʼ��ƽΪ��

	return ++iic_object_count;
}


/********************************************************
 * �������ܣ�����IIC���߶���
 * ��    �Σ�object��IIC���߶�����
 * �� �� ֵ��IIC���߶���NULL=���ΪNULL����δ������ָ���Ķ���
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
static sIicDev_t *iic_object_search(IICHandle_t handle)
{
	if(handle > IIC_OBJECT_COUNT || handle == NULL)
	{
		return NULL; // handle�쳣
	}
	
	return &sIicDev[handle - 1];
}


/********************************************************
 * �������ܣ�IIC�����ʼ�ź�
 * ��    �Σ�handle��IIC���߶�����
 * �� �� ֵ��0=�ɹ���1=���ΪNULL
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
unsigned int iic_start(IICHandle_t handle)
{
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 1;
	}
	
	// SDA��SCL�ڸߵ�ƽʱ��SDA���ͱ�ʾSTART
	// SCL���ͣ���ʾ���Դ�������
	gpio_write_pin(psIicDev->sda_pin, 1);
	gpio_write_pin(psIicDev->clk_pin, 1);
	delayUs(6); // ����ʱ�����4.7us
	gpio_write_pin(psIicDev->sda_pin, 0); // ������ʼ�ź�
	delayUs(6); // ����ʱ�����4us
	gpio_write_pin(psIicDev->clk_pin, 0); // ׼�����ͻ��������
	return 0;
}


/********************************************************
 * �������ܣ�IIC���ֹͣ�ź�
 * ��    �Σ�handle��IIC���߶�����
 * �� �� ֵ��0=�ɹ���1=���ΪNULL
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
unsigned int iic_stop(IICHandle_t handle)
{
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 1;
	}
	
	// SDA��SCL�ڵ͵�ƽʱ��SDA���߱�ʾSTOP
	// SCL���ߣ���ʾ�������ݽ���
	gpio_write_pin(psIicDev->clk_pin, 0);
	gpio_write_pin(psIicDev->sda_pin, 0);
	gpio_write_pin(psIicDev->clk_pin, 1); // �������ݴ���
	delayUs(6); // ����ʱ�����4us
	gpio_write_pin(psIicDev->sda_pin, 1); // ����ֹͣ�ź�
	delayUs(6); // ����ʱ�����4.7us
	return 0;
}


/********************************************************
 * �������ܣ�IIC�����ֽ�����(���õȴ�Ӧ��Ĺ���)
 * ��    �Σ�handle��IIC���߶�����
             data������
 * �� �� ֵ��0=���ͳɹ�(�յ�Ӧ��)��1=����ʧ��(Ӧ��ʱ)
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
unsigned int iic_write(IICHandle_t handle, unsigned char data)
{
	unsigned char timeout = 0;
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 2;
	}
	
	for(unsigned char i = 0; i < 8; i++)
	{
		gpio_write_pin(psIicDev->sda_pin, data & (0x80 >> i)); // �ȷ���λ
		gpio_write_pin(psIicDev->clk_pin, 1);
		delayUs(6); // ����ʱ�����4.7us
		gpio_write_pin(psIicDev->clk_pin, 0); // ����ʱ�ӿ�ʼ���ݴ���
		delayUs(2);
	}
	
	// �ȴ��ӻ�Ӧ��
	// ע���˴�SDA��������͵�ƽ����������stop�źţ�����IIC���߽���
	gpio_config(psIicDev->sda_pin, eGPIO_IN_UP, 0);
	gpio_write_pin(psIicDev->clk_pin, 1); // ֪ͨ�ӻ�����Ӧ���ź�
	delayUs(2);
	while(gpio_read_pin(psIicDev->sda_pin) != 0)
	{
		if(timeout++ > 0xF0)
		{
			timeout = 0xFF; // Ӧ��ʱ
			break;
		}
	}
	
	gpio_config(psIicDev->sda_pin, eGPIO_OUT_PP_UP, 0);
	gpio_write_pin(psIicDev->clk_pin, 0);
	return (timeout == 0xFF)? 1 : 0;
}


/********************************************************
 * �������ܣ�IIC���ֽ�����
 * ��    �Σ�handle��IIC���߶�����
             ack=0��ʾIIC������Ҫ��Ӧ�𣬷�����ҪӦ��
 * �� �� ֵ�����յ�������
 * �� �� �ߣ���־��
 * ά�����ڣ�2020��5��7��
 * �޶���־������
 ********************************************************/
unsigned char iic_read(IICHandle_t handle, unsigned char ack)
{
	unsigned char data = 0;
	sIicDev_t *psIicDev = iic_object_search(handle);
	if(psIicDev == NULL)
	{
		return 2;
	}
	
	// ע���˴�SDA��������͵�ƽ����������stop�źţ�����IIC���߽���
	gpio_config(psIicDev->sda_pin, eGPIO_IN_UP, 0);
	for(unsigned char i = 0; i < 8; i++)
	{
		gpio_write_pin(psIicDev->clk_pin, 1);
		delayUs(4);
		if(gpio_read_pin(psIicDev->sda_pin) != 0)
		{
			data |= 0x80 >> i; // �ȶ���λ
		}
		gpio_write_pin(psIicDev->clk_pin, 0);
		delayUs(2);
	}
	
	// ����Ӧ���ź�
	gpio_config(psIicDev->sda_pin, eGPIO_OUT_PP_UP, ack);
	gpio_write_pin(psIicDev->clk_pin, 1);
	delayUs(5); // ����ʱ�����4us
	gpio_write_pin(psIicDev->clk_pin, 0);
	return data;
}
