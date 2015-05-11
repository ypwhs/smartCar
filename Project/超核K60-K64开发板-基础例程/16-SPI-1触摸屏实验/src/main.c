#include "gpio.h"
#include "uart.h"
#include "ads7843.h"
#include "ili9320.h"
#include "spi.h"

#include "gui.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：SPI触摸屏实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：在触控屏上画图    
*/

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("spi touch screen test\r\n");
    
    /* 初始化SPI2 接口连接触控屏的触控芯片*/
    SPI_QuickInit(SPI2_SCK_PD12_SOUT_PD13_SIN_PD14, kSPI_CPOL0_CPHA0, 2*1000*1000);

    PORT_PinMuxConfig(HW_GPIOD, 11, kPinAlt2); /* SPI2_PCS0 */
    /* 初始化触控芯片 使用CS0片选 */
    ads7843_init(HW_SPI2, HW_SPI_CS0);
    
    GUI_Init();//液晶屏界面初始化
    GUI_SetBkColor(GUI_WHITE); //设置背景色
    GUI_SetColor(GUI_BLACK);   //设置前景色
    GUI_Clear();  //清屏
    GUI_SetPenSize(3); //设置点的大小
    GUI_DispString("Measurement of\nA/D converter values");
    while (1)
    {
        GUI_PID_STATE TouchState;
        int xPhys, yPhys;
        GUI_TOUCH_GetState(&TouchState);  /* 获得触控位置 */
        xPhys = GUI_TOUCH_GetxPhys();     /* 获得x轴的测量值 */
        yPhys = GUI_TOUCH_GetyPhys();     /* 获得y轴的测量值 */
        /* 显示测量结果 */
        GUI_SetColor(GUI_BLUE);
        GUI_DispStringAt("Analog input:\n", 0, 20);
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(xPhys, 4);
        GUI_DispString(", y:");
        GUI_DispDec(yPhys, 4);
        /* Display the according position */
        GUI_SetColor(GUI_RED);
        GUI_GotoY(GUI_GetDispPosY() + 4);
        GUI_DispString("\nPosition:\n");
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(TouchState.x,4);
        GUI_DispString(", y:");
        GUI_DispDec(TouchState.y,4);
        GUI_DrawPoint(TouchState.x, TouchState.y);
        GUI_TOUCH_Exec();
    }
}


