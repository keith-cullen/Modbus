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
#include "mb_rtu_slave.h"
#include "mb_log.h"

static ssize_t mb_rtu_slave_send_resp(mb_rtu_slave_t *slave, mb_rtu_adu_t *resp)
{
    ssize_t num = 0;
    char msg_buf[256] = {0};
    char buf[MB_RTU_ADU_MAX_LEN] = {0};

    num = mb_rtu_adu_format_resp(resp, buf, sizeof(buf));
    if (num < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_rtu_adu_to_str(resp, msg_buf, sizeof(msg_buf));
    mb_log_info("sending response: %s", msg_buf);
    return mb_rtu_con_send(&slave->con, buf, num);
}

static ssize_t mb_rtu_slave_send_err_resp(mb_rtu_slave_t *slave, mb_rtu_adu_t *req, mb_rtu_adu_t *resp, int error)
{
    int ret = 0;

    mb_rtu_adu_set_header(resp, slave->addr);
    ret = mb_pdu_set_err_resp(&resp->pdu, req->pdu.func_code + 0x80, error);
    if (ret < 0)
    {
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    return mb_rtu_slave_send_resp(slave, resp);
}

static int mb_rtu_slave_handle_diag(mb_rtu_slave_t *slave, mb_rtu_adu_t *req, mb_rtu_adu_t *resp)
{
    uint16_t val16 = 0;
    int ret = 0;

    switch (req->pdu.diag_req.sub_func)
    {
    case MB_PDU_QUERY_DATA:
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_QUERY_DATA, req->pdu.diag_req.data, req->pdu.diag_req.num_data);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_CLEAR_COUNTERS:
        slave->bus_msg_count = 0;
        slave->bus_com_err_count = 0;
        slave->slave_excep_err_count = 0;
        slave->slave_msg_count = 0;
        slave->slave_no_resp_count = 0;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_CLEAR_COUNTERS, req->pdu.diag_req.data, req->pdu.diag_req.num_data);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_BUS_MSG_COUNT:
        val16 = slave->bus_msg_count;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_BUS_MSG_COUNT, &val16, 1);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_BUS_COM_ERR_COUNT:
        val16 = slave->bus_com_err_count;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_BUS_COM_ERR_COUNT, &val16, 1);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_SLAVE_EXCEP_ERR_COUNT:
        val16 = slave->slave_excep_err_count;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_SLAVE_EXCEP_ERR_COUNT, &val16, 1);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_SLAVE_MSG_COUNT:
        val16 = slave->slave_msg_count;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_SLAVE_MSG_COUNT, &val16, 1);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    case MB_PDU_SLAVE_NO_RESP_COUNT:
        val16 = slave->slave_no_resp_count;
        mb_rtu_adu_set_header(resp, slave->addr);
        ret = mb_pdu_set_diag_resp(&resp->pdu, MB_PDU_SLAVE_NO_RESP_COUNT, &val16, 1);
        if (ret < 0)
        {
            return -MB_PDU_EXCEPT_SERVER_DEV_FAIL;
        }
        return 0;
    default:
        return -MB_PDU_EXCEPT_ILLEGAL_FUNC;
    }
}

