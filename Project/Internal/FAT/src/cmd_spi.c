#include "shell.h"
#include "spi.h"
#include "gpio.h"
#include "board.h"
#include "spi_abstraction.h"
#include "w25qxx.h"
#include "sram.h"
#include "ads7843.h"
#include "dma.h"

static uint8_t W25QXX_READ_ID_TABLE[] = {0x90, 0x00, 0x00, 0x00, 0xFF, 0xFF};
static uint8_t test_buffer[8];
void SPI_ISR(void)
{
    static uint8_t i = 1;
    static uint16_t temp = 0;
    if(i == 7)
    {
        SPI_ITDMAConfig(BOARD_SPI_INSTANCE, kSPI_IT_TCF, false);
        shell_printf("ID:0x%X\r\n", temp);
        return ;
    }
    if(i == 6)
    {
        temp <<= 8;
    }
    temp |= SPI_ReadWriteByte(BOARD_SPI_INSTANCE,HW_CTAR0, W25QXX_READ_ID_TABLE[i], 1, kSPI_PCS_KeepAsserted);
    i++;
}

extern int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance);
static int DO_SPI_FLASH(int argc, char * const argv[])
{
    uint32_t i;
    int x,y;
    #define SPI_FLASH_TEST_LEN  (1024*512)
    volatile uint8_t* buf_test = SRAM_START_ADDRESS;
    uint32_t ret;
    static struct spi_bus bus;
    ret = kinetis_spi_bus_init(&bus, HW_SPI2);
    ret = ads7843_init(&bus, 0);
    ret = w25qxx_init(&bus, 1);
    
    
    for(i=0;i<100;i++)
    {
        ads7843_readX(&x);
        ads7843_readY(&y);
        printf("X:%04d Y:%04d\r", x, y);
        DelayMs(10);
    }
    /* probe w25qxx */
    if(w25qxx_probe())
    {
        printf("no w25qxx deived found\r\n");
        return 1;
    }
    printf("%s detected!\r\n", w25qxx_get_name());
    /*
    for(i=0;i<SPI_FLASH_TEST_LEN;i++)
    {
        buf_test[i] = i;
    }
    if(w25qxx_write(0, (uint8_t*)buf_test, SPI_FLASH_TEST_LEN))
    {
        printf("w25qxx write failed\r\n");
        return 1;
    }
    memset((uint8_t*)buf_test,0, SPI_FLASH_TEST_LEN);
    if(w25qxx_read(0, (uint8_t*)buf_test, SPI_FLASH_TEST_LEN))
    {
        printf("w25qxx read failed\r\n");
        return 1;
    }
    for(i=0;i<SPI_FLASH_TEST_LEN;i++)
    {
        if(buf_test[i] != i%256)
        {
           printf("error:[0x%X]:%d\r\n", i, buf_test[i]);
        }
    }
    */
    printf("spi flash test finish!\r\n");
    return 0;
}


