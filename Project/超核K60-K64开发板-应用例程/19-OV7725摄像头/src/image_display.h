#ifndef _IMAGE_DISPLAY_H_
#define _IMAGE_DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "chgui.h"

//!< API functions
void GUI_DispCCDImage(int x, int y, int xSize, int ySize, uint8_t** ppData);
void SerialDispCCDImage(int xSize, int ySize, uint8_t** ppData); 



#endif

