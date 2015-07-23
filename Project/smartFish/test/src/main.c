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


void setSpeed(int speed){
    if(speed<0)speed=0;
    if(speed>15)speed=15;
    char aspeed = 0xD0+speed;
    //char sss[] = {0xAA, 0X91, aspeed, 0xFC, 0};
    //printf("%s", sss);
    UART_WriteByte(HW_UART4, 0x91);
    UART_WriteByte(HW_UART4, aspeed);
}

void turn(int dir){
    dir = 15 - dir;
    dir -= 8;
    if(dir<0)dir=0;
    if(dir>15)dir=15;
    char aspeed = 0xE0+dir;
    //char sss[] = {0xAA, 0X91, aspeed, 0xFC, 0};
    //printf("%s", sss);
    UART_WriteByte(HW_UART4, 0x91);
    UART_WriteByte(HW_UART4, aspeed);
}


/* 请将I2C.H中的 I2C_GPIO_SIM 改为 1 */

// 改变图像大小
//0: 80x60
//1: 160x120
//2: 240x180
#define IMAGE_SIZE  1

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
uint8_t gCCD_RAM[(OV7620_H+16)*((OV7620_W/8)+8)]={0};   //使用内部RAM

/* 行指针 */
uint8_t * gpHREF[OV7620_H+16];

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
        //printf("OV7725 set image error\r\n");
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

    //printf("OV7725 test\r\n");

    //检测摄像头
    if(SCCB_Init(I2C0_SCL_PB00_SDA_PB01))
    {
        //printf("no ov7725device found!\r\n");
        while(1);
    }
    //printf("OV7620 setup complete\r\n");

    //每行数据指针
    for(i=0; i<OV7620_H+16; i++)
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

// IMG
uint8_t IMG[OV7620_W+16][OV7620_H+8]={0};   //使用内部RAM

void findCenter();

int average;

/* 接收完成一场后 用户处理函数 */
static void UserApp(uint32_t vcount)
{
    int x=0;
    int y=0;
    int i=0;
    
    for(y=0;y<HEIGHT;y++)
        for(x=0;x<WIDTH/8;x++)
            for(i=0; i<8; i++)
                IMG[x*8+i][y] = (gpHREF[y][x+1]>>i)%2;
    //将图片从OV7620_H*OV7620_W/8映射到OV7620_H*OV7620_W

    
    
//    if(printflag){
//        printflag = false;
//        //打印出图像
//        //printf("start\r\n");
//        for(y=0;y<HEIGHT;y++){
//            for(x=0;x<WIDTH;x++){
//                if(IMG[x][y])UART_WriteByte(HW_UART4, '1');
//                else UART_WriteByte(HW_UART4, '0');
//            }
//            UART_WriteByte(HW_UART4, 0x0d);
//            UART_WriteByte(HW_UART4, 0x0a);
//        }
//    }
    
    findCenter();
    
    //打印到屏幕上
    for(y=0;y<8;y++){
        LED_WrCmd(0xb0 + y); //0xb0+0~7表示页0~7
        LED_WrCmd(0x00); //0x00+0~16表示将128列分成16组其地址在某组中的第几列
        LED_WrCmd(0x10); //0x10+0~16表示将128列分成16组其地址所在第几组
        for(x=0;x<WIDTH/2;x++){
            uint8_t data = 0;
            for(i=0;i<8 && (y*8+i)*2 < HEIGHT;i++){
                data += (IMG[x*2][(y*8+i)*2] > 0 | IMG[x*2+1][(y*8+i)*2] > 0)<<(i);
                //data += (IMG[x][y*8+i])<<i;
            }
            LED_WrDat(data);
        }
    }
    
    
}

int turns = 0;

int counter = 0;
int counter2 = 0;
int counter3 = 0;

int louyoucount = 0;
int louyoucount2 = 0;

int zhuanwanflag = 0;

