#include <rtthread.h>
#include <rthw.h>
#include <components.h>
#include <stdint.h>

void rt_system_comonent_init(void)
{

#ifdef RT_USING_MODULE
    rt_system_module_init();
#endif

#ifdef RT_USING_LWIP
	/* initialize lwip stack */
	/* register ethernetif device */
	eth_system_device_init();

	/* initialize lwip system */
	lwip_system_init();
	rt_kprintf("TCP/IP initialized!\n");
#endif

#ifdef RT_USING_DFS
	/* initialize the device file system */
	dfs_init();

#ifdef RT_USING_DFS_ELMFAT
	/* initialize the elm chan FatFS file system*/
	elm_init();
#endif

#if defined(RT_USING_DFS_NFS) && defined(RT_USING_LWIP)
	/* initialize NFSv3 client file system */
	nfs_init();
#endif

#ifdef RT_USING_DFS_YAFFS2
	dfs_yaffs2_init();
#endif

#ifdef RT_USING_DFS_UFFS
	dfs_uffs_init();
#endif

#ifdef RT_USING_DFS_JFFS2
	dfs_jffs2_init();
#endif

#ifdef RT_USING_DFS_ROMFS
	dfs_romfs_init();
#endif

#ifdef RT_USING_DFS_RAMFS
	dfs_ramfs_init();
#endif

#ifdef RT_USING_DFS_DEVFS
	devfs_init();
#endif
#endif /* end of RT_USING_DFS */

#ifdef RT_USING_NEWLIB
	libc_system_init(RT_CONSOLE_DEVICE_NAME);
#else
	/* the pthread system initialization will be initiallized in libc */
#ifdef RT_USING_PTHREADS 
	pthread_system_init();
#endif
#endif

#ifdef RT_USING_RTGUI
	rtgui_system_server_init();
#endif

#ifdef RT_USING_USB_HOST
	rt_usb_host_init();
#endif
}

void DelayMs(uint32_t ms)
{
    if(ms < (1000/RT_TICK_PER_SECOND)) rt_thread_delay(1);
    else rt_thread_delay(ms/(1000/RT_TICK_PER_SECOND));  
}

void SysTick_Handler(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

__weak void init_thread_entry(void* parameter)
{
    rt_kprintf("%s empty init thread!\r\n", __func__);
}

/* redefine fputc */
int fputc(int ch,FILE *f)
{
    rt_kprintf("%c", ch);
    return ch;
}


void _init_entry(void* parameter)
{
    rt_thread_t tid;
    
    rt_system_comonent_init();
    
//    #if defined(RT_USING_DFS_ROMFS) 
//    dfs_mount(RT_NULL, "/", "rom", 0, &romfs_root);
//    #endif
    
    /* init thread */
    tid = rt_thread_create("init", init_thread_entry, RT_NULL, 1024, 0x20, 20);                       
    if (tid != RT_NULL)
        rt_thread_startup(tid);		

    tid = rt_thread_self();
    rt_thread_delete(tid);
}


void rt_application_init(void)
{
    rt_thread_t tid;
    
    /* internal init thread */
    tid = rt_thread_create("init0", _init_entry, RT_NULL, 512, 5, 20);                       
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}