static int DO_SPI_DMA(int argc, char * const argv[])
{
    static  uint32_t SendData[10];
    static  uint32_t RevData[10];
    SendData[0] = 0x80020090;
    SendData[1] = 0x80020001;
    SendData[2] = 0x80020002;
    SendData[3] = 0x80020090;
    SendData[4] = 0x80020001;
    SendData[5] = 0x80020002;
    SendData[6] = 0x80020002;
    
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    DMA_InitStruct1.chl = HW_DMA_CH0;
    DMA_InitStruct1.chlTriggerSource = SPI2_TRAN_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = sizeof(uint32_t);
    DMA_InitStruct1.majorLoopCnt = 6;
    
    DMA_InitStruct1.sAddr = (uint32_t)&SendData[0];
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = sizeof(uint32_t);
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_32;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)&SPI2->PUSHR; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_32;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    
    DMA_InitStruct1.chl = HW_DMA_CH1;
    DMA_InitStruct1.chlTriggerSource = SPI2_REV_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = sizeof(uint32_t);
    DMA_InitStruct1.majorLoopCnt = 6;
    
    DMA_InitStruct1.sAddr = (uint32_t)&SPI2->POPR;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_32;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)&RevData[0]; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = sizeof(uint32_t);
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_32;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    
    SPI_ITDMAConfig(HW_SPI2, kSPI_DMA_TFFF, true);
    SPI_ITDMAConfig(HW_SPI2, kSPI_DMA_RFDF, true);   
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_EnableRequest(HW_DMA_CH1);
    
    DelayMs(30);
    printf("DMA sending now ...\r\n");
    printf("CH0:%d\r\n", DMA_GetMajorLoopCount(HW_DMA_CH0));
    printf("CH1:%d\r\n", DMA_GetMajorLoopCount(HW_DMA_CH1));
    printf("0x%08X\r\n", RevData[0]);
    printf("0x%08X\r\n", RevData[1]);
    printf("0x%08X\r\n", RevData[2]);
    printf("0x%08X\r\n", RevData[3]);
    printf("0x%08X\r\n", RevData[4]);
    printf("0x%08X\r\n", RevData[5]);
}


int CMD_SPI(int argc, char *const argv[])
{
    uint8_t data[6];
    uint32_t instance;
    printf("SPI TEST CMD\r\n");
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    instance = SPI_QuickInit(SPI2_SCK_PD12_SOUT_PD13_SIN_PD14, kSPI_CPOL0_CPHA0, 20*1000*1000);
    PORT_PinMuxConfig(HW_GPIOD, 12, kPinAlt2); //
    PORT_PinMuxConfig(HW_GPIOD, 13, kPinAlt2); //
    PORT_PinMuxConfig(HW_GPIOD, 14, kPinAlt2); //    

    PORT_PinMuxConfig(HW_GPIOD, 11, kPinAlt2); //SPI2_PCS0
    PORT_PinMuxConfig(HW_GPIOD, 15, kPinAlt2); //SPI2_PCS1
    
    SPI_EnableTxFIFO(HW_SPI2, true);
    SPI_EnableRxFIFO(HW_SPI2, true);
    
    
   // SPI_ReadWriteByte(instance, HW_CTAR0, 0x90, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
   // DelayMs(20);
    if((argc == 2) && (!strcmp(argv[1], "FLASH")))
    {
        return DO_SPI_FLASH(argc, argv);
    }
    if((argc == 2) && (!strcmp(argv[1], "DMA")))
    {
        return DO_SPI_DMA(argc, argv);
    }

    printf("param error\r\n");
    PEout(6) = !PEout(6);
    SPI_ReadWriteByte(instance, HW_CTAR0, 0x90, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
    SPI_ReadWriteByte(instance, HW_CTAR0, 0x00, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
    SPI_ReadWriteByte(instance, HW_CTAR0, 0x00, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
    SPI_ReadWriteByte(instance, HW_CTAR0, 0x00, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
   // SPI_WaitSync(instance);
    data[0] = SPI_ReadWriteByte(instance, HW_CTAR0, 0xFF, HW_SPI_CS1, kSPI_PCS_KeepAsserted);
    data[1] = SPI_ReadWriteByte(instance, HW_CTAR0, 0xFF, HW_SPI_CS1, kSPI_PCS_ReturnInactive);
    PEout(6) = !PEout(6);
    printf("data:0x%02X 0x%02X\r\n", data[0], data[1]);
    //初始化SPI
   // SPI_QuickInit(BOARD_SPI_MAP, kSPI_CPOL0_CPHA1, 1*1000);
    //安装回调函数
   // SPI_CallbackInstall(BOARD_SPI_INSTANCE, SPI_ISR);
    //开启SPI中断 
   // SPI_ITDMAConfig(BOARD_SPI_INSTANCE, kSPI_IT_TCF);
    return CMD_RET_USAGE;
}


SHELL_EXPORT_CMD(CMD_SPI, SPI, SPI);

