/*
 * File      : request.c
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 * 2011-08-05     Bernard      fixed the query issue
 * 2011-08-18     Bernard      fixed chrome query issue
 */

#include "request.h"
#include <string.h>
#include <stdlib.h>
#include "util.h"

/**
 * parse a query
 */
static void _webnet_request_parse_query(struct webnet_request* request)
{
	char* ptr;
	rt_uint32_t index;

	if ((request->query == RT_NULL) || (*request->query == '\0')) return; /* no query */

	/* copy query */
	request->query = rt_strdup(request->query);
	
	/* get the query counter */
	ptr = request->query;
	request->query_counter = 1;
	while (*ptr)
	{
		if (*ptr == '&') 
		{
			while ((*ptr == '&') && (*ptr != '\0')) ptr ++;
			if (*ptr == '\0') break;

			request->query_counter ++;
		}
		ptr ++;
	}
	if (request->query_counter == 0) return; /* no query */

	/* allocate query item */
	request->query_items = (struct webnet_query_item*) rt_malloc (sizeof(struct webnet_query_item) 
		* request->query_counter);
	if (request->query_items == RT_NULL) 
	{
		request->result_code = 500;
		return;
	}

	ptr = request->query;
	for (index = 0; index < request->query_counter; index ++)
	{
		request->query_items[index].name = ptr;
		request->query_items[index].value = RT_NULL;

		/* get value or goto next item */
		while ((*ptr != '&') && (*ptr != '\0'))
		{
			/* get value */
			if (*ptr == '=')
			{
				*ptr = '\0'; ptr ++;
				request->query_items[index].value = ptr;
			}
			else ptr ++;
		}

		if (*ptr == '\0') break;
		if (*ptr == '&') 
		{
			/* make a item */
			*ptr = '\0'; ptr ++;
			while (*ptr == '&' && *ptr != '\0')ptr ++;
			if (*ptr == '\0') break;
		}
	}
}

/**
 * copy string from request and the field_copied set to TRUE
 */
static void _webnet_request_copy_str(struct webnet_request* request)
{
	if (request->path != RT_NULL) request->path = rt_strdup(request->path);
	if (request->host != RT_NULL) request->host = rt_strdup(request->host);
	if (request->cookie != RT_NULL) request->cookie = rt_strdup(request->cookie);
	if (request->user_agent != RT_NULL) request->user_agent = rt_strdup(request->user_agent);
	if (request->authorization != RT_NULL) request->authorization = rt_strdup(request->authorization);
	if (request->accept_language != RT_NULL) request->accept_language = rt_strdup(request->accept_language);
	if (request->referer != RT_NULL) request->referer = rt_strdup(request->referer);

	/* not copy query, which is copied in parse query function */
	/* if (request->query != RT_NULL) request->query = rt_strdup(request->query); */

	request->field_copied = RT_TRUE;
}

/**
 * to check whether a query on the http request.
 */
rt_bool_t webnet_request_has_query(struct webnet_request* request, char* name)
{
	rt_uint32_t index;

	for (index = 0; index < request->query_counter; index ++)
	{
		if (strncmp(request->query_items[index].name, name, strlen(name)) == 0)
			return RT_TRUE;
	}

	return RT_FALSE;
}

/**
 * get query value according to the name
 */
const char* webnet_request_get_query(struct webnet_request* request, char* name)
{
	rt_uint32_t index;

	for (index = 0; index < request->query_counter; index ++)
	{
		if (strncmp(request->query_items[index].name, name, strlen(name)) == 0)
			return request->query_items[index].value;
	}

	return RT_NULL;
}

/**
 * parse web request
 */
