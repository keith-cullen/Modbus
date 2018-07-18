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
#include <string.h>
#include <time.h>
#include "mb_tcp_client.h"
#include "mb_tcp_adu.h"
#include "mb_log.h"

#define SERVER_ADDR   "127.0.0.1"
#define SERVER_PORT   10000
#define AUTH_ADDR     "127.0.0.1"  /* authorised server address */
#define TIMEOUT_SEC   0
#define TIMEOUT_USEC  500000
#define TRANS_ID      1
#define PROTO_ID      0
#define START_ADDR    0x0
#define QUANT_REGS    1

int main(void)
{
    struct timeval timeout = {TIMEOUT_SEC, TIMEOUT_USEC};
    mb_tcp_client_t client = {{0}};
    mb_tcp_adu_t resp = {0};
    mb_tcp_adu_t req = {0};
    int ret = 0;

    mb_log_set_level(MB_LOG_DEBUG);
    mb_tcp_client_create(&client, timeout);
    ret = mb_tcp_client_authorise_addr(&client, AUTH_ADDR);
    if (ret < 0)
    {
        mb_log_error("failed to authorise server address: %s", strerror(-ret));
        mb_tcp_client_destroy(&client);
        return EXIT_FAILURE;
    }
    mb_log_notice("reading holding register[%d]", START_ADDR + 1);
    mb_tcp_adu_set_header(&req, TRANS_ID, PROTO_ID, MB_TCP_CLIENT_UNIT_ID);
    ret = mb_pdu_set_rd_hold_regs_req(&req.pdu, START_ADDR, QUANT_REGS);
    if (ret < 0)
    {
        mb_log_error("failed to set TCP ADU, ret: %d", ret);
        mb_tcp_client_destroy(&client);
        return EXIT_FAILURE;
    }
    ret = mb_tcp_client_exchange(&client, SERVER_ADDR, SERVER_PORT, &req, &resp);
    if (ret < 0)
    {
        mb_log_error("failed to exchange with server: %s", strerror(-ret));
        mb_tcp_client_destroy(&client);
        return EXIT_FAILURE;
    }
    mb_log_notice("holding register[%d]: 0x%04x",
                  req.pdu.rd_hold_regs_req.start_addr + 1,
                  resp.pdu.rd_hold_regs_resp.reg_val[0]);
    mb_tcp_client_destroy(&client);
    return EXIT_SUCCESS;
}
