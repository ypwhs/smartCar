/**
  ******************************************************************************
  * @file    systick.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片ARM内核中的SysTick模块的底层功能函数
  ******************************************************************************
  */
  
#include "systick.h"
#include "common.h"

static uint32_t fac_us = 0;     //!< usDelay Mut
static uint32_t fac_ms = 0;


//! @defgroup CHKinetis
//! @{


//! @defgroup SysTick
//! @brief SysTick API functions
//! @{

 /**
 * @brief  初始化SysTick时钟
 * @code
 *     // 初始化SysTick时钟 设定中断周期为10000us(10ms)
 *      SYSTICK_Init(10000);
 * @endcode
 * @param  timeInUs: 中断周期 单位us
 * @retval None
 */
void SYSTICK_Init(uint32_t timeInUs)
{
    /* Set clock source = core clock */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 
    CLOCK_GetClockFrequency(kCoreClock, &fac_us);
    fac_us /= 1000000;
    fac_ms = fac_us * 1000;
    /* every 100ms in a int */
    SysTick->LOAD = fac_us * timeInUs;
}

 /**
 * @brief  初始化SysTick为延时应用 初始化后就可以调用 DelayMs DelayUs
 * @code
 *      //将SysTick用作延时的初始化 初始化后系统延时20ms
 *      SYSTICK_DelayInit();
 *      SYSTICK_DelayMs(20);
 * @endcode
 * @param  None
 * @retval None
 */
void SYSTICK_DelayInit(void)
{
    SYSTICK_Init(1234);
    SYSTICK_Cmd(true);
    SYSTICK_ITConfig(false);
}

 /**
 * @brief  开启或者停止SysTick时钟
 * @code
 *      //开启时钟
 *      SYSTICK_Cmd(true);
 * @endcode
 * @param  NewState:使能或者关闭
 *         @arg true :使能
 *         @arg false:停止
 * @retval None
 */
void SYSTICK_Cmd(bool NewState)
{
    (true == NewState)?(SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk):(SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk);
}

 /**
 * @brief  开启SysTick中断
 * @code
 *      //开启中断功能
 *      SYSTICK_ITConfig(true);
 * @endcode
 * @param  NewState:使能或者关闭
 *         @arg true :使能
 *         @arg false:禁止
 * @retval None
 */
void SYSTICK_ITConfig(bool NewState)
{
    (true == NewState)?(SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk):(SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk);
}

 /**
 * @brief 微秒级延时函数
 * @code
 *      //延时100us
 *      SYSTICK_DelayUs(100);
 * @endcode
 * @param  us:延时时间 单位us
 * @retval None
 */
void SYSTICK_DelayUs(uint32_t us)
{
    uint32_t temp;
    SysTick->LOAD = us * fac_us;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
}

 /**
 * @brief  毫秒级延时函数
 * 
 * @code
 *      //延时100ms
 *      SYSTICK_DelayMs(100);
 * @endcode
 * @param  us:延时
 * @retval None
 */
void SYSTICK_DelayMs(uint32_t ms)
{
    uint32_t temp;
    uint32_t i;
    SysTick->LOAD = fac_ms;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    for(i = 0; i < ms; i++)
	{
		SysTick->VAL = 0;
		do
		{
			temp = SysTick->CTRL;
		}
        while((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
	}
}
 
 
//! @}

//! @}

