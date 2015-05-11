#include <rtthread.h>
#include <rthw.h>
#include "finsh.h"

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

MSH_CMD_EXPORT(kill, kill a thread)