static int mb_rtu_slave_con_exchange(mb_rtu_slave_t *slave)
{
    mb_rtu_adu_t resp = {0};
    mb_rtu_adu_t req = {0};
    ssize_t num = 0;
    char msg_buf[256] = {0};
    char buf[MB_RTU_ADU_MAX_LEN] = {0};
    int ret = 0;

    num = mb_rtu_con_recv(&slave->con, buf, sizeof(buf));
    if (num < 0)
    {
        if (num == -EBADMSG)
        {
            slave->bus_com_err_count++;
            mb_log_debug("bus communication error: %d", slave->bus_com_err_count);
        }
        return num;
    }
    if ((num < MB_RTU_ADU_MIN_LEN) || (!mb_rtu_adu_check_crc((const uint8_t *)buf, num)))
    {
        slave->bus_com_err_count++;
        mb_log_debug("bus communication error: %d", slave->bus_com_err_count);
        return -EBADMSG;
    }
    slave->bus_msg_count++;
    mb_log_debug("bus message count: %d", slave->bus_msg_count);
    if ((buf[0] != slave->addr) && (buf[0] != MB_RTU_ADU_BROADCAST_ADDR))
    {
        return 0;
    }
    slave->slave_msg_count++;
    mb_log_debug("slave message count: %d", slave->slave_msg_count);
    if (buf[0] == MB_RTU_ADU_BROADCAST_ADDR)
    {
        slave->slave_no_resp_count++;
        mb_log_debug("slave no response count: %d", slave->slave_no_resp_count);
        if (!mb_rtu_adu_valid_broadcast_req(&req))
        {
            slave->slave_excep_err_count++;
            mb_log_debug("slave exception error count: %d", slave->slave_excep_err_count);
            return -EBADMSG;  /* convert modbus error to errno value */
        }
    }
    num = mb_rtu_adu_parse_req(&req, buf, num);
    if (num < 0)
    {
        slave->slave_excep_err_count++;
        mb_log_debug("slave exception error count: %d", slave->slave_excep_err_count);
        return -EBADMSG;  /* convert modbus error to errno value */
    }
    mb_rtu_adu_to_str(&req, msg_buf, sizeof(msg_buf));
    if (req.addr == slave->addr)
        mb_log_info("received unicast request: %s", msg_buf);
    else
        mb_log_info("received broadcast request: %s", msg_buf);
    if (req.pdu.func_code == MB_PDU_DIAG)
    {
        mb_log_info("calling internal diagnostics handler");
        ret = mb_rtu_slave_handle_diag(slave, &req, &resp);
    }
    else
    {
        mb_log_info("calling handler callback");
        ret = (*slave->handler)(slave, &req, &resp);
    }
    if (ret < 0)
    {
        slave->slave_excep_err_count++;
        mb_log_debug("slave exception error count: %d", slave->slave_excep_err_count);
    }
    if (req.addr == MB_RTU_ADU_BROADCAST_ADDR)
    {
        if (ret < 0)
        {
            return -EBADMSG;  /* convert modbus error to errno value */
        }
        return 0;
    }
    else
    {
        if (ret < 0)
        {
            mb_rtu_slave_send_err_resp(slave, &req, &resp, -ret);
            return -EBADMSG;  /* convert modbus error to errno value */
        }
        return mb_rtu_slave_send_resp(slave, &resp);
    }
}

int mb_rtu_slave_create(mb_rtu_slave_t *slave, const char *dev, int addr, mb_rtu_slave_handler_t handler)
{
    int ret = 0;

    memset(slave, 0, sizeof(mb_rtu_slave_t));
    if ((addr < MB_RTU_ADU_MIN_UNICAST_ADDR) || (addr > MB_RTU_ADU_MAX_UNICAST_ADDR))
    {
        return -EINVAL;
    }
    slave->addr = addr;
    ret = mb_rtu_con_create(&slave->con, dev);
    if (ret < 0)
    {
        memset(slave, 0, sizeof(mb_rtu_slave_t));
        return ret;
    }
    slave->handler = handler;
    mb_log_notice("slave bound to '%s'", dev);
    mb_log_notice("idle");
    return 0;
}

void mb_rtu_slave_destroy(mb_rtu_slave_t *slave)
{
    mb_rtu_con_destroy(&slave->con);
    memset(slave, 0, sizeof(mb_rtu_slave_t));
}

int mb_rtu_slave_run(mb_rtu_slave_t *slave)
{
    int ret = 0;

    while (1)
    {
        ret = mb_rtu_slave_con_exchange(slave);
        if (ret < 0)
        {
            mb_log_warn("exchange: %s", strerror(-ret));
        }
        mb_log_notice("idle");
    }
    return 0;
}
