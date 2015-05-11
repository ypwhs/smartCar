#ifndef _CHGUI_H_
#define _CHGUI_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
	#define NULL 0
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

typedef struct
{
    char* mame;
    uint8_t  x_size;
    uint8_t  y_size;
    const char* data;
}chgui_font_t;



//API functions
void CHGUI_Init(void);
void GUI_DrawPixel(int color, int x, int y);
void GUI_DispChar(char c, int x, int y);
int GUI_printf(int x, int y, const char *format,...);
void GUI_HLine(int c ,int x1, int x2, int y);
void GUI_VLine(int c ,int y1, int y2, int x);
void GUI_Fill(int c ,int x1, int y1, int x2, int y2);


#endif
