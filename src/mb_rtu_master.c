/*
 * Copyright (c) 2017 Keith Cullen.
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
#include <sys/timerfd.h>
#include "mb_rtu_master.h"
#include "mb_log.h"

int mb_rtu_master_create(mb_rtu_master_t *master, const char *dev)
{
    int ret = 0;

    memset(master, 0, sizeof(mb_rtu_master_t));
    ret = mb_rtu_con_create(&master->con, dev);
    if (ret < 0)
    {
        return ret;
    }
    master->timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (master->timer_fd < 0)
    {
        mb_rtu_con_destroy(&master->con);
        memset(master, 0, sizeof(mb_rtu_master_t));
        return -errno;
    }
    mb_log_notice("master bound to '%s'", dev);
    mb_log_notice("idle");
    return 0;
}

void mb_rtu_master_destroy(mb_rtu_master_t *master)
{
    close(master->timer_fd);
    mb_rtu_con_destroy(&master->con);
    memset(master, 0, sizeof(mb_rtu_master_t));
}

int mb_rtu_master_exchange(mb_rtu_master_t *master, mb_rtu_adu_t *req, mb_rtu_adu_t *resp)
{
    ssize_t num = 0;
    char msg_buf[256] = {0};
    char buf[MB_RTU_ADU_MAX_LEN] = {0};
    int ret = 0;

    if ((req->addr < MB_RTU_ADU_MIN_UNICAST_ADDR) || (req->addr > MB_RTU_ADU_MAX_UNICAST_ADDR))
    {
        return -EINVAL;
    }
    num = mb_rtu_adu_format_req(req, buf, sizeof(buf));
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_rtu_adu_to_str(req, msg_buf, sizeof(msg_buf));
    mb_log_info("sending unicast request: %s", msg_buf);
    num = mb_rtu_con_send(&master->con, buf, num);
    if (num < 0)
    {
        return num;
    }
    mb_log_debug("starting response timer");
    ret = mb_rtu_con_start_timer(master->timer_fd, MB_RTU_MASTER_RESPONSE_TIMEOUT_SEC, MB_RTU_MASTER_RESPONSE_TIMEOUT_NSEC);
    if (ret < 0)
    {
        return ret;
    }
    num = mb_rtu_con_recv_timeout(&master->con, buf, sizeof(buf), master->timer_fd);
    if (num < 0)
    {
        return num;
    }
    num = mb_rtu_adu_parse_resp(resp, buf, num);
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_rtu_adu_to_str(resp, msg_buf, sizeof(msg_buf));
    mb_log_info("received response: %s", msg_buf);
    mb_log_notice("idle");
    return 0;
}

int mb_rtu_master_broadcast(mb_rtu_master_t *master, mb_rtu_adu_t *req)
{
    ssize_t num = 0;
    fd_set readfds = {{0}};
    char msg_buf[256] = {0};
    char buf[MB_RTU_ADU_MAX_LEN] = {0};
    int ret = 0;

    if (req->addr != MB_RTU_ADU_BROADCAST_ADDR)
    {
        return -EINVAL;
    }
    num = mb_rtu_adu_format_req(req, buf, sizeof(buf));
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_rtu_adu_to_str(req, msg_buf, sizeof(msg_buf));
    mb_log_info("sending broadcast request: %s", msg_buf);
    num = mb_rtu_con_send(&master->con, buf, num);
    if (num < 0)
    {
        return num;
    }
    ret = mb_rtu_con_start_timer(master->timer_fd, MB_RTU_MASTER_TURNAROUND_DELAY_SEC, MB_RTU_MASTER_TURNAROUND_DELAY_NSEC);
    if (ret < 0)
    {
        return ret;
    }
    FD_ZERO(&readfds);
    FD_SET(master->timer_fd, &readfds);
    ret = select(master->timer_fd + 1, &readfds, NULL, NULL, NULL);
    if (ret < 0)
    {
        return -errno;
    }
    ret = mb_rtu_con_read_timer(master->timer_fd);
    if (ret < 0)
    {
        return ret;
    }
    mb_log_notice("idle");
    return 0;
}
