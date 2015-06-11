/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KV10P48M75RM Rev.2, July 2013
**     Version:             rev. 1.0, 2013-05-09
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2013 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2013-05-09)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MKV10Z7
 * @version 1.0
 * @date 2013-05-09
 * @brief Device specific configuration file for MKV10Z7 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MKV10Z7.h"

#define DISABLE_WDOG    1

#define CLOCK_SETUP     1
/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 72MHz, BusClock = 24MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (FEE) mode
         Reference clock source for MCG module is an external crystal 10MHz
         Core clock = 75MHz, BusClock = 25MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external crystal 10MHz with no multiplication
         Core clock = 10MHz, BusClock = 10MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 10000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            72000000u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 10000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            75000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 10000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            10000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 2) */

/* ----------------------------------------------------------------------------
   -- Non-volatile IRC user trim field
   ---------------------------------------------------------------------------- */

#define NON_VOLATILE_IRC_USER_TRIM(offset)            (((uint8_t*)0x000003FCu)[offset])

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

int main(void);

void SystemInit (void) {
#if (DISABLE_WDOG)
  /* Disable the WDOG module */
  /* WDOG_UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = (uint16_t)0xC520u;                   /* Key 1 */
  /* WDOG_UNLOCK : WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK  = (uint16_t)0xD928u;                  /* Key 2 */
  /* WDOG_STCTRLH: DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = (uint16_t)0x01D2u;
#endif /* (DISABLE_WDOG) */
#if (CLOCK_SETUP == 0)
  /* SIM->CLKDIV1: OUTDIV1=0,OUTDIV4=2, OUTDIV5EN=1, OUTDIV5=1  */
  SIM->CLKDIV1 = (uint32_t)0x00029000UL;              /* Update system prescalers */
  /* Switch to FEI Mode */
  /* MCG->C1: CLKS=0, FRDIV=0, IREFS=1, IRCLKEN=1, IREFSTEN=0 */
  MCG->C1 = (uint8_t)0x06U;
  /* MCG->C2: LOCRE0=0, FCFTRIM=FCFTRIM_Trim, RANGE0=0,HGO0=0,EREFS0=0,LP=0,IRCS=0 */
  MCG->C2 = (uint8_t)(((uint8_t)0x40U) & NON_VOLATILE_IRC_USER_TRIM(2));
  /* MCG->C3: SCTRIM=SCTRIM_Trim */
  MCG->C3 = NON_VOLATILE_IRC_USER_TRIM(3);
  /* MCG->C4: DMX32=1, DRST_DRS=2, FCTRIM=FCTRIM_Trim, SCFTRIM=SCFTRIM_Trim */
  MCG->C4 = (uint8_t)(((uint8_t)0xC0U) | (uint8_t)(((uint8_t)0x1FU) & NON_VOLATILE_IRC_USER_TRIM(2)));
  /* MCG->C6: CME0=0 */
  MCG->C6 = (uint8_t)0x00U;
  while((uint8_t)(MCG->S & (uint8_t)0x1DU) != (uint8_t)0x10U) { /* Check that IREFST=1, CLKST=0, IRCST=0. */
  }
#elif (CLOCK_SETUP == 1)
  /* SIM->SCGC5: PORTE=0, PORTD=0, PORTC=0, PORTB=0, PORTA=1, LPTMR=0 */
  SIM->SCGC5 = (uint32_t)0x00040380UL;                /* Enable clock gate for ports to enable pin routing */
  /* SIM->CLKDIV1: OUTDIV1=0,OUTDIV4=2, OUTDIV5EN=1, OUTDIV5=1  */
  SIM->CLKDIV1 = (uint32_t)0x00029000UL;              /* Update system prescalers */
  /* PORTA->PCR18: ISF=1, IRQC=0, MUX=0, DSE=0, PFE=0, SRE=0, PE=0, PS=1 */
  PORTA->PCR[18] = (uint32_t)0x01000001UL;
  /* PORTA->PCR19: ISF=1, IRQC=0, MUX=0, DSE=0, PFE=0, SRE=0, PE=0, PS=1 */
  PORTA->PCR[19] = (uint32_t)0x01000001UL;
  /* Switch to FEE Mode */
  /* MCG->C2: LOCRE0=0, FCFTRIM=FCFTRIM_Trim, RANGE0=1, HGO0=0, EREFS0=1, LP=0, IRCS=0 */
  MCG->C2 = (uint8_t)(((uint8_t)0x40U & NON_VOLATILE_IRC_USER_TRIM(2)) | (uint8_t)0x14U);
  /* OSC0->CR: ERCLKEN=1, EREFSTEN=0, SC2P=0, SC4P=0, SC8P=0, SC16P=0 */
  OSC0->CR = (uint8_t)0x80U;
  while((uint8_t)(MCG->S & (uint8_t)0x02U) != (uint8_t)0x02U) { /* Check that OSCINIT0=1 */
  }
  /* MCG_C1: CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=1, IREFSTEN=0 */
  MCG->C1 = (uint8_t)0x1AU;
  while((uint8_t)(MCG->S & (uint8_t)0x1DU) != (uint8_t)0x00U) { /* Check that Check that IREFST=0, CLKST=0, IRCST=0. */
  }
  /* MCG->C3: SCTRIM=SCTRIM_Trim */
  MCG->C3 = NON_VOLATILE_IRC_USER_TRIM(3);
  /* MCG->C4: DMX32=0, DRST_DRS=2, FCTRIM=FCTRIM_Trim, SCFTRIM=SCFTRIM_Trim */
  MCG->C4 = (uint8_t)(((uint8_t)0x40U) | (uint8_t)(((uint8_t)0x1FU) & NON_VOLATILE_IRC_USER_TRIM(2)));
  /* MCG->C6: CME0=0 */
  MCG->C6 = (uint8_t)0x00U;
#elif (CLOCK_SETUP == 2)
  /* SIM->SCGC5: PORTE=0, PORTD=0, PORTC=0, PORTB=0, PORTA=1, LPTMR=0 */
  SIM->SCGC5 = (uint32_t)0x00040380UL;                /* Enable clock gate for ports to enable pin routing */
  /* PORTA->PCR18: ISF=1, IRQC=0, MUX=0, DSE=0, PFE=0, SRE=0, PE=0, PS=1 */
  PORTA->PCR[18] = (uint32_t)0x01000001UL;
  /* PORTA->PCR19: ISF=1, IRQC=0, MUX=0, DSE=0, PFE=0, SRE=0, PE=0, PS=1 */
  PORTA->PCR[19] = (uint32_t)0x01000001UL;
  /* Switch to FEE Mode */
  /* MCG->C2: LOCRE0=0, FCFTRIM=FCFTRIM_Trim, RANGE0=1, HGO0=0, EREFS0=1, LP=0, IRCS=0 */
  MCG->C2 = (uint8_t)(((uint8_t)0x40U & NON_VOLATILE_IRC_USER_TRIM(2)) | (uint8_t)0x14U);
  /* OSC0->CR: ERCLKEN=1, EREFSTEN=0, SC2P=0, SC4P=0, SC8P=0, SC16P=0 */
  OSC0->CR = (uint8_t)0x80U;
  while((uint8_t)(MCG->S & (uint8_t)0x02U) != (uint8_t)0x02U) { /* Check that OSCINIT0=1 */
  }
  /* MCG_C1: CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=1, IREFSTEN=1 */
  MCG->C1 = (uint8_t)0x9BU;
  while((uint8_t)(MCG->S & (uint8_t)0x1DU) != (uint8_t)0x08U) { /* Check that Check that IREFST=0, CLKST=2, IRCST=0. */
  }
  /* SIM->CLKDIV1: OUTDIV1=0,OUTDIV4=0, OUTDIV5EN=1, OUTDIV5=0  */
  SIM->CLKDIV1 = (uint32_t)0x00008000UL;              /* Update system prescalers */
  /* MCG->C2: LOCRE0=0, FCFTRIM=FCFTRIM_Trim, RANGE0=1, HGO0=0, EREFS0=1, LP=1, IRCS=0 */
  MCG->C2 = (uint8_t)(((uint8_t)0x40U & NON_VOLATILE_IRC_USER_TRIM(2)) | (uint8_t)0x16U);
  while((uint8_t)(MCG->S & (uint8_t)0x01U) != (uint8_t)0x00U) { /* Check that Check that IRCST=0. */
  }
#endif /* (CLOCK_SETUP == 2) */
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {
  uint32_t MCGOUTClock;                               /* Variable to store output clock frequency of the MCG module */
	
  switch ((uint8_t)(MCG->C1 & MCG_C1_CLKS_MASK)) {
    case 0x00u:                                       /* Output of FLL is selected */
      if ((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u) {    /* External reference clock is selected */
        MCGOUTClock = (uint32_t)((uint32_t)CPU_XTAL_CLK_HZ >> (uint8_t)((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));                                       /* System oscillator drives MCG clock */
        if ((MCG->C2 & MCG_C2_RANGE0_MASK) != 0x0u) {
          MCGOUTClock /= 32u;                         /* If high range is enabled, additional 32 divider is active */
        }; /* ((MCG->C2 & MCG_C2_RANGE0_MASK) != 0x0u) */
      } else { /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u)) */
        MCGOUTClock = CPU_INT_SLOW_CLK_HZ;            /* The slow internal reference clock is selected */
      } /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u)) */
      /* Select correct multiplier to calculate the MCG output clock  */
      switch (MCG->C4 & (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK)) {
        case 0x0u:
          MCGOUTClock *= 640u;
          break;
        case 0x20u:
          MCGOUTClock *= 1280u;
          break;
        case 0x40u:
          MCGOUTClock *= 1920u;
          break;
        case 0x60u:
          MCGOUTClock *= 2560u;
          break;
        case 0x80u:
          MCGOUTClock *= 732u;
          break;
        case 0xA0u:
          MCGOUTClock *= 1464u;
          break;
        case 0xC0u:
          MCGOUTClock *= 2197u;
          break;
        case 0xE0u:
          MCGOUTClock *= 2929u;
          break;
        default:
          break;
        }
      break;
    case 0x40u:                                       /* Internal reference clock is selected */
      if ((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u) {
        MCGOUTClock = CPU_INT_SLOW_CLK_HZ;            /* Slow internal reference clock selected */
      } else { /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u)) */
        MCGOUTClock = (uint32_t)((uint32_t)CPU_INT_FAST_CLK_HZ >> (uint8_t)((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT)); /* Fast internal reference clock selected */
      } /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u)) */
      break;
    case 0x80u:                                       /* External reference clock is selected */
      MCGOUTClock = CPU_XTAL_CLK_HZ;
      break;
    default:
      return;                                         /* Reserved value */
  }
  SystemCoreClock = (MCGOUTClock / (1u + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));
}
