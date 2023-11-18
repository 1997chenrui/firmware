#ifndef LS165_HC595_H_
#define LS165_HC595_H_

#include <stdint.h>

extern volatile uint32_t HC595_Data;
void HC595_WriteData(uint32_t *state,uint8_t cnt,uint32_t data);
void HC595_WriteBit(uint32_t *state,uint8_t cnt,uint8_t BitId,uint8_t val);
void HC595_ToggleBit(uint32_t *state,uint8_t cnt,uint8_t BitId);
uint32_t LS165_ReadData(uint8_t cnt);
uint8_t LS165_ReadBit(uint8_t cnt,uint8_t BitId);

#endif /* __74LS165_DRV__ */


