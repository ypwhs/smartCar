
#include "gui_appdef.h"

rt_mq_t guimq;

void gui_thread_entry(void* parameter)
{
    guimq = rt_mq_create("gui_mq", sizeof(gui_msg_t), 6, RT_IPC_FLAG_FIFO);
    gui_msg_t msg;
    
    GUI_Init();
    GUI_DispString("gui system actived!\r\n");
    GUI_Delay(100);

    //GUI_CURSOR_Show();
    mkdir("/SF/SYS", 0);
    GUI_AppAutoCalibration("/SF/SYS/TDATA");
    GUI_CreateDesktopDialog();
	while(1)
	{
        if(rt_mq_recv(guimq, &msg, sizeof(gui_msg_t), 1) == RT_EOK)
        {
            if(msg.exec != RT_NULL)
            {
                msg.exec(msg.parameter);
            } 
        }
        GUI_Exec();
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


int ui_startup(int argc, char** argv)
{
    rt_thread_t tid;
    
    /* this task can not be single */
    tid = rt_thread_find("gui_exe");
    if(tid != RT_NULL) return -1;
    
    /* create gui thread and run */
    tid = rt_thread_create("gui_exe", gui_thread_entry, RT_NULL, (1024*4), 0x13, 20);                                
    rt_thread_startup(tid);

    tid = rt_thread_create("guit_exe", guit_thread_entry, RT_NULL, (512), 0x14, 20);                                                      
    rt_thread_startup(tid);	

    return 0;
}


#ifdef FINSH_USING_MSH
#include "finsh.h"

MSH_CMD_EXPORT(ui_startup, start GUI app.);
#endif

