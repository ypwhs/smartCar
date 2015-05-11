/**
  ******************************************************************************
  * @file    dma.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片DMA模块的底层功能函数
  ******************************************************************************
  */

#include "dma.h"
#include "common.h"

static DMA_CallBackType DMA_CallBackTable[4] = {NULL};

static const IRQn_Type DMA_IRQnTable[] = 
{
    DMA0_IRQn,
    DMA1_IRQn,
    DMA2_IRQn,
    DMA3_IRQn,
};



/**
 * @brief  初始化DMA模块
 * @param  DMA_InitStruct :DMA初始化配置结构体，详见dma.h
 * @retval None
 */
void DMA_Init(DMA_InitTypeDef *DMA_InitStruct)
{
	/* enable DMA and DMAMUX clock */
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;    
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
    
    /* disable chl first */
    DMA0->DMA[DMA_InitStruct->chl].DCR &= ~DMA_DCR_ERQ_MASK;
    
    /* dma chl source config */
    DMAMUX0->CHCFG[DMA_InitStruct->chl] = DMAMUX_CHCFG_SOURCE(DMA_InitStruct->chlTriggerSource);
    
    /* trigger mode */
    switch(DMA_InitStruct->triggerSourceMode)
    {
        case kDMA_TriggerSource_Normal:
            DMAMUX0->CHCFG[DMA_InitStruct->chl] &= ~DMAMUX_CHCFG_TRIG_MASK;
            break;
        case kDMA_TriggerSource_Periodic:
            DMAMUX0->CHCFG[DMA_InitStruct->chl] |= DMAMUX_CHCFG_TRIG_MASK;
            break;
        default:
            break;
    }
    
    /* transfer bytes cnt */
    DMA0->DMA[DMA_InitStruct->chl].DSR_BCR = DMA_DSR_BCR_BCR(DMA_InitStruct->transferByteCnt);
    
    /* source config */
    DMA0->DMA[DMA_InitStruct->chl].SAR  = DMA_InitStruct->sAddr;
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_SSIZE(DMA_InitStruct->sDataWidth);
    (DMA_InitStruct->sAddrIsInc)?(DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_SINC_MASK):(DMA0->DMA[DMA_InitStruct->chl].DCR &= ~DMA_DCR_SINC_MASK);
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_SMOD(DMA_InitStruct->sMod);
    
    /* dest config */
    DMA0->DMA[DMA_InitStruct->chl].DAR  = DMA_InitStruct->dAddr;
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_DSIZE(DMA_InitStruct->sDataWidth);
    (DMA_InitStruct->dAddrIsInc)?(DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_DINC_MASK):(DMA0->DMA[DMA_InitStruct->chl].DCR &= ~DMA_DCR_DINC_MASK);
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_DMOD(DMA_InitStruct->dMod);
    
    /* defaut: cycle steal */
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_CS_MASK;
    
    /* defaut: enable auto disable req */
    DMA0->DMA[DMA_InitStruct->chl].DCR |= DMA_DCR_D_REQ_MASK;
    
    /* enable chl */
    DMAMUX0->CHCFG[DMA_InitStruct->chl] |= DMAMUX_CHCFG_ENBL_MASK;
}

/**
 * @brief  获得 DMA Counter 计数值
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval 计数值
 */
uint32_t DMA_GetTransferByteCnt(uint8_t chl)
{
    return (DMA0->DMA[chl].DSR_BCR & DMA_DSR_BCR_BCR_MASK )>>DMA_DSR_BCR_BCR_SHIFT;
}

void DMA_SetTransferByteCnt(uint8_t chl, uint32_t val)
{
    DMA0->DMA[chl].DSR_BCR = DMA_DSR_BCR_BCR(val);
}

/**
 * @brief  使能通道响应传输
 * @code
 *     //开启DMA 的0通道进行数据传输
 *     DMA_EnableRequest(HW_DMA_CH0);
 * @endcode
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval None
 */
