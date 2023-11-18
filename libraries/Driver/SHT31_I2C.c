#include "SHT31_I2C.h" 
#include "delayus.h" 
#include <stdio.h>
#include <stm32f4xx.h>
#include "SHT31_IO.h" 
#define DELAY_USEC 100

static uint8_t sensirion_wait_while_clock_stretching(void)
{
    uint8_t timeout = 2;

    while (--timeout) {
        if (sensirion_SCL_read())
            return STATUS_OK;
        sensirion_sleep_usec(DELAY_USEC);
    }

    return STATUS_FAIL;
}

static uint8_t sensirion_i2c_write_byte(uint8_t data)
{
    uint8_t nack, i;
    for (i = 7; i >= 0; i--) {
        sensirion_SCL_out();
        if ((data >> i) & 0x01)
            sensirion_SDA_in();
        else
            sensirion_SDA_out();
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
        sensirion_sleep_usec(DELAY_USEC);
        if (sensirion_wait_while_clock_stretching())
            return STATUS_FAIL;
    }
    sensirion_SCL_out();
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    if (sensirion_wait_while_clock_stretching())
        return STATUS_FAIL;
    nack = (sensirion_SDA_read() != 0);
    sensirion_SCL_out();

    return nack;
}

static uint8_t sensirion_i2c_read_byte(uint8_t ack)
{
    s8 i;
    uint8_t data = 0;
    sensirion_SDA_in();
    for (i = 7; i >= 0; i--) {
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
        if (sensirion_wait_while_clock_stretching())
            return STATUS_FAIL;
        data |= (sensirion_SDA_read() != 0) << i;
        sensirion_SCL_out();
    }
    if (ack)
        sensirion_SDA_out();
    else
        sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    sensirion_sleep_usec(DELAY_USEC);
    if (sensirion_wait_while_clock_stretching())
        return STATUS_FAIL;
    sensirion_SCL_out();
    sensirion_SDA_in();

    return data;
}

static uint8_t sensirion_i2c_start(void)
{
    sensirion_SCL_in();
    if (sensirion_wait_while_clock_stretching())
        return STATUS_FAIL;

    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_out();
    sensirion_sleep_usec(DELAY_USEC);
    return STATUS_OK;
}

static void sensirion_i2c_stop(void)
{
    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
}

uint8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
    uint8_t ret;
    u16 i;

    ret = sensirion_i2c_start();
    if (ret != STATUS_OK)
        return ret;

    ret = sensirion_i2c_write_byte(address << 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        ret = sensirion_i2c_write_byte(data[i]);
        if (ret != STATUS_OK) {
            sensirion_i2c_stop();
            break;
        }
    }
    sensirion_i2c_stop();
    return ret;
}

uint8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
    uint8_t ret;
    uint8_t send_ack;
    u16 i;

    ret = sensirion_i2c_start();
    if (ret != STATUS_OK)
        return ret;

    ret = sensirion_i2c_write_byte((address << 1) | 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        send_ack = i < (count - 1); 
        data[i] = sensirion_i2c_read_byte(send_ack);
    }
    sensirion_i2c_stop();
    return STATUS_OK;
}

void sensirion_i2c_init()
{
    sensirion_init_pins();
    sensirion_SCL_in();
    sensirion_SDA_in();
}
