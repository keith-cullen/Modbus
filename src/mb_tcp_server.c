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
#include <arpa/inet.h>
#include <sys/select.h>
#include "mb_tcp_server.h"
#include "mb_log.h"

#define MB_TCP_SERVER_BUF_LEN  128
#define MB_TCP_SERVER_BACKLOG  10

static ssize_t mb_tcp_server_send_resp(mb_tcp_server_t *server, int index, mb_tcp_adu_t *resp)
{
    ssize_t num = 0;
    char msg_buf[256] = {0};
    char buf[MB_TCP_ADU_MAX_LEN] = {0};

    num = mb_tcp_adu_format_resp(resp, buf, sizeof(buf));
    if (num < 0)
    {
        return -EBADMSG;
    }
    mb_tcp_adu_to_str(resp, msg_buf, sizeof(msg_buf));
    mb_log_info("[%d] sending: %s", index, msg_buf);
    return mb_tcp_con_send(&server->con[index], buf, num);
}

static ssize_t mb_tcp_server_send_err_resp(mb_tcp_server_t *server, int index, mb_tcp_adu_t *req, mb_tcp_adu_t *resp, int error)
{
    int ret = 0;

    mb_tcp_adu_set_header(resp, req->trans_id, req->proto_id, MB_TCP_SERVER_UNIT_ID);
    ret = mb_pdu_set_err_resp(&resp->pdu, req->pdu.func_code + 0x80, error);
    if (ret < 0)
    {
        return -EBADMSG;
    }
    return mb_tcp_server_send_resp(server, index, resp);
}

static size_t mb_tcp_server_con_exchange(mb_tcp_server_t *server, int index)
{
    mb_tcp_con_t *con = NULL;
    mb_tcp_adu_t resp = {0};
    mb_tcp_adu_t req = {0};
    char msg_buf[256] = {0};
    ssize_t num = 0;
    int ret = 0;

    con = &server->con[index];
    num = mb_tcp_con_recv(con);
    if (num <= 0)
    {
        return num;
    }
    num = mb_tcp_adu_parse_req(&req, con->rx_buf, con->rx_end);
    if (num < 0)
    {
        mb_tcp_server_send_err_resp(server, index, &req, &resp, -num);
        return -EBADMSG;
    }
    mb_tcp_adu_to_str(&req, msg_buf, sizeof(msg_buf));
    mb_log_info("[%d] received: %s", index, msg_buf);
    mb_tcp_con_consume(con, num);
    mb_log_info("[%d] calling handler callback", index);
    ret = (*server->handler)(server, &req, &resp);
    if (ret < 0)
    {
        mb_tcp_server_send_err_resp(server, index, &req, &resp, -ret);
        return -EBADMSG;
    }
    return mb_tcp_server_send_resp(server, index, &resp);
}

static int mb_tcp_server_find_empty_con(mb_tcp_server_t *server)
{
    mb_tcp_con_t *oldest = NULL;
    mb_tcp_con_t *con = NULL;
    int j = 0;
    int i = 0;

    for (i = 0; i < MB_TCP_SERVER_MAX_CON; i++)
    {
        con = &server->con[i];
        if (!mb_tcp_con_is_active(con))
        {
            mb_log_debug("found empty connection %d", i);
            return i;
        }
        else if ((oldest == NULL) || (con->last_use < oldest->last_use))
        {
            oldest = con;
            j = i;
        }
    }
    mb_log_debug("closing oldest connection %d", j);
    mb_tcp_con_close(oldest);
    return j;
}

int mb_tcp_server_create(mb_tcp_server_t *server, const char *host, uint16_t port, mb_tcp_server_handler_t handler)
{
    struct sockaddr_in server_sin = {0};
    int opt_val = 0;
    int ret = 0;
    int i = 0;

    memset(server, 0, sizeof(mb_tcp_server_t));
    server->sd = MB_TCP_SERVER_SOCKET_CLOSED;
    mb_ip_auth_list_create(&server->auth);
    for (i = 0; i < MB_TCP_SERVER_MAX_CON; i++)
        mb_tcp_con_create(&server->con[i], i);
    server->handler = handler;
    server->sd = socket(PF_INET, SOCK_STREAM, 0);
    if (server->sd == -1)
    {
        mb_tcp_server_destroy(server);
        return -errno;
    }
    opt_val = 1;
    ret = setsockopt(server->sd, SOL_SOCKET, SO_REUSEADDR, &opt_val, (socklen_t)sizeof(opt_val));
    if (ret < 0)
    {
        mb_tcp_server_destroy(server);
        return -errno;
    }
    server_sin.sin_family = AF_INET;
    server_sin.sin_port = htons(port);
    ret = inet_pton(AF_INET, host, &server_sin.sin_addr);
    if (ret < 0)
    {
        mb_tcp_server_destroy(server);
        return -errno;
    }
    if (ret == 0)
    {
        mb_tcp_server_destroy(server);
        return -EINVAL;
    }
    ret = bind(server->sd, (struct sockaddr *)&server_sin, sizeof(server_sin));
    if (ret < 0)
    {
        mb_tcp_server_destroy(server);
        return -errno;
    }
    ret = mb_tcp_con_set_non_blocking(server->sd);
    if (ret < 0)
    {
        mb_tcp_server_destroy(server);
        return ret;
    }
    mb_log_info("bound to address %s and port %d", host, port);
    return 0;
}