void findCenter(){
    int y = 0;
    int x = 0;
    
    int s = 0;
    
    int sum = 0;
    int ave = 0;
    int aves[HEIGHT+8] = {0};
    int whites[HEIGHT+8] = {0};
    
    for(y=0;y<HEIGHT;y++)
    {
        ave=0;
        sum=0;
        for(x=0;x<WIDTH;x++)
        {
            if(!IMG[x][y]){
                ave += x;
                whites[y]++;
                sum ++;
            }
        }
        
        ave /= sum;
        IMG[ave][y]=1;
        aves[y]=ave;
        s += ave;
    }
    
    average = s/119;
    

    if(printflag){
        printflag = false;
        printf("aves\r\n");
        for(y=0;y<HEIGHT;y++){
            printf("%d\r\n", aves[y]);
        }
        
        printf("whites\r\n");
        for(y=0;y<HEIGHT;y++){
            printf("%d\r\n", whites[y]);
        }

    }
    
    
    #define dotmax 20
    #define dotpercent 0.8
    #define ALERT 10
    
    //漏油点检测
    
    int louyou = 0;
    for(y=100;y<110;y++)louyou += whites[y];
    
    
    if(louyou < 230 && louyou > 120){
        louyoucount++;
        louyoucount2=0;
    }else {
        if(louyoucount>0)louyoucount--;
        louyoucount2++;
        if(louyoucount2>50){
            louyoucount=0;
            louyoucount2=0;
        }
    }
    
    if(louyoucount>2){
        PCout(ALERT)=1;
    }else{
        PCout(ALERT)=0;
    }
    
    //转弯检测
    
    int zhuanwan = 0;
    for(y=0;y<HEIGHT;y++)if(whites[y]<2)zhuanwan++;
    int zhuanwan2 = 0;
    //for(y=0;y<HEIGHT;y++)if(whites[y]>30)zhuanwan2++;
    //if(zhuanwan2 < 5)zhuanwan = 0;
    

    turns = (average-80)/8+zhuanwan/2;
    
    if(zhuanwan>5){
        if(zhuanwanflag=0)counter3=400;
        else if(zhuanwan>20)counter3=120;
        zhuanwanflag = 1;
    }
    
    if(counter3){
        turns=15;
        counter3--;
    }
    
    if(zhuanwan==0){
        if(turns>3)turns=3;
        if(turns<-3)turns=-3;
    }
    
    counter ++;
    if(counter > 2){
        counter2 ++;
        if(counter2<10){
            turn(turns);
        }else{
            if(zhuanwan)setSpeed(10);
            else 
                setSpeed(15);
            counter2 = 0;
        }
        counter = 0;
    }
    
    //if(zhuanwan)DelayMs(1200);
    
    char buf[20]={0};
    sprintf(buf, "z=%d  ", zhuanwan);
    LED_P8x16Str(80, 0, buf);
    
    sprintf(buf, "a=%d  ", average);
    LED_P8x16Str(80, 1, buf);
    
    sprintf(buf, "t=%d  ", turns);
    LED_P8x16Str(80, 2, buf);
    
    sprintf(buf, "l=%d  ", louyou);
    LED_P8x16Str(80, 3, buf);
    
    
}

void PIT_ISR(void)
{

}



char lastrec = 0;

//串口接收中断
void UART_RX_ISR(uint16_t byteRec){
    //打印整个图像
    if(byteRec == ' ')printflag = true;
}

void uart4(uint16_t byteRec){
    //打印整个图像
    if(byteRec == ' ')printflag = true;
}


int main(void)
{
    GPIO_QuickInit(HW_GPIOC, ALERT, kGPIO_Mode_OPP);
    //PCout(ALERT)=1;
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 9600);
    UART_QuickInit(UART4_RX_PC14_TX_PC15, 9600);
    
    
    /* 注册中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);
    UART_CallbackRxInstall(HW_UART4, uart4);

    /* 开启UART Rx中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
    UART_ITDMAConfig(HW_UART4, kUART_IT_Rx, true);

    initOLED();
    LED_Fill(0x00);
    LED_P8x16Str(0, 0, "Hello YPW");
    LED_P8x16Str(0, 1, "init Camera");
    initCamera();
    
    /* 开启PIT中断 */
    //PIT_QuickInit(HW_PIT_CH0, 1000*500);
    //PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    //PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    PCout(ALERT)=0;
    LED_Fill(0x00);
    
    //setSpeed(0);
    WDOG_QuickInit(1500);
    while(1)
    {
        DelayMs(50);
        WDOG_Refresh();
        PDout(10)=!PDout(10);
    }
}
