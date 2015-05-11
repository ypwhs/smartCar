/*
 * File      : writespeed.c
 * This file is part of RT-TestCase in RT-Thread RTOS
 * COPYRIGHT (C) 2010, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-02-10     Bernard      first version
 */
#include <rtthread.h>
#include <dfs_posix.h>
#include "GUI.H"

    
static void _WriteByte2File(U8 Data, void *p)
{
    static rt_uint32_t cnt;
    cnt++;
    if(!(cnt%3000)) rt_kprintf(">");
    write(*(int*)p, &Data, 1);
}

#ifdef FINSH_USING_MSH
#include "finsh.h"

void ui_screenshot(int argc, char** argv)
{
    int hFile;
    if(argc == 1)
    {
        hFile = open("/SD/SYS/PIC/SCREEN.BMP", O_WRONLY | O_CREAT | O_TRUNC, 0);
    }
    else
    {
        hFile = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0);
    }
    
    if (hFile < 0)
    {
        rt_kprintf("open file:%s failed\n", argv[1]);
        return;
    }
    GUI_BMP_Serialize(_WriteByte2File, &hFile);
    close(hFile);
    rt_kprintf("\r\n");
}


MSH_CMD_EXPORT(ui_screenshot, <cmd> /SD/cut.bmp);
#endif

