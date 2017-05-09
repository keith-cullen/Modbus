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
#include <arpa/inet.h>
#include <sys/select.h>
#include "mb_tcp_client.h"
#include "mb_log.h"

static ssize_t mb_tcp_client_con_exchange(mb_tcp_client_t *client, int index, mb_tcp_adu_t *req, mb_tcp_adu_t *resp)
{
    struct timeval timeout = {0};
    mb_tcp_con_t *con = NULL;
    fd_set read_fds = {{0}};
    ssize_t num = 0;
    char msg_buf[256] = {0};
    char buf[MB_TCP_ADU_MAX_LEN] = {0};
    int ret = 0;

    con = &client->con[index];
    num = mb_tcp_adu_format_req(req, buf, sizeof(buf));
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_tcp_adu_to_str(req, msg_buf, sizeof(msg_buf));
    mb_log_info("[%d] sending: %s", index, msg_buf);
    num = mb_tcp_con_send(con, buf, num);
    if (num <= 0)
    {
        return num;
    }
    FD_ZERO(&read_fds);
    FD_SET(con->sd, &read_fds);
    timeout = client->timeout;
    while (1)
    {
        ret = select(con->sd + 1, &read_fds, NULL, NULL, &timeout);
        if (ret < 0)
        {
            return -errno;
        }
        if (ret == 0)
        {
            return -ETIMEDOUT;
        }
        if (!FD_ISSET(con->sd, &read_fds))
        {
            continue;
        }
        num = mb_tcp_con_recv(con);
        if (num > 0)
        {
            break;
        }
        else if (num != -EAGAIN)
        {
            return num;
        }
    }
    num = mb_tcp_adu_parse_resp(resp, con->rx_buf, con->rx_end);
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_tcp_con_consume(con, num);
    mb_tcp_adu_to_str(resp, msg_buf, sizeof(msg_buf));
    mb_log_info("[%d] received: %s", index, msg_buf);
    return num;
}

static int mb_tcp_client_find_con(mb_tcp_client_t *client, struct sockaddr_in *sin)
{
    mb_tcp_con_t *con = NULL;
    int i = 0;

    for (i = 0; i < MB_TCP_CLIENT_MAX_CON; i++)
    {
        con = &client->con[i];
        if (memcmp(&con->sin, sin, sizeof(struct sockaddr_in)) == 0)
        {
            mb_log_debug("found existing connection %d", i);
            return i;
        }
    }
    mb_log_debug("no existing connection found");
    return -1;
}

static int mb_tcp_client_find_empty_con(mb_tcp_client_t *client)
{
    mb_tcp_con_t *oldest = NULL;
    mb_tcp_con_t *con = NULL;
    int i = 0;
    int j = 0;

    for (i = 0; i < MB_TCP_CLIENT_MAX_CON; i++)
    {
        con = &client->con[i];
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
    mb_tcp_con_close(con);
    return j;
}

void mb_tcp_client_create(mb_tcp_client_t *client, struct timeval timeout)
{
    int i = 0;

    memset(client, 0, sizeof(mb_tcp_client_t));
    mb_ip_auth_list_create(&client->auth);
    for (i = 0; i < MB_TCP_CLIENT_MAX_CON; i++)
        mb_tcp_con_create(&client->con[i], i);
    client->timeout = timeout;
}

void mb_tcp_client_destroy(mb_tcp_client_t *client)
{
    int i = 0;

    for (i = 0; i < MB_TCP_CLIENT_MAX_CON; i++)
        mb_tcp_con_destroy(&client->con[i]);
    mb_ip_auth_list_destroy(&client->auth);
    memset(client, 0, sizeof(mb_tcp_client_t));
}

int mb_tcp_client_authorise_addr(mb_tcp_client_t *client, const char *str)
{
    mb_log_debug("authorising address %s", str);
    return mb_ip_auth_list_add_str(&client->auth, str);
}

static int mb_tcp_client_con_open(mb_tcp_client_t *client, int index, struct sockaddr_in *sin)
{
    mb_tcp_con_t *con = NULL;
    int ret = 0;
    int sd = 0;

    con = &client->con[index];
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        return -errno;
    }
    ret = connect(sd, (struct sockaddr *)sin, sizeof(struct sockaddr_in));
    if (ret < 0)
    {
        return -errno;
    }
    ret = mb_tcp_con_set_non_blocking(con->sd);
    if (ret < 0)
    {
        return ret;
    }
    mb_tcp_con_open(con, sd, sin);
    return 0;
}

int mb_tcp_client_exchange(mb_tcp_client_t *client, const char *host, in_port_t port, mb_tcp_adu_t *req, mb_tcp_adu_t *resp)
{
    struct sockaddr_in server_sin = {0};
    ssize_t num = 0;
    int index = 0;
    int ret = 0;

    server_sin.sin_family = AF_INET;
    server_sin.sin_port = htons(port);
    ret = inet_pton(AF_INET, host, &server_sin.sin_addr);
    if (ret < 0)
    {
        return -errno;
    }
    if (ret == 0)
    {
        return -EINVAL;
    }
    index = mb_tcp_client_find_con(client, &server_sin);
    if (index >= 0)
    {
        num = mb_tcp_client_con_exchange(client, index, req, resp);
        if (num > 0)
        {
            return num;
        }
        else if (num < 0)
        {
            mb_log_warn("exchange: %s", strerror(-num));
        }
        mb_tcp_con_close(&client->con[index]);
    }
    mb_log_debug("attempting to establish new connection");
    ret = mb_ip_auth_list_check_addr(&client->auth, &server_sin.sin_addr);
    if (ret < 0)
    {
        return ret;
    }
    if (ret == 0)
    {
        mb_log_warn("rejecting unauthorised connection to address %s and port %u", host, port);
        return -EACCES;
    }
    mb_log_info("connection with address %s and port %u authorised", host, port);
    index = mb_tcp_client_find_empty_con(client);
    ret = mb_tcp_client_con_open(client, index, &server_sin);
    if (ret < 0)
    {
        return ret;
    }
    num = mb_tcp_client_con_exchange(client, index, req, resp);
    if (num == 0)
    {
        mb_log_info("[%d] connection closed remotely", index);
        mb_tcp_con_close(&client->con[index]);
    }
    else if (num < 0)
    {
        mb_tcp_con_close(&client->con[index]);
    }
    return num;
}
