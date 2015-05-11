/*
 * File      : mimetype.c
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#include <string.h>
#include <rtthread.h>
#include "util.h"

struct webnet_mime_entry
{
    const char* name;
	const char* type;
};

static const struct webnet_mime_entry mime_tables[] =
{
    { "html",	"text/html" },
    { "htm",	"text/html" },
    { "css",	"text/css" },
    { "pdf",	"application/pdf" },
    { "gif",	"image/gif" },
    { "png",	"image/png" },
    { "jpeg",	"image/jpeg" },
    { "jpg",	"image/jpeg" },
    { "avi",	"video/x-msvideo" },
    { "mp3",	"audio/mpeg" },
    { "ogg",	"audio/x-oggvorbis" },
    { "wav",	"audio/x-wav" },
    { "class",	"application/octet-stream" },
    { "js",		"application/x-javascript" },
    { "tar",	"application/x-tar" },
    { "zip",	"application/zip" },
    { "xml",	"text/xml" },
    { RT_NULL,  RT_NULL }
};

/**
 * get mime type according to URL
 */
const char* mime_get_type(const char* url)
{
	rt_uint32_t index;

	index = 0;
	if (url == RT_NULL) return mime_tables[0].type;

	while (mime_tables[index].name != RT_NULL)
	{
		if (str_end_with(url, mime_tables[index].name))
			return mime_tables[index].type;

		index ++;
	}

	/* return text/html as default */
	return mime_tables[0].type;
}

