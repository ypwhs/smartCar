/**
  ******************************************************************************
  * @file    pdb.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */

#include "pdb.h"

static PDB_CallBackType PDB_CallBackTable[1] = {NULL};

/**
 * @brief  软件触发PDB开始转换一次  
 * @retval None
 */
void PDB_SoftwareTrigger(void)
{
    PDB0->SC |= PDB_SC_SWTRIG_MASK;
}

/**
 * @brief  设置PDB转换频率
 * @retval None
 */
static void _PDB_SetCounterPeriod(uint32_t srcClock, uint32_t timeInUs)
{
    static const uint8_t MULT[4] = {1, 10, 20, 40};
    uint32_t clkDiv,mult;
    uint32_t i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<4;j++)
        {
            if((srcClock/1000000)*timeInUs/((1<<i)*MULT[j]) < 0xFFFF)
            {
                clkDiv = i;
                mult = j;
                break;
            }
        }
    }
    
    LIB_TRACE("clkDiv:%d Mult:%d\r\n",clkDiv, mult);
    
    /* clk div */
    PDB0->SC &= ~PDB_SC_PRESCALER_MASK;
    PDB0->SC |= PDB_SC_PRESCALER(clkDiv);
    
    /* muti */
    PDB0->SC &= ~PDB_SC_MULT_MASK;
    PDB0->SC |= PDB_SC_MULT(mult);
    
    LIB_TRACE("MOD:%d\r\n", (srcClock/1000000)*timeInUs/((1<<clkDiv)*MULT[mult]));
    PDB0->MOD = (srcClock/1000000)*timeInUs/((1<<clkDiv)*MULT[mult]);
    PDB0->IDLY = (srcClock/1000000)*timeInUs/((1<<clkDiv)*MULT[mult]);
}

/**
 * @brief  获得PDB Mod 寄存器
 * @retval MOD寄存器值
 */
uint32_t PDB_GetMODValue(void)
{
    return PDB0->MOD;
}

void PDB_QuickInit(PDB_TriggerSrc_Type triggerSrc, uint32_t timeInUs)
{
    uint32_t clock;
    PDB_InitTypeDef  PDB_InitStruct1;
    
    CLOCK_GetClockFrequency(kBusClock, &clock);

    PDB_InitStruct1.inputTrigSource = kPDB_SoftwareTrigger;  /* software Trigger */
    PDB_InitStruct1.isContinuesMode = true;
    PDB_InitStruct1.srcClock = clock;
    PDB_InitStruct1.timeInUs = timeInUs;
    PDB_Init(&PDB_InitStruct1);
}

void PDB_Init(PDB_InitTypeDef * PDB_InitStruct)
{
    /* enable clock gate */
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK ;
    
    /* reset control register */
	PDB0->SC = 0x00;

    /* trigger source */
	PDB0->SC |= PDB_SC_TRGSEL(PDB_InitStruct->inputTrigSource);

    /* if continues mode */
    (PDB_InitStruct->isContinuesMode)?(PDB0->SC |= PDB_SC_CONT_MASK):(PDB0->SC &= ~PDB_SC_CONT_MASK);
    
    /* set PDB period */
    _PDB_SetCounterPeriod(PDB_InitStruct->srcClock, PDB_InitStruct->timeInUs);

    /* enable PDB */
	PDB0->SC |= PDB_SC_PDBEN_MASK; 

    /* enable LOCK */
	PDB0->SC |= PDB_SC_LDOK_MASK;
}

/**
 * @brief  设置PDB触发ADC
 * @param  adcInstance: 需要触发的ADC模块号 如HW_ADC0
 * @param  adcMux:     ADC转换通道
 * @param  dlyValue:   延时计数值
 * @param  status:     开关
 * @retval None
 */
void PDB_SetADCPreTrigger(uint32_t adcInstance, uint32_t adcMux, uint32_t dlyValue, bool status)
{
    /* disable PDB */
    (status)?
    (PDB0->CH[adcInstance].C1 |= PDB_C1_EN(1<<adcMux)):
    (PDB0->CH[adcInstance].C1 &= ~PDB_C1_EN(1<<adcMux));
    
    (status)?
    (PDB0->CH[adcInstance].C1 |= PDB_C1_TOS(1<<adcMux)):
    (PDB0->CH[adcInstance].C1 &= ~PDB_C1_TOS(1<<adcMux));
}

void PDB_SetBackToBackMode(uint32_t adcInstance, uint32_t adcMux, bool status)
{
    (status)?
    (PDB0->CH[adcInstance].C1 |= PDB_C1_BB(1<<adcMux)):
    (PDB0->CH[adcInstance].C1 &= ~PDB_C1_BB(1<<adcMux));
}

void PDB_ITDMAConfig(PDB_ITDMAConfig_Type config, bool status)
{
    /* enable clock gate */
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;
    
    if(!status)
    {
        NVIC_DisableIRQ(PDB0_IRQn);
        return;
    }
    
    switch(config)
    {
        case kPDB_IT_CF:
            (status)?
            (PDB0->SC |= PDB_SC_PDBIE_MASK):
            (PDB0->SC &= ~PDB_SC_PDBIE_MASK);
            NVIC_EnableIRQ(PDB0_IRQn);
            break; 
        case kPDB_DMA_CF:
            (status)?
            (PDB0->SC |= PDB_SC_DMAEN_MASK):
            (PDB0->SC &= ~PDB_SC_DMAEN_MASK);   
        default:
            break;
    }
}


void PDB_CallbackInstall(PDB_CallBackType AppCBFun)
{
    /* enable clock gate */
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;
    if(AppCBFun != NULL)
    {
        PDB_CallBackTable[0] = AppCBFun;
    }
}

void PDB0_IRQHandler(void)
{
    /* clear IT pending flags */
    PDB0->SC &= ~PDB_SC_PDBIF_MASK;
    
    if(PDB_CallBackTable[0])
    {
        PDB_CallBackTable[0]();
    }
}
 
