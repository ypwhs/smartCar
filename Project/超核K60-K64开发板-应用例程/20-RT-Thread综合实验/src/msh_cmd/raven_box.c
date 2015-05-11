#include <rtthread.h>
#include "common.h"
#include "components.h"
 
#ifdef FINSH_USING_MSH
#include <finsh.h>

static int reboot(int argc, char** argv)
{
    NVIC_SystemReset();
    return 0;
}
MSH_CMD_EXPORT(reboot, reboot);


extern struct rt_object_information rt_object_container[];

static long _kill(char *name)
{
    struct rt_thread *thread;
    struct rt_list_node *node;
    rt_uint8_t *ptr;
    struct rt_list_node *list = &rt_object_container[RT_Object_Class_Thread].object_list;
    for (node = list->next; node != list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, list);
        if(!strcmp(thread->name, name))
        {
            rt_kprintf("kill %s now!\r\n", name);
            rt_thread_delete(thread);
        }
    }
    return 0;
}


int kill(int argc, char ** argv)
{
    if(argc == 2)
    {
        _kill(argv[1]); 
    }
    return 0;
}

MSH_CMD_EXPORT(kill, kill a thread);


int mountnfs(const char * host)
{
    int r;
    if(!(netif_list->flags & NETIF_FLAG_UP)) 
    {
        rt_kprintf("no network\r\n");
        return -1;
    }

    r = dfs_mount("e0", "/NFS", "nfs", 0, host);
    (!r)?(rt_kprintf("mount OK\r\n")):(rt_kprintf("mount failed\r\n"));
    return 0;
}

FINSH_FUNCTION_EXPORT(mountnfs, eg:mountnfs("192.168.1.101:/"))


static int mount(int argc, char** argv)
{
    int ret;
    char *device, *fs, *path;
    
    fs = argv[1];
    device = argv[2];
    path = argv[3];
    
    if(argc < 4)
        return 1;

    ret = dfs_mount(device, path, fs, 0, argv[4]);

    if(ret)
        printf("%s mount on %s fail!%d\r\n", device, path, ret);
    else
        printf("%s mount on %s ok!\r\n", device, path);
    
    return ret;
}

MSH_CMD_EXPORT(mount, mount elm sd0 /dev/sd);

static int cpufreq(int argc, char** argv)
{
    uint32_t clock, tick;
    CLOCK_GetClockFrequency(kCoreClock, &clock);
    rt_kprintf("CoreClock:%dHz\r\n", clock);
    CLOCK_GetClockFrequency(kBusClock, &clock);
    rt_kprintf("BusClock:%dHz\r\n", clock);
    
    tick = rt_tick_get();
    rt_kprintf("tick:%d | time:%ds\r\n", tick, tick/RT_TICK_PER_SECOND);
    return 0;
}
MSH_CMD_EXPORT(cpufreq, cpufreq);

#endif


//    rt_kprintf("NETWORK\r\n");
//    rt_device_t dev;
//    dev = rt_device_find("e0");
//    if(dev)
//    {
//        enet_phy_data phy_data;
//        extern void list_if(void);
//        list_if();
//        dev->control(dev, NIOCTL_GET_PHY_DATA, &phy_data);
//        rt_kprintf("\r\n");
//        rt_kprintf("TX:%05d | %05dKB\r\n", phy_data.tx_fcnt, phy_data.tx_dcnt/1024);
//        rt_kprintf("RX:%05d | %05dKB\r\n", phy_data.rx_fcnt, phy_data.rx_dcnt/1024);
//    }
//    return 0;