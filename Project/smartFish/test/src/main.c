#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"

#include "i2c.h"
#include "ov7725.h"

#include "oled_spi.h"

#include "ftm.h"
#include "lptmr.h"
#include "pit.h"


/* 请将I2C.H中的 I2C_GPIO_SIM 改为 1 */

// 改变图像大小
//0: 80x60
//1: 160x120
//2: 240x180
#define IMAGE_SIZE  0

#if (IMAGE_SIZE  ==  0)
#define OV7620_W    (80)
#define OV7620_H    (60)

#elif (IMAGE_SIZE == 1)
#define OV7620_W    (160)
#define OV7620_H    (120)

#elif (IMAGE_SIZE == 2)
#define OV7620_W    (240)
#define OV7620_H    (180)

#else
#error "Image Size Not Support!"
#endif

#define WIDTH OV7620_W
#define HEIGHT OV7620_H-1

// 图像内存池
uint8_t gCCD_RAM[(OV7620_H)*((OV7620_W/8)+1)];   //使用内部RAM

/* 行指针 */
uint8_t * gpHREF[OV7620_H+1];

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

static void UserApp(uint32_t vcount);
//定义一帧结束后的用户函数

/* 状态机定义 */
typedef enum
{
    TRANSFER_IN_PROCESS, //数据在处理
    NEXT_FRAME,          //下一帧数据
}OV7620_Status;

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
    r = ov7725_set_image_size((ov7725_size)IMAGE_SIZE);
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
        PORTE->ISFR = 0xFFFFFFFF;   //这里可以改PORTE
        h_counter = 0;
        return;
    }
}

void initCamera(){
    uint32_t i;

    printf("OV7725 test\r\n");

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
    DMA_InitStruct1.chlTriggerSource = PORTE_DMAREQ;    //这里可以改PORTE
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = ((OV7620_W/8) +1);

    DMA_InitStruct1.sAddr = (uint32_t)&PTE->PDIR + BOARD_OV7620_DATA_OFFSET/8;  //这里可以改PTE
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


bool printflag = false;
//串口接收中断
void UART_RX_ISR(uint16_t byteRec){
    //打印整个图像
    if(byteRec == ' ')printflag = true;

}
// IMG
uint8_t IMG[OV7620_W][OV7620_H];   //使用内部RAM


void findCenter();

int average;

/* 接收完成一场后 用户处理函数 */
static void UserApp(uint32_t vcount)
{
    for(int y=0;y<OV7620_H-1;y++)
        for(int x=0;x<OV7620_W/8;x++)
            for(int i=0; i<8; i++)
                IMG[x*8+i][y] = (gpHREF[y][x+1]>>i)%2;
    //将图片从OV7620_H*OV7620_W/8映射到OV7620_H*OV7620_W

    //findCenter();
    
    if(printflag){
        printflag = false;
        //打印出图像
        printf("start\r\n");
        for(int y=0;y<OV7620_H-1;y++){
            for(int x=0;x<OV7620_W;x++){
                printf("%d", IMG[x][y]);
            }
            printf("\r\n");
        }
        
    }
    
    
    //打印到屏幕上
    for(int y=0;y<8;y++){
        LED_WrCmd(0xb0 + y); //0xb0+0~7表示页0~7
        LED_WrCmd(0x00); //0x00+0~16表示将128列分成16组其地址在某组中的第几列
        LED_WrCmd(0x10); //0x10+0~16表示将128列分成16组其地址所在第几组
        for(int x=0;x<80;x++){
            uint8_t data = 0;
            for(int i=0;i<8 && y*8+i < HEIGHT ;i++){
                //data += (IMG[x*2][(y*8+i)*2] > 0 | IMG[x*2+1][(y*8+i)*2] > 0)<<(i);
                data += (IMG[x][y*8+i])<<i;
            }
            LED_WrDat(data);
        }
    }
    
}

int centers[HEIGHT] = {0};

int err2 = 0;
void findCenter(){
    int y=HEIGHT;

    int center = WIDTH/2;       //赛道中心
    int s = 0;          //中心累积求和
    int sum = 0;        //中心数
    
    int left;
    int right;
    
    int err = 0;
    
    while(y){
        left = center-1;
        right = center+1;
        
        while(left){
            if((IMG[left][y]==1)&&(IMG[left+1][y]==0))break;
            left--;
        }
        while(right < WIDTH){
            if((IMG[right-1][y]==0)&&(IMG[right][y]==1))break;
            right++;
        }
        
        center = (left+right)/2;
        centers[sum] = center;
        if(right-left<10){
            err++;
            break;
        }
        if(err>12)break;
        s += center;
        sum ++;
        IMG[center][y] = 1;
        y--;
    }
    
    char buf[20]={0};
    
    if(sum>10){
        average = s/sum;
        average -= 80;
        LED_P8x16Str(80, 3, buf);
        //执行动作
        
        err2=0;
    }else{
        err2++;
        if(err2>200){
            //turn(0);
            //while(1);
        }
    }

    sprintf(buf, "a=%d ", average);
    LED_P8x16Str(80, 1, buf);
    sprintf(buf, "h=%d ", sum);
    LED_P8x16Str(80, 2, buf);
}

void PIT_ISR(void)
{
    printf("Hello World\r\n");
}

int main(void)
{
    GPIO_QuickInit(HW_GPIOC, 14, kGPIO_Mode_OPP);
    PCout(14)=1;
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    
    /* 注册中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);

    /* 开启UART Rx中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);

    initOLED();
    LED_Fill(0x00);
    LED_P8x16Str(0, 0, "Hello YPW");
    LED_P8x16Str(0, 1, "init Camera");
    initCamera();
    
    /* 开启PIT中断 */
    //PIT_QuickInit(HW_PIT_CH0, 1000*100);
    //PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    //PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    PCout(14)=0;
    LED_Fill(0x00);
    
    WDOG_QuickInit(1000);
    while(1)
    {
        WDOG_Refresh();
        DelayMs(10);
    }
}
