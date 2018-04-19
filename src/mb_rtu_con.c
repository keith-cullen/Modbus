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
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/timerfd.h>
#include "mb_rtu_con.h"
#include "mb_log.h"

static int mb_rtu_con_set_non_blocking(int fd)
{
    int flags = 0;
    int ret = 0;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        return -errno;
    }
    ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (ret < 0)
    {
        return -errno;
    }
    return 0;
}

int mb_rtu_con_start_timer(int fd, time_t sec, long nsec)
{
    struct itimerspec its =
    {
        .it_interval =
        {
            .tv_sec = 0,
            .tv_nsec = 0
        },
        .it_value =
        {
            .tv_sec = sec,
            .tv_nsec = nsec
        }
    };
    int ret = 0;

    ret = timerfd_settime(fd, 0, &its, NULL);
    if (ret < 0)
    {
        return -errno;
    }
    return 0;
}

int mb_rtu_con_read_timer(int fd)
{
    uint64_t count = 0;
    ssize_t num = 0;

    num = read(fd, &count, sizeof(count));
    if ((num < 0) && (errno != EAGAIN))
    {
        return -errno;
    }
    return count ? 1 : 0;
}

int mb_rtu_con_create(mb_rtu_con_t *con, const char *dev)
{
    struct termios options = {0};
    fd_set readfds = {{0}};
    int max_fd = 0;
    int ret = 0;

    memset(con, 0, sizeof(mb_rtu_con_t));
    con->serial_fd = open(dev, O_RDWR | O_NOCTTY);
    if (con->serial_fd < 0)
    {
        memset(con, 0, sizeof(mb_rtu_con_t));
        return -errno;
    }
    ret = mb_rtu_con_set_non_blocking(con->serial_fd);
    if (ret < 0)
    {
        close(con->serial_fd);
        memset(con, 0, sizeof(mb_rtu_con_t));
        return ret;
    }
    tcflush(con->serial_fd, TCIFLUSH);
    cfmakeraw(&options);
    options.c_cflag |= CREAD | CLOCAL | PARENB;
    ret = cfsetispeed(&options, MB_RTU_CON_BAUD_RATE);
    if (ret < 0)
    {
        close(con->serial_fd);
        memset(con, 0, sizeof(mb_rtu_con_t));
        return -errno;
    }
    ret = cfsetospeed(&options, MB_RTU_CON_BAUD_RATE);
    if (ret < 0)
    {
        close(con->serial_fd);
        memset(con, 0, sizeof(mb_rtu_con_t));
        return -errno;
    }
    tcsetattr(con->serial_fd, TCSANOW, &options);
    con->t15_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (con->t15_fd < 0)
    {
        close(con->serial_fd);
        memset(con, 0, sizeof(mb_rtu_con_t));
        return -errno;
    }
    con->t35_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (con->t35_fd < 0)
    {
        close(con->t15_fd);
        close(con->serial_fd);
        memset(con, 0, sizeof(mb_rtu_con_t));
        return -errno;
    }
    while (1)
    {
        ret = mb_rtu_con_start_timer(con->t35_fd, MB_RTU_CON_T35_SEC, MB_RTU_CON_T35_NSEC);
        if (ret < 0)
        {
            return ret;
        }
        FD_ZERO(&readfds);
        FD_SET(con->serial_fd, &readfds);
        FD_SET(con->t35_fd, &readfds);
        max_fd = con->serial_fd;
        if (con->t35_fd > max_fd)
            max_fd = con->t35_fd;
        ret = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ret < 0)
        {
            return -errno;
        }
        ret = mb_rtu_con_read_timer(con->t35_fd);
        if (ret < 0)
        {
            return ret;
        }
        if (ret)
        {
            break;  /* timer expired */
        }
    }
    return 0;
}

void mb_rtu_con_destroy(mb_rtu_con_t *con)
{
    close(con->t35_fd);
    close(con->t15_fd);
    close(con->serial_fd);
    memset(con, 0, sizeof(mb_rtu_con_t));
}

ssize_t mb_rtu_con_send(mb_rtu_con_t *con, const char *buf, size_t len)
{
    ssize_t num = 0;
    fd_set readfds = {{0}};
    int ret = 0;

    num = write(con->serial_fd, buf, len);
    if (num < 0)
    {
        return -errno;
    }
    ret = mb_rtu_con_start_timer(con->t35_fd, MB_RTU_CON_T35_SEC, MB_RTU_CON_T35_NSEC);
    if (ret < 0)
    {
        return ret;
    }
    FD_ZERO(&readfds);
    FD_SET(con->t35_fd, &readfds);
    ret = select(con->t35_fd + 1, &readfds, NULL, NULL, NULL);
    if (ret < 0)
    {
        return -errno;
    }
    ret = mb_rtu_con_read_timer(con->t35_fd);
    if (ret < 0)
    {
        return ret;
    }
    mb_log_debug("sent %d bytes", num);
    return num;
}

