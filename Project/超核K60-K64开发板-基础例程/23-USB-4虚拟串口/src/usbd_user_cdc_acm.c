/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_user_cdc_acm.c
 *      Purpose: USB Device (USBD) - Communication Device Class (CDC) - 
 *               Abstract Control Model (ACM) - User module
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
 
//#include <RTL.h>
#include <rl_usb.h>
#include "UART.h"

//UART_Configuration UART_Config;

/** \brief  Vitual COM Port initialization

    The function inititalizes the hardware resources of the port used as 
    the Virtual COM Port.

    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortInitialize (void) { 

//  return (UART_Initialize ());
    return 1;
}


/** \brief  Vitual COM Port uninitialization

    The function uninititalizes/releases the hardware resources of the port used 
    as the Virtual COM Port.

    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortUninitialize (void) { 

 // return (UART_Uninitialize ());
    return 1;
}


/** \brief  Vitual COM Port reset

    The function resets the internal states of the port used 
    as the Virtual COM Port.

    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortReset (void) { 

  //return (UART_Reset ());
    return 1;
}


/** \brief  Virtual COM Port change communication settings

    The function changes communication settings of the port used as the 
    Virtual COM Port.

    \param [in]         line_coding  Pointer to the loaded CDC_LINE_CODING structure.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortSetLineCoding (CDC_LINE_CODING *line_coding) {

//  UART_Config.Baudrate    = line_coding->dwDTERate;
//  UART_Config.DataBits    = (UART_DataBits) line_coding->bDataBits;
//  UART_Config.Parity      = (UART_Parity)   line_coding->bParityType;
 // UART_Config.StopBits    = (UART_StopBits) line_coding->bCharFormat;
//  UART_Config.FlowControl = UART_FLOW_CONTROL_NONE;

 // return (UART_SetConfiguration (&UART_Config));
    return 1;
}


/** \brief  Vitual COM Port retrieve communication settings

    The function retrieves communication settings of the port used as the 
    Virtual COM Port.

    \param [in]         line_coding  Pointer to the CDC_LINE_CODING structure.
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortGetLineCoding (CDC_LINE_CODING *line_coding) {

//  if (UART_GetConfiguration (&UART_Config)) {
//    line_coding->dwDTERate   = UART_Config.Baudrate;
//    line_coding->bDataBits   = UART_Config.DataBits;
//    line_coding->bParityType = UART_Config.Parity;
//    line_coding->bCharFormat = UART_Config.StopBits;
//    return (1);
//  }

  return (0);
}


/** \brief  Virtual COM Port set control line state

    The function sets control line state on the port used as the
    Virtual COM Port.

    \param [in]         ctrl_bmp Control line settings bitmap (
                          0. bit - DTR state, 
                          1. bit - RTS state).
    \return             0        Function failed.
    \return             1        Function succeeded.
 */

int32_t USBD_CDC_ACM_PortSetControlLineState (uint16_t ctrl_bmp) {

  return (1);
}
