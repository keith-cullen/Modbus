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

#ifndef MB_RTU_SLAVE_H
#define MB_RTU_SLAVE_H

#include "mb_rtu_con.h"
#include "mb_rtu_adu.h"

struct mb_rtu_slave;

typedef int (*mb_rtu_slave_handler_t)(struct mb_rtu_slave *slave, mb_rtu_adu_t *req, mb_rtu_adu_t *resp);

typedef struct mb_rtu_slave
{
    int addr;
    mb_rtu_con_t con;
    mb_rtu_slave_handler_t handler;
    int bus_msg_count;                                  /* Return Bus Message Count */
    int bus_com_err_count;                              /* Return Bus Communication Error Count */
    int slave_excep_err_count;                          /* Return Slave Exception Error Count */
    int slave_msg_count;                                /* Return Slave Message Count */
    int slave_no_resp_count;                            /* Return Slave No Response Count */
}
mb_rtu_slave_t;

int mb_rtu_slave_create(mb_rtu_slave_t *slave, const char *dev, int addr, mb_rtu_slave_handler_t handler);
void mb_rtu_slave_destroy(mb_rtu_slave_t *slave);
int mb_rtu_slave_run(mb_rtu_slave_t *slave);

#endif
