/*
** ###################################################################
**     Processors:          MKL03Z32CAF4
**                          MKL03Z32VFG4
**                          MKL03Z16VFG4
**                          MKL03Z8VFG4
**                          MKL03Z32VFK4
**                          MKL03Z16VFK4
**                          MKL03Z8VFK4
**
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KL03P24M48SF0RM, Rev 2, Apr 2014
**     Version:             rev. 1.2, 2014-04-30
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2014 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2013-12-11)
**         Initial version.
**     - rev. 1.1 (2014-04-16)
**         Update of the I2C module (SMBUS feature).
**         Update of the MCG_Light module.
**         Added register file system (RFSYS).
**     - rev. 1.2 (2014-04-30)
**         PEx compatibility macros has been added.
**
** ###################################################################
*/

/*!
 * @file MKL03Z4
 * @version 1.2
 * @date 2014-04-30
 * @brief Device specific configuration file for MKL03Z4 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MKL03Z4.h"



/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {

#if (DISABLE_WDOG)
  /* Disable the COP module */
  /* SIM_COPC: COPCLKSEL=0,COPDBGEN=0,COPSTPEN=0,COPT=0,COPCLKS=0,COPW=0 */
  SIM->COPC = (uint32_t)0x00u;
#endif /* (DISABLE_WDOG) */
#if (CLOCK_SETUP == 0)
  /* SIM->CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
                 SIM_CLKDIV1_OUTDIV4(0x01); /* Update system prescalers */
  /* MCG->SC: FCRDIV=1 */
  MCG->SC = MCG_SC_FCRDIV(0x01);       /* Set the LIRC1 divider*/
  /* MCG->MC: HIRC=0,LIRC_DIV2=0 */
  MCG->MC = MCG_MC_LIRC_DIV2(0x00);    /* Set the LIRC2 divider*/
  /* OSC->CR: ERCLKEN=0,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
  OSC->CR = (uint32_t)0x00u;         /* Disable External reference */
  /* MCG->C2: EREFS0=0,IRCS=1 */
  MCG->C2 = MCG_C2_IRCS_MASK;         /* Enable LIRC 8MHz */
  /* Switch to LIRC 8MHz Mode */
  /* MCG->C1: CLKS=1,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = MCG_C1_CLKS(0x01) |
            MCG_C1_IRCLKEN_MASK;       /* Enable LIRC and select LIRC as a clock source */
  while((MCG->S & MCG_S_CLKST_MASK) != 0x04u) {} /* Check that the clock source is the LIRC clock. */
#elif (CLOCK_SETUP == 1)
  /* SIM->CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
                 SIM_CLKDIV1_OUTDIV4(0x01); /* Set system prescalers */
  /* MCG->SC: FCRDIV=1 */
  MCG->SC = MCG_SC_FCRDIV(0x01);       /* LIRC1 divider not used - leave the default value*/
  /* MCG->MC: HIRC=1,LIRC_DIV2=0 */
  MCG->MC = MCG_MC_HIRCEN_MASK;    /* Enable HIRC clock source*/
  /* OSC->CR: ERCLKEN=0,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
  OSC->CR = (uint32_t)0x00u;         /* Disable External reference */
  /* MCG->C2: EREFS0=0,IRCS=1 */
  MCG->C2 = MCG_C2_IRCS_MASK;         /* Not used - leave default value */
  /* Switch to HIRC Mode */
  /* MCG->C1: CLKS=0,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = MCG_C1_CLKS(0x00) |
            MCG_C1_IRCLKEN_MASK;       /* Leave LIRC enabled and select HIRC as a clock source */
  while((MCG->S & MCG_S_CLKST_MASK) != 0x00u) {} /* Check that the clock source is the HIRC clock. */
