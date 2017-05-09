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

#ifndef MB_RTU_CON_H
#define MB_RTU_CON_H

#include <stddef.h>
#include <time.h>
#include <termios.h>
#include <sys/types.h>
#include "mb_rtu_adu.h"

#define MB_RTU_CON_BAUD_RATE  B19200
#define MB_RTU_CON_T15_SEC    0
#define MB_RTU_CON_T15_NSEC   859375
#define MB_RTU_CON_T35_SEC    0
#define MB_RTU_CON_T35_NSEC   2005208

typedef struct
{
    int serial_fd;
    int t15_fd;
    int t35_fd;
}
mb_rtu_con_t;

int mb_rtu_con_create(mb_rtu_con_t *con, const char *dev);
void mb_rtu_con_destroy(mb_rtu_con_t *con);
int mb_rtu_con_start_timer(int fd, time_t sec, long nsec);
int mb_rtu_con_read_timer(int fd);
ssize_t mb_rtu_con_send(mb_rtu_con_t *con, const char *buf, size_t len);
ssize_t mb_rtu_con_recv(mb_rtu_con_t *con, char *buf, size_t len);
ssize_t mb_rtu_con_recv_timeout(mb_rtu_con_t *con, char *buf, size_t len, int timer_fd);

#endif
