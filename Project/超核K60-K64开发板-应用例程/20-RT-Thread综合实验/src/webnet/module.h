/*
 * File      : module.h
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 * 2011-08-06     Bernard      add webnet_alias_set declaration
 */

#ifndef __WEBNET_MODULE_H__
#define __WEBNET_MODULE_H__

#include "session.h"

/* initialization event */
#define WEBNET_EVENT_INIT			(1 << 0)
/* map uri request to physical url */
#define WEBNET_EVENT_URI_PHYSICAL	(1 << 1)
/* uri request */
#define WEBNET_EVENT_URI_POST		(1 << 2)
/* header of response */
#define WEBNET_EVENT_RSP_HEADER		(1 << 3)
/* file content of response */
#define WEBNET_EVENT_RSP_FILE		(1 << 4)

/* continue other modules */
#define WEBNET_MODULE_CONTINUE		0
/* this session is finished */
#define WEBNET_MODULE_FINISHED		1

int webnet_module_handle_event(struct webnet_session* session, int event);
int webnet_module_system_dofile(struct webnet_session* session);
int webnet_module_handle_uri(struct webnet_session* session);

/* module APIs */
void webnet_asp_add_var(const char* name, void (*handler)(struct webnet_session* session));
void webnet_auth_set(const char* path, const char* username_password);
void webnet_cgi_set_root(const char* root);
void webnet_cgi_register(const char* name, void (*handler)(struct webnet_session* session));
void webnet_alias_set(char* old_path, char* new_path);

/* module function pre-declaration */
int webnet_module_alias(struct webnet_session* sesion, int event);
int webnet_module_auth(struct webnet_session* session, int event);
int webnet_module_asp(struct webnet_session* session, int event);
int webnet_module_cgi(struct webnet_session* session, int event);
int webnet_module_dirindex(struct webnet_session* session, int event);
int webnet_module_log(struct webnet_session* session, int event);
int webnet_module_lua(struct webnet_session* sesion, int event);
int webnet_module_ssl(struct webnet_session* sesion, int event);

#endif

