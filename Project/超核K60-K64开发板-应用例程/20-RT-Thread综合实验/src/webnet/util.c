/*
 * File      : util.c
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#include "util.h"
#include <rtthread.h>

rt_inline int tohex(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

int str_begin_with(const char *s, const char *t)
{
	if (strncasecmp(s, t, strlen(t)) == 0) return 1;

	return 0;
}

int str_end_with(const char* s, const char* t)
{
	const char* se;
	register int s_len, t_len;

	s_len = strlen(s); t_len = strlen(t);

	if (s_len < t_len) return 0;

	se = s + s_len - t_len;
	if (strncasecmp(se, t, t_len) == 0) return 1;

	return 0;
}

char *str_decode_path(char *path)
{
    int x1;
    int x2;
    char *src = path;
    char *dst = path;
    char last = *path;

    if (last != '/')
        return RT_NULL;

	while (*++src)
	{
        if (*src == '%' &&
            (x1 = tohex(*(src + 1))) >= 0 &&
            (x2 = tohex(*(src + 2))) >= 0) 
		{
            src += 2;
            if ((*src = x1 * 16 + x2) == 0) break;
        }

        if (*src == '\\') *src = '/';
        if ((last != '.' && last != '/') || (*src != '.' && *src != '/'))
            *dst++ = last = *src;
    }

    *dst = 0;

    return path;
}

static const unsigned char base64_table[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char *str_base64_encode(const char* src)
{
	unsigned char *out, *pos;
	const unsigned char *end, *in;
	size_t olen;
	int len;

	len = strlen(src);
	olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / 72; /* line feeds */
	olen++; /* nul termination */

	out = (unsigned char*)rt_malloc(olen);
	if (out == RT_NULL) return RT_NULL;

	end = (const unsigned char*)src + len;
	in = (const unsigned char*)src;
	pos = out;
	while (end - in >= 3)
	{
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
	}

	if (end - in)
	{
        *pos++ = base64_table[in[0] >> 2];

        if (end - in == 1)
		{
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
		else
		{
            *pos++ = base64_table[((in[0] & 0x03) << 4) |
                                  (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
	}

	*pos = '\0';
	return (char*)out;
}

char* str_normalize_path(char* fullpath)
{
	char *dst0, *dst, *src;

	src = fullpath;
	dst = fullpath;
	
	dst0 = dst;
	while (1)
	{
		char c = *src;

		 if (c == '.')
		 {
			 if (!src[1]) src ++; /* '.' and ends */
			 else if (src[1] == '/')
			 {
				 /* './' case */
				 src += 2;

				 while ((*src == '/') && (*src != '\0')) src ++;
				 continue;
			 }
			 else if (src[1] == '.')
			 {
				 if (!src[2])
				 {
					/* '..' and ends case */
					 src += 2;
					 goto up_one;
				 }
				 else if (src[2] == '/')
				 {
					/* '../' case */
					 src += 3;

					 while ((*src == '/') && (*src != '\0')) src ++;
					 goto up_one;
				 }
			 }
		 }

		 /* copy up the next '/' and erase all '/' */
		 while ((c = *src++) != '\0' && c != '/') *dst ++ = c;

		 if (c == '/')
		 {
			 *dst ++ = '/';
			 while (c == '/') c = *src++;

			 src --;
		 }
		 else if (!c) break;

		 continue;

up_one:
		dst --;
		if (dst < dst0) return RT_NULL;
		while (dst0 < dst && dst[-1] != '/') dst --;
	}

	*dst = '\0';

	/* remove '/' in the end of path if exist */
	dst --;
	if ((dst != fullpath) && (*dst == '/')) *dst = '\0';

	return fullpath;
}

