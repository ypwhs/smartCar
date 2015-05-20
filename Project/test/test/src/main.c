#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"

#include "i2c.h"
#include "ov7725.h"

#include "oled_spi.h"

/* 请将I2C.H中的 I2C_GPIO_SIM 改为 1 */

// 改变图像大小
//0: 80x60
//1: 160x120
//2: 240x180
//#define IMAGE_SIZE  0
//
//#if (IMAGE_SIZE  ==  0)
//#define OV7620_W    (80)
//#define OV7620_H    (60)
//
//#elif (IMAGE_SIZE == 1)
//#define OV7620_W    (160)
//#define OV7620_H    (120)
//
//#elif (IMAGE_SIZE == 2)
//#define OV7620_W    (240)
//#define OV7620_H    (180)
//
//#else
//#error "Image Size Not Support!"
//#endif

#define OV7620_W    (80)
#define OV7620_H    (60)

// 图像内存池
static uint8_t gCCD_RAM[(OV7620_H)*((OV7620_W/8)+1)];   //使用内部RAM

/* 行指针 */
static uint8_t * gpHREF[OV7620_H+1];

/* 引脚定义 PCLK VSYNC HREF 接到同一个PORT上 */
#define BOARD_OV7620_PCLK_PORT      HW_GPIOE
#define BOARD_OV7620_PCLK_PIN       (8)
#define BOARD_OV7620_VSYNC_PORT     HW_GPIOE
#define BOARD_OV7620_VSYNC_PIN      (10)
#define BOARD_OV7620_HREF_PORT      HW_GPIOE
#define BOARD_OV7620_HREF_PIN       (9)
/*
摄像头数据引脚PTA8-PTA15 只能填入 0 8 16三个值
0 :PTA0-PTA7
8 :PTA8-PTA15
16:PTA16-PTA24
*/
#define BOARD_OV7620_DATA_OFFSET    (0)

/* 状态机定义 */
typedef enum
{
    TRANSFER_IN_PROCESS, //数据在处理
    NEXT_FRAME,          //下一帧数据
}OV7620_Status;

void printBin(uint8_t data){
    int i;
    for(i=7;i>=0;i--){
        printf("%c", (data>>i)%2>0?'*':'.');
    }
}

/* 接收完成一场后 用户处理函数 */
static void UserApp(uint32_t vcount)
{
    for(int i=0;i<OV7620_W/8;i++)
        printBin(gpHREF[10][i+1]);
    printf("\n");

}

int SCCB_Init(uint32_t I2C_MAP)
{
    int r;
    uint32_t instance;
    instance = I2C_QuickInit(I2C_MAP, 50*1000);
    r = ov7725_probe(instance);
    if(r)
    {
        return 1;
    }
    r = ov7725_set_image_size((ov7725_size)0);
    if(r)
    {
        printf("OV7725 set image error\r\n");
        return 1;
    }
    return 0;
}

//行中断和场中断都使用PTE中断
void OV_ISR(uint32_t index)
{
    static uint8_t status = TRANSFER_IN_PROCESS;
    static uint32_t h_counter, v_counter;
   // uint32_t i;

    /* 行中断 */
    if(index & (1 << BOARD_OV7620_HREF_PIN))
    {
        DMA_SetDestAddress(HW_DMA_CH2, (uint32_t)gpHREF[h_counter++]);
        //i = DMA_GetMajorLoopCount(HW_DMA_CH2);
        DMA_SetMajorLoopCounter(HW_DMA_CH2, (OV7620_W/8)+1);
        DMA_EnableRequest(HW_DMA_CH2);

        return;
    }
    /* 场中断 */
    if(index & (1 << BOARD_OV7620_VSYNC_PIN))
    {
        GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
        GPIO_ITDMAConfig(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_IT_FallingEdge, false);
        switch(status)
        {
            case TRANSFER_IN_PROCESS: //接受到一帧数据调用用户处理
                    UserApp(v_counter++);
                    //printf("i:%d %d\r\n", h_counter, i);
                    status = NEXT_FRAME;
                    h_counter = 0;

                break;
            case NEXT_FRAME: //等待下次传输
                status =  TRANSFER_IN_PROCESS;
                break;
            default:
                break;
        }
        GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
        GPIO_ITDMAConfig(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_IT_FallingEdge, true);
        PORTE->ISFR = 0xFFFFFFFF;
        h_counter = 0;
        return;
    }
}

void initCamera(){

    uint32_t i;
    printf("OV7725 init\r\n");

    //检测摄像头
    if(SCCB_Init(I2C0_SCL_PB00_SDA_PB01))
    {
        printf("no ov7725device found!\r\n");
        while(1);
    }
    printf("OV7620 setup complete\r\n");

    //每行数据指针
    for(i=0; i<OV7620_H+1; i++)
    {
        gpHREF[i] = (uint8_t*)&gCCD_RAM[i*OV7620_W/8];
    }

    DMA_InitTypeDef DMA_InitStruct1 = {0};

    /* 场中断  行中断 像素中断 */
    GPIO_QuickInit(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_Mode_IPD);

    /* install callback */
    GPIO_CallbackInstall(BOARD_OV7620_VSYNC_PORT, OV_ISR);

    GPIO_ITDMAConfig(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_DMA_RisingEdge, true);

    /* 初始化数据端口 */
    for(i=0;i<8;i++)
    {
        GPIO_QuickInit(HW_GPIOE, BOARD_OV7620_DATA_OFFSET+i, kGPIO_Mode_IFT);
    }

    //DMA配置
    DMA_InitStruct1.chl = HW_DMA_CH2;
    DMA_InitStruct1.chlTriggerSource = PORTE_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = ((OV7620_W/8) +1);

    DMA_InitStruct1.sAddr = (uint32_t)&PTE->PDIR + BOARD_OV7620_DATA_OFFSET/8;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;

    DMA_InitStruct1.dAddr = (uint32_t)gpHREF[0];
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;

    /* initialize DMA moudle */
    DMA_Init(&DMA_InitStruct1);
}


int main(void)
{

    DelayInit();
    /* 打印串口及小灯 */

    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);

    initCamera();
    initOLED();

    while(1)
    {

    }
}