void mb_tcp_server_destroy(mb_tcp_server_t *server)
{
    int i = 0;

    for (i = 0; i < MB_TCP_SERVER_MAX_CON; i++)
        mb_tcp_con_destroy(&server->con[i]);
    mb_ip_auth_list_destroy(&server->auth);
    if (server->sd != MB_TCP_SERVER_SOCKET_CLOSED)
        close(server->sd);
    memset(server, 0, sizeof(mb_tcp_server_t));
}

int mb_tcp_server_authorise_addr(mb_tcp_server_t *server, const char *str)
{
    mb_log_debug("authorising address %s", str);
    return mb_ip_auth_list_add_str(&server->auth, str);
}

static int mb_tcp_server_handle_new_con(mb_tcp_server_t *server)
{
    struct sockaddr_in client_sin = {0};
    const char *p = NULL;
    socklen_t client_sin_len = 0;
    char buf[MB_TCP_SERVER_BUF_LEN] = {0};
    int index = 0;
    int ret = 0;
    int sd = 0;

    client_sin_len = sizeof(struct sockaddr_in);
    sd = accept(server->sd, (struct sockaddr *)&client_sin, &client_sin_len);
    if (sd < 0)
    {
        return -errno;
    }
    ret = mb_tcp_con_set_non_blocking(sd);
    if (ret < 0)
    {
        close(sd);
        return -errno;
    }
    ret = mb_ip_auth_list_check_addr(&server->auth, &client_sin.sin_addr);
    if (ret < 0)
    {
        close(sd);
        return ret;
    }
    if (ret == 0)
    {
        close(sd);
        mb_log_warn("rejecting unauthorised connection with address %s and port %u", buf, ntohs(client_sin.sin_port));
        return -EACCES;
    }
    p = inet_ntop(AF_INET, &client_sin.sin_addr, buf, sizeof(buf));
    if (p == NULL)
    {
        close(sd);
        return -errno;
    }
    mb_log_info("connection with address %s and port %u authorised", buf, ntohs(client_sin.sin_port));
    index = mb_tcp_server_find_empty_con(server);
    mb_tcp_con_open(&server->con[index], sd, &client_sin);
    return 0;
}

int mb_tcp_server_run(mb_tcp_server_t *server)
{
    mb_tcp_con_t *con = NULL;
    fd_set read_fds = {{0}};
    ssize_t num = 0;
    int max_fd = 0;
    int ret = 0;
    int i = 0;

    ret = listen(server->sd, MB_TCP_SERVER_BACKLOG);
    if (ret < 0)
    {
        return -errno;
    }
    mb_log_notice("listening...");
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(server->sd, &read_fds);
        max_fd = server->sd;
        for (i = 0; i < MB_TCP_SERVER_MAX_CON; i++)
        {
            con = &server->con[i];
            if (mb_tcp_con_is_active(con))
            {
                FD_SET(con->sd, &read_fds);
                if (con->sd > max_fd)
                    max_fd = con->sd;
            }
        }
        ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (ret < 0)
        {
            return -errno;
        }
        for (i = 0; i < MB_TCP_SERVER_MAX_CON; i++)
        {
            con = &server->con[i];
            if ((mb_tcp_con_is_active(con)) && (FD_ISSET(con->sd, &read_fds)))
            {
                num = mb_tcp_server_con_exchange(server, i);
                if (num == 0)
                {
                    mb_tcp_con_close(con);
                }
                else if ((num < 0) && (num != -EAGAIN))
                {
                    mb_log_warn("[%d] exchange: %s", i, strerror(-num));
                    mb_tcp_con_close(con);
                }
            }
        }
        if (FD_ISSET(server->sd, &read_fds))
        {
            ret = mb_tcp_server_handle_new_con(server);
            if (ret < 0)
            {
                return ret;
            }
        }
    }
    return 0;
}
