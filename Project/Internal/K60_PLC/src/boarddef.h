#ifndef __CH_BOARDDEF_H__
#define __CH_BOARDDEF_H__

#include "chlib_k.h"

#define     BOARD_UART_INSTANCE     HW_UART0
#define     BOARD_UART_MAP          UART0_RX_PD06_TX_PD07

#define     LED0_PORT   HW_GPIOE
#define     LED0_PIN    (10)
#define     LED1_PORT   HW_GPIOE
#define     LED1_PIN    (11)
#define     LED2_PORT   HW_GPIOE
#define     LED2_PIN    (12)
#define     LED3_PORT   HW_GPIOE
#define     LED3_PIN    (24)

#define     LED0  PEout(LED0_PIN)
#define     LED1  PEout(LED1_PIN)
#define     LED2  PEout(LED2_PIN)
#define     LED3  PEout(LED3_PIN)


#define ADC_CHANNELS        {6, 7, 8, 9,10,11,12,13,10,11,12,13,14,15,21,22};
#define ADC_PORTS           {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};


#define DIN_PINS            {27, 26, 25, 24, 17, 16};
#define DIN_PORTS           { 0,  0,  0,  0,  0,  0};
#define DIN0                PAin(27)
#define DIN1                PAin(26)
#define DIN2                PAin(25)
#define DIN3                PAin(24)
#define DIN4                PAin(17)
#define DIN5                PAin(16)


#define DOUT_PINS           {15, 14, 13, 12, 11, 10};
#define DOUT_PORTS          { 0,  0,  0,  0,  0,  0};
#define DOUT0               PAout(15)
#define DOUT1               PAout(14)
#define DOUT2               PAout(13)
#define DOUT3               PAout(12)
#define DOUT4               PAout(11)
#define DOUT5               PAout(10)

#define RELAY_PINS          {21, 20, 19, 18, 17, 16};
#define RELAT_PORTS         { 1,  1,  1,  1,  1,  1};
#define RELAY0              PBout(21)
#define RELAY1              PBout(20)
#define RELAY2              PBout(19)
#define RELAY3              PBout(18)
#define RELAY4              PBout(17)
#define RELAY5              PBout(16)


#define RS485_CH1_MAP       UART5_RX_PE09_TX_PE08
#define RS485_CH2_MAP       UART3_RX_PE05_TX_PE04
#define RS485_CH3_MAP       UART4_RX_PC14_TX_PC15

#define BOARD_I2C_MAP       I2C1_SCL_PE01_SDA_PE00


//函数变量声明



#endif

