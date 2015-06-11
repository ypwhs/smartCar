#ifndef __BOARDS_H__
#define __BOARDS_H__

//!< 这个文件定义不同开发板上的硬件资源

#ifdef RAVEN

//!< LED 端口 及 引脚
#define BOARD_NAME                      "RAVEN"
#define BOARD_UART_BAUDRATE             115200
#define BOARD_UART_DEBUG_MAP            UART0_RX_PD06_TX_PD07
#define BOARD_BUZZER_FTM_MAP            FTM0_CH3_PA06
#define BOARD_BUZZER_GPIO_PORT          (HW_GPIOA)
#define BOARD_BUZZER_GPIO_PIN           (6)
#define BOARD_LED_GPIO_BASES            {HW_GPIOE, HW_GPIOE, HW_GPIOE, HW_GPIOE}
#define BOARD_LED_PIN_BASES             {6, 7, 11, 12}
#define BOARD_I2C_MAP                   I2C0_SCL_PB02_SDA_PB03
#define BOARD_I2C_INSTANCE              (0)
#define BOARD_KEY_GPIO_BASES            {HW_GPIOE, HW_GPIOE, HW_GPIOA}
#define BOARD_KEY_PIN_BASES             {26, 27,  4}
#define BOARD_ADC_MAP                   ADC0_SE19_DM0
#define BOARD_LPTMR_MAP                 LPTMR_ALT2_PC05

#define BOARD_SPI_MAP                   SPI2_SCK_PD12_SOUT_PD13_SIN_PD14
#define BOARD_SPI_INSTANCE              HW_SPI2
#define BOARD_TP_SPI_PCSN               (0)
#define BOARD_FLASH_SPI_PCSN            (1)

#define BOARD_OV7620_PCLK_PORT      HW_GPIOA
#define BOARD_OV7620_PCLK_PIN       (7)
#define BOARD_OV7620_VSYNC_PORT     HW_GPIOA
#define BOARD_OV7620_VSYNC_PIN      (16)
#define BOARD_OV7620_HREF_PORT      HW_GPIOA
#define BOARD_OV7620_HREF_PIN       (17)
#define BOARD_OV7620_DATA_PORT      HW_GPIOA //PA8-15
#define BOARD_OV7620_DATA_OFFSET    (8)

#define BOARD_NRF2401_CE_PORT       HW_GPIOE
#define BOARD_NRF2401_CE_PIN        (0)

#define BOARD_CLKOUT_PORT           HW_GPIOA
#define BOARD_CLKOUT_PIN            (11)

#define BOARD_I2C_SCL_PORT          HW_GPIOB
#define BOARD_I2C_SDA_PORT          HW_GPIOB
#define BOARD_I2C_SCL_PIN           2
#define BOARD_I2C_SDA_PIN           3

#elif LZ_K60
#define BOARD_NAME                      "LZ_K60"
#define BOARD_UART_BAUDRATE             115200
#define BOARD_UART_DEBUG_MAP            UART0_RX_PA15_TX_PA14
#define BOARD_LED_GPIO_BASES            {HW_GPIOE, HW_GPIOE, HW_GPIOE, HW_GPIOE}
#define BOARD_LED_PIN_BASES             {24, 25, 26, 27}
#define BOARD_I2C_SCL_PORT          HW_GPIOB
#define BOARD_I2C_SDA_PORT          HW_GPIOB
#define BOARD_I2C_SCL_PIN           2
#define BOARD_I2C_SDA_PIN           3

#elif CHK60EVB1

#define BOARD_UART_DEBUG_MAP        UART4_RX_PC14_TX_PC15
#define BOARD_SPI_MAP               SPI2_SCK_PB21_SOUT_PB22_SIN_PB23
#define BOARD_SPI_INSTANCE          HW_SPI2
#define BOARD_TXP2046_SPI_MAP       SPI2_SCK_PB21_SOUT_PB22_SIN_PB23
#define BOARD_TXP2046_SPI_PCS_PORT  HW_GPIOB
#define BOARD_TXP2046_SPI_PCS_PIN   20
#define BOARD_TXP2046_SPI_PCS       (0)
#define BOARD_FLASH_SPI_PCS         (1)

