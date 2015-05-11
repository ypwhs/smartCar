#include "shell.h"
#include "common.h"
#include "systick.h"
#include "tsi.h"
#include "gpio.h"


#define TSI_THRESHOLD   (2000)

void TSI_ISR(void)
{
    uint32_t i;
    static bool inital_it = true;
    static int32_t inital_value[16];
    int32_t tsi_value[16];
    uint32_t out_of_rang_flag = 0;
    TSI_ITDMAConfig(kTSI_IT_Disable);
    if(inital_it)
    {
        /* 记录采集第一次的初始值 */
        for(i = 1; i < 5; i++)
        {
            inital_value[i] = TSI_GetCounter(i);
        }
        inital_it = false;        
    }
    for(i = 1; i < 5; i++)
    {
        tsi_value[i] = TSI_GetCounter(i);
    }
    /* 判断是否超出范围 */
    for(i = 1; i < 5; i++)
    {
        if(ABS(tsi_value[i] - inital_value[i]) > TSI_THRESHOLD)
        {
            out_of_rang_flag |= (1<<i);
        }
        else
        {
            out_of_rang_flag &= ~(1<<i);
        }
    }


    static uint32_t last_chl_array;
    uint32_t value;
    
   // printf("0x%X\r", out_of_rang_flag);
    
    value = out_of_rang_flag ^ last_chl_array;
    if(value & out_of_rang_flag & (1<<1))
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
    }
    if(value & out_of_rang_flag & (1<<2))
    {
        GPIO_ToggleBit(HW_GPIOE, 12);
    }
    if(value & out_of_rang_flag & (1<<3))
    {
        GPIO_ToggleBit(HW_GPIOE, 11);
    }
    if(value & out_of_rang_flag & (1<<4))
    {
        GPIO_ToggleBit(HW_GPIOE, 7);
    }
    last_chl_array = out_of_rang_flag;
    
    TSI_ITDMAConfig(kTSI_IT_EndOfScan);

}

int CMD_TSI(int argc, char * const argv[])
{
    printf("TSI TEST\r\n");
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 11, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 12, kGPIO_Mode_OPP);
    TSI_QuickInit(TSI0_CH1_PA00);
    TSI_QuickInit(TSI0_CH2_PA01);
    TSI_QuickInit(TSI0_CH3_PA02);
    TSI_QuickInit(TSI0_CH4_PA03);
    TSI_CallbackInstall(TSI_ISR);
    TSI_ITDMAConfig(kTSI_IT_EndOfScan);

}



const cmd_tbl_t CommandFun_TSI = 
{
    .name = "TSI",
    .maxargs = 4,
    .repeatable = 1,
    .cmd = CMD_TSI,
    .usage = "TSI",
    .complete = NULL,
    .help = "TSI"
};
