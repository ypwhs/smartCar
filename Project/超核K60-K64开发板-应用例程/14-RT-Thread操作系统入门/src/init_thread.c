#include <rtthread.h>
#include "chlib_k.h"
#include "drv_uart.h"
#include "shell.h"

/* 线程1 入口 */
void t1_thread_entry(void* parameter)
{
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}

/* 线程2 入口 */
void t2_thread_entry(void* parameter)
{
    GPIO_QuickInit(HW_GPIOE, 7, kGPIO_Mode_OPP);
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 7);
        DelayMs(500);
    } 
}

/*  RTT 入口函数 相当于无操作系统下的 main 函数
    RTT 成功启动后 会自动执行 init_thread_entry 这个线程
*/
void init_thread_entry(void* parameter)
{

    rt_thread_t tid;
    /* 创建线程 t1 堆栈大小256 优先级0x24 时间片为20ms */
    tid = rt_thread_create("t1", t1_thread_entry, RT_NULL, 256, 0x24, 20);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
  
    /* 创建线程 t2 堆栈大小256 优先级0x24 时间片为20ms*/
    tid = rt_thread_create("t2", t2_thread_entry, RT_NULL, 256, 0x24, 20);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
   
    /* 启动shell 控制台系统 可选 */

    rt_hw_uart_init();
    rt_console_set_device("uart0");
    
    rt_kprintf("hello rt-thread!\r\n");
    
    /* 命令行系统启动 */
    finsh_system_init();

    /* 删除 init_thread_entry 这个线程 */
    tid = rt_thread_self();
    rt_thread_delete(tid); 
}

