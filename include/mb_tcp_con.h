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

#ifndef MB_TCP_CON_H
#define MB_TCP_CON_H

#include <stddef.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "mb_tcp_adu.h"

#define MB_TCP_CON_SOCKET_CLOSED  0

#define mb_tcp_con_is_active(con)  ((con)->sd != MB_TCP_CON_SOCKET_CLOSED)

typedef struct
{
    int index;
    int sd;
    time_t last_use;
    struct sockaddr_in sin;
    char rx_buf[MB_TCP_ADU_MAX_LEN];
    size_t rx_end;
}
mb_tcp_con_t;

int mb_tcp_con_set_non_blocking(int sd);
void mb_tcp_con_create(mb_tcp_con_t *con, int index);
void mb_tcp_con_destroy(mb_tcp_con_t *con);
void mb_tcp_con_open(mb_tcp_con_t *con, int sd, struct sockaddr_in *sin);
void mb_tcp_con_close(mb_tcp_con_t *con);
ssize_t mb_tcp_con_send(mb_tcp_con_t *con, char *buf, size_t len);
ssize_t mb_tcp_con_recv(mb_tcp_con_t *con);
void mb_tcp_con_consume(mb_tcp_con_t *con, size_t num);

#endif
