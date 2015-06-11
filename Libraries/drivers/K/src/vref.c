/**
  ******************************************************************************
  * @file    vref.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
 #include "vref.h"
 #include "common.h"
 
 /**
 * @brief  初始化VREF 模块
 * @param  DAC_InitTypeDef: 初始化结构
 * @retval None
 */
void VREF_Init(VREF_InitTypeDef *VREF_InitStruct)
{
    /* open clock gate */
    SIM->SCGC4 |= SIM_SCGC4_VREF_MASK;
    /* enable moudle */
    #ifdef VREF_SC_ICOMPEN_MASK
    VREF->SC = VREF_SC_VREFEN_MASK | VREF_SC_REGEN_MASK | VREF_SC_ICOMPEN_MASK | VREF_SC_MODE_LV(VREF_InitStruct->bufferMode);
    VREF->TRM |= VREF_TRM_CHOPEN_MASK;
    #else
    VREF->SC = VREF_SC_VREFEN_MASK | VREF_SC_REGEN_MASK  | VREF_SC_MODE_LV(VREF_InitStruct->bufferMode);
    #endif
    
}

 /**
 * @brief  快速初始化VREF模块
 * @note   VREF被初始化后 需要经过大概35MS 才能有稳定的参考电压输出
 * @retval None
 */
void VREF_QuickInit(void)
{
    VREF_InitTypeDef  VREF_InitStruct1;
    VREF_InitStruct1.instance = 0;
    VREF_InitStruct1.bufferMode = kVREF_BufferMode_Lv1;
    VREF_Init(&VREF_InitStruct1);
}

 /**
 * @brief  恢复VREF到默认状态
 * @retval None
 */
void VREF_DeInit(void)
{
    SIM->SCGC4 &= ~SIM_SCGC4_VREF_MASK;
    VREF->SC = 0;
}

 /**
 * @brief  设置VREF校准值
 * @param  校准值
 * @retval None
 */
void VREF_SetTrimValue(uint32_t val)
{
    VREF->TRM &= ~VREF_TRM_TRIM_MASK;
    VREF->TRM |= VREF_TRM_TRIM(val);
}
 
