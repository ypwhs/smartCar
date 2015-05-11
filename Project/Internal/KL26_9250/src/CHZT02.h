#ifndef CHZT02_H__
#define CHZT02_H__
#include "i2c.h"
#define CHZT02_SensorEN_Group 			HW_GPIOD
#define CHZT02_SensorEN_Pin				7
#define CHZT02_SensorEN_OutputMode 		kGPIO_Mode_OPP
#define CHZT02_SensorEnable				0
#define CHZT02_SensorDisable			1
#define CHZT02_I2C_MAP				 	I2C0_SCL_PB00_SDA_PB01


int8_t CHZT02_Init(void);
int8_t CHZT02_Running (void);
#else
#endif
