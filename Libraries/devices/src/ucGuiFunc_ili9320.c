#include "ili9320.h"
#include "ads7843.h"
#include "common.h"

/* 重定义 GUI接口函数 */

#define SAMP_CNT 4
#define SAMP_CNT_DIV2 2
static int buf[2];
/* 滤波 */
static int ads_filter(int* buf)
{
    int i, j, k, min;
    int temp;
    int tempXY[2][SAMP_CNT];

    for(i=0; i<SAMP_CNT; i++)
    {
        ads7843_readX(&buf[0]);
        ads7843_readY(&buf[1]);
        tempXY[0][i] = buf[0];
        tempXY[1][i] = buf[1];
    }
    for(k=0; k<2; k++)
    {
        for(i=0; i<SAMP_CNT-1; i++)
        {
            min=i;
            for (j=i+1; j<SAMP_CNT; j++)
            {
                if (tempXY[k][min] > tempXY[k][j]) min=j;
            }
            temp = tempXY[k][i];
            tempXY[k][i] = tempXY[k][min];
            tempXY[k][min] = temp;
        }
        if((tempXY[k][SAMP_CNT_DIV2]-tempXY[k][SAMP_CNT_DIV2-1]) > 5)
        return 1;
        buf[k] = (tempXY[k][SAMP_CNT_DIV2]+tempXY[k][SAMP_CNT_DIV2-1]) / 2;
    }
    return 0;
}

int GUI_TOUCH_X_MeasureX(void)
{
    ads_filter(buf);
    return buf[0];
}

int GUI_TOUCH_X_MeasureY(void)
{
    return buf[1];
}

int LCD_L0_Init(void)
{
    ili9320_init();
    return 0;
}

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex)
{
    ili9320_write_pixel(x, y, PixelIndex);
}

unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
    return ili9320_read_pixel(x, y); 
}

