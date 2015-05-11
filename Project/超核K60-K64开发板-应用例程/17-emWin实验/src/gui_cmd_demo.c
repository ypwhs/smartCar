#include <rtthread.h>
#include <finsh.h>
#include "GUI.H"



void gui_thread_entry(void* parameter)
{
    U32 i;

    GUI_Init();
    GUI_DispString("gui system actived!\r\n");
    GUI_CURSOR_Show();
    TOUCH_MainTask();
    MainTask();
    while(1)
    {
        rt_thread_delay(1);
    }
}

void guit_thread_entry(void* parameter)
{
	while(1)
	{
        GUI_TOUCH_Exec();
        rt_thread_delay(1);
	}
}


int cmd_gui_start(int argc, char** argv)
{
    rt_thread_t tid;
    
    /* this task can not be single */
    tid = rt_thread_find("gui_exe");
    if(tid != RT_NULL) return -1;
    
    /* create gui thread and run */
    tid = rt_thread_create("gui_exe", gui_thread_entry, RT_NULL, (1024*8), 0x27, 20);                                                      
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);		
    }
    
    tid = rt_thread_create("guit_exe", guit_thread_entry, RT_NULL, (1024), 0x26, 20);                                                      
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);		
    }

    //TOUCH_MainTask();
    return 0;
}



