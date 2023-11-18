#ifndef __SHT31_I2C_H 
#define __SHT31_I2C_H  
#include <stdint.h>
#include <stm32f4xx.h>
#include <stdio.h>

//static uint8_t sensirion_wait_while_clock_stretching(void);
//static uint8_t sensirion_i2c_write_byte(uint8_t data);
//static uint8_t sensirion_i2c_read_byte(uint8_t ack);
//static uint8_t sensirion_i2c_start(void);
//static void sensirion_i2c_stop(void);
uint8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count);
uint8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count);
void sensirion_i2c_init();

#endif