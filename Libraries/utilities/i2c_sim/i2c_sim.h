#ifndef __FSL_I2C_SIM_H__
#define __FSL_I2C_SIM_H__

#include <stdint.h>

//!< API functions
void I2C_SIM_Init(uint32_t instance, uint8_t sdaPin, uint8_t sclPin);
int I2C_SIM_BurstRead(uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t* buf, uint32_t len);
int I2C_SIM_BurstWrite(uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t *buf, uint32_t len);
uint8_t I2C_SIM_Probe(uint8_t addr);

#endif
