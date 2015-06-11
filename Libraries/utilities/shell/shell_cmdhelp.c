/**
  ******************************************************************************
  * @file    shell_cmdhelp.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#include "shell.h"

extern cmd_tbl_t *_syscall_table_begin;
extern cmd_tbl_t *_syscall_table_end;
              
 /*!
 * @brief help command.
 */
int DoHelp(int argc, char * const argv[])
{
    uint8_t i;
    const cmd_tbl_t * cmdp;
    if (argc == 1)
    {
        i = 0;
        for(cmdp = _syscall_table_begin; cmdp < _syscall_table_end; cmdp++)
        {
            shell_printf("%-*s(%d)- %s\r\n", 8, cmdp->name, i++, cmdp->usage);
        }
    }
    if (argc == 2)
    {
        i = 0;
        for(cmdp = _syscall_table_begin; cmdp < _syscall_table_end; cmdp++)
        {
            if (!strcmp(argv[1], cmdp->name))
            {
                if (cmdp->help != NULL)
                {
                    shell_printf("%-*s- %s\r\n", 8, cmdp->name, cmdp->help);
                    return CMD_RET_SUCCESS;
                }
                else
                {
                    shell_printf ("- No additional help available.\r\n");
                    return CMD_RET_SUCCESS;
                }
            }
        }
        shell_printf ("- No command available.\r\n");
    }
    return CMD_RET_SUCCESS;
}

 /*!
 * @brief help command auto complete function.
 */
static int DoHelpComplete(int argc, char * const argv[], char last_char, int maxv, char * cmdv[])
{
    uint8_t str_len;
    uint8_t found = 0;
    str_len = strlen(argv[argc-1]);
    const cmd_tbl_t * cmdp;
    if(argc == 2)
    {
        for(cmdp = _syscall_table_begin; cmdp < _syscall_table_end; cmdp++)
        {
            if (!strncmp(argv[argc-1], cmdp->name, str_len))
            {
                cmdv[found] = cmdp->name;
                found++;
            }  
        }
    }
    return found;
}

const cmd_tbl_t __fsym_help SECTION("FSymTab")=            
{
    "help",                            
    8,                                                      
    0,                                                      
    DoHelp,                                                   
    "print command description/usage",                           
    "\r\n"
            "	- print brief description of all commands\r\n"
            "help <command>\r\n"
            "	- print detailed usage of 'command'\r\n",                                             
    DoHelpComplete,                                                      
};