void webnet_request_parse(struct webnet_request* request, char* buffer, int length)
{
	char *ch;
	char *request_buffer;
	char *content_length;

	RT_ASSERT(request != RT_NULL);
	RT_ASSERT(request->session != RT_NULL);

	content_length = RT_NULL;
	request_buffer = buffer;
	/* web request begin with method */
	if (str_begin_with(request_buffer, "GET "))
	{
		request->method = WEBNET_GET; request_buffer += 4;
	}
	else if (str_begin_with(request_buffer, "PUT "))
	{
		request->method = WEBNET_PUT; request_buffer += 4;
	}
	else if (str_begin_with(request_buffer, "POST "))
	{
		request->method = WEBNET_POST; request_buffer += 5;
	}
	else if (str_begin_with(request_buffer, "HEADER "))
	{
		request->method = WEBNET_HEADER; request_buffer += 7;
	}
	else
	{
		/* Not implemented for other method */
		request->result_code = 501;
		return ;
	}

	/* get path */
	request->path = request_buffer;
	ch = strchr(request_buffer, ' ');
	if (ch == RT_NULL) 
	{
		/* Bad Request */
		request->result_code = 400;
		return;
	}
	*ch++ = '\0';
	request_buffer = ch;

	/* check path, whether there is a query */
	ch = strchr(request->path, '?');
	if (ch != RT_NULL)
	{
		*ch++ = '\0';
		while (*ch == ' ') ch ++;
		/* set query */
		request->query = ch;
		/* copy query and parse parameter */
		_webnet_request_parse_query(request);
	}

	/* check protocol */
	if (!str_begin_with(request_buffer, "HTTP/1"))
	{
		/* Not implemented, webnet does not support HTTP 0.9 protocol */
		request->result_code = 501;
		return;
	}

	ch = strstr(request_buffer, "\r\n");
	if (ch == RT_NULL)
	{
		/* Bad Request */
		request->result_code = 400;
		return;
	}
	*ch ++ = '\0'; *ch ++ = '\0';
	request_buffer = ch;

	for (;;)
	{
		if (str_begin_with(request_buffer, "\r\n"))
		{
			/* get content length */
			if (content_length != RT_NULL)
				request->content_length = atoi(content_length);

			if (request->method == WEBNET_POST) /* POST method */
			{
				request->query = request_buffer += 2;

				/* check content */
				if (*request->query != '\0') 
					_webnet_request_parse_query(request);
				else
				{
					struct webnet_session* session;
					int read_bytes;

					session = request->session;
					read_bytes = webnet_session_read(session, (char*)session->buffer, session->buffer_length);
					session->buffer_offset = read_bytes;
					if (read_bytes == request->content_length)
					{
						request->query = (char*)session->buffer;
						_webnet_request_parse_query(request);
					}
					else
					{
						request->query = RT_NULL;
					}
				}
			}

			/* end of http request */
			request->result_code = 200;

			/* made a string field copy */
			_webnet_request_copy_str(request);
			return;
		}

		if (*request_buffer == '\0')
		{
			/* end of http request */
			request->result_code = 200;

			/* made a string field copy */
			_webnet_request_copy_str(request);
			return;
		}

		if (str_begin_with(request_buffer, "Host:"))
		{
			/* get host */
			request_buffer += 5;
			while (*request_buffer == ' ') request_buffer ++;
			request->host = request_buffer;
		}
		else if (str_begin_with(request_buffer, "User-Agent:"))
		{
			/* get user agent */
			request_buffer += 11;
			while (*request_buffer == ' ') request_buffer ++;
			request->user_agent = request_buffer;
		}
		else if (str_begin_with(request_buffer, "Accept-Language:"))
		{
			/* get accept language */
			request_buffer += 16;
			while (*request_buffer == ' ') request_buffer ++;
			request->accept_language = request_buffer;
		}
		else if (str_begin_with(request_buffer, "Content-Length:"))
		{
			/* get content length */
			request_buffer += 15;
			while (*request_buffer == ' ') request_buffer ++;
			content_length = request_buffer;
		}
		else if (str_begin_with(request_buffer, "Referer:"))
		{
			/* get referer */
			request_buffer += 8;
			while (*request_buffer == ' ') request_buffer ++;
			request->referer = request_buffer;
		}
#ifdef WEBNET_USING_KEEPALIVE
		else if (str_begin_with(request_buffer, "Connection:"))
		{
			/* set default connection to keep alive */
			request->connection = WEBNET_CONN_KEEPALIVE;

			/* get connection */
			request_buffer += 11;
			while (*request_buffer == ' ') request_buffer ++;

			if (str_begin_with(request_buffer, "close"))
				request->connection = WEBNET_CONN_CLOSE;
			else if (str_begin_with(request_buffer, "Keep-Alive"))
				request->connection = WEBNET_CONN_KEEPALIVE;
		}
#endif
#ifdef WEBNET_USING_COOKIE
		else if (str_begin_with(request_buffer, "Cookie:"))
		{
			/* get cookie */
			request_buffer += 7;
			while (*request_buffer == ' ') request_buffer ++;
			request->cookie = request_buffer;
		}
#endif
#ifdef WEBNET_USING_AUTH
		else if (str_begin_with(request_buffer, "Authorization: Basic"))
		{
			/* get authorization */
			request_buffer += 20;
			while (*request_buffer == ' ') request_buffer ++;
			request->authorization = request_buffer;
		}
#endif

		ch = strstr(request_buffer, "\r\n");
		if (ch == RT_NULL) 
		{
			/* Bad Request */
			request->result_code = 400;
			return;
		}
		/* terminal field */
		*ch ++ = '\0';
		*ch ++ = '\0';

		request_buffer = ch;
	}
}

struct webnet_request* webnet_request_create()
{
	struct webnet_request* request;

	request = (struct webnet_request*) rt_malloc (sizeof(struct webnet_request));
	if (request != RT_NULL)
	{
		rt_memset(request, 0, sizeof(struct webnet_request));
		request->field_copied = RT_FALSE;
	}

	return request;
}

void webnet_request_destory(struct webnet_request* request)
{
	if (request != RT_NULL)
	{
		if (request->field_copied == RT_TRUE)
		{
			if (request->path != RT_NULL) rt_free(request->path);
			if (request->host != RT_NULL) rt_free(request->host);
			if (request->cookie != RT_NULL) rt_free(request->cookie);
			if (request->user_agent != RT_NULL) rt_free(request->user_agent);
			if (request->authorization != RT_NULL) rt_free(request->authorization);
			if (request->accept_language != RT_NULL) rt_free(request->accept_language);
			if (request->referer != RT_NULL) rt_free(request->referer);
			if (request->query != RT_NULL) rt_free(request->query);
			if (request->query_items != RT_NULL) rt_free(request->query_items);
		}

		/* free request memory block */
		rt_free(request);
	}
}

