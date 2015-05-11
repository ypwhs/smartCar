#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "spi.h"
#include "ili9163.h"
#include "gui.h"


int main(void)
{
    uint32_t i;
    DelayInit();
    /* 初始化一个引脚 设置为推挽输出 */
    GPIO_QuickInit(HW_GPIOC, 16, kGPIO_Mode_OPP);
    /* 初始化一个串口 使用UART0端口的PTD6引脚和PTD7引脚作为接收和发送，默认设置 baud 115200 */
    UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);
    printf("ili9163 lcd test\r\n");
    GUI_Init();
    GUI_DispString("ucGUI");
    GUI_DispString(GUI_GetVersionString());
    GUI_DispString("\r\nHello world!");
    while(1) 
    {
        GUI_DispDecAt( i++, 20,20,4);
        if (i>9999)
        {
            i=0;
        }
    }
}


