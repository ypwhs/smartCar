#include <rtthread.h>
#include "board.h"
#include "components.h"
#include "chlib_k.h"
#include "sram.h"
#include "drv_uart.h"


void rt_hw_ksz8041_init(void);
void rt_hw_dflash_init(void);
void rt_hw_sd_init(void);


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
    
    rt_hw_sd_init();
    rt_hw_dflash_init();
    
    if(dfs_mount("dflash0", "/", "elm", 0, 0))
    {
        rt_kprintf("format root file system!...\r\n");
        dfs_mkfs("elm", "dflash0");
        dfs_mount("dflash0", "/", "elm", 0, 0);
    }
    
    mkdir("/dev");
    mkdir("/dev/SD", 0x777);
    if(dfs_mount("sd0", "/dev/SD", "elm", 0, 0))
        rt_kprintf("sd mount on /SD failed\r\n");

    rt_hw_ksz8041_init();
    
    rt_kprintf("waitting for connection...");
    
    /* tcp server demp */
    tcpserv();

    tid = rt_thread_self();
    rt_thread_delete(tid); 
}

