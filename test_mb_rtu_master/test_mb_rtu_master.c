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
#define QUANT_COILS  1
#define REG_VAL     0x5678

int read_coils(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    int ret = 0;

    mb_log_notice("read coils[%d:%d]", SLAVE_ADDR, SLAVE_ADDR + QUANT_COILS);
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_rd_coils_req(&req.pdu, SLAVE_ADDR, QUANT_COILS);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if (resp.pdu.func_code == MB_PDU_RD_COILS)
    {
        mb_log_notice("coils[%d:%s]: 0x%02x", resp.pdu.rd_coils_resp.coil_stat[0]);
    }
    return 0;
}

int read_hold_reg(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    int ret = 0;

    mb_log_notice("read holding register[%d]", START_ADDR + 1);
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_rd_hold_regs_req(&req.pdu, START_ADDR, QUANT_REGS);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if (resp.pdu.func_code == MB_PDU_RD_HOLD_REGS)
    {
        mb_log_notice("holding register[%d]: 0x%04x",
                      req.pdu.rd_hold_regs_req.start_addr + 1,
                      resp.pdu.rd_hold_regs_resp.reg_val[0]);
    }
    return 0;
}

int write_hold_reg(mb_rtu_master_t *master)
{
    mb_rtu_adu_t req = {0};

    mb_log_notice("write holding register[%d]: 0x%04x", START_ADDR + 1, REG_VAL);
    mb_rtu_adu_set_header(&req, MB_RTU_ADU_BROADCAST_ADDR);
    mb_pdu_set_wr_sing_reg_req(&req.pdu, START_ADDR, REG_VAL);
    return mb_rtu_master_broadcast(master, &req);
}

int query_data(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val16[2] = {0x1234, 0x5678};
    int ret = 0;

    mb_log_notice("query data: 0x%04x, 0x%04x", val16[0], val16[1]);
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_QUERY_DATA, val16, sizeof(val16) / sizeof(val16[0]));
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_QUERY_DATA))
    {
        mb_log_notice("query data: 0x%04x, 0x%04x", resp.pdu.diag_resp.data[0], resp.pdu.diag_resp.data[1]);
    }
    return 0;
}

int bus_msg_count(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("bus message count");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_BUS_MSG_COUNT, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_BUS_MSG_COUNT))
    {
        mb_log_notice("bus message count: %d", resp.pdu.diag_resp.data[0]);
    }
    return 0;
}

int bus_com_err_count(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("bus communication error count");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_BUS_COM_ERR_COUNT, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_BUS_COM_ERR_COUNT))
    {
        mb_log_notice("bus communication error count: %d", resp.pdu.diag_resp.data[0]);
    }
    return 0;
}

int slave_excep_err_count(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("slave exception error count");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_SLAVE_EXCEP_ERR_COUNT, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_SLAVE_EXCEP_ERR_COUNT))
    {
        mb_log_notice("slave exception error count: %d", resp.pdu.diag_resp.data[0]);
    }
    return 0;
}

int slave_msg_count(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("slave message count");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_SLAVE_MSG_COUNT, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_SLAVE_MSG_COUNT))
    {
        mb_log_notice("slave message count: %d", resp.pdu.diag_resp.data[0]);
    }
    return 0;
}

int slave_no_resp_count(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("slave no response count");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_SLAVE_NO_RESP_COUNT, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    if ((resp.pdu.func_code == MB_PDU_DIAG) && (resp.pdu.diag_resp.sub_func == MB_PDU_SLAVE_NO_RESP_COUNT))
    {
        mb_log_notice("slave no response count: %d", resp.pdu.diag_resp.data[0]);
    }
    return 0;
}

int clear_counters(mb_rtu_master_t *master)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    uint16_t val = 0;
    int ret = 0;

    mb_log_notice("clear counters");
    mb_rtu_adu_set_header(&req, SLAVE_ADDR);
    ret = mb_pdu_set_diag_req(&req.pdu, MB_PDU_CLEAR_COUNTERS, &val, 1);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_exchange(master, &req, &resp);
    if (ret < 0)
    {
        return ret;
    }
    return 0;
}

int broadcast_read_hold_reg(mb_rtu_master_t *master)
{
    mb_rtu_adu_t req = {0};
    int ret = 0;

    mb_log_notice("broadcast read holding register[%d]", START_ADDR + 1);
    mb_rtu_adu_set_header(&req, 0);
    ret = mb_pdu_set_rd_hold_regs_req(&req.pdu, START_ADDR, QUANT_REGS);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    ret = mb_rtu_master_broadcast(master, &req);
    if (ret < 0)
    {
        return ret;
    }
    return 0;
}

int main(int argc, char **argv)
{
    mb_rtu_master_t master = {{0}};
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
    ret = read_hold_reg(&master);
    if (ret < 0)
    {
        mb_log_error("failed to read holding register: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = write_hold_reg(&master);
    if (ret < 0)
    {
        mb_log_error("failed to write holding register: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = read_hold_reg(&master);
    if (ret < 0)
    {
        mb_log_error("failed to read holding register: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = read_coils(&master);
    if (ret < 0)
    {
        mb_log_error("failed to read coils: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = query_data(&master);
    if (ret < 0)
    {
        mb_log_error("failed to query data: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = bus_msg_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get bus message count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = bus_com_err_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get bus communication error count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = slave_excep_err_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get slave exception error count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = slave_msg_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get slave message count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = slave_no_resp_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get slave no response count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = clear_counters(&master);
    if (ret < 0)
    {
        mb_log_error("failed to clear counters: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = bus_msg_count(&master);
    if (ret < 0)
    {
        mb_log_error("failed to get bus message count: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    ret = broadcast_read_hold_reg(&master);
    if (ret < 0)
    {
        mb_log_error("failed to broadcast read holding register: %s", strerror(-ret));
        mb_rtu_master_destroy(&master);
        return EXIT_FAILURE;
    }
    mb_rtu_master_destroy(&master);
    return EXIT_SUCCESS;
}
