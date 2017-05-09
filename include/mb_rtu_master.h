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

#ifndef MB_RTU_MASTER_H
#define MB_RTU_MASTER_H

#include "mb_rtu_con.h"
#include "mb_rtu_adu.h"

#define MB_RTU_MASTER_TURNAROUND_DELAY_SEC   0
#define MB_RTU_MASTER_TURNAROUND_DELAY_NSEC  100000000
#define MB_RTU_MASTER_RESPONSE_TIMEOUT_SEC   1
#define MB_RTU_MASTER_RESPONSE_TIMEOUT_NSEC  0

typedef struct
{
    mb_rtu_con_t con;
    int timer_fd;
}
mb_rtu_master_t;

int mb_rtu_master_create(mb_rtu_master_t *master, const char *dev);
void mb_rtu_master_destroy(mb_rtu_master_t *master);
int mb_rtu_master_exchange(mb_rtu_master_t *master, mb_rtu_adu_t *req, mb_rtu_adu_t *resp);
int mb_rtu_master_broadcast(mb_rtu_master_t *master, mb_rtu_adu_t *req);

#endif
