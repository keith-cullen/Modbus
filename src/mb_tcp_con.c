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

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "mb_tcp_con.h"
#include "mb_log.h"

static int mb_tcp_con_rx_complete(mb_tcp_con_t *con)
{
    uint16_t len = 0;

    if (con->rx_end < MB_TCP_ADU_LEN_OFF + sizeof(uint16_t))
        return 0;
    len = ntohs(*(uint16_t *)(con->rx_buf + MB_TCP_ADU_LEN_OFF));
    if (MB_TCP_ADU_LEN_OFF + sizeof(uint16_t) + len > con->rx_end)
        return 0;
    return 1;
}

int mb_tcp_con_set_non_blocking(int sd)
{
    int flags = 0;
    int ret = 0;

    flags = fcntl(sd, F_GETFL, 0);
    if (flags < 0)
    {
        return -errno;
    }
    ret = fcntl(sd, F_SETFL, flags | O_NONBLOCK);
    if (ret < 0)
    {
        return -errno;
    }
    return 0;
}

void mb_tcp_con_create(mb_tcp_con_t *con, int index)
{
    memset(con, 0, sizeof(mb_tcp_con_t));
    con->index = index;
    con->sd = MB_TCP_CON_SOCKET_CLOSED;
}

void mb_tcp_con_destroy(mb_tcp_con_t *con)
{
    if (con->sd != MB_TCP_CON_SOCKET_CLOSED)
    {
        mb_tcp_con_close(con);
    }
    memset(con, 0, sizeof(mb_tcp_con_t));
}

void mb_tcp_con_open(mb_tcp_con_t *con, int sd, struct sockaddr_in *sin)
{
    con->sd = sd;
    con->last_use = time(NULL);
    memcpy(&con->sin, sin, sizeof(struct sockaddr_in));
    mb_log_info("[%d] connection opened", con->index);
}

void mb_tcp_con_close(mb_tcp_con_t *con)
{
    close(con->sd);
    con->sd = MB_TCP_CON_SOCKET_CLOSED;
    mb_log_info("[%d] connection closed locally", con->index);
}

ssize_t mb_tcp_con_send(mb_tcp_con_t *con, char *buf, size_t len)
{
    ssize_t num = 0;

    con->last_use = time(NULL);
    num = send(con->sd, buf, len, 0);
    if (num == 0)
    {
        mb_log_info("[%d] connection closed remotely", con->index);
        return 0;
    }
    else if (num < 0)
    {
        return -errno;
    }
    mb_log_debug("[%d] sent %d bytes", con->index, num);
    return num;
}

ssize_t mb_tcp_con_recv(mb_tcp_con_t *con)
{
    ssize_t num = 0;

    con->last_use = time(NULL);
    num = recv(con->sd, con->rx_buf + con->rx_end, sizeof(con->rx_buf) - con->rx_end, 0);
    if (num == 0)
    {
        mb_log_info("[%d] connection closed remotely", con->index);
        return 0;
    }
    else if (num < 0)
    {
        return -errno;
    }
    mb_log_debug("[%d] received %d bytes", con->index, num);
    con->rx_end += num;
    if (!mb_tcp_con_rx_complete(con))
    {
        mb_log_debug("[%d] buffering received message fragment", con->index);
        return -EAGAIN;  /* need to receive more data before processing a complete message */
    }
    mb_log_debug("[%d] received complete message", con->index);
    return num;
}

void mb_tcp_con_consume(mb_tcp_con_t *con, size_t num)
{
    /* shift receive buffer left in case of back-to-back messages */
    memmove(con->rx_buf, con->rx_buf + num, con->rx_end - num);
    con->rx_end -= num;
    if (con->rx_end > 0)
    {
        mb_log_debug("[%d] buffering received back-to-back message", con->index);
    }
}
