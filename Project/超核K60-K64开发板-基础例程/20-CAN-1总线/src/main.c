#include "gpio.h"
#include "uart.h"
#include "can.h"


#ifdef MK64F12
#error "yandld: MK64 only has CAN0, no CAN1"
#endif

#define CAN_TX_ID  0x10
#define CAN_RX_ID  0x56

/*
     实验名称：CAN通信测试
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用CAN1模块的3号邮箱采用中断的方式接收来自0x56的数据
         使用2号邮箱向0x10地址的设备发生数据，时间间隔是500毫秒 
*/

/* CAN通信 中断回调函数，在中断中处理接收到的数据 */
void CAN_ISR(void)
{
    static uint32_t cnt;
    uint8_t buf[8];
    uint8_t len;
    if(CAN_ReadData(HW_CAN1, 3, buf, &len) == 0)
    {
        printf("DataReceived:%d ", cnt++);
        while(len--)
        {
            printf("[%d]:0x%X ", len, buf[len]);
        }
        printf("\r\n");
    }
}


int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("CAN test\r\n");
    /* 初始化 CAN 使用CAN1模块的PTE24/25引脚，通信速度为125k*/
    CAN_QuickInit(CAN1_TX_PE24_RX_PE25, kCAN_Baudrate_125K);
    
    /* 设置接收中断 安装回调函数 */
    CAN_CallbackInstall(HW_CAN1, CAN_ISR);
    /* 开启CAN通信中断接收功能，3号邮箱 */
    CAN_ITDMAConfig(HW_CAN1,3, kCAN_IT_RX);
    
    /* 设置 3号邮箱为CAN接收邮箱 */
    CAN_SetReceiveMB(HW_CAN1, 3, CAN_RX_ID);
    while(1)
    {
        /* 使用邮箱2 发送ID:0x10 发送 "CAN TEST" */
        CAN_WriteData(HW_CAN1, 2, CAN_TX_ID, (uint8_t *)"CAN TEST", 8); /* 使用邮箱2 发送ID:0x10 发送 "CAN TEST" */
        DelayMs(500);
        GPIO_ToggleBit(HW_GPIOE, 6);
    }
}


