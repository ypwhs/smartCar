#include <rtthread.h>
#include <dfs_posix.h>

/* this thread make SD card removable */

#define SD_DEVICE_NAME   "sd0"

void sd_thread_entry(void* parameter)
{
    rt_thread_t thread = rt_thread_self();
    int r;
    rt_device_t dev = rt_device_find(SD_DEVICE_NAME);
    if(dev == RT_NULL)
    {
        rt_kprintf("cannot find %s device\r\n", SD_DEVICE_NAME);
        rt_thread_suspend(thread); 
    }
    
    dfs_mount(SD_DEVICE_NAME, "/SD", "elm", 0, 0);

	while(1)
	{
        if(dev->open_flag == RT_DEVICE_OFLAG_CLOSE)
        {
            dfs_unmount("/SD");
            r = dfs_mount(SD_DEVICE_NAME, "/SD", "elm", 0, 0);
            if(!r)
            {
                rt_kprintf("%s mounted on /SD !\r\n", SD_DEVICE_NAME);
            }
        }
        rt_thread_delay(50);
	}
}