#elif ATOM


#elif URANUS

#define BOARD_LED_GPIO_BASES        {HW_GPIOA}
#define BOARD_LED_PIN_BASES         {1}
#define BOARD_I2C_INSTANCE          HW_I2C0
#define BOARD_I2C_MAP               I2C0_SCL_PE19_SDA_PE18
#define BOARD_SPI_MAP               SPI0_SCK_PC05_SOUT_PC06_SIN_PC07
#define BOARD_SPI_CS_PORT           HW_GPIOC
#define BOARD_SPI_CS_PIN            (2)
#define BOARD_SPI_CS                (2)
#define BOARD_NRF2401_IRQ_PORT      HW_GPIOB
#define BOARD_NRF2401_IRQ_PIN       (1)
#define BOARD_UART_DEBUG_MAP        UART1_RX_PC03_TX_PC04
#define BOARD_UART_INSTANCE         (HW_UART1)


#elif NET_5000
#define BOARD_NAME					"NET_5000"
#define BOARD_UART_BAUDRATE             115200
#define BOARD_LED_GPIO_BASES        {HW_GPIOC}
#define BOARD_LED_PIN_BASES         {12,13}
#define BOARD_I2C_INSTANCE          HW_I2C0
#define BOARD_I2C_MAP               I2C0_SCL_PE19_SDA_PE18
#define BOARD_SPI_MAP               SPI0_SCK_PC05_SOUT_PC06_SIN_PC07
#define BOARD_UART_DEBUG_MAP        UART5_RX_PE08_TX_PE09
#define BOARD_UART_INSTANCE         (HW_UART5

#elif defined(RADIUM)

#define BOARD_NAME                  "RADIUM"
#define BOARD_UART_DEBUG_MAP        UART0_RX_PA01_TX_PA02
#define BOARD_LED_GPIO_BASES        { HW_GPIOE, HW_GPIOD }
#define BOARD_LED_PIN_BASES         { 6, 7 }
#define BOARD_I2C_MAP               I2C0_SCL_PE19_SDA_PE18
#define BOARD_ADC_MAP               ADC0_SE1A_PE16
#define BOARD_LPTMR_MAP             LPTMR_ALT2_PC05
#define BOARD_KEY_GPIO_BASES        {0}
#define BOARD_KEY_PIN_BASES         {0}
#define BOARD_SPI_MAP               SPI1_SCK_PE02_MOSI_PE01_MISO_PE00
#define BOARD_SPI_CS_PORT           HW_GPIOE
#define BOARD_SPI_CS_PIN            4
#define BOARD_FTM_MAP               TPM0_CH3_PC04

#elif defined(FRDM)

#if defined(MK64F12)
#define BOARD_NAME                  "frdmk64f120m"
#define BOARD_UART_DEBUG_MAP        UART0_RX_PB16_TX_PB17
#define BOARD_UART_BAUDRATE         (115200)
#define BOARD_I2C_MAP               I2C0_SCL_PE24_SDA_PE25
#define BOARD_LED_GPIO_BASES        {HW_GPIOB, HW_GPIOB, HW_GPIOE}
#define BOARD_LED_PIN_BASES         {21, 22, 26}
#define BOARD_LPTMR_MAP             LPTMR_ALT1_PA19
#define BOARD_KEY_GPIO_BASES        {HW_GPIOA, HW_GPIOC}
#define BOARD_KEY_PIN_BASES         {4, 6}
#define BOARD_ADC_MAP               ADC1_SE18
#define BOARD_CLKOUT_PORT           HW_GPIOC
#define BOARD_CLKOUT_PIN            3
#define BOARD_FTM_MAP               FTM0_CH3_PA06
#define BOARD_I2C_SDA_PORT          HW_GPIOD
#define BOARD_I2C_SCL_PORT          HW_GPIOD
#define BOARD_I2C_SDA_PIN           5
#define BOARD_I2C_SCL_PIN           6

#endif

#else

#error  "Please Define a Board (RAVEN)(URANUS)"

#endif



#endif

