#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "spi.h"
#include "ili9163.h"
#include "chgui.h"


static struct spi_bus bus;
extern int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance);


int DoILI9163(int argc, char * const argv[])
{

   kinetis_spi_bus_init(&bus, HW_SPI1);
    PORT_PinMuxConfig(HW_GPIOE, 0, kPinAlt2);  
    PORT_PinMuxConfig(HW_GPIOE, 1, kPinAlt2);  
    PORT_PinMuxConfig(HW_GPIOE, 2, kPinAlt2);  
    ili9163_init(&bus, 0);
    GUI_printf(0, 0, "%s%d\r\n", "Numis:",13);
    return 0;
}

SHELL_EXPORT_CMD(DoILI9163, ili9163 , ili9163 test)

