#ifndef __SHT31_IO_H 
#define __SHT31_IO_H  
#include <stdint.h>

void sensirion_SDA_in();
void sensirion_SDA_out();
uint8_t sensirion_SDA_read();
void sensirion_SCL_in();
void sensirion_SCL_out();
uint8_t sensirion_SCL_read();
void sensirion_sleep_usec(int useconds);

#endif
