#include <rtthread.h>
#include "common.h"
 
#ifdef FINSH_USING_MSH
#include <finsh.h>

int cmd_reset(int argc, char** argv)
{
    NVIC_SystemReset();
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_reset, __cmd_reset, Reset.);

#endif





