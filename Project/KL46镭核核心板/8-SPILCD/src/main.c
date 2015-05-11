#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "tpm.h"
#include "lptmr.h"
#include "adc.h"
#include "spi.h"
#include "ili9163.h"
#include "chgui.h"

static struct spi_bus bus;
extern int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance);


int main(void)
{
    int value;
    DelayInit();
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    kinetis_spi_bus_init(&bus, HW_SPI1);
    PORT_PinMuxConfig(HW_GPIOE, 0, kPinAlt2);  
    PORT_PinMuxConfig(HW_GPIOE, 1, kPinAlt2);  
    PORT_PinMuxConfig(HW_GPIOE, 2, kPinAlt2);  
    ili9163_init(&bus, 0);
    GUI_printf(0, 0, "HelloWorld", "Numis:",13);
    GUI_printf(0, 10, "%d", 1234);
    while(1)
    {

    }
}


