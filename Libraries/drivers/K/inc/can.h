/**
  ******************************************************************************
  * @file    can.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.4.10
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#ifndef __CH_LIB_CAN_H__
#define __CH_LIB_CAN_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "common.h"  

//!< hardware instances
#define HW_CAN0  (0x00U)  //CAN0模块
#define HW_CAN1  (0x01U)  //CAN1模块  
     
//CAN总线速度选择
typedef enum 
{
	kCAN_Baudrate_25K,   //CAN通信速度为25KHz
	kCAN_Baudrate_50K,
	kCAN_Baudrate_100K,
	kCAN_Baudrate_125K,
	kCAN_Baudrate_250K,
	kCAN_Baudrate_500K,
	kCAN_Baudrate_1000K, //CAN通信速度为1MHz
}CAN_Baudrate_Type;

typedef enum
{
    kCAN_Frame_Remote,  //远程帧
    kCAN_Frame_Data,    //数据帧
}CAN_Frame_Type;


typedef struct
{
    uint32_t                instance;  //CAN通信端口 HW_CAN0/HW_CAN1
    CAN_Baudrate_Type       baudrate;  //CAN通信速度设置
}CAN_InitTypeDef;


//!< CAN QuickInit macro                            
#define CAN1_TX_PE24_RX_PE25   (0xB0A1U)  //CAN1模块 发送引脚为PTE24 接收引脚为PTE25
#define CAN0_TX_PA12_RX_PA13   (0x9880U)  //CAN0模块 发送引脚为PTA12 接收引脚为PTA13
#define CAN0_TX_PB18_RX_PB19   (0xA488U)  //CAN0模块 发送引脚为PTB18 接收引脚为PTB19
#define CAN1_TX_PC17_RX_PC16   (0xA091U)  //CAN1模块 发送引脚为PTC17 接收引脚为PTC16


/*!< 中断及DMA配置 */
typedef enum
{
    kCAN_IT_Tx_Disable,  //关闭发送中断
    kCAN_IT_Rx_Disable,  //关闭接收中断
    kCAN_IT_Tx,          //开启发送中断
    kCAN_IT_RX,          //开启发送中断
}CAN_ITDMAConfig_Type;

/*!< CAN 回调函数声明 */
typedef void (*CAN_CallBackType)(void);

//!< API functions
void CAN_SetReceiveMask(uint32_t instance, uint32_t mb, uint32_t mask);
uint32_t CAN_IsMessageBoxBusy(uint32_t instance, uint32_t mb);
uint32_t CAN_WriteData(uint32_t instance, uint32_t mb, uint32_t id, uint8_t* buf, uint8_t len);
uint32_t CAN_ReadData(uint32_t instance, uint32_t mb, uint8_t *buf, uint8_t *len);
uint32_t CAN_QuickInit(uint32_t CANxMAP, CAN_Baudrate_Type baudrate);
void CAN_ITDMAConfig(uint32_t instance, uint32_t mb, CAN_ITDMAConfig_Type config);
void CAN_SetReceiveMB(uint32_t instance, uint32_t mb, uint32_t id);
void CAN_CallbackInstall(uint32_t instance, CAN_CallBackType AppCBFun);
void CAN_Init(CAN_InitTypeDef* CAN_InitStruct);


#endif
