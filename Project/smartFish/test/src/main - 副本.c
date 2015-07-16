#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "spi.h"
#include "oled_spi.h"
#include "camera_dma.h"
#include "i2c.h"
#include "ov7725.h"
#include "dma.h"

void delay(int times){
    DelayMs(times);
}

unsigned char as[128]={0};
int as_pos = 0;
void UART_RX_ISR(uint16_t byteRec){
    as[as_pos++]=byteRec;
    as[as_pos]=0;
    if(as_pos>64){
        as_pos=0;
        as[0]=0;
    }
    GPIO_ToggleBit(HW_GPIOC, 3);
    printf("%c", byteRec);
    if(as[0]>0)LED_P8x16Str(0, 0, as);
    else LED_Fill(0);
}

/* 接收完成一场后 用户处理函数 */
void UserApp(uint32_t vcount)
{
    for(int y=0;y<4;y++)
    {
        for(int x=0;x<OV7620_W/8;x++)
        {
            printBin(gpHREF[10][x]);
            //LED_SetPos(x*8, y);
            //LED_WrDat(gpHREF[y][x]);
        }
        printf("\n");
    }

        //printf("%d", gpHREF[10][i]>127);
        //printBin(gpHREF[10][i]);
        //printf("%d,", gpHREF[10][i]);
    //printf("\n");
}

int main(void)
{
    DelayInit();
    /* 打印串口及小灯 */
    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);

    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    //UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);
    /* 注册中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);

    /* 开启UART Rx中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);

    printf("OLED test\r\n");

    initOLED();
    initCamera();
    while(1)
    {
        LED_Fill(0x01);
        //GPIO_ToggleBit(HW_GPIOC, 3);
        //delay(500);
    }
}
