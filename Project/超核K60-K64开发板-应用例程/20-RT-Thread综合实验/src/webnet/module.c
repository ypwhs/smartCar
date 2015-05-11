/*
 * File      : module.c
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#include "module.h"
#include <string.h>
#include <dfs_posix.h>

#include "util.h"
#include "mimetype.h"

static int _webnet_module_system_init(struct webnet_session* session, int event)
{
#ifdef WEBNET_USING_LOG
	webnet_module_log(session, event);
#endif

#ifdef WEBNET_USING_SSL
	webnet_module_ssl(session, event);
#endif

#ifdef WEBNET_USING_CGI
	webnet_module_cgi(session, event);
#endif

	return WEBNET_MODULE_CONTINUE;
}

static int _webnet_module_system_uri_physical(struct webnet_session* session, int event)
{
	int result;
	result = WEBNET_MODULE_CONTINUE;
	
#ifdef WEBNET_USING_LOG
		webnet_module_log(session, event);
#endif

#ifdef WEBNET_USING_ALIAS
	result = webnet_module_alias(session, event);
	if (result == WEBNET_MODULE_FINISHED) return result;
#endif

#ifdef WEBNET_USING_AUTH
	result = webnet_module_auth(session, event);
	if (result == WEBNET_MODULE_FINISHED) return result;
#endif

#ifdef WEBNET_USING_CGI
	result = webnet_module_cgi(session, event);
	if (result == WEBNET_MODULE_FINISHED) return result;
#endif

	return result;
}

/* send a file to http client */
int webnet_module_system_dofile(struct webnet_session* session)
{
	int fd;	/* file descriptor */
	const char* mimetype;
	rt_size_t file_length;
	struct webnet_request* request;

	RT_ASSERT(session != RT_NULL);
	request = session->request;
	RT_ASSERT(request != RT_NULL);
	
	/* get mime type */
	mimetype = mime_get_type(request->path);
	fd = open(request->path, O_RDONLY, 0);
	if (fd < 0) 
	{
		request->result_code = 404;
		return WEBNET_MODULE_FINISHED;
	}

	/* get file size */
	file_length = lseek(fd, 0, SEEK_END);
	/* seek to beginning of file */
	lseek(fd, 0, SEEK_SET);
	
	/* send file to remote */
	request->result_code = 200;
	webnet_session_set_header(session, mimetype, request->result_code, "OK", file_length);
	if (file_length <= 0)
	{
		close(fd);
		return WEBNET_MODULE_FINISHED;
	}

	if (request->method != WEBNET_HEADER)
	{
		rt_size_t size, readbytes;
		while (file_length)
		{
            if (file_length > WEBNET_SESSION_BUFSZ)
                size = (rt_size_t) WEBNET_SESSION_BUFSZ;
			else
				size = file_length;

            readbytes = read(fd, session->buffer, size);
            if (readbytes <= 0)
                /* no more data */
                break;
			
            if (webnet_session_write(session, session->buffer, readbytes) == 0)
                break;

            file_length -= (long) readbytes;
        }
	}

	/* close file */
	close(fd);

	return WEBNET_MODULE_FINISHED;
}

static int _webnet_module_system_uri_post(struct webnet_session* session, int event)
{
	int result;
	result = WEBNET_MODULE_CONTINUE;

#ifdef WEBNET_USING_LOG
	webnet_module_log(session, event);
#endif

#ifdef WEBNET_USING_ASP
	result = webnet_module_asp(session, event);
	if (result == WEBNET_MODULE_FINISHED) return result;
#endif

#ifdef WEBNET_USING_INDEX
	result = webnet_module_dirindex(session, event);
	if (result == WEBNET_MODULE_FINISHED) return result;
#endif

	result = webnet_module_system_dofile(session);
	if (result == WEBNET_MODULE_FINISHED) return result;

	return WEBNET_MODULE_CONTINUE;
}

static int _webnet_module_system_response_header(struct webnet_session* session, int event)
{
	int result;
	result = WEBNET_MODULE_CONTINUE;

	return result;
}

static int _webnet_module_system_response_file(struct webnet_session* session, int event)
{
	int result;
	result = WEBNET_MODULE_CONTINUE;

	return result;
}

int webnet_module_handle_event(struct webnet_session* session, int event)
{
	switch (event)
	{
	case WEBNET_EVENT_INIT:
		return _webnet_module_system_init(session, event);
	case WEBNET_EVENT_URI_PHYSICAL:
		return _webnet_module_system_uri_physical(session, event);
	case WEBNET_EVENT_URI_POST:
		return _webnet_module_system_uri_post(session, event);
	case WEBNET_EVENT_RSP_HEADER:
		return _webnet_module_system_response_header(session, event);
	case WEBNET_EVENT_RSP_FILE:
		return _webnet_module_system_response_file(session, event);
	default:
		RT_ASSERT(0);
		break;
	}

	return WEBNET_MODULE_CONTINUE;
}

/* default index file */
static const char* default_files[] = 
{
	"",
	"/index.html",
	"/index.htm",
	RT_NULL
};

/**
 * handle uri
 * there are two phases on uri handling:
 * - map url to physical
 * - url handling
 */
int webnet_module_handle_uri(struct webnet_session* session)
{
	int result, fd;
	char* full_path;
	rt_uint32_t index;
	struct webnet_request* request;
	
	RT_ASSERT(session != RT_NULL);
	/* get request */
	request = session->request;
	RT_ASSERT(request != RT_NULL);

	/* map uri to physical */
	result = webnet_module_handle_event(session, WEBNET_EVENT_URI_PHYSICAL);
	if (result == WEBNET_MODULE_FINISHED) return result;

	/* made a full physical path */
	full_path = (char*) rt_malloc (WEBNET_PATH_MAX);
	RT_ASSERT(full_path != RT_NULL);

	index = 0;
	while (default_files[index] != RT_NULL)
	{
		/* made a full path */
		rt_snprintf(full_path, WEBNET_PATH_MAX, "%s/%s%s", 
			WEBNET_ROOT, request->path, default_files[index]);
		/* normalize path */
		str_normalize_path(full_path);

		fd = open(full_path, O_RDONLY, 0);
		if (fd >= 0)
		{
			/* close file descriptor */
			close(fd);
			break;
		}

		index ++;
	}
	/* no this file */
	if (default_files[index] == RT_NULL)
	{
		/* use old full path */
		rt_snprintf(full_path, WEBNET_PATH_MAX, "%s/%s", WEBNET_ROOT, request->path);
		/* normalize path */
		str_normalize_path(full_path);
	}

	/* mark path as full physical path */
	rt_free(request->path);
	request->path = full_path;

	/* check uri valid */
	if (!str_begin_with(request->path, WEBNET_ROOT))
	{
		/* not found */
		request->result_code = 404;
		return WEBNET_MODULE_FINISHED;
	}

	/* uri post handle */
	return webnet_module_handle_event(session, WEBNET_EVENT_URI_POST);
}
