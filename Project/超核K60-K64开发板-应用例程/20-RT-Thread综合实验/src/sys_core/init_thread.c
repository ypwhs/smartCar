#include <rtthread.h>
#include "board.h"
#include "components.h"
#include "spi_flash_w25qxx.h"
#include "chlib_k.h"
#include "sram.h"
#include "drv_spi.h"
#include "drv_uart.h"

void rt_hw_ksz8041_init(void);
void rt_hw_spi_init(void);
void rt_hw_sd_init(void);
void rt_hw_rtc_init(void);
void rt_hw_dflash_init(void);
void rt_hw_dram_init(void);
    
void usb_thread_entry(void* parameter);
void sd_thread_entry(void* parameter);
int ui_startup(int argc, char** argv);


rt_err_t ads7843_init(const char * name, const char * spi_device_name);

void init_thread_entry(void* parameter)
{
    rt_thread_t tid;
    
    #ifndef FRDM
    SRAM_Init();
    rt_system_heap_init((void*)(SRAM_ADDRESS_BASE), (void*)(SRAM_ADDRESS_BASE + SRAM_SIZE));
    #else
    rt_system_heap_init((void*)(0x1FFF0000), (void*)(0x1FFF0000 + 0x10000));
    #endif
    
    rt_thread_delay(1);
    
    rt_hw_uart_init();
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
    rt_kprintf("rt-thread system start!\r\n");
    
    finsh_system_init();
    
    rt_hw_spi_init();
    rt_hw_sd_init();
    rt_hw_rtc_init();
    rt_hw_dflash_init();
    rt_hw_dram_init();
    
    ads7843_init("ads7843", "spi20");
    w25qxx_init("sf0", "spi21");

    if(dfs_mount("sf0", "/", "elm", 0, 0))
    {
        dfs_mkfs("elm", "sf0");
        dfs_mount("sf0", "/", "elm", 0, 0);
    }
        
//    if(dfs_mount("sd0", "/", "elm", 0, 0))
//        rt_kprintf("sd mount failed\r\n");
    
    at24cxx_init("at24c02", "i2c0");

    #ifndef FRDM
    ui_startup(RT_NULL, RT_NULL);
    #endif
    
    tid = rt_thread_create("usb", usb_thread_entry, RT_NULL, (1024*1), 0x15, 20);                                                      
    rt_thread_startup(tid);	

    
    rt_hw_ksz8041_init();
    
    tid = rt_thread_self();
    rt_thread_delete(tid); 
}

