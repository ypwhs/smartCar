/**
  ******************************************************************************
  * @file    shell.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __shell_H__
#define __shell_H__

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "shell_config.h"


#ifndef SHELL_CB_SIZE
#define SHELL_CB_SIZE   (128)
#endif

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS  (8)
#endif

#ifndef HIST_MAX
#define HIST_MAX        (10)
#endif

#ifdef __CC_ARM                         /* ARM Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  @ x
#endif

typedef struct
{
    uint8_t (*sh_getc)(void);
    void    (*sh_putc)(uint8_t ch);
}shell_io_install_t;

#ifdef SHELL_CONFIG_USE_STDIO
#define shell_printf(fmt,args...)	printf (fmt ,##args)
#endif

typedef struct  
{
    char		*name;		/* Command Name			*/
    uint8_t		maxargs;	/* maximum number of arguments	*/
    uint8_t		repeatable;	/* autorepeat allowed?		*/
    int         (*cmd)(int argc, char * const argv[]);  /* Implementation function	*/
    char		*usage;		/* Usage message	(short)	*/
    char		*help;		/* Help  message	(long)	*/
    int         (*complete)(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]); 	/* do auto completion on the arguments */
}cmd_tbl_t;


#define SHELL_EXPORT_CMD(func, cmd, usage)                      \
    const char __fsym_##cmd##_name[] = #cmd;                    \
    const char __fsym_##cmd##_usage[] = #usage;                 \
    const cmd_tbl_t __fsym_##cmd SECTION("FSymTab")=            \
    {                                                           \
        (char *)__fsym_##cmd##_name,                            \
        8,                                                      \
        0,                                                      \
        func,                                                   \
        (char *)__fsym_##cmd##_usage,                           \
        0,                                                      \
        0,                                                      \
    };
    
#define SHELL_EXPORT_CMD_EX(func, complete, cmd, usage, help)   \
    const char __fsym_##cmd##_name[] = #cmd;                    \
    const char __fsym_##cmd##_usage[] = #usage;                 \
    const char __fsym_##cmd##_help[] = #help;                   \
    const cmd_tbl_t __fsym_##cmd SECTION("FSymTab")=            \
    {                                                           \
        (char *)__fsym_##cmd##_name,                            \
        8,                                                      \
        0,                                                      \
        func,                                                   \
        (char *)__fsym_##cmd##_usage,                           \
        (char *)__fsym_##cmd##_help,                            \
        complete,                                               \
    }; 
    

typedef enum
{
    CMD_RET_SUCCESS = 0,    /* 0 = Success */
    CMD_RET_FAILURE = 1,    /* 1 = Failure */
    CMD_RET_USAGE = -1,     /* Failure, please report 'usage' error */
}command_ret_t;

//!< API funcions
void shell_init(void);
void shell_main_loop(char * prompt);
//!< internal functions
int shell_printf(const char * format,...);
const cmd_tbl_t *shell_find_command (const char * cmd);
void shell_io_install(shell_io_install_t * IOInstallStruct);
char ** shell_get_hist_data_list(uint8_t * num, uint8_t * cur_index);


#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
 
 
