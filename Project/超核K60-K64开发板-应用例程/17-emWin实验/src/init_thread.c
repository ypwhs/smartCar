#include <rtthread.h>
#include <rthw.h>

#include "board.h"
#include "components.h"
#include "drv_ksz8041.h"
#include "spi_flash_w25qxx.h"
#include "gpio.h"
#include "spi.h"
#include "sram.h"
#include "drv_spi.h"

void led_thread_entry(void* parameter);

rt_err_t touch_ads7843_init(const char * name, const char * spi_device_name);

void init_thread_entry(void* parameter)
{
    //SRAM_Init();
    rt_system_heap_init((void*)0x1FFF0000, (void*)(0x1FFF0000 + 0x10000));
    
    
    struct rt_spi_device *spi_device;
    rt_thread_t tid;
   
    
    rt_hw_spi_bus_init(HW_SPI2, "spi2");

    PORT_PinMuxConfig(HW_GPIOD, 14, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOD, 13, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOD, 12, kPinAlt2); 
    
    /* attacted spi2 - 1*/
    {
        static struct rt_spi_device spi_device;
        static struct kinetis_spi_cs spi_cs_0;
        spi_cs_0.ch = 1;
        PORT_PinMuxConfig(HW_GPIOD, 15, kPinAlt2); //SPI2_PCS1
        rt_spi_bus_attach_device(&spi_device, "spi21", "spi2", &spi_cs_0);
    }
    
    /* attacted spi2 - 0*/
    {
        static struct rt_spi_device spi_device;
        static struct kinetis_spi_cs spi_cs_0;
        spi_cs_0.ch = 0;
        PORT_PinMuxConfig(HW_GPIOD, 11, kPinAlt2); //SPI2_PCS0
        rt_spi_bus_attach_device(&spi_device, "spi20", "spi2", &spi_cs_0);
    }
    
    touch_ads7843_init("ads7843", "spi20");
    
    tid = rt_thread_create("led", led_thread_entry, RT_NULL, 256, 0x24, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);

    extern int cmd_gui_start(int argc, char** argv);
    cmd_gui_start(0, RT_NULL);
    
    tid = rt_thread_self();
    rt_thread_delete(tid); 
}