static int mb_rtu_con_recv_wait(mb_rtu_con_t *con)
{
    fd_set readfds = {{0}};
    int ret = 0;

    FD_ZERO(&readfds);
    FD_SET(con->serial_fd, &readfds);
    ret = select(con->serial_fd + 1, &readfds, NULL, NULL, NULL);
    if (ret < 0)
    {
        return -errno;
    }
    return 0;
}

static int mb_rtu_con_recv_wait_timeout(mb_rtu_con_t *con, int timer_fd)
{
    fd_set readfds = {{0}};
    int max_fd = 0;
    int ret = 0;

    FD_ZERO(&readfds);
    FD_SET(con->serial_fd, &readfds);
    FD_SET(timer_fd, &readfds);
    max_fd = con->serial_fd;
    if (timer_fd > max_fd)
        max_fd = timer_fd;
    ret = select(max_fd + 1, &readfds, NULL, NULL, NULL);
    if (ret < 0)
    {
        return -errno;
    }
    ret = mb_rtu_con_read_timer(timer_fd);
    if (ret < 0)
    {
        return ret;
    }
    if (ret)
    {
        return -ETIMEDOUT;
    }
    return 0;
}

static ssize_t mb_rtu_con_recv_data(mb_rtu_con_t *con, char *buf, size_t len)
{
    ssize_t count = 0;
    ssize_t num = 0;
    fd_set readfds = {{0}};
    int max_fd = 0;
    int nok = 0;
    int ret = 0;

    while (1)
    {
        ret = mb_rtu_con_start_timer(con->t15_fd, MB_RTU_CON_T15_SEC, MB_RTU_CON_T15_NSEC);
        if (ret < 0)
        {
            return ret;
        }
        ret = mb_rtu_con_start_timer(con->t35_fd, MB_RTU_CON_T35_SEC, MB_RTU_CON_T35_NSEC);
        if (ret < 0)
        {
            return ret;
        }
        FD_ZERO(&readfds);
        FD_SET(con->serial_fd, &readfds);
        FD_SET(con->t15_fd, &readfds);
        FD_SET(con->t35_fd, &readfds);
        max_fd = con->serial_fd;
        if (con->t15_fd > max_fd)
            max_fd = con->t15_fd;
        if (con->t35_fd > max_fd)
            max_fd = con->t35_fd;
        ret = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ret < 0)
        {
            return -errno;
        }
        ret = mb_rtu_con_read_timer(con->t15_fd);
        if (ret < 0)
        {
            return ret;
        }
        if (ret)
        {
            break;  /* timer expired */
        }
        num = read(con->serial_fd, buf, len);
        if (num < 0)
        {
            return -errno;
        }
        buf += num;
        len -= num;
        count += num;
    }
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(con->t35_fd, &readfds);
        max_fd = con->t35_fd;
        if (!nok)
        {
            FD_SET(con->serial_fd, &readfds);
            if (con->serial_fd > max_fd)
                max_fd = con->serial_fd;
        }
        ret = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ret < 0)
        {
            return -errno;
        }
        ret = mb_rtu_con_read_timer(con->t35_fd);
        if (ret < 0)
        {
            return ret;
        }
        if (ret)
        {
            break;  /* timer expired */
        }
        nok = 1;
        mb_log_debug("received data in between t1.5 and t3.5");
    }
    if (nok)
    {
        return -EBADMSG;  /* received data in between t1.5 and t3.5 */
    }
    mb_log_debug("received %d bytes", count);
    return count;
}

ssize_t mb_rtu_con_recv(mb_rtu_con_t *con, char *buf, size_t len)
{
    int ret = 0;

    ret = mb_rtu_con_recv_wait(con);
    if (ret < 0)
    {
        return ret;
    }
    return mb_rtu_con_recv_data(con, buf, len);
}

ssize_t mb_rtu_con_recv_timeout(mb_rtu_con_t *con, char *buf, size_t len, int timer_fd)
{
    int ret = 0;

    ret = mb_rtu_con_recv_wait_timeout(con, timer_fd);
    if (ret < 0)
    {
        return ret;
    }
    return mb_rtu_con_recv_data(con, buf, len);
}
