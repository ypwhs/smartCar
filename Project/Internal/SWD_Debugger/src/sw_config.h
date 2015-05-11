

#ifndef __CH_SW_CONFIG_H__
#define __CH_SW_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#define PIN_DELAY()         DelayUs(1)
#define TRESET_INSTANCE     (HW_GPIOB)
#define SWD_INSTANCE        (HW_GPIOC)

#define TCK_PIN         (5)
#define TMS_PIN         (6)
#define TRST_PIN        (1)

#define TRST_LOW()	 		PBout(TRST_PIN) = 0
#define TRST_HIGH()	 		PBout(TRST_PIN) = 1
#define TCK_LOW() 			PCout(TCK_PIN) = 0
#define TCK_HIGH()			PCout(TCK_PIN) = 1
#define TMS_LOW() 			PCout(TMS_PIN) = 0
#define TMS_HIGH() 			PCout(TMS_PIN) = 1
#define TMS_INPUT()	 	    PCin(TMS_PIN)


#define TMS_WR()            PTB->PDDR |= (1 << TMS_PIN)
#define TMS_RD()            PTB->PDDR &= ~(1 << TMS_PIN)
#define DELAY()             DelayUs(1)

static inline void PIN_SWDIO_OUT(uint32_t bit)
{
    PCout(TMS_PIN) = bit;
}

static inline void SW_CLOCK_CYCLE(void)
{
    TCK_LOW();
    PIN_DELAY();
    TCK_HIGH();
    PIN_DELAY();
}

static inline void SW_WRITE_BIT(uint32_t bit)
{
    PCout(TMS_PIN) = bit;
    TCK_LOW();
    PIN_DELAY();
    TCK_HIGH();
    PIN_DELAY();
}

static inline uint32_t SW_READ_BIT(void)
{
    uint32_t bit;
    TCK_LOW();
    PIN_DELAY();
    bit = TMS_INPUT();
    TCK_HIGH();
    PIN_DELAY();
    return bit;
}

static inline void swd_io_init(void)
{
    GPIO_QuickInit(SWD_INSTANCE, TCK_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(SWD_INSTANCE, TMS_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(TRESET_INSTANCE, TRST_PIN, kGPIO_Mode_OOD);
    
    /* release trst pin */
    TMS_WR();
    TMS_LOW();
    TCK_LOW();
    TRST_HIGH();
}

static inline void SWJ_PortOff(void)
{
    
}

#endif


