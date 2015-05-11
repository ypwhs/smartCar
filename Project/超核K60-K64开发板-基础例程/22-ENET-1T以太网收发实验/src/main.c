#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "enet.h"
#include "ksz8041.h"

#define     ENET_TYPE_ARP   {0x08, 0x06}
#define     ENET_TYPE_IP    {0x08, 0x00}
uint8_t     gCfgLoca_MAC[] = {0x22, 0x22, 0x22, 0x00, 0x00, 0x01};
uint8_t     gCfgDest_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t     gTxBuffer[1500];
uint8_t     gCfgEnet_Type[] = ENET_TYPE_ARP;
 
void ENET_ISR(void)
{
    static uint32_t cnt = 0;
    uint32_t len;
    len = ENET_MacReceiveData(gTxBuffer);
    if(len)
    {
        printf("enet frame received len:%d @ %d\r\n", len, cnt++);
    }
}

/* 需要将开发版的以太网跳线短接 CRSDV  RXD0 RXD1 */

int main(void)
{
    int r;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("ENET test!\r\n");
    printf("RMII clock is fiexd to OSCERCLK and must be 50Mhz\r\n");
    
    /* enable PinMux */
    PORT_PinMuxConfig(HW_GPIOB, 0, kPinAlt4);
	PORT_PinPullConfig(HW_GPIOB, 0, kPullUp);
	PORT_PinOpenDrainConfig(HW_GPIOB, 0, ENABLE);

    PORT_PinMuxConfig(HW_GPIOB, 1, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 5, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 12, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 13, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 14, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 15, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 16, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 17, kPinAlt4);
    
    r = ksz8041_init();
    if(r)
    {
        printf("ksz8041 init failed! code:%d\r\n", r);
        while(1);
    }
    if(!ksz8041_is_linked())
    {
        printf("no wire connected\r\n");
    }
    
    ENET_InitTypeDef ENET_InitStruct1;
    ENET_InitStruct1.pMacAddress = gCfgLoca_MAC;
    ENET_InitStruct1.is10MSpped = ksz8041_is_phy_10m_speed();
    ENET_InitStruct1.isHalfDuplex = !ksz8041_is_phy_full_dpx();
    ENET_Init(&ENET_InitStruct1);
    ENET_CallbackRxInstall(ENET_ISR);
    ENET_ITDMAConfig(kENET_IT_RXF);

    gTxBuffer[0]  = gCfgDest_MAC[0];
    gTxBuffer[1]  = gCfgDest_MAC[1];
    gTxBuffer[2]  = gCfgDest_MAC[2];
    gTxBuffer[3]  = gCfgDest_MAC[3];
    gTxBuffer[4]  = gCfgDest_MAC[4];
    gTxBuffer[5]  = gCfgDest_MAC[5];
    
    gTxBuffer[6]  = gCfgLoca_MAC[0];
    gTxBuffer[7]  = gCfgLoca_MAC[1];
    gTxBuffer[8]  = gCfgLoca_MAC[2];
    gTxBuffer[9]  = gCfgLoca_MAC[3];
    gTxBuffer[10] = gCfgLoca_MAC[4];
    gTxBuffer[11] = gCfgLoca_MAC[5];
  
    gTxBuffer[12] = gCfgEnet_Type[0];
    gTxBuffer[13] = gCfgEnet_Type[1];
    
    while(1)
    {
        ENET_MacSendData(gTxBuffer, sizeof(gTxBuffer));
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


