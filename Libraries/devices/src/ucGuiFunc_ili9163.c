#include "ili9163.h"
#include "spi.h"
#include "common.h"


static struct spi_bus bus;
extern int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance);

int LCD_L0_Init(void)
{
    kinetis_spi_bus_init(&bus, HW_SPI0);
    PORT_PinMuxConfig(HW_GPIOA, 14, kPinAlt2); // SPI0_CS0
    PORT_PinMuxConfig(HW_GPIOA, 15, kPinAlt2); // SPI0_SCK
    PORT_PinMuxConfig(HW_GPIOA, 16, kPinAlt2); // SPI0_SOUT
    PORT_PinMuxConfig(HW_GPIOA, 17, kPinAlt2); // SPI0_SIN

    ili9163_init(&bus, HW_SPI_CS0);
    return 0;
}

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex)
{
    ili9163_write_pixel(x, y, PixelIndex);
}

