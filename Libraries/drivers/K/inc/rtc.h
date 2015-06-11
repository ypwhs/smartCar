/**
  ******************************************************************************
  * @file    rtc.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_RTC_H__
#define __CH_LIB_RTC_H__

#include <stdint.h>
#include "common.h"


typedef struct 
{
   uint16_t year;    /*!< 年Range from 200 to 2099.*/
   uint16_t month;   /*!< 月Range from 1 to 12.*/
   uint16_t day;     /*!< 日Range from 1 to 31 (depending on month).*/
   uint16_t hour;    /*!< 时Range from 0 to 23.*/
   uint16_t minute;  /*!< 分Range from 0 to 59.*/
   uint8_t second;   /*!< 秒Range from 0 to 59.*/
}RTC_DateTime_Type;


typedef enum
{
    kRTC_OScLoad_0PF,   /* 电容配置 */
    kRTC_OScLoad_2PF,
    kRTC_OScLoad_4PF,
    kRTC_OScLoad_8PF,
    kRTC_OScLoad_16PF,
}RTC_OscLoad_Type;


typedef struct
{
    RTC_OscLoad_Type        oscLoad;            // OSC 电容负载
}RTC_InitTypeDef;

//!< interrupt and DMA select
typedef enum
{
    kRTC_IT_TimeAlarm,             /* 开启闹钟中断 */
    kRTC_IT_TimeOverflow,          /* 开启时间溢出中断 */    
}RTC_ITDMAConfig_Type;

//!< CallbackType
typedef void (*RTC_CallBackType)(void);

//!<API functions
void RTC_QuickInit(void);
void RTC_GetDateTime(RTC_DateTime_Type * datetime);
void RTC_SetDateTime(RTC_DateTime_Type * datetime);
void RTC_ITDMAConfig(RTC_ITDMAConfig_Type config, bool status);
uint32_t RTC_IsTimeValid(void);
void RTC_CallbackInstall(RTC_CallBackType AppCBFun);
//!< function which may not be used
void RTC_Init(RTC_InitTypeDef * RTC_InitStruct);
int  RTC_GetWeekFromYMD(int year, int month, int days);
uint32_t RTC_GetTSR(void);
void RTC_SetTSR(uint32_t val);
void RTC_SetAlarm(RTC_DateTime_Type * datetime);
void RTC_SetCompensation(uint32_t compensationInterval, uint32_t timeCompensation);



#endif
