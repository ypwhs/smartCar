/**
  ******************************************************************************
  * @file    common.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
	* @note    此文件为内部文件，用户无需调用和修改  
  ******************************************************************************
  */
#include "common.h"
#include <string.h>


#define MCGOUT_TO_CORE_DIVIDER           (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT) + 1)
#define MCGOUT_TO_SYSTEM_DIVIDER         (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT) + 1)
#define MCGOUT_TO_BUS_DIVIDER            (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT) + 1)
#define MCGOUT_TO_PERIPHERAL_DIVIDER     (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT) + 1)
#define MCGOUT_TO_FLEXBUS_DIVIDER        (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV3_MASK)>>SIM_CLKDIV1_OUTDIV3_SHIFT) + 1)
#define MCGOUT_TO_FLASH_DIVIDER          (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT) + 1)

 /**
 * @brief  获得系统各个总线时钟的频率
 * @code
 *         //获得总线时钟频率
 *         uint32_t BusClock;
 *         CLOCK_GetClockFrequency(kBusClock, &BusClock);
 *         //将总线时钟频率显示出来
 *         printf("BusClock:%dHz\r\n", BusClock);
 * @endcode
 * @param  clockName:时钟名称
 *         @arg kCoreClock    :内核时钟
 *         @arg kSystemClock  :系统时钟 = 内核时钟
 *         @arg kBusClock     :总线时钟
 *         @arg kFlexBusClock :Flexbus总线时钟
 *         @arg kFlashClock   :Flash总线时钟
 * @param  FrequenctInHz: 获得频率数据的指针 单位Hz
 * @retval 0: 成功 非0: 错误
 */
int32_t CLOCK_GetClockFrequency(CLOCK_Source_Type clockName, uint32_t* FrequenctInHz)
{
    uint32_t MCGOutClock = 0;
    /* calualte MCGOutClock system_MKxxx.c must not modified */
    SystemCoreClockUpdate();
    MCGOutClock = SystemCoreClock * MCGOUT_TO_CORE_DIVIDER;
    switch (clockName)
    {
        case kCoreClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_CORE_DIVIDER;
            break;
        case kSystemClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_CORE_DIVIDER;
            break;	
        case kBusClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_BUS_DIVIDER;
            break;
        case kFlashClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_FLASH_DIVIDER;	
            break;
        case kMCGOutClock:
            *FrequenctInHz = MCGOutClock;
            break;
        default:
            return 1;
    }
    return 0;
}

 /**
 * @brief  编码快速初始化结构 用户不需调用
 *
 * @param  type: 快速初始化结构体指针
 * @retval       32位快速初始化编码
 */
uint32_t QuickInitEncode(QuickInit_Type * type)
{
    return *(uint32_t*)type;
}

 /**
 * @brief  解码快速初始化结构 用户不需调用
 *
 * @param  map: 32位快速初始化编码
 * @param  type: 快速初始化结构指针
 * @retval None
 */
void QuickInitDecode(uint32_t map, QuickInit_Type * type)
{
    QuickInit_Type * pMap = (QuickInit_Type*)&(map);
    memcpy(type, pMap, sizeof(QuickInit_Type));  
}

void DWT_DelayInit(void)
{
    /* enable DEM */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    /* enable counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void DWT_DelayUs(uint32_t us)
{
    uint32_t startts, endts, ts;
    startts = DWT->CYCCNT;
    ts =  us * (SystemCoreClock /(1000*1000) ); 
    endts = startts + ts;      
    if(endts > startts)  
    {
        while(DWT->CYCCNT < endts);       
    }
    else
    {
        while(DWT->CYCCNT > endts);
        while(DWT->CYCCNT < endts);
    }
}

void DWT_DelayMs(uint32_t ms)
{
    DWT_DelayUs(ms*1000);
}

/**
 * @brief  延时初始化函数
 * @code
 *      // 完成延时初始化配置，
 *      //使用内核的SYStic模块实现延时功能
 *        DelayInit();
 * @endcode
 * @param  None
 * @retval None
 */
#pragma weak DelayInit
void DelayInit(void)
{
    DWT_DelayInit();
}

/**
 * @brief 抽象毫秒级的延时设置函数
 * @code
 *      // 实现500ms的延时功能
 *        DelayMs(500);
 * @endcode
 * @param  ms :需要延时的时间，单位毫秒
 * @retval None
 * @note  首先需要完成延时初始化配置
 */

#pragma weak DelayMs
void DelayMs(uint32_t ms)
{
    DWT_DelayMs(ms);
}

/**
 * @brief 抽象微秒级的延时设置函数
 * @code
 *      // 实现500us的延时功能
 *        DelayUs(500);
 * @endcode
 * @param  us :需要延时的时间，单位微秒
 * @retval None
 * @note  首先需要完成延时初始化配置
 */
#pragma weak DelayUs
void DelayUs(uint32_t us)
{
    DWT_DelayUs(us);
}



#if (defined(LIB_DEBUG))

void assert_failed(char * file, uint32_t line)
{
    LIB_TRACE("assert failed @ %s in %d\r\n", file, line);
	//断言失败检测
	while(1);
}
#endif

/* 非可屏蔽中断 non maskable interrupt*/
void NMI_Handler(void)
{
    /* clear NMI pending bit */
    // MCM->ISR |= MCM_ISR_NMI_MASK;
    // printf("NMI ENTER\r\n");
}


#if (defined(LIB_DEBUG) && defined(DEBUG_FAULT_HANDLER))


void HardFault_Handler(void)
{
    printf("HardFault_Handler\r\n");
    while(1);
}

void BusFault_Handler(void)
{
    printf("BusFault_Handler\r\n");
    while(1);
}

#endif


/*
    for(i=0;i<ARRAY_SIZE(I2C_QuickInitTable);i++)
    {
        printf("(0X%08XU)\r\n", QuickInitEncode(&I2C_QuickInitTable[i]));
    }
*/

