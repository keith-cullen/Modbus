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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "mb_rtu_master.h"
#include "mb_rtu_adu.h"
#include "mb_log.h"

#define SLAVE_ADDR  1
#define START_ADDR  0x0
#define QUANT_REGS  1

int main(int argc, char **argv)
{
    mb_rtu_master_t master = {{0}};
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    const char *dev = NULL;
    int ret = 0;

    mb_log_set_level(MB_LOG_DEBUG);
    if (argc != 2)
    {
        mb_log_info("usage: test_mb_rtu_master dev\n");
        return EXIT_FAILURE;
    }
    dev = argv[1];
    ret = mb_rtu_master_create(&master, dev);
    if (ret < 0)
    {
        mb_log_error("failed to create master: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    mb_log_notice("reading holding register[%d]", START_ADDR + 1);
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_rd_hold_regs_req(&req.pdu, START_ADDR, QUANT_REGS);
    if (ret < 0)
    {
        mb_log_error("failed to set RTU ADU, ret: %d", ret);
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = mb_rtu_master_exchange(&master, &req, &resp);
    if (ret < 0)
    {
        mb_log_error("failed to exchange with slave: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    mb_log_notice("holding register[%d]: 0x%04x",
                  req.pdu.rd_hold_regs_req.start_addr + 1,
                  resp.pdu.rd_hold_regs_resp.reg_val[0]);
    mb_rtu_master_destroy(&master);
    return EXIT_SUCCESS;
}
