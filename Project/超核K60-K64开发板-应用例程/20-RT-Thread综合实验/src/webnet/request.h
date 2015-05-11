/*
 * File      : request.h
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 * 2011-08-18     Bernard      added content length field
 */
#ifndef __WEBNET_REQUEST_H__
#define __WEBNET_REQUEST_H__

#include <rtthread.h>
#include "session.h"

/* http request method */
enum webnet_method
{
	WEBNET_GET,
	WEBNET_PUT,
	WEBNET_POST,
	WEBNET_HEADER,
};

/* http connection status */
enum webnet_connection
{
	WEBNET_CONN_CLOSE,
	WEBNET_CONN_KEEPALIVE,
};

/* http request structure */
struct webnet_request
{
	enum webnet_method method;
	int result_code;
	int content_length;

	/* the corresponding session */
	struct webnet_session *session;

	/* path and authorization */
	char* path;
	char* host;
	char* authorization;

	char* user_agent;
	char* accept_language;
	char* cookie;
	char* referer;

	/* query information */
	char* query;
	struct webnet_query_item* query_items;
	rt_uint16_t query_counter;

	enum webnet_connection connection;

	/* whether the string filed is copied */
	rt_bool_t field_copied;
};

struct webnet_request* webnet_request_create(void);
void webnet_request_destory(struct webnet_request* request);

void webnet_request_parse(struct webnet_request* request, char* buffer, int length);

rt_bool_t webnet_request_has_query(struct webnet_request* request, char* name);
const char* webnet_request_get_query(struct webnet_request* request, char* name);

#endif