void DMA_EnableRequest(uint8_t chl)
{
    DMA0->DMA[chl].DCR |= DMA_DCR_ERQ_MASK;
}


/**
 * @brief  禁止通道响应传输
 * @code
 *     //开启DMA 的0通道进行数据传输
 *     DMA_EnableRequest(HW_DMA_CH0);
 * @endcode
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval None
 */
void DMA_DisableRequest(uint8_t chl)
{
    DMA0->DMA[chl].DCR &= ~DMA_DCR_ERQ_MASK;
}


/**
 * @brief  每次一个触发信号, 只触发一次传输
 */
void DMA_EnableCycleSteal(uint8_t chl, bool flag)
{
    (flag)?
    (DMA0->DMA[chl].DCR |= DMA_DCR_CS_MASK):
    (DMA0->DMA[chl].DCR &= ~DMA_DCR_CS_MASK);
}

/**
 * @brief  在Majloop 结束后  是否自动关闭Request
 */
void DMA_EnableAutoDisableRequest(uint8_t chl , bool flag)
{
    (flag)?
    (DMA0->DMA[chl].DCR |= DMA_DCR_D_REQ_MASK):
    (DMA0->DMA[chl].DCR &= ~DMA_DCR_D_REQ_MASK);
}


/**
 * @brief  设置DMA模块指定通道的目标地址
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param address: 32位的目标数据地址
 * @retval None
 */
void DMA_SetDestAddress(uint8_t ch, uint32_t address)
{
    DMA0->DMA[ch].DAR = address;
}

uint32_t DMA_GetDestAddress(uint8_t ch)
{
    return DMA0->DMA[ch].DAR;
}

/**
 * @brief  设置DMA模块指定通道的源地址
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param address: 32位的源数据地址
 * @retval None
 */
void DMA_SetSourceAddress(uint8_t ch, uint32_t address)
{
    DMA0->DMA[ch].SAR = address;
}

uint32_t DMA_GetSourceAddress(uint8_t ch)
{
    return DMA0->DMA[ch].SAR;
}


/**
 * @brief  设置DMA传输完成中断
 * @code
 *     //开启DMA 的0通道的传输完成中断功能
 *     DMA_StartTransfer(HW_DMA_CH0);
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param config: 配置模式
 *         @arg kDMA_IT_Half_Disable 禁止DMA传输一半中断触发
 *         @arg kDMA_IT_Major_Disable 禁止DMA传输完成中断触发
 *         @arg kDMA_IT_Half 开启DMA传输一半中断触发
 *         @arg kDMA_IT_Major 开启DMA传世完成中断触发
 * @retval None
 */
void DMA_ITConfig(uint8_t chl, DMA_ITConfig_Type config, bool status)
{
    if(status)
    {
        NVIC_EnableIRQ(DMA_IRQnTable[chl]);
    }
    
    switch(config)
    {
        case kDMA_IT_Major:
            (status)?
            (DMA0->DMA[chl].DCR |= DMA_DCR_EINT_MASK):
            (DMA0->DMA[chl].DCR &= DMA_DCR_EINT_MASK);
            break; 
        default:
            break;
    }
}

/**
 * @brief  注册中断回调函数
 * @param  chl: DMA通道号
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param AppCBFun: 回调函数指针
 * @retval None
 */
void DMA_CallbackInstall(uint8_t chl, DMA_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        DMA_CallBackTable[chl] = AppCBFun;
    }
}

void DMA0_IRQHandler(void)
{
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    if(DMA_CallBackTable[0]) DMA_CallBackTable[0]();
}

void DMA1_IRQHandler(void)
{
    DMA0->DMA[1].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    if(DMA_CallBackTable[1]) DMA_CallBackTable[1]();
}

void DMA2_IRQHandler(void)
{
    DMA0->DMA[2].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    if(DMA_CallBackTable[2]) DMA_CallBackTable[2]();
}

void DMA3_IRQHandler(void)
{
    DMA0->DMA[3].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    if(DMA_CallBackTable[3]) DMA_CallBackTable[3]();
}

