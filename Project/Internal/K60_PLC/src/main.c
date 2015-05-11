// dirver lib
#include "chlib_k.h"
#include <absacc.h> 
#include "mb85rc64.h"

// board pin define
#include "boarddef.h"

#include "unit_test.h"

#define PIT0_TIME_IN_US    (625)
/**console check*/
#define PIT1_TIME_IN_US    (50*1000)

// global vars
uint16_t gADCValue[16]  __at (0x20000000);

const int gADC_InstanceTable[] =  ADC_PORTS;
const int gADC_ChnTable[] = ADC_CHANNELS;

//fft fifo pit放到单独的文件 UART 
void PIT0_ISR(void);
void PIT1_ISR(void);
void UART0_RX_ISR(uint16_t data);
void UART3_RX_ISR(uint16_t data);
void UART3_TX_ISR(uint16_t * pbyteToSend);
void UART4_RX_ISR(uint16_t data);
void UART4_TX_ISR(uint16_t * pbyteToSend);
void UART5_RX_ISR(uint16_t data);
void UART5_TX_ISR(uint16_t * pbyteToSend);


int main(void)
{
    uint32_t i;
    int r;
    
    // UART and Delay init
    DelayInit();
    UART_QuickInit(BOARD_UART_MAP, 115200);
    UART_QuickInit(RS485_CH1_MAP, 115200);
    UART_QuickInit(RS485_CH2_MAP, 115200);
    UART_QuickInit(RS485_CH3_MAP, 115200);
    
    //UART0 ISR

    UART_CallbackRxInstall(HW_UART0, UART0_RX_ISR); //打印串口
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);

    //UART3 TX RX 中断

    UART_CallbackRxInstall(HW_UART3, UART3_RX_ISR);
    UART_CallbackTxInstall(HW_UART3, UART3_TX_ISR);
    UART_ITDMAConfig(HW_UART3, kUART_IT_Rx, true);
    UART_ITDMAConfig(HW_UART3, kUART_IT_Tx, true);
    
    //UART4 

    UART_CallbackRxInstall(HW_UART4, UART4_RX_ISR);
    UART_CallbackTxInstall(HW_UART4, UART4_TX_ISR);
    UART_ITDMAConfig(HW_UART4, kUART_IT_Rx, true);
    UART_ITDMAConfig(HW_UART4, kUART_IT_Tx, true);
    
    //UART5 

    UART_CallbackRxInstall(HW_UART5, UART5_RX_ISR);
    UART_CallbackTxInstall(HW_UART5, UART5_TX_ISR);
    UART_ITDMAConfig(HW_UART5, kUART_IT_Rx, true);
    UART_ITDMAConfig(HW_UART5, kUART_IT_Tx, true);
    
    // start running systick
    SYSTICK_Init(1000*1000);
    SYSTICK_Cmd(true);
        
    //init LED
    GPIO_QuickInit(LED0_PORT, LED0_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(LED1_PORT, LED1_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(LED2_PORT, LED2_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(LED3_PORT, LED3_PIN, kGPIO_Mode_OPP);

    // init RTC
    RTC_QuickInit();

    TestClock();
    TestADCTime();
    TestLED();
    TestMB85RC64();
    TestRTC();
    TestRS485IntSend();
    
    extern int GetPcValue(void);
    //测试调用汇编子程序
    
    printf("ASM sun func call test\r\n");
    printf("call GetPCValue: PC:0x%X\r\n", GetPcValue());
    
    // Init ADC 
    ADC_InitTypeDef ADC_InitStruct;


    ADC_InitStruct.instance = HW_ADC0;
    ADC_InitStruct.clockDiv = kADC_ClockDiv8; //可以改成 kADC_ClockDiv4 kADC_ClockDiv8 增加采样时间
    ADC_InitStruct.resolutionMode = kADC_SingleDiff12or13;
    ADC_InitStruct.triggerMode = kADC_TriggerSoftware;
    ADC_InitStruct.singleOrDiffMode = kADC_Single;
    ADC_InitStruct.continueMode = kADC_ContinueConversionDisable;
    ADC_InitStruct.hardwareAveMode = kADC_HardwareAverage_4;
    ADC_InitStruct.vref = kADC_VoltageVREF;
    ADC_Init(&ADC_InitStruct);
    ADC_InitStruct.instance = HW_ADC1;
    ADC_Init(&ADC_InitStruct);

    
    // init PIT
    PIT_QuickInit(HW_PIT_CH0, PIT0_TIME_IN_US); 
    PIT_CallbackInstall(HW_PIT_CH0, PIT0_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    printf("%dus Interrupt Enabled\r\n", PIT0_TIME_IN_US);
    
    // init PIT
    PIT_QuickInit(HW_PIT_CH1, PIT1_TIME_IN_US); 
    PIT_CallbackInstall(HW_PIT_CH1, PIT1_ISR);
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
    printf("%dus Interrupt Enabled\r\n", PIT1_TIME_IN_US);

    while(1)
    {
        for(i=0;i<8;i++)
        {
        //   printf("[%d]:%d ", i, gADCValue[i]); //显示所有数据
        }
        printf("\r");
    }
}


