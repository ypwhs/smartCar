#ifndef __CH_MB85RC64_H__
#define __CH_MB85RC64_H__

#include <stdint.h>
#include <stdbool.h>

//! API functions
void MB85RC64_Init(uint32_t I2CMAP);
int MB85RC64_Read(uint16_t pos, uint8_t * buf, uint32_t len);
int MB85RC64_WriteByte(uint16_t pos, uint8_t data);
uint8_t MB85RC64_ReadByte(uint16_t pos);
bool MB85RC64_Test(uint32_t MAP);

#endif



