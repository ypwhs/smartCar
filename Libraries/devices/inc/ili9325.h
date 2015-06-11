#ifndef __ILI9325_H__
#define __ILI9325_H__

#include <stdint.h>

//FlexBus总线定义
#define ILI9325_BASE                    (0x70000000)
#define ILI9325_COMMAND_ADDRESS         *(unsigned short *)0x70000000
#define ILI9325_DATA_ADDRESS            *(unsigned short *)0x78000000

//写数据，写命令
#define WMLCDCOM(cmd)	   {ILI9325_COMMAND_ADDRESS = cmd;}
#define WMLCDDATA(data)	   {ILI9325_DATA_ADDRESS = data;}

#define LCD_X_MAX   (240)
#define LCD_Y_MAX   (320)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//!< API functions
void ili9325_init(void);
void ili9325_clear(int c);
void ili9325_write_pixel(int x, int y, int c);
void ili9325_set_window(int x, int y, int xlen, int ylen);
int ili9325_read_pixel(int x, int y);



#endif

