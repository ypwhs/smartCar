/*
 * File      : webnet.h
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#ifndef __WEBNET_H__
#define __WEBNET_H__

/* webnet configuration */
#define WEBNET_USING_KEEPALIVE
#define WEBNET_USING_COOKIE
//#define WEBNET_USING_AUTH
//#define WEBNET_USING_CGI
//#define WEBNET_USING_ASP
//#define WEBNET_USING_INDEX
//#define WEBNET_USING_LOG
//#define WEBNET_USING_ALIAS

#define MAX_SERV                 32			/* Maximum number of webnet services. Don't need too many */

#define WEBNET_THREAD_NAME      "webnet"	/* webnet thread name */
#define WEBNET_PRIORITY         21			/* webnet thread priority */
#define WEBNET_THREAD_STACKSIZE 1024		/* webnet thread stack size */
#define WEBNET_PORT				80			/* webnet server listen port */
#define WEBNET_ROOT				"/SD/webnet"	/* webnet server root directory */
#define WEBNET_SERVER			"Server: webnet (RT-Thread)\r\n"
#define WEBNET_PATH_MAX			256			/* maxiaml path length in webnet */

void webnet_init(void);

/* Pre-declaration */
struct webnet_session;
/* webnet query item definitions */
struct webnet_query_item
{
	char* name;
	char* value;
};

/* WebNet APIs */
/* get mimetype according to URL */
const char* mime_get_type(const char* url);

/* add a APS variable */
void webnet_asp_add_var(const char* name, void (*handler)(struct webnet_session* session));
void webnet_cgi_set_root(const char* root);
void webnet_cgi_register(const char* name, void (*handler)(struct webnet_session* session));
void webnet_auth_set(const char* path, const char* username_password);

#endif

