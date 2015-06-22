#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"

#include "i2c.h"
#include "ov7725.h"

#include "oled_spi.h"

#include "ftm.h"
#include "lptmr.h"


#define offset 77
void turn(int angel){
    angel += offset;
    int pwm = (int)((angel/90.0 + 0.5) * 500);  //90度是1.5ms
    printf("turn:%d\r\n", pwm);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0, pwm);
}

#define DRIVER_PWM_WIDTH 1000
void initDriver(){
    printf("initPWM\r\n");

    //for(int i=0;i<0;i++)
    GPIO_QuickInit(HW_GPIOC, 0, kGPIO_Mode_OPP);
    PCout(0)=1;
    //使能INH

    FTM_PWM_QuickInit(FTM1_CH0_PA12, kPWM_EdgeAligned, 50);     //设置FTM，边沿对齐模式
    turn(0);
    //初始化舵机

    FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH1_PC02, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH2_PC03, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH3_PC04, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    //初始化电机PWM输出
    
}

void setSpeed(int spd){
    if(spd>0){
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0);
        
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
    }else
    {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, -spd);
        
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, -spd);
    }
}



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

// IMG
uint8_t gIMG[OV7620_W][OV7620_H];   //使用内部RAM

void findLine(){
    for(int y=0;y<OV7620_H;y++){
        for(int x=0;x<OV7620_W;x++){
            if(gIMG[x][y]==0 && gIMG[x+1][y]){
                x++;continue;
            }
            if(gIMG[x][y]&&gIMG[x+1][y]){
                gIMG[x][y]=0;
            }
        }
    }
}
#define DELTA_MAX 5
int dirsum;
int average;
void findCenter(){
    
    int center = 30;
    int lastcenter = 30;
    int delta = 0;
    int left, right;
    int y;
    
    dirsum = 0;
    average = 0;
    int sum = 0;
    
    for(y=OV7620_H-2;y>0;y--){
        for(left = lastcenter;left>0;left--)
            if(gIMG[left][y])break;
        for(right = lastcenter;right<OV7620_W;right++)
            if(gIMG[right][y])break;
        
        center = (left+right)/2;
        delta = center - lastcenter;

        if(y!=OV7620_H-2){
            if(delta>DELTA_MAX | delta<-DELTA_MAX){
                for(int x=0;x<OV7620_W;x++)
                    gIMG[x][y]=1;
                break;
            }else {
                dirsum += delta;
                average += center;
                sum ++;
            }
            //printf("%d\r\n",delta);
            
        }
        lastcenter = center;
        for(int x=0;x<OV7620_W;x++)
                gIMG[x][y]=0;
        gIMG[center][y]=1;
    }
    
    
    
    if(sum < 10){
        setSpeed(0);
        turn(0);
        DelayMs(1000);
    }else {
        average /= sum;
        setSpeed(2000);
        turn(average-38);
    }
    
    
    
    //printf("\r\n");
    
}

bool printflag = false;

/* 接收完成一场后 用户处理函数 */
static void UserApp(uint32_t vcount)
{
    for(int y=0;y<OV7620_H-1;y++)
        for(int x=0;x<OV7620_W/8;x++)
            for(int i=0; i<8; i++)
                gIMG[x*8+i][y] = (gpHREF[y][x+1]>>(7-i))%2;
    //将图片从OV7620_H*OV7620_W/8映射到OV7620_H*OV7620_W

    findLine();
    findCenter();
    
    if(printflag){
        printflag = false;
        for(int y=0;y<OV7620_H-1;y++){
             for(int x=0;x<OV7620_W;x++){
                printf("%d",gIMG[x][y]);
            }
            printf("\r\n");
        }
        printf("\r\n");
    }
    
    for(int y=0;y<8;y++){
        LED_WrCmd(0xb0 + y); //0xb0+0~7表示页0~7
        LED_WrCmd(0x00); //0x00+0~16表示将128列分成16组其地址在某组中的第几列
        LED_WrCmd(0x10); //0x10+0~16表示将128列分成16组其地址所在第几组
        for(int x=0;x<80;x++){
            uint8_t data = 0;
            for(int i=0;i<8 && y*8+i<OV7620_H ;i++)
                data += gIMG[x][y*8+i]<<(i);
            LED_WrDat(data);
        }
    }
    
    char buf[20] = {0};
    sprintf(buf, "s=%d ", dirsum);
    LED_P8x16Str(80, 0, buf);
    
    sprintf(buf, "a=%d ", average);
    LED_P8x16Str(80, 1, buf);
    
}

//串口接收中断
void UART_RX_ISR(uint16_t byteRec){
    //打印整个图像
    printflag = true;

}

int main(void)
{

    DelayInit();
    /* 打印串口及小灯 */

    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    /* 注册中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);

    /* 开启UART Rx中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);

    initDriver();
    setSpeed(2000);
    initOLED();
    initCamera();
    

    while(1)
    {

    }
}