#elif (CLOCK_SETUP == 2)
  /* SIM->SCGC5: PORTA=1 */
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Enable clock gate for port to enable pin routing */
  /* SIM->CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
                 SIM_CLKDIV1_OUTDIV4(0x00); /* Set system prescalers */
  /* PORTA_PCR3: ISF=0,MUX=0 */
  PORTA->PCR[3] &= (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07));
  /* PORTA_PCR4: ISF=0,MUX=0 */
  PORTA->PCR[4] &= (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07));
  /* MCG->SC: FCRDIV=1 */
  MCG->SC = MCG_SC_FCRDIV(0x01);       /* LIRC1 divider not used - leave the default value*/
  /* MCG->MC: HIRC=0,LIRC_DIV2=0 */
  MCG->MC = MCG_MC_LIRC_DIV2(0x00);    /* Not used - leave the default value */
  /* MCG->C2: EREFS0=1,IRCS=1 */
  MCG->C2 = MCG_C2_EREFS0_MASK | MCG_C2_IRCS_MASK;   /* Select external crystal, for LIRC - leave default value */
  /* OSC->CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
  OSC->CR = OSC_CR_ERCLKEN_MASK;         /* Enable External reference */
  /* Switch to EXT Mode */
  /* MCG->C1: CLKS=0,IRCLKEN=0,IREFSTEN=0 */
  MCG->C1 = MCG_C1_CLKS(0x02);       /* Disable LIRC and select EXT as a clock source */
  while((MCG->S & MCG_S_CLKST_MASK) != 0x08u) {} /* Check that the clock source is the EXT clock. */
#elif (CLOCK_SETUP == 3)
  /* MCG->MC: HIRC=1 */
  MCG->MC |= MCG_MC_HIRCEN_MASK;    /* Enable HIRC clock source*/
  /* MCG->C1: CLKS=0,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = MCG_C1_CLKS(0x00) |
            MCG_C1_IRCLKEN_MASK;       /* Leave LIRC enabled and select HIRC as a clock source */
  while((MCG->S & MCG_S_CLKST_MASK) != 0x00u) {} /* Check that the clock source is the HIRC clock. */
  /* SIM->CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
                 SIM_CLKDIV1_OUTDIV4(0x01); /* Set system prescalers */
  /* MCG->SC: FCRDIV=0 */
  MCG->SC = MCG_SC_FCRDIV(0x00);       /* Set the LIRC1 divider to 1*/
  /* MCG->MC: HIRC=0,LIRC_DIV2=0 */
  MCG->MC = MCG_MC_LIRC_DIV2(0x00);    /* Set the LIRC2 divider to 1 */
  /* OSC->CR: ERCLKEN=0,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
  OSC->CR = (uint32_t)0x00u;         /* Disable External reference */
  /* MCG->C2: EREFS0=0,IRCS=0 */
  MCG->C2 = (uint32_t)0x00u;         /* Enable LIRC 2MHz */
  /* Switch to LIRC 2MHz Mode */
  /* MCG->C1: CLKS=1,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = MCG_C1_CLKS(0x01) |
            MCG_C1_IRCLKEN_MASK;       /* Enable LIRC and select LIRC as a clock source */
  while((MCG->S & MCG_S_CLKST_MASK) != 0x04u) {} /* Check that the clock source is the LIRC clock. */
#endif

}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

  uint32_t ICSOUTClock;                                                        /* Variable to store output clock frequency of the ICS module */
  uint8_t Divider;

  if ((MCG->S & MCG_S_CLKST_MASK) == 0x04u) {
    /* LIRC reference clock is selected */
    ICSOUTClock = CPU_INT_SLOW_CLK_HZ;
    Divider = (uint8_t)(1u << ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT));
    ICSOUTClock = (ICSOUTClock / Divider);  /* Calculate the divided LIRC clock */
  } else if ((MCG->S & MCG_S_CLKST_MASK) == 0x0u) {
    /* HIRC reference clock is selected */
    ICSOUTClock = CPU_INT_FAST_CLK_HZ;
  } else if ((MCG->S & MCG_S_CLKST_MASK) == 0x80u) {
    /* External reference clock is selected */
    ICSOUTClock = CPU_XTAL_CLK_HZ;
  } else {
    /* Reserved value */
    return;
  }
  SystemCoreClock = (ICSOUTClock / (1u + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));

}
