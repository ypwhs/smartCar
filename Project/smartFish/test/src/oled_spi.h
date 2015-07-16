#include "common.h"
#include "spi.h"
#include "gpio.h"

#define uchar8 unsigned char

void initOLED();
void LED_WrDat(uchar8 ucData);
void LED_WrCmd(uchar8 ucCmd);
void LED_SetPos(uchar8 ucIdxX, uchar8 ucIdxY);
void LED_Fill(uchar8 ucData);
void LED_P8x16Str(uchar8 ucIdxX, uchar8 ucIdxY, uchar8 ucDataStr[]);