#include "chlib_k.h"
#include "sram.h"

#define OV7620_W    (320) /* 每行有多少像素 */
#define OV7620_H    (240) /* 高度有多少行 */
#define OV7620_I2C_CHIP_ADDR   0x21   /* I2C OV7620 I2C地址 */

//uint8_t CCDBufferPool[OV7620_W*OV7620_H];   //使用内部RAM
volatile uint8_t * CCDBufferPool = SRAM_START_ADDRESS; //使用外部SRAM

/* CCD内存池 */
uint8_t * CCDBuffer[OV7620_H];
/* 引脚定义 */
#define BOARD_OV7620_PCLK_PORT      HW_GPIOA
#define BOARD_OV7620_PCLK_PIN       (7)   //摄像头时钟引脚
#define BOARD_OV7620_VSYNC_PORT     HW_GPIOA
#define BOARD_OV7620_VSYNC_PIN      (16) //摄像头像素中断
#define BOARD_OV7620_HREF_PORT      HW_GPIOA
#define BOARD_OV7620_HREF_PIN       (17) //摄像头行中断
#define BOARD_OV7620_DATA_OFFSET    (8) /* 摄像头数据引脚PTA8-PTA15 只能为 0 8 16 24 */

/* 状态机定义 */
typedef enum
{
    TRANSFER_IN_PROCESS, //数据在处理
    NEXT_FRAME,          //下一帧数据
}OV7620_Status;

static void UserApp(void);

/* 摄像头场中断处理函数 */
void OV7620_ISR(uint32_t pinArray)
{
    if(pinArray & (1<<BOARD_OV7620_VSYNC_PIN))
    {
    static uint8_t status = TRANSFER_IN_PROCESS;  
    switch(status)
    {
        case TRANSFER_IN_PROCESS: /* 正在传输 */
            /* 查看DMA是否完成传送 */
            if(DMA_IsMajorLoopComplete(HW_DMA_CH2) == 0)
            {
                GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_RisingEdge, false);
                /* 开始处理用户 函数 */
                UserApp();
                /* 用户函数处理完毕 */
                status = NEXT_FRAME;
                /* 处理完用户函数后 继续开启场中断 准备下一场数据到达 */
                GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_RisingEdge, true);  
            }
            else
            {
                status = TRANSFER_IN_PROCESS;
            }
            break;
        case NEXT_FRAME:
            /* 传输完成 复位buffer地址 开始下一场传输 */
            DMA_SetDestAddress(HW_DMA_CH2, (uint32_t)CCDBuffer[0]);
            DMA_EnableRequest(HW_DMA_CH2);
            status =  TRANSFER_IN_PROCESS;
            break;
        default:
            break;
    }
    }

}

static uint16_t RGB2COLOR(uint8_t RR,uint8_t GG,uint8_t BB)
{
  return (((RR/8)<<11)+((GG/8)<<6)+BB/8); 
}

/* 接收完成一场后 用户处理函数 */
static void UserApp(void)
{
    uint32_t i,j;
    static uint32_t cnt;
    printf("SYNC cnt:%d\r\n", cnt++); 
    for(i=0;i<OV7620_H;i++)
    {
        for(j=0;j<OV7620_W;j++)
        {
            ili9320_write_pixel(OV7620_H - i, OV7620_W- j, RGB2COLOR(CCDBuffer[i][j], CCDBuffer[i][j], CCDBuffer[i][j]));
        }
    } 
}

//ov7620摄像头初始化配置
static void OV7620_Init(void)
{
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    uint32_t i;
    //把开辟的内存池付给指针
    for(i=0;i< OV7620_H;i++)
    {
        CCDBuffer[i] = (uint8_t *) &CCDBufferPool[i*OV7620_W];
    }
    /* 场中断  行中断 像素中断 */
    GPIO_QuickInit(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_Mode_IPD);
    /* 初始化数据端口 */
    for(i=0;i<8;i++)
    {
        GPIO_QuickInit(HW_GPIOA, BOARD_OV7620_DATA_OFFSET+i, kGPIO_Mode_IFT);
    }
    GPIO_CallbackInstall(BOARD_OV7620_VSYNC_PORT, OV7620_ISR);
    GPIO_ITDMAConfig(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_DMA_RisingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_RisingEdge, true);
  //  GPIO_ITDMAConfig(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_DMA_RisingEdge); //实际并没有用到
    DMA_InitStruct1.chl = HW_DMA_CH2;
    DMA_InitStruct1.chlTriggerSource = PORTA_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = OV7620_W;
    DMA_InitStruct1.majorLoopCnt = OV7620_H;
    
    DMA_InitStruct1.sAddr = (uint32_t)&PTA->PDIR + BOARD_OV7620_DATA_OFFSET/8;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    
    DMA_InitStruct1.dAddr = (uint32_t)CCDBuffer[0];
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;

    DMA_Init(&DMA_InitStruct1);
    /* 开始传输 */
    DMA_EnableRequest(HW_DMA_CH2); 
}

void OV7620_Reset(void)
{
    SCCB_WriteSingleRegister(HW_I2C0, OV7620_I2C_CHIP_ADDR, 0x12, 0x80);
}

void SCCB_Init(void)
{
    I2C_QuickInit(I2C0_SCL_PB00_SDA_PB01, 100*1000);
    /* 只是复位 全部做默认设置 */
    /* Yandld: 如果是默认出厂配置, 则无需配置SCCB */
    OV7620_Reset();
    DelayMs(10);
    /* 默认配置如下 */
    /* 0x12,0x64
       0x06,0xA0
       0x03,0x80 */
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("OV7620 test\r\n");
    
    ili9320_init();
    SRAM_Init();
    /* 摄像头速度非常快 把FLexbus 总线速度调到最高 */
    SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV3_MASK;
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0);
    OV7620_Init();
    SCCB_Init();
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


