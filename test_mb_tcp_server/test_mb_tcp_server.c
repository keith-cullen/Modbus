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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "mb_tcp_server.h"
#include "mb_tcp_adu.h"
#include "mb_tcp_log.h"

#define HOST_ADDR  "127.0.0.1"
#define HOST_PORT  10000        /* using the standard port 502 requires root privileges */
#define AUTH_ADDR  "127.0.0.1"  /* authorised client address */

#define HOLD_REG_ADDR   0x0
#define HOLD_REG_QUANT  1

uint16_t hold_reg = 0x1234;

static int handle(mb_tcp_server_t *server, mb_tcp_adu_t *req, mb_tcp_adu_t *resp)
{
    int ret = 0;

    switch (req->pdu.func_code)
    {
    case MB_PDU_RD_HOLD_REGS:
        if (req->pdu.rd_hold_regs_req.start_addr != HOLD_REG_ADDR)
        {
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        }
        if (req->pdu.rd_hold_regs_req.quant_regs != HOLD_REG_QUANT)
        {
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        }
        mb_tcp_adu_set_header(resp, req->trans_id, req->proto_id, MB_TCP_SERVER_UNIT_ID);
        ret = mb_pdu_set_rd_hold_regs_resp(&resp->pdu, HOLD_REG_QUANT * 2, &hold_reg);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return ret;
    default:
        return -MB_PDU_EXCEPT_ILLEGAL_FUNC;
    }
    return 0;  /* should never reach here */
}

int main(void)
{
    mb_tcp_server_t server = {0};
    int ret = 0;

    mb_tcp_log_set_level(MB_TCP_LOG_DEBUG);
    ret = mb_tcp_server_create(&server, HOST_ADDR, HOST_PORT, handle);
    if (ret < 0)
    {
        fprintf(stderr, "Error: %s\n", strerror(-ret));
        return EXIT_FAILURE;
    }
    ret = mb_tcp_server_authorise_addr(&server, AUTH_ADDR);
    if (ret < 0)
    {
        fprintf(stderr, "Error: %s\n", strerror(-ret));
        mb_tcp_server_destroy(&server);
        return EXIT_FAILURE;
    }
    ret = mb_tcp_server_run(&server);
    if (ret < 0)
    {
        fprintf(stderr, "Error: %s\n", strerror(-ret));
        mb_tcp_server_destroy(&server);
        return EXIT_FAILURE;
    }
    mb_tcp_server_destroy(&server);
    return EXIT_SUCCESS;
}
