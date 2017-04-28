/*
 * Copyright (c) 2016 Keith Cullen.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MB_TCP_CLIENT_H
#define MB_TCP_CLIENT_H

#include <stdint.h>
#include <time.h>
#include <netinet/in.h>
#include "mb_tcp_con.h"
#include "mb_ip_auth.h"
#include "mb_tcp_adu.h"

#define MB_TCP_CLIENT_MAX_CON        4
#define MB_TCP_CLIENT_SOCKET_CLOSED  0
#define MB_TCP_CLIENT_UNIT_ID        0xff  /* unit id used in client requests */

typedef struct
{
    mb_ip_auth_list_t auth;
    mb_tcp_con_t con[MB_TCP_CLIENT_MAX_CON];
    struct timeval timeout;
}
mb_tcp_client_t;

void mb_tcp_client_create(mb_tcp_client_t *client, struct timeval timeout);
void mb_tcp_client_destroy(mb_tcp_client_t *client);
int mb_tcp_client_authorise_addr(mb_tcp_client_t *client, const char *str);
int mb_tcp_client_exchange(mb_tcp_client_t *client, const char *host, int16_t port, mb_tcp_adu_t *req, mb_tcp_adu_t *resp);

#endif
