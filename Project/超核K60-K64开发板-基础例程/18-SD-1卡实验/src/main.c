#include "gpio.h"
#include "uart.h"
#include "sd.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：SD卡实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：获取内存卡的大小，读取0扇区的数据，使用串口发送出去
      小灯周期性闪烁，闪烁时间间隔500ms     
*/
uint8_t sd_buffer[512];  //SD卡数据缓存

int main(void)
{
    uint32_t i;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("SD test\r\n");
    printf("please insert SD card...\r\n");
    //SD卡模块快速初始化，设置速度为20000000
    SD_QuickInit(20000000);
    //获取SD卡的容量
    printf("SD size:%dMB\r\n", SD_GetSizeInMB());
    /* 读取0扇区数据，每一个扇区512字节 */
    SD_ReadSingleBlock(0, sd_buffer);
    /* 打印0扇区数据 */
    printf("sectoer 0 data:\r\n");
    for(i = 0; i < 512; i++)
    {
        printf("0x%02X ", sd_buffer[i]);
    }
    while(1)
    {
        //小灯闪烁
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


