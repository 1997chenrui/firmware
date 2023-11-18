#include "stm32f4xx.h"
#include "ws2812b.h"
#include "delayUs.h"

static void ws2812b_delay(unsigned int count)
{
	while(count--)
	{
		__NOP();
	}
}

static void ws2812b_write_byte(WS2812B_t* self, unsigned char wbyte)
{
	for(unsigned char i=0; i<8; ++i)
	{
		if(wbyte & (0x80 >> i))
		{
			self->m_write_pin(1);
			ws2812b_delay(15);
			self->m_write_pin(0);
		}
		else
		{
			self->m_write_pin(1);
			self->m_write_pin(0);
			ws2812b_delay(15);
		}
	}
}

static void ws2812b_write_color(WS2812B_t* self, unsigned int color)
{
	ws2812b_write_byte(self, (unsigned char)(color >> 16)); // green
	ws2812b_write_byte(self, (unsigned char)(color >> 8)); // red
	ws2812b_write_byte(self, (unsigned char)(color)); // blue
}

// color: byte0:blue, byte1:red, byte2:green
void ws2812b_write(WS2812B_t* self, unsigned int* color, unsigned char count)
{
	
	// ע: ������ʱ��Ҫ���ر��ϸ�,���Ա���Ҫ���жϱ���
	__disable_irq();
	for(unsigned char i=0; i<count; ++i)
	{
		ws2812b_write_color(self, color[i]);
	}
	
	__enable_irq();
	
	// ÿ֡���ݵļ���������50us
	delayUs(50);
}

void ws2812b_init(struct _WS2812B_t* self, WS2812B_PIN_WRITE write_pin)
{
    self->m_write_pin =  write_pin;
    self->write = ws2812b_write;
}











