/*
 * File      : util.h
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#ifndef __WEBNET_UTIL_H__
#define __WEBNET_UTIL_H__

#include <string.h>

int str_begin_with(const char *s, const char *t);
int str_end_with(const char* s, const char* t);
char *str_decode_path(char *path);
char *str_base64_encode(const char* src);
char* str_normalize_path(char* fullpath);

#endif

