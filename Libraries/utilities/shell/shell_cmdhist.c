/**
  ******************************************************************************
  * @file    shell_cmdhist.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#include "shell.h"

#ifdef SHELL_CONFIG_USE_HIST
static int DoHist(int argc, char * const argv[])
{
    uint8_t num, cur_index;
    uint8_t i = 0;
    char ** pplist = shell_get_hist_data_list(&num, &cur_index);
    shell_printf("history:\r\n");
    if (cur_index >= num)
    {
        return CMD_RET_SUCCESS;
    }
    for (i = cur_index; i < num; i++)
    {
        if((pplist[i] != NULL) && (pplist[i][0] != '\0'))
        {
            shell_printf("(%d) %s\r\n", i - cur_index, pplist[i]); 
        }       
    }
    for (i = 0; i < cur_index; i++)
    {
        if((pplist[i] != NULL) && (pplist[i][0] != '\0'))
        {
            shell_printf("(%d) %s\r\n", num - cur_index + i, pplist[i]);  
        }
    }
    return CMD_RET_SUCCESS;
}

const cmd_tbl_t __fsym_hist SECTION("FSymTab")= 
{
    .name = "history",
    .maxargs = 2,
    .repeatable = 1,
    .cmd = DoHist,
    .usage = "print history",
    .complete = NULL,
    .help = NULL,
};
#endif

