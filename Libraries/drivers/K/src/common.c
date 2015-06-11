/**
  ******************************************************************************
  * @file    common.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
	* @note    ´ËÎÄ¼þÎªÄÚ²¿ÎÄ¼þ£¬ÓÃ»§ÎÞÐèµ÷ÓÃºÍÐÞ¸Ä  
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
 * @brief  »ñµÃÏµÍ³¸÷¸ö×ÜÏßÊ±ÖÓµÄÆµÂÊ
 * @code
 *         //»ñµÃ×ÜÏßÊ±ÖÓÆµÂÊ
 *         uint32_t BusClock;
 *         CLOCK_GetClockFrequency(kBusClock, &BusClock);
 *         //½«×ÜÏßÊ±ÖÓÆµÂÊÏÔÊ¾³öÀ´
 *         printf("BusClock:%dHz\r\n", BusClock);
 * @endcode
 * @param  clockName:Ê±ÖÓÃû³Æ
 *         @arg kCoreClock    :ÄÚºËÊ±ÖÓ
 *         @arg kSystemClock  :ÏµÍ³Ê±ÖÓ = ÄÚºËÊ±ÖÓ
 *         @arg kBusClock     :×ÜÏßÊ±ÖÓ
 *         @arg kFlexBusClock :Flexbus×ÜÏßÊ±ÖÓ
 *         @arg kFlashClock   :Flash×ÜÏßÊ±ÖÓ
 * @param  FrequenctInHz: »ñµÃÆµÂÊÊý¾ÝµÄÖ¸Õë µ¥Î»Hz
 * @retval 0: ³É¹¦ ·Ç0: ´íÎó
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
 * @brief  ½øÈëµÍ¹¦ºÄÄ£Ê½
 * @param  enSleepOnExit:ÔÚÏµÍ³»½ÐÑÊ±ºò ÊÇ·ñ¼ÌÐø½øÈëµÍ¹¦ºÄ
 * @retval 0: ³É¹¦ ·Ç0: ´íÎó
 * @note  ÈÎºÎÖÐ¶Ï ¶¼¿ÉÒÔ»½ÐÑCPU
 */
void EnterSTOPMode(bool enSleepOnExit)
{
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    if (enSleepOnExit)
    {
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }
    else
    {
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    }
    
    /* WFI instruction will start entry into STOP mode */
    __ASM("WFI");
}

 /**
 * @brief  ±àÂë¿ìËÙ³õÊ¼»¯½á¹¹ ÓÃ»§²»Ðèµ÷ÓÃ
 *
 * @param  type: ¿ìËÙ³õÊ¼»¯½á¹¹ÌåÖ¸Õë
 * @retval       32Î»¿ìËÙ³õÊ¼»¯±àÂë
 */
uint32_t QuickInitEncode(QuickInit_Type * type)
{
    return *(uint32_t*)type;
}

 /**
 * @brief  ½âÂë¿ìËÙ³õÊ¼»¯½á¹¹ ÓÃ»§²»Ðèµ÷ÓÃ
 *
 * @param  map: 32Î»¿ìËÙ³õÊ¼»¯±àÂë
 * @param  type: ¿ìËÙ³õÊ¼»¯½á¹¹Ö¸Õë
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
 * @brief  ÑÓÊ±³õÊ¼»¯º¯Êý
 * @code
 *      // Íê³ÉÑÓÊ±³õÊ¼»¯ÅäÖÃ£¬
 *      //Ê¹ÓÃÄÚºËµÄSYSticÄ£¿éÊµÏÖÑÓÊ±¹¦ÄÜ
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
 * @brief ³éÏóºÁÃë¼¶µÄÑÓÊ±ÉèÖÃº¯Êý
 * @code
 *      // ÊµÏÖ500msµÄÑÓÊ±¹¦ÄÜ
 *        DelayMs(500);
 * @endcode
 * @param  ms :ÐèÒªÑÓÊ±µÄÊ±¼ä£¬µ¥Î»ºÁÃë
 * @retval None
 * @note  Ê×ÏÈÐèÒªÍê³ÉÑÓÊ±³õÊ¼»¯ÅäÖÃ
 */

#pragma weak DelayMs
void DelayMs(uint32_t ms)
{
    DWT_DelayMs(ms);
}

/**
 * @brief ³éÏóÎ¢Ãë¼¶µÄÑÓÊ±ÉèÖÃº¯Êý
 * @code
 *      // ÊµÏÖ500usµÄÑÓÊ±¹¦ÄÜ
 *        DelayUs(500);
 * @endcode
 * @param  us :ÐèÒªÑÓÊ±µÄÊ±¼ä£¬µ¥Î»Î¢Ãë
 * @retval None
 * @note  Ê×ÏÈÐèÒªÍê³ÉÑÓÊ±³õÊ¼»¯ÅäÖÃ
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
	//¶ÏÑÔÊ§°Ü¼ì²â
	while(1);
}
#endif

/* ·Ç¿ÉÆÁ±ÎÖÐ¶Ï non maskable interrupt*/
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
      __asm("BKPT #0x03"); 
    while(1);
}

void BusFault_Handler(void)
{
    printf("BusFault_Handler\r\n");
      __asm("BKPT #0x03"); 
    while(1);
}

#endif


/*
    for(i=0;i<ARRAY_SIZE(I2C_QuickInitTable);i++)
    {
        printf("(0X%08XU)\r\n", QuickInitEncode(&I2C_QuickInitTable[i]));
    }
*/

