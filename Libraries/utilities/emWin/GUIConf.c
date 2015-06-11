

#include "GUI.h"
#include "DIALOG.h"
#include <rtthread.h>
#include "sram.h"

#define GUI_NUMBYTES  (1024*256)
#define GUI_BLOCKSIZE 4

static void GUI_Log(const char *s)
{
    rt_kprintf("GUI error:%s\r\n", s);
}

static void _EnableSkinning(int OnOff) {
  if (OnOff) {
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    BUTTON_SetDefaultSkin  (BUTTON_SKIN_FLEX);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  } else {
    FRAMEWIN_SetDefaultSkinClassic();
    BUTTON_SetDefaultSkinClassic();
    DROPDOWN_SetDefaultSkinClassic();
    HEADER_SetDefaultSkinClassic();
    SCROLLBAR_SetDefaultSkinClassic();
  }
}

void GUI_X_Config(void)
{
    //static U32 aMemory[GUI_NUMBYTES / 4];
   // U32 * aMemory = (U32*)rt_malloc(GUI_NUMBYTES);
    U32 *aMemory = (void*)(SRAM_ADDRESS_BASE);
    if(aMemory == RT_NULL)
    {
        rt_kprintf("GUI:not memory\r\n");
        return;
    }
    GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
    GUI_SetOnErrorFunc(GUI_Log);
    GUITASK_SetMaxTask(5);
    GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
    GUI_SetDefaultFont(GUI_FONT_6X8);
    _EnableSkinning(1);
}

/*************************** End of file ****************************/
