/*
 * File      : webnet.c
 * This file is part of RT-Thread RTOS/WebNet Server
 * COPYRIGHT (C) 2011, Shanghai Real-Thread Technology Co., Ltd
 *
 * All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-08-02     Bernard      the first version
 */

#include <rtthread.h>
#include "lwip/sockets.h"

#include "webnet.h"
#include "session.h"
#include "module.h"

/**
 * webnet thread entry
 */
static void webnet_thread(void *parameter)
{
    int listenfd;
    fd_set readset, tempfds;
    fd_set writeset;
    int i, maxfdp1;
    struct sockaddr_in webnet_saddr;

    /* First acquire our socket for listening for connections */
    listenfd = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    LWIP_ASSERT("webnet_thread(): Socket create failed.", listenfd >= 0);

    memset(&webnet_saddr, 0, sizeof(webnet_saddr));
    webnet_saddr.sin_family = AF_INET;
    webnet_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    webnet_saddr.sin_port = htons(WEBNET_PORT);     /* webnet server port */

    if (lwip_bind(listenfd, (struct sockaddr *) &webnet_saddr, sizeof(webnet_saddr)) == -1)
        LWIP_ASSERT("webnet_thread(): socket bind failed.", 0);

    /* Put socket into listening mode */
    if (lwip_listen(listenfd, MAX_SERV) == -1)
        LWIP_ASSERT("webnet_thread(): listen failed.", 0);

	/* initalize module (no session at present) */
	webnet_module_handle_event(RT_NULL, WEBNET_EVENT_INIT);

    /* Wait forever for network input: This could be connections or data */
    for (;;)
    {
        /* Determine what sockets need to be in readset */
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_SET(listenfd, &readset);

		/* set fds in each sessions */
		maxfdp1 = webnet_sessions_set_fds(&readset, &writeset);
		if (maxfdp1 < listenfd + 1) maxfdp1 = listenfd + 1;

		/* use temporary fd set in select */
		tempfds = readset;
        /* Wait for data or a new connection */
        i = lwip_select(maxfdp1, &tempfds, 0, 0, 0);
        if (i == 0) continue;

        /* At least one descriptor is ready */
        if (FD_ISSET(listenfd, &tempfds))
        {
        	struct webnet_session* accept_session;
            /* We have a new connection request */
			accept_session = webnet_session_create(listenfd);
			if (accept_session == RT_NULL)
			{
                /* create session failed, just accept and then close */
                int sock;
                struct sockaddr cliaddr;
                socklen_t clilen;

				clilen = sizeof(struct sockaddr_in);
                sock = lwip_accept(listenfd, &cliaddr, &clilen);
                if (sock >= 0) lwip_close(sock);
            }
			else
			{
				/* add read fdset */
				FD_SET(accept_session->socket, &readset);
			}
        }

		webnet_sessions_handle_fds(&tempfds, &writeset);
    }
}

void webnet_init(void)
{
	rt_thread_t tid;

	tid = rt_thread_create(WEBNET_THREAD_NAME,
		webnet_thread, RT_NULL,
		WEBNET_THREAD_STACKSIZE, WEBNET_PRIORITY, 5);

	if (tid != RT_NULL) rt_thread_startup(tid);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(webnet_init, start web server);
#endif

