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

#include <string.h>
#include <arpa/inet.h>
#include "mb_pdu.h"

int mb_pdu_set(mb_pdu_t *pdu, mb_pdu_type_t type, uint8_t func_code, const uint8_t *data, uint16_t data_len)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (data_len > MB_PDU_MAX_DATA_LEN)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = type;
    pdu->func_code = func_code;
    memcpy(pdu->def.buf, data, data_len);
    pdu->data_len = data_len;
    return 0;
}

int mb_pdu_set_rd_coils_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_coils)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_coils < MB_PDU_RD_COILS_MIN_QUANT_COILS)
     || (quant_coils > MB_PDU_RD_COILS_MAX_QUANT_COILS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_coils;
    if (end_addr > MB_PDU_RD_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_COILS;
    pdu->rd_coils_req.start_addr = start_addr;
    pdu->rd_coils_req.quant_coils = quant_coils;
    return 0;
}

int mb_pdu_set_rd_coils_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *coil_stat)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (byte_count > MB_PDU_RD_COILS_MAX_BYTE_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_COILS;
    pdu->rd_coils_resp.byte_count = byte_count;
    memcpy(pdu->rd_coils_resp.coil_stat, coil_stat, byte_count);
    return 0;
}

int mb_pdu_set_rd_disc_ips_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ips)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_ips < MB_PDU_RD_DISC_IPS_MIN_QUANT_IPS)
     || (quant_ips > MB_PDU_RD_DISC_IPS_MAX_QUANT_IPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ips;
    if (end_addr > MB_PDU_RD_DISC_IPS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_DISC_IPS;
    pdu->rd_disc_ips_req.start_addr = start_addr;
    pdu->rd_disc_ips_req.quant_ips = quant_ips;
    return 0;
}

int mb_pdu_set_rd_disc_ips_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *ip_stat)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (byte_count > MB_PDU_RD_DISC_IPS_MAX_BYTE_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_DISC_IPS;
    pdu->rd_disc_ips_resp.byte_count = byte_count;
    memcpy(pdu->rd_disc_ips_resp.ip_stat, ip_stat, byte_count);
    return 0;
}

int mb_pdu_set_rd_hold_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_regs < MB_PDU_RD_HOLD_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_RD_HOLD_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_RD_HOLD_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_HOLD_REGS;
    pdu->rd_hold_regs_req.start_addr = start_addr;
    pdu->rd_hold_regs_req.quant_regs = quant_regs;
    return 0;
}

int mb_pdu_set_rd_hold_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *reg_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((byte_count & 0x01)
     || (byte_count > MB_PDU_RD_HOLD_REGS_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_HOLD_REGS;
    pdu->rd_hold_regs_resp.byte_count = byte_count;
    memcpy(pdu->rd_hold_regs_resp.reg_val, reg_val, byte_count);
    return 0;
}

int mb_pdu_set_rd_ip_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ip_regs)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_ip_regs < MB_PDU_RD_IP_REGS_MIN_QUANT_IP_REGS)
     || (quant_ip_regs > MB_PDU_RD_IP_REGS_MAX_QUANT_IP_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ip_regs;
    if (end_addr > MB_PDU_RD_IP_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_IP_REGS;
    pdu->rd_ip_regs_req.start_addr = start_addr;
    pdu->rd_ip_regs_req.quant_ip_regs = quant_ip_regs;
    return 0;
}

int mb_pdu_set_rd_ip_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *ip_reg)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((byte_count & 0x001)
     || (byte_count > MB_PDU_RD_IP_REGS_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_IP_REGS;
    pdu->rd_ip_regs_resp.byte_count = byte_count;
    memcpy(pdu->rd_ip_regs_resp.ip_reg, ip_reg, byte_count);
    return 0;
}

void mb_pdu_set_wr_sing_coil_req(mb_pdu_t *pdu, uint16_t op_addr, bool op_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_WR_SING_COIL;
    pdu->wr_sing_coil_req.op_addr = op_addr;
    pdu->wr_sing_coil_req.op_val = op_val;
}

void mb_pdu_set_wr_sing_coil_resp(mb_pdu_t *pdu, uint16_t op_addr, bool op_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_WR_SING_COIL;
    pdu->wr_sing_coil_resp.op_addr = op_addr;
    pdu->wr_sing_coil_resp.op_val = op_val;
}

void mb_pdu_set_wr_sing_reg_req(mb_pdu_t *pdu, uint16_t reg_addr, uint16_t reg_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_WR_SING_REG;
    pdu->wr_sing_reg_req.reg_addr = reg_addr;
    pdu->wr_sing_reg_req.reg_val = reg_val;
}

void mb_pdu_set_wr_sing_reg_resp(mb_pdu_t *pdu, uint16_t reg_addr, uint16_t reg_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_WR_SING_REG;
    pdu->wr_sing_reg_resp.reg_addr = reg_addr;
    pdu->wr_sing_reg_resp.reg_val = reg_val;
}

void mb_pdu_set_rd_except_stat_req(mb_pdu_t *pdu)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_EXCEPT_STAT;
}

void mb_pdu_set_rd_except_stat_resp(mb_pdu_t *pdu, uint8_t out_data)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_EXCEPT_STAT;
    pdu->rd_except_stat_resp.out_data = out_data;
}

int mb_pdu_set_diag_req(mb_pdu_t *pdu, uint16_t sub_func, const uint16_t *data, uint8_t num_data)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_data > MB_PDU_DIAG_MAX_NUM_DATA)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_DIAG;
    pdu->diag_req.sub_func = sub_func;
    memcpy(pdu->diag_req.data, data, 2 * num_data);
    pdu->diag_req.num_data = num_data;
    return 0;
}

int mb_pdu_set_diag_resp(mb_pdu_t *pdu, uint16_t sub_func, const uint16_t *data, uint8_t num_data)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_data > MB_PDU_DIAG_MAX_NUM_DATA)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_DIAG;
    pdu->diag_resp.sub_func = sub_func;
    memcpy(pdu->diag_resp.data, data, 2 * num_data);
    pdu->diag_resp.num_data = num_data;
    return 0;
}

void mb_pdu_set_get_com_ev_cntr_req(mb_pdu_t *pdu)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_GET_COM_EV_CNTR;
}

void mb_pdu_set_get_com_ev_cntr_resp(mb_pdu_t *pdu, uint16_t status, uint16_t ev_cnt)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_GET_COM_EV_CNTR;
    pdu->get_com_ev_cntr_resp.status = status ? 0xffff : 0x0000;
    pdu->get_com_ev_cntr_resp.ev_cnt = ev_cnt;
}

void mb_pdu_set_get_com_ev_log_req(mb_pdu_t *pdu)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_GET_COM_EV_LOG;
}

int mb_pdu_set_get_com_ev_log_resp(mb_pdu_t *pdu, uint16_t status, uint16_t ev_cnt, uint16_t msg_cnt, const uint8_t *event, uint8_t num_events)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_events > MB_PDU_GET_COM_EV_LOG_MAX_NUM_EVENTS)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_GET_COM_EV_LOG;
    pdu->get_com_ev_log_resp.byte_count = num_events + 6;
    pdu->get_com_ev_log_resp.status = status;
    pdu->get_com_ev_log_resp.ev_cnt = ev_cnt;
    pdu->get_com_ev_log_resp.msg_cnt = msg_cnt;
    memcpy(pdu->get_com_ev_log_resp.event, event, num_events);
    return 0;
}

int mb_pdu_set_wr_mult_coils_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ops, const uint8_t *op_val)
{
    uint32_t end_addr = 0;
    uint8_t byte_count = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_ops < MB_PDU_WR_MULT_COILS_MIN_QUANT_OPS)
     || (quant_ops > MB_PDU_WR_MULT_COILS_MAX_QUANT_OPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ops;
    if (end_addr > MB_PDU_WR_MULT_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    byte_count = quant_ops >> 3;
    if (quant_ops & 0x0007)
        byte_count++;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_WR_MULT_COILS;
    pdu->wr_mult_coils_req.start_addr = start_addr;
    pdu->wr_mult_coils_req.quant_ops = quant_ops;
    pdu->wr_mult_coils_req.byte_count = byte_count;
    memcpy(pdu->wr_mult_coils_req.op_val, op_val, byte_count);
    return 0;
}

int mb_pdu_set_wr_mult_coils_resp(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ops)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_ops < MB_PDU_WR_MULT_COILS_MIN_QUANT_OPS)
     || (quant_ops > MB_PDU_WR_MULT_COILS_MAX_QUANT_OPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ops;
    if (end_addr > MB_PDU_WR_MULT_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_WR_MULT_COILS;
    pdu->wr_mult_coils_resp.start_addr = start_addr;
    pdu->wr_mult_coils_resp.quant_ops = quant_ops;
    return 0;
}

int mb_pdu_set_wr_mult_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs, uint8_t byte_count, const uint16_t *reg_val)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_regs < MB_PDU_WR_MULT_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (byte_count !=  2 * quant_regs)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_WR_MULT_REGS;
    pdu->wr_mult_regs_req.start_addr = start_addr;
    pdu->wr_mult_regs_req.quant_regs = quant_regs;
    pdu->wr_mult_regs_req.byte_count = byte_count;
    memcpy(pdu->wr_mult_regs_req.reg_val, reg_val, byte_count);
    return 0;
}

int mb_pdu_set_wr_mult_regs_resp(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs)
{
    uint32_t end_addr = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_regs < MB_PDU_WR_MULT_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_WR_MULT_REGS;
    pdu->wr_mult_regs_resp.start_addr = start_addr;
    pdu->wr_mult_regs_resp.quant_regs = quant_regs;
    return 0;
}

void mb_pdu_set_rep_server_id_req(mb_pdu_t *pdu)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_REP_SERVER_ID;
}

int mb_pdu_set_rep_server_id_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *server_id, bool run_ind_status)
{
    uint8_t server_id_len = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((byte_count < MB_PDU_REP_SERVER_ID_MIN_BYTE_COUNT)
     || (byte_count > MB_PDU_REP_SERVER_ID_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_REP_SERVER_ID;
    pdu->rep_server_id_resp.byte_count = byte_count;
    server_id_len = byte_count - 1;
    memcpy(pdu->rep_server_id_resp.server_id, server_id, server_id_len);
    pdu->rep_server_id_resp.run_ind_status = run_ind_status;
    return 0;
}

int mb_pdu_set_rd_file_rec_req(mb_pdu_t *pdu, const mb_pdu_rd_file_rec_req_sub_req_t *sub_req, size_t num_sub_req)
{
    const mb_pdu_rd_file_rec_req_sub_req_t *src_sub_req = NULL;
    mb_pdu_rd_file_rec_req_sub_req_t *dst_sub_req = NULL;
    unsigned i = 0;
    uint32_t end_addr = 0;
    uint8_t byte_count = num_sub_req * MB_PDU_RD_FILE_REC_REQ_SUB_REQ_NUM_BYTES;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((byte_count < MB_PDU_RD_FILE_REC_MIN_BYTE_COUNT)
     || (byte_count > MB_PDU_RD_FILE_REC_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_FILE_REC;
    pdu->rd_file_rec_req.byte_count = byte_count;
    for (i = 0; i < num_sub_req; i++)
    {
        dst_sub_req = &pdu->rd_file_rec_req.sub_req[i];
        src_sub_req = &sub_req[i];
        if (src_sub_req->ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        end_addr = (uint32_t)src_sub_req->rec_num + (uint32_t)src_sub_req->rec_len;
        if (end_addr > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        dst_sub_req->file_num = src_sub_req->file_num;
        dst_sub_req->rec_num = src_sub_req->rec_num;
        dst_sub_req->rec_len = src_sub_req->rec_len;
    }
    return 0;
}

int mb_pdu_set_rd_file_rec_resp(mb_pdu_t *pdu, const mb_pdu_rd_file_rec_resp_sub_req_t *sub_req, size_t num_sub_req)
{
    const mb_pdu_rd_file_rec_resp_sub_req_t *src_sub_req = NULL;
    mb_pdu_rd_file_rec_resp_sub_req_t *dst_sub_req = NULL;
    unsigned resp_data_len = 0;
    unsigned i = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_sub_req > MB_PDU_RD_FILE_REC_MAX_NUM_SUB_REQ)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_FILE_REC;
    for (i = 0; i < num_sub_req; i++)
    {
        dst_sub_req = &pdu->rd_file_rec_resp.sub_req[i];
        src_sub_req = &sub_req[i];
        if ((src_sub_req->file_resp_len < MB_PDU_RD_FILE_REC_MIN_FILE_RESP_LEN)
         || (src_sub_req->file_resp_len > MB_PDU_RD_FILE_REC_MAX_FILE_RESP_LEN))
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        dst_sub_req->file_resp_len = src_sub_req->file_resp_len;
        memcpy(dst_sub_req->rec_data, src_sub_req->rec_data, src_sub_req->file_resp_len);
        resp_data_len += 1 + src_sub_req->file_resp_len;  /* sizeof(file_resp_len) + file_resp_len */
    }
    if ((resp_data_len < MB_PDU_RD_FILE_REC_MIN_RESP_DATA_LEN)
     || (resp_data_len > MB_PDU_RD_FILE_REC_MAX_RESP_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_file_rec_resp.resp_data_len = resp_data_len;
    return 0;
}

int mb_pdu_set_wr_file_rec_req(mb_pdu_t *pdu, const mb_pdu_wr_file_rec_sub_req_t *sub_req, size_t num_sub_req)
{
    const mb_pdu_wr_file_rec_sub_req_t *src_sub_req = NULL;
    mb_pdu_wr_file_rec_sub_req_t *dst_sub_req = NULL;
    unsigned req_data_len = 0;
    unsigned i = 0;
    uint32_t end_addr = 0;
    uint16_t num_bytes = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_sub_req > MB_PDU_WR_FILE_REC_MAX_NUM_SUB_REQ)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_WR_FILE_REC;
    for (i = 0; i < num_sub_req; i++)
    {
        dst_sub_req = &pdu->wr_file_rec_req.sub_req[i];
        src_sub_req = &sub_req[i];
        if (src_sub_req->ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->rec_len > MB_PDU_WR_FILE_REC_MAX_REC_LEN)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        end_addr = (uint32_t)src_sub_req->rec_num + (uint32_t)src_sub_req->rec_len;
        if (end_addr > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        dst_sub_req->file_num = src_sub_req->file_num;
        dst_sub_req->rec_num = src_sub_req->rec_num;
        dst_sub_req->rec_len = src_sub_req->rec_len;
        num_bytes = 2 * src_sub_req->rec_len;
        memcpy(dst_sub_req->rec_data, src_sub_req->rec_data, num_bytes);
        req_data_len += 7 + num_bytes;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + 2 * rec_len */
    }
    if ((req_data_len < MB_PDU_WR_FILE_REC_MIN_REQ_DATA_LEN)
     || (req_data_len > MB_PDU_WR_FILE_REC_MAX_REQ_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_file_rec_req.req_data_len = req_data_len;
    return 0;
}

int mb_pdu_set_wr_file_rec_resp(mb_pdu_t *pdu, const mb_pdu_wr_file_rec_sub_req_t *sub_req, size_t num_sub_req)
{
    const mb_pdu_wr_file_rec_sub_req_t *src_sub_req = NULL;
    mb_pdu_wr_file_rec_sub_req_t *dst_sub_req = NULL;
    unsigned resp_data_len = 0;
    unsigned i = 0;
    uint32_t end_addr = 0;
    uint16_t num_bytes = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if (num_sub_req > MB_PDU_WR_FILE_REC_MAX_NUM_SUB_REQ)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_WR_FILE_REC;
    for (i = 0; i < num_sub_req; i++)
    {
        dst_sub_req = &pdu->wr_file_rec_resp.sub_req[i];
        src_sub_req = &sub_req[i];
        if (src_sub_req->ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        if (src_sub_req->rec_len > MB_PDU_WR_FILE_REC_MAX_REC_LEN)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        end_addr = (uint32_t)src_sub_req->rec_num + (uint32_t)src_sub_req->rec_len;
        if (end_addr > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        dst_sub_req->file_num = src_sub_req->file_num;
        dst_sub_req->rec_num = src_sub_req->rec_num;
        dst_sub_req->rec_len = src_sub_req->rec_len;
        num_bytes = 2 * src_sub_req->rec_len;
        memcpy(dst_sub_req->rec_data, src_sub_req->rec_data, num_bytes);
        resp_data_len += 7 + num_bytes;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + 2 * rec_len */
    }
    if ((resp_data_len < MB_PDU_WR_FILE_REC_MIN_RESP_DATA_LEN)
     || (resp_data_len > MB_PDU_WR_FILE_REC_MAX_RESP_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_file_rec_resp.resp_data_len = resp_data_len;
    return 0;
}

void mb_pdu_set_mask_wr_reg_req(mb_pdu_t *pdu, uint16_t ref_addr, uint16_t and_mask, uint16_t or_mask)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_MASK_WR_REG;
    pdu->mask_wr_reg_req.ref_addr = ref_addr;
    pdu->mask_wr_reg_req.and_mask = and_mask;
    pdu->mask_wr_reg_req.or_mask = or_mask;
}

void mb_pdu_set_mask_wr_reg_resp(mb_pdu_t *pdu, uint16_t ref_addr, uint16_t and_mask, uint16_t or_mask)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_MASK_WR_REG;
    pdu->mask_wr_reg_resp.ref_addr = ref_addr;
    pdu->mask_wr_reg_resp.and_mask = and_mask;
    pdu->mask_wr_reg_resp.or_mask = or_mask;
}

int mb_pdu_set_rd_wr_mult_regs_req(mb_pdu_t *pdu, uint16_t rd_start_addr, uint16_t quant_rd, uint16_t wr_start_addr, uint16_t quant_wr, const uint16_t *wr_reg_val)
{
    uint32_t end_addr = 0;
    uint8_t wr_byte_count = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((quant_rd < MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_RD)
     || (quant_rd > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_RD))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)rd_start_addr + (uint32_t)quant_rd;
    if (end_addr > MB_PDU_RD_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    if ((quant_wr < MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_WR)
     || (quant_wr > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_WR))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    end_addr = (uint32_t)wr_start_addr + (uint32_t)quant_wr;
    if (end_addr > MB_PDU_RD_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_WR_MULT_REGS;
    pdu->rd_wr_mult_regs_req.rd_start_addr = rd_start_addr;
    pdu->rd_wr_mult_regs_req.quant_rd = quant_rd;
    pdu->rd_wr_mult_regs_req.wr_start_addr = wr_start_addr;
    pdu->rd_wr_mult_regs_req.quant_wr = quant_wr;
    wr_byte_count = (uint8_t)quant_wr * 2;
    pdu->rd_wr_mult_regs_req.wr_byte_count = wr_byte_count;
    memcpy(pdu->rd_wr_mult_regs_req.wr_reg_val, wr_reg_val, wr_byte_count);
    return 0;
}

int mb_pdu_set_rd_wr_mult_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *rd_reg_val)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if ((byte_count > MB_PDU_RD_WR_MULT_REGS_MAX_WR_BYTE_COUNT)
     || (byte_count & 0x01))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_WR_MULT_REGS;
    pdu->rd_wr_mult_regs_resp.byte_count = byte_count;
    memcpy(pdu->rd_wr_mult_regs_resp.rd_reg_val, rd_reg_val, byte_count);
    return 0;
}

void mb_pdu_set_rd_fifo_q_req(mb_pdu_t *pdu, uint16_t fifo_ptr_addr)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_RD_FIFO_Q;
    pdu->rd_fifo_q_req.fifo_ptr_addr = fifo_ptr_addr;
}

int mb_pdu_set_rd_fifo_q_resp(mb_pdu_t *pdu, uint16_t fifo_count, const uint16_t *fifo_val_reg)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (fifo_count > MB_PDU_RD_FIFO_Q_MAX_FIFO_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_RD_FIFO_Q;
    pdu->rd_fifo_q_resp.byte_count = 2 * (fifo_count + 1);
    pdu->rd_fifo_q_resp.fifo_count = fifo_count;
    memcpy(pdu->rd_fifo_q_resp.fifo_val_reg, fifo_val_reg, 2 * fifo_count);
    return 0;
}

int mb_pdu_set_enc_if_trans_req(mb_pdu_t *pdu, uint8_t mei_type, const uint8_t *mei_data, uint8_t mei_data_len)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (mei_data_len > MB_PDU_ENC_IF_TRANS_MAX_MEI_DATA_LEN)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_REQ;
    pdu->func_code = MB_PDU_ENC_IF_TRANS;
    pdu->enc_if_trans_req.mei_type = mei_type;
    memcpy(pdu->enc_if_trans_req.mei_data, mei_data, mei_data_len);
    pdu->enc_if_trans_req.mei_data_len = mei_data_len;
    return 0;
}

int mb_pdu_set_enc_if_trans_resp(mb_pdu_t *pdu, uint8_t mei_type, const uint8_t *mei_data, uint8_t mei_data_len)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (mei_data_len > MB_PDU_ENC_IF_TRANS_MAX_MEI_DATA_LEN)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->type = MB_PDU_RESP;
    pdu->func_code = MB_PDU_ENC_IF_TRANS;
    pdu->enc_if_trans_resp.mei_type = mei_type;
    memcpy(pdu->enc_if_trans_resp.mei_data, mei_data, mei_data_len);
    pdu->enc_if_trans_resp.mei_data_len = mei_data_len;
    return 0;
}

int mb_pdu_set_err_resp(mb_pdu_t *pdu, uint8_t func_code, uint8_t except_code)
{
    memset(pdu, 0, sizeof(mb_pdu_t));
    if (func_code < 0x80)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    switch (except_code)
    {
    case MB_PDU_EXCEPT_ILLEGAL_FUNC:
    case MB_PDU_EXCEPT_ILLEGAL_ADDR:
    case MB_PDU_EXCEPT_ILLEGAL_VAL:
    case MB_PDU_EXCEPT_SERVER_DEV_FAIL:
    case MB_PDU_EXCEPT_ACK:
    case MB_PDU_EXCEPT_SERVER_DEV_BUSY:
    case MB_PDU_EXCEPT_MEM_PARITY_ERROR:
    case MB_PDU_EXCEPT_GATEWAY_PATH_UNAVAIL:
    case MB_PDU_EXCEPT_GATEWAY_TARGET_NO_RESP:
        break;
    default:
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    }
    pdu->type = MB_PDU_ERR;
    pdu->func_code = func_code;
    pdu->err.except_code = except_code;
    return 0;
}

static ssize_t mb_pdu_format_rd_coils_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_coils_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_coils */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_coils_req.quant_coils);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_coils_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    byte_count = pdu->rd_coils_resp.byte_count;
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* coil_stat */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_coils_resp.coil_stat, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_rd_disc_ips_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_disc_ips_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ips */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_disc_ips_req.quant_ips);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_disc_ips_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    byte_count = pdu->rd_disc_ips_resp.byte_count;
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* ip_stat */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_disc_ips_resp.ip_stat, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_rd_hold_regs_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_hold_regs_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_hold_regs_req.quant_regs);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_hold_regs_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    byte_count = pdu->rd_hold_regs_resp.byte_count;
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* hold_reg */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_hold_regs_resp.reg_val, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < byte_count; i += 2)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_rd_ip_regs_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_ip_regs_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ip_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_ip_regs_req.quant_ip_regs);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_ip_regs_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    byte_count = pdu->rd_ip_regs_resp.byte_count;
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* hold_reg */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_ip_regs_resp.ip_reg, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < byte_count; i += 2)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_wr_sing_coil_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* op_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_coil_req.op_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = pdu->wr_sing_coil_req.op_val
          ? htons(MB_PDU_WR_SING_COIL_ON_VAL)
          : htons(MB_PDU_WR_SING_COIL_OFF_VAL);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_wr_sing_coil_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* op_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_coil_resp.op_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = pdu->wr_sing_coil_resp.op_val
          ? htons(MB_PDU_WR_SING_COIL_ON_VAL)
          : htons(MB_PDU_WR_SING_COIL_OFF_VAL);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_wr_sing_reg_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* reg_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_reg_req.reg_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* reg_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_reg_req.reg_val);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_wr_sing_reg_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* reg_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_reg_resp.reg_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* reg_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_sing_reg_resp.reg_val);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_except_stat_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_rd_except_stat_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* out_data */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->rd_except_stat_resp.out_data;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_diag_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    uint8_t num_data = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* sub_func */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->diag_req.sub_func);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* data */
    num_data = pdu->diag_req.num_data;
    byte_count = 2 * num_data;
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->diag_req.data, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < num_data; i++)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_diag_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    uint8_t num_data = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* sub_func */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->diag_resp.sub_func);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* data */
    num_data = pdu->diag_resp.num_data;
    byte_count = 2 * num_data;
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->diag_resp.data, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < num_data; i++)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_get_com_ev_cntr_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_get_com_ev_cntr_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* status */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->get_com_ev_cntr_resp.status);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* ev_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->get_com_ev_cntr_resp.ev_cnt);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_get_com_ev_log_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_get_com_ev_log_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    uint8_t num_events = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    num_events = pdu->get_com_ev_log_resp.byte_count - 6;
    buf[0] = pdu->get_com_ev_log_resp.byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* status */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->get_com_ev_log_resp.status);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* ev_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->get_com_ev_log_resp.ev_cnt);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* msg_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->get_com_ev_log_resp.msg_cnt);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* event */
    if (len < num_events)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->get_com_ev_log_resp.event, num_events);
    num += num_events;
    buf += num_events;
    len -= num_events;

    return num;
}

static ssize_t mb_pdu_format_wr_mult_coils_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_coils_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ops */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_coils_req.quant_ops);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = pdu->wr_mult_coils_req.byte_count;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* op_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->wr_mult_coils_req.op_val, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_wr_mult_coils_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_coils_resp.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ops */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_coils_resp.quant_ops);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_wr_mult_regs_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_regs_req.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_regs_req.quant_regs);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = pdu->wr_mult_regs_req.byte_count;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* reg_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->wr_mult_regs_req.reg_val, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < byte_count; i += 2)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_wr_mult_regs_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_regs_resp.start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->wr_mult_regs_resp.quant_regs);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rep_server_id_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_rep_server_id_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint8_t server_id_len = 0;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = pdu->rep_server_id_resp.byte_count;
    server_id_len = byte_count - 1;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* server_id */
    if (len < server_id_len)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rep_server_id_resp.server_id, server_id_len);
    num += server_id_len;
    buf += server_id_len;
    len -= server_id_len;

    /* run_ind_status */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->rep_server_id_resp.run_ind_status ? 0xff : 0x00;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_format_rd_file_rec_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    mb_pdu_rd_file_rec_req_sub_req_t *sub_req = NULL;
    unsigned i = 0;
    unsigned j = 0;
    uint16_t val16 = 0;
    uint8_t byte_count = 0;
    uint8_t ref_type = MB_PDU_FILE_REC_REF_TYPE;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = pdu->rd_file_rec_req.byte_count;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < byte_count; i += MB_PDU_RD_FILE_REC_REQ_SUB_REQ_NUM_BYTES, j++)
    {
        sub_req = &pdu->rd_file_rec_req.sub_req[j];

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        buf[0] = ref_type;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->file_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_len);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;
    }
    return num;
}

static ssize_t mb_pdu_format_rd_file_rec_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    mb_pdu_rd_file_rec_resp_sub_req_t *sub_req = NULL;
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t rec_data_len = 0;
    uint8_t resp_data_len = 0;
    uint8_t ref_type = MB_PDU_FILE_REC_REF_TYPE;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* resp_data_len */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    resp_data_len = pdu->rd_file_rec_resp.resp_data_len;
    buf[0] = resp_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < resp_data_len; j++)
    {
        sub_req = &pdu->rd_file_rec_resp.sub_req[j];

        /* file_resp_len */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        buf[0] = sub_req->file_resp_len;
        num += 1;
        buf += 1;
        len -= 1;

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        buf[0] = ref_type;
        num += 1;
        buf += 1;
        len -= 1;

        /* rec_data */
        rec_data_len = sub_req->file_resp_len - 1;  /* -1 for sizeof(ref_type) */
        if (len < rec_data_len)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(buf, sub_req->rec_data, rec_data_len);
        p = (uint16_t *)buf;
        for (k = 0; k < rec_data_len; k += 2)
        {
            val16 = htons(*p);
            memcpy(p, &val16, 2);
            p++;
        }
        num += rec_data_len;
        buf += rec_data_len;
        len -= rec_data_len;

        i += 2 + rec_data_len;  /* sizeof(file_resp_len) + sizeof(ref_type) + rec_data_len */
    }
    return num;
}

static ssize_t mb_pdu_format_wr_file_rec_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    mb_pdu_wr_file_rec_sub_req_t *sub_req = NULL;
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint16_t rec_data_len = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t req_data_len = 0;
    uint8_t ref_type = MB_PDU_FILE_REC_REF_TYPE;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* req_data_len */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    req_data_len = pdu->wr_file_rec_req.req_data_len;
    buf[0] = req_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < req_data_len; j++)
    {
        sub_req = &pdu->wr_file_rec_req.sub_req[j];

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        buf[0] = ref_type;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->file_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_len);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_data */
        rec_data_len = 2 * sub_req->rec_len;
        if (len < rec_data_len)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(buf, sub_req->rec_data, rec_data_len);
        p = (uint16_t *)buf;
        for (k = 0; k < rec_data_len; k += 2)
        {
            val16 = htons(*p);
            memcpy(p, &val16, 2);
            p++;
        }
        num += rec_data_len;
        buf += rec_data_len;
        len -= rec_data_len;

        i += 7 + rec_data_len;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + rec_data_len */
    }
    return num;
}

static ssize_t mb_pdu_format_wr_file_rec_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    mb_pdu_wr_file_rec_sub_req_t *sub_req = NULL;
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint16_t rec_data_len = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t resp_data_len = 0;
    uint8_t ref_type = MB_PDU_FILE_REC_REF_TYPE;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* req_data_len */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    resp_data_len = pdu->wr_file_rec_resp.resp_data_len;
    buf[0] = resp_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < resp_data_len; j++)
    {
        sub_req = &pdu->wr_file_rec_resp.sub_req[j];

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        buf[0] = ref_type;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->file_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_num);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        val16 = htons(sub_req->rec_len);
        memcpy(buf, &val16, 2);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_data */
        rec_data_len = 2 * sub_req->rec_len;
        if (len < rec_data_len)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(buf, sub_req->rec_data, rec_data_len);
        p = (uint16_t *)buf;
        for (k = 0; k < rec_data_len; k += 2)
        {
            val16 = htons(*p);
            memcpy(p, &val16, 2);
            p++;
        }
        num += rec_data_len;
        buf += rec_data_len;
        len -= rec_data_len;

        i += 7 + rec_data_len;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + rec_data_len */
    }
    return num;
}

static ssize_t mb_pdu_format_mask_wr_reg_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* ref_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_req.ref_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* and_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_req.and_mask);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* or_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_req.or_mask);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_mask_wr_reg_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* ref_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_resp.ref_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* and_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_resp.and_mask);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* or_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->mask_wr_reg_resp.or_mask);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_wr_mult_regs_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t wr_byte_count = 0;
    uint16_t quant_wr = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* rd_start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_wr_mult_regs_req.rd_start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_rd */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_wr_mult_regs_req.quant_rd);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* wr_start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_wr_mult_regs_req.wr_start_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_wr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    quant_wr = pdu->rd_wr_mult_regs_req.quant_wr;
    val16 = htons(quant_wr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* wr_byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] =  pdu->rd_wr_mult_regs_req.wr_byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* wr_reg_val */
    wr_byte_count = 2 * quant_wr;
    if (len < wr_byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_wr_mult_regs_req.wr_reg_val, wr_byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < quant_wr; i++)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += wr_byte_count;
    buf += wr_byte_count;
    len -= wr_byte_count;

    return num;
}

static ssize_t mb_pdu_format_rd_wr_mult_regs_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = pdu->rd_wr_mult_regs_resp.byte_count;
    buf[0] = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* rd_reg_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_wr_mult_regs_resp.rd_reg_val, byte_count);
    p = (uint16_t *)buf;
    for (i = 0; i < byte_count; i += 2)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_format_rd_fifo_q_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* fifo_ptr_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_fifo_q_req.fifo_ptr_addr);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_format_rd_fifo_q_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t fifo_count = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* byte_count */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(pdu->rd_fifo_q_resp.byte_count);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* fifo_count */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    fifo_count = pdu->rd_fifo_q_resp.fifo_count;
    val16 = htons(fifo_count);
    memcpy(buf, &val16, 2);
    num += 2;
    buf += 2;
    len -= 2;

    /* fifo_val_reg */
    if (len < 2 * fifo_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->rd_fifo_q_resp.fifo_val_reg, 2 * fifo_count);
    p = (uint16_t *)buf;
    for (i = 0; i < fifo_count; i++)
    {
        val16 = htons(*p);
        memcpy(p, &val16, 2);
        p++;
    }
    num += 2 * fifo_count;
    buf += 2 * fifo_count;
    len -= 2 * fifo_count;

    return num;
}

static ssize_t mb_pdu_format_enc_if_trans_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint8_t mei_data_len = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_type */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->enc_if_trans_req.mei_type;
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_data */
    mei_data_len = pdu->enc_if_trans_req.mei_data_len;
    if (len < mei_data_len)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->enc_if_trans_req.mei_data, mei_data_len);
    num += mei_data_len;
    buf += mei_data_len;
    len -= mei_data_len;

    return num;
}

static ssize_t mb_pdu_format_enc_if_trans_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    uint8_t mei_data_len = 0;
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_type */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->enc_if_trans_resp.mei_type;
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_data */
    mei_data_len = pdu->enc_if_trans_resp.mei_data_len;
    if (len < mei_data_len)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, pdu->enc_if_trans_resp.mei_data, mei_data_len);
    num += mei_data_len;
    buf += mei_data_len;
    len -= mei_data_len;

    return num;
}

static ssize_t mb_pdu_format_err_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    ssize_t num = 0;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->func_code;
    num += 1;
    buf += 1;
    len -= 1;

    /* except_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = pdu->err.except_code;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

ssize_t mb_pdu_format_req(mb_pdu_t *pdu, char *buf, size_t len)
{
    switch (pdu->func_code)
    {
    case MB_PDU_RD_COILS:
        return mb_pdu_format_rd_coils_req(pdu, buf, len);
    case MB_PDU_RD_DISC_IPS:
        return mb_pdu_format_rd_disc_ips_req(pdu, buf, len);
    case MB_PDU_RD_HOLD_REGS:
        return mb_pdu_format_rd_hold_regs_req(pdu, buf, len);
    case MB_PDU_RD_IP_REGS:
        return mb_pdu_format_rd_ip_regs_req(pdu, buf, len);
    case MB_PDU_WR_SING_COIL:
        return mb_pdu_format_wr_sing_coil_req(pdu, buf, len);
    case MB_PDU_WR_SING_REG:
        return mb_pdu_format_wr_sing_reg_req(pdu, buf, len);
    case MB_PDU_RD_EXCEPT_STAT:
        return mb_pdu_format_rd_except_stat_req(pdu, buf, len);
    case MB_PDU_DIAG:
        return mb_pdu_format_diag_req(pdu, buf, len);
    case MB_PDU_GET_COM_EV_CNTR:
        return mb_pdu_format_get_com_ev_cntr_req(pdu, buf, len);
    case MB_PDU_GET_COM_EV_LOG:
        return mb_pdu_format_get_com_ev_log_req(pdu, buf, len);
    case MB_PDU_WR_MULT_COILS:
        return mb_pdu_format_wr_mult_coils_req(pdu, buf, len);
    case MB_PDU_WR_MULT_REGS:
        return mb_pdu_format_wr_mult_regs_req(pdu, buf, len);
    case MB_PDU_REP_SERVER_ID:
        return mb_pdu_format_rep_server_id_req(pdu, buf, len);
    case MB_PDU_RD_FILE_REC:
        return mb_pdu_format_rd_file_rec_req(pdu, buf, len);
    case MB_PDU_WR_FILE_REC:
        return mb_pdu_format_wr_file_rec_req(pdu, buf, len);
    case MB_PDU_MASK_WR_REG:
        return mb_pdu_format_mask_wr_reg_req(pdu, buf, len);
    case MB_PDU_RD_WR_MULT_REGS:
        return mb_pdu_format_rd_wr_mult_regs_req(pdu, buf, len);
    case MB_PDU_RD_FIFO_Q:
        return mb_pdu_format_rd_fifo_q_req(pdu, buf, len);
    case MB_PDU_ENC_IF_TRANS:
        return mb_pdu_format_enc_if_trans_req(pdu, buf, len);
    }
    return -MB_PDU_EXCEPT_ILLEGAL_FUNC;
}

ssize_t mb_pdu_format_resp(mb_pdu_t *pdu, char *buf, size_t len)
{
    switch (pdu->func_code)
    {
    case MB_PDU_RD_COILS:
        return mb_pdu_format_rd_coils_resp(pdu, buf, len);
    case MB_PDU_RD_DISC_IPS:
        return mb_pdu_format_rd_disc_ips_resp(pdu, buf, len);
    case MB_PDU_RD_HOLD_REGS:
        return mb_pdu_format_rd_hold_regs_resp(pdu, buf, len);
    case MB_PDU_RD_IP_REGS:
        return mb_pdu_format_rd_ip_regs_resp(pdu, buf, len);
    case MB_PDU_WR_SING_COIL:
        return mb_pdu_format_wr_sing_coil_resp(pdu, buf, len);
    case MB_PDU_WR_SING_REG:
        return mb_pdu_format_wr_sing_reg_resp(pdu, buf, len);
    case MB_PDU_RD_EXCEPT_STAT:
        return mb_pdu_format_rd_except_stat_resp(pdu, buf, len);
    case MB_PDU_DIAG:
        return mb_pdu_format_diag_resp(pdu, buf, len);
    case MB_PDU_GET_COM_EV_CNTR:
        return mb_pdu_format_get_com_ev_cntr_resp(pdu, buf, len);
    case MB_PDU_GET_COM_EV_LOG:
        return mb_pdu_format_get_com_ev_log_resp(pdu, buf, len);
    case MB_PDU_WR_MULT_COILS:
        return mb_pdu_format_wr_mult_coils_resp(pdu, buf, len);
    case MB_PDU_WR_MULT_REGS:
        return mb_pdu_format_wr_mult_regs_resp(pdu, buf, len);
    case MB_PDU_REP_SERVER_ID:
        return mb_pdu_format_rep_server_id_resp(pdu, buf, len);
    case MB_PDU_RD_FILE_REC:
        return mb_pdu_format_rd_file_rec_resp(pdu, buf, len);
    case MB_PDU_WR_FILE_REC:
        return mb_pdu_format_wr_file_rec_resp(pdu, buf, len);
    case MB_PDU_MASK_WR_REG:
        return mb_pdu_format_mask_wr_reg_resp(pdu, buf, len);
    case MB_PDU_RD_WR_MULT_REGS:
        return mb_pdu_format_rd_wr_mult_regs_resp(pdu, buf, len);
    case MB_PDU_RD_FIFO_Q:
        return mb_pdu_format_rd_fifo_q_resp(pdu, buf, len);
    case MB_PDU_ENC_IF_TRANS:
        return mb_pdu_format_enc_if_trans_resp(pdu, buf, len);
    default:
        if (pdu->func_code >= 0x80)
            return mb_pdu_format_err_resp(pdu, buf, len);
        else
            return -MB_PDU_EXCEPT_ILLEGAL_FUNC;
    }
    return 0;
}

static ssize_t mb_pdu_parse_rd_coils_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_coils = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->rd_coils_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_coils */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_coils = ntohs(val16);
    if ((quant_coils < MB_PDU_RD_COILS_MIN_QUANT_COILS)
     || (quant_coils > MB_PDU_RD_COILS_MAX_QUANT_COILS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_coils_req.quant_coils = quant_coils;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_coils */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_coils;
    if (end_addr > MB_PDU_RD_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rd_coils_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if (byte_count > MB_PDU_RD_COILS_MAX_BYTE_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_coils_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* coil_stat */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(pdu->rd_coils_resp.coil_stat, buf, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_parse_rd_disc_ips_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_ips = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->rd_disc_ips_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ips */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_ips = ntohs(val16);
    if ((quant_ips < MB_PDU_RD_DISC_IPS_MIN_QUANT_IPS)
     || (quant_ips > MB_PDU_RD_DISC_IPS_MAX_QUANT_IPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_disc_ips_req.quant_ips = quant_ips;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_ips */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ips;
    if (end_addr > MB_PDU_RD_DISC_IPS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rd_disc_ips_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if (byte_count > MB_PDU_RD_DISC_IPS_MAX_BYTE_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_disc_ips_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* coil_stat */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(pdu->rd_disc_ips_resp.ip_stat, buf, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_parse_rd_hold_regs_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_regs = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->rd_hold_regs_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_regs = ntohs(val16);
    if ((quant_regs < MB_PDU_RD_HOLD_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_RD_HOLD_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_hold_regs_req.quant_regs = quant_regs;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_ips */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_RD_HOLD_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rd_hold_regs_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((byte_count & 0x01)
     || (byte_count > MB_PDU_RD_HOLD_REGS_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_hold_regs_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* reg_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->rd_hold_regs_resp.reg_val;
    memcpy(p, buf, byte_count);
    for (i = 0; i < byte_count >> 1; i++)
    {
        *p = ntohs(*p);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_parse_rd_ip_regs_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_ip_regs = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->rd_ip_regs_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ip_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_ip_regs = ntohs(val16);
    if ((quant_ip_regs < MB_PDU_RD_IP_REGS_MIN_QUANT_IP_REGS)
     || (quant_ip_regs > MB_PDU_RD_IP_REGS_MAX_QUANT_IP_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_ip_regs_req.quant_ip_regs = quant_ip_regs;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_ips */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ip_regs;
    if (end_addr > MB_PDU_RD_IP_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rd_ip_regs_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((byte_count & 0x01)
     || (byte_count > MB_PDU_RD_IP_REGS_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_ip_regs_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* ip_reg */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->rd_ip_regs_resp.ip_reg;
    memcpy(p, buf, byte_count);
    for (i = 0; i < byte_count >> 1; i++)
    {
        *p = ntohs(*p);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_parse_wr_sing_coil_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t op_val = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* op_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_coil_req.op_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* op_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    op_val = ntohs(val16);
    if ((op_val != 0x0000)
     && (op_val != 0xff00))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_sing_coil_req.op_val = op_val ? true : false;
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_wr_sing_coil_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t op_val = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* op_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_coil_resp.op_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* op_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    op_val = ntohs(val16);
    if ((op_val != 0x0000)
     && (op_val != 0xff00))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_sing_coil_resp.op_val = op_val ? true : false;
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_wr_sing_reg_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* reg_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_reg_req.reg_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* reg_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_reg_req.reg_val = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_wr_sing_reg_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* reg_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_reg_resp.reg_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* reg_val */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->wr_sing_reg_resp.reg_val = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_rd_except_stat_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    ssize_t num = 0;

    /* out_data */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_except_stat_resp.out_data = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_parse_diag_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    /* sub_func */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->diag_req.sub_func = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* data */
    /* assume that all remaining data in buf belongs to this PDU */
    if ((len & 0x01)
     || (len < 2)
     || (len > MB_PDU_DIAG_MAX_NUM_DATA))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->diag_req.data;
    memcpy(p, buf, len);
    for (i = 0; i < len; i += 2)
    {
        *p = ntohs(*p);
        p++;
    }
    pdu->diag_req.num_data = len / 2;
    num += len;
    buf += len;
    len = 0;

    return num;
}

static ssize_t mb_pdu_parse_diag_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    /* sub_func */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->diag_resp.sub_func = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* data */
    /* assume that all remaining data in buf belongs to this PDU */
    if (len & 0x01)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (len > 2 * MB_PDU_DIAG_MAX_NUM_DATA)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->diag_resp.data;
    memcpy(p, buf, len);
    for (i = 0; i < len; i += 2)
    {
        *p = ntohs(*p);
        p++;
    }
    pdu->diag_resp.num_data = len / 2;
    num += len;
    buf += len;
    len = 0;

    return num;
}

static ssize_t mb_pdu_parse_get_com_ev_cntr_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* status */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->get_com_ev_cntr_resp.status = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* ev_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->get_com_ev_cntr_resp.ev_cnt = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_get_com_event_log_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    uint8_t byte_count = 0;
    uint8_t num_events = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if (byte_count < MB_PDU_GET_COM_EV_LOG_MIN_BYTE_COUNT)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->get_com_ev_log_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* status */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->get_com_ev_log_resp.status = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* ev_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->get_com_ev_log_resp.ev_cnt = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* msg_cnt */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->get_com_ev_log_resp.msg_cnt = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* events */
    num_events = byte_count - MB_PDU_GET_COM_EV_LOG_MIN_BYTE_COUNT;
    if (num_events > MB_PDU_GET_COM_EV_LOG_MAX_NUM_EVENTS)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (len < num_events)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(pdu->get_com_ev_log_resp.event, buf, num_events);
    num += num_events;
    buf += num_events;
    len -= num_events;

    return num;
}

static ssize_t mb_pdu_parse_wr_mult_coils_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t val16 = 0;
    uint16_t quant_ops_bytes = 0;
    uint16_t quant_ops = 0;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->wr_mult_coils_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ops */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_ops = ntohs(val16);
    if ((quant_ops < MB_PDU_WR_MULT_COILS_MIN_QUANT_OPS)
     || (quant_ops > MB_PDU_WR_MULT_COILS_MAX_QUANT_OPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_coils_req.quant_ops = quant_ops;
    num += 2;
    buf += 2;
    len -= 2;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    quant_ops_bytes = quant_ops >> 3;
    if (quant_ops & 0x0007)
        quant_ops_bytes++;
    if (quant_ops_bytes != byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_coils_req.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* op_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(pdu->wr_mult_coils_req.op_val, buf, byte_count);
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    /* start_addr + quant_ops */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ops;
    if (end_addr > MB_PDU_WR_MULT_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_wr_mult_coils_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_ops = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->wr_mult_coils_resp.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_ops */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_ops = ntohs(val16);
    if ((quant_ops < MB_PDU_WR_MULT_COILS_MIN_QUANT_OPS)
     || (quant_ops > MB_PDU_WR_MULT_COILS_MAX_QUANT_OPS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_coils_resp.quant_ops = quant_ops;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_ops */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_ops;
    if (end_addr > MB_PDU_WR_MULT_COILS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_wr_mult_regs_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_regs = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->wr_mult_regs_req.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_regs = ntohs(val16);
    if ((quant_regs < MB_PDU_WR_MULT_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_regs_req.quant_regs = quant_regs;
    num += 2;
    buf += 2;
    len -= 2;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((uint16_t)byte_count != 2 * quant_regs)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_regs_req.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* reg_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->wr_mult_regs_req.reg_val;
    memcpy(p, buf, byte_count);
    for (i = 0; i < quant_regs; i++)
    {
        *p = ntohs(*p);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    /* start_addr + quant_regs */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rep_server_id_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint8_t run_ind_status = 0;
    uint8_t server_id_len = 0;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((byte_count < MB_PDU_REP_SERVER_ID_MIN_BYTE_COUNT)
     || (byte_count > MB_PDU_REP_SERVER_ID_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rep_server_id_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* server_id */
    server_id_len = byte_count - 1;
    if (len < server_id_len)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(pdu->rep_server_id_resp.server_id, buf, server_id_len);
    num += server_id_len;
    buf += server_id_len;
    len -= server_id_len;

    /* run_ind_status */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    run_ind_status = buf[0];
    if ((run_ind_status != 0x00)
     && (run_ind_status != 0xff))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rep_server_id_resp.run_ind_status = run_ind_status ? true : false;
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

static ssize_t mb_pdu_parse_wr_mult_regs_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t start_addr = 0;
    uint16_t quant_regs = 0;
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    start_addr = ntohs(val16);
    pdu->wr_mult_regs_resp.start_addr = start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_regs */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_regs = ntohs(val16);
    if ((quant_regs < MB_PDU_WR_MULT_REGS_MIN_QUANT_REGS)
     || (quant_regs > MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_mult_regs_resp.quant_regs = quant_regs;
    num += 2;
    buf += 2;
    len -= 2;

    /* start_addr + quant_regs */
    end_addr = (uint32_t)start_addr + (uint32_t)quant_regs;
    if (end_addr > MB_PDU_WR_MULT_REGS_MAX_ADDR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    return num;
}

static ssize_t mb_pdu_parse_rd_file_rec_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    mb_pdu_rd_file_rec_req_sub_req_t sub_req = {0};
    unsigned i = 0;
    unsigned j = 0;
    uint32_t max_rec_num = 0;
    uint16_t val16 = 0;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((byte_count < MB_PDU_RD_FILE_REC_MIN_BYTE_COUNT)
     || (byte_count > MB_PDU_RD_FILE_REC_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_file_rec_req.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < byte_count; i += MB_PDU_RD_FILE_REC_REQ_SUB_REQ_NUM_BYTES, j++)
    {
        memset(&sub_req, 0, sizeof(mb_pdu_rd_file_rec_req_sub_req_t));

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        sub_req.ref_type = buf[0];
        if (sub_req.ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.file_num = ntohs(val16);
        if (sub_req.file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_num = ntohs(val16);
        if (sub_req.rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_len = ntohs(val16);
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num + rec_len */
        max_rec_num = (uint32_t)sub_req.rec_num + (uint32_t)sub_req.rec_len;
        if (max_rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

        /* sub_rec */
        memcpy(&pdu->rd_file_rec_req.sub_req[j], &sub_req, sizeof(mb_pdu_rd_file_rec_req_sub_req_t));
    }
    return num;
}

static ssize_t mb_pdu_parse_rd_file_rec_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    mb_pdu_rd_file_rec_resp_sub_req_t sub_req = {0};
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint16_t *p = NULL;
    uint8_t resp_data_len = 0;
    uint8_t file_resp_len = 0;
    uint8_t num_bytes = 0;
    ssize_t num = 0;

    /* resp_data_len */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    resp_data_len = buf[0];
    if ((resp_data_len < MB_PDU_RD_FILE_REC_MIN_RESP_DATA_LEN)
     || (resp_data_len > MB_PDU_RD_FILE_REC_MAX_RESP_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_file_rec_resp.resp_data_len = resp_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < resp_data_len; j++)
    {
        memset(&sub_req, 0, sizeof(mb_pdu_rd_file_rec_resp_sub_req_t));

        /* file_resp_len */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        file_resp_len = buf[0];
        if (((file_resp_len & 0x01) == 0)
         || (file_resp_len < MB_PDU_RD_FILE_REC_MIN_FILE_RESP_LEN)
         || (file_resp_len > MB_PDU_RD_FILE_REC_MAX_FILE_RESP_LEN))
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        sub_req.file_resp_len = file_resp_len;
        num += 1;
        buf += 1;
        len -= 1;

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        sub_req.ref_type = buf[0];
        if (sub_req.ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 1;
        buf += 1;
        len -= 1;

        /* rec_data */
        num_bytes = file_resp_len - 1;
        if (len < num_bytes)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        p = sub_req.rec_data;
        memcpy(p, buf, num_bytes);
        for (k = 0; k < num_bytes; k += 2)
        {
            *p = ntohs(*p);
            p++;
        }
        num += num_bytes;
        buf += num_bytes;
        len -= num_bytes;

        /* sub_req */
        memcpy(&pdu->rd_file_rec_resp.sub_req[j], &sub_req, sizeof(mb_pdu_rd_file_rec_resp_sub_req_t));

        i += file_resp_len + 1;
    }

    return num;
}

static ssize_t mb_pdu_parse_wr_file_rec_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    mb_pdu_wr_file_rec_sub_req_t sub_req = {0};
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint32_t max_rec_num = 0;
    uint16_t req_data_len = 0;
    uint16_t num_bytes = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    req_data_len = buf[0];
    if ((req_data_len < MB_PDU_WR_FILE_REC_MIN_REQ_DATA_LEN)
     || (req_data_len > MB_PDU_WR_FILE_REC_MAX_REQ_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_file_rec_req.req_data_len = req_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < req_data_len; j++)
    {
        memset(&sub_req, 0, sizeof(mb_pdu_wr_file_rec_sub_req_t));

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        sub_req.ref_type = buf[0];
        if (sub_req.ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.file_num = ntohs(val16);
        if (sub_req.file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_num = ntohs(val16);
        if (sub_req.rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_len = ntohs(val16);
        if (sub_req.rec_len > MB_PDU_WR_FILE_REC_MAX_REC_LEN)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num + rec_len */
        max_rec_num = (uint32_t)sub_req.rec_num + (uint32_t)sub_req.rec_len;
        if (max_rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

        /* rec_data */
        num_bytes = sub_req.rec_len * 2;
        if (len < num_bytes)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(sub_req.rec_data, buf, num_bytes);
        p = sub_req.rec_data;
        for (k = 0; k < sub_req.rec_len; k++)
        {
            *p = ntohs(*p);
            p++;
        }
        num += num_bytes;
        buf += num_bytes;
        len -= num_bytes;

        /* sub_rec */
        memcpy(&pdu->wr_file_rec_req.sub_req[j], &sub_req, sizeof(mb_pdu_wr_file_rec_sub_req_t));

        i += 7 + num_bytes;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + 2 * rec_len */
    }

    return num;
}

static ssize_t mb_pdu_parse_wr_file_rec_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    mb_pdu_wr_file_rec_sub_req_t sub_req = {0};
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    uint32_t max_rec_num = 0;
    uint16_t resp_data_len = 0;
    uint16_t num_bytes = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    ssize_t num = 0;

    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    resp_data_len = buf[0];
    if ((resp_data_len < MB_PDU_WR_FILE_REC_MIN_RESP_DATA_LEN)
     || (resp_data_len > MB_PDU_WR_FILE_REC_MAX_RESP_DATA_LEN))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->wr_file_rec_resp.resp_data_len = resp_data_len;
    num += 1;
    buf += 1;
    len -= 1;

    for (i = 0, j = 0; i < resp_data_len; j++)
    {
        memset(&sub_req, 0, sizeof(mb_pdu_wr_file_rec_sub_req_t));

        /* ref_type */
        if (len < 1)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        sub_req.ref_type = buf[0];
        if (sub_req.ref_type != MB_PDU_FILE_REC_REF_TYPE)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 1;
        buf += 1;
        len -= 1;

        /* file_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.file_num = ntohs(val16);
        if (sub_req.file_num < MB_PDU_FILE_REC_MIN_FILE_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_num = ntohs(val16);
        if (sub_req.rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_len */
        if (len < 2)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(&val16, buf, 2);
        sub_req.rec_len = ntohs(val16);
        if (sub_req.rec_len > MB_PDU_WR_FILE_REC_MAX_REC_LEN)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;
        num += 2;
        buf += 2;
        len -= 2;

        /* rec_num + rec_len */
        max_rec_num = (uint32_t)sub_req.rec_num + (uint32_t)sub_req.rec_len;
        if (max_rec_num > MB_PDU_FILE_REC_MAX_REC_NUM)
            return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

        /* rec_data */
        num_bytes = sub_req.rec_len * 2;
        if (len < num_bytes)
            return -MB_PDU_EXCEPT_ILLEGAL_VAL;
        memcpy(sub_req.rec_data, buf, num_bytes);
        p = sub_req.rec_data;
        for (k = 0; k < sub_req.rec_len; k++)
        {
            *p = ntohs(*p);
            p++;
        }
        num += num_bytes;
        buf += num_bytes;
        len -= num_bytes;

        /* sub_rec */
        memcpy(&pdu->wr_file_rec_resp.sub_req[j], &sub_req, sizeof(mb_pdu_wr_file_rec_sub_req_t));

        i += 7 + num_bytes;  /* sizeof(ref_type) + sizeof(file_num) + sizeof(rec_num) + sizeof(rec_len) + 2 * rec_len */
    }

    return num;
}

static ssize_t mb_pdu_parse_mask_wr_reg_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* ref_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_req.ref_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* and_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_req.and_mask = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* or_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_req.or_mask = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_mask_wr_reg_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* ref_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_resp.ref_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* and_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_resp.and_mask = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* or_mask */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->mask_wr_reg_resp.or_mask = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_rd_wr_mult_regs_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint32_t end_addr = 0;
    uint16_t rd_start_addr = 0;
    uint16_t wr_start_addr = 0;
    uint16_t quant_rd = 0;
    uint16_t quant_wr = 0;
    uint16_t val16 = 0;
    uint16_t *p = NULL;
    unsigned i = 0;
    uint8_t wr_byte_count = 0;
    ssize_t num = 0;

    /* rd_start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    rd_start_addr = ntohs(val16);
    pdu->rd_wr_mult_regs_req.rd_start_addr = rd_start_addr;
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_rd */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_rd = ntohs(val16);
    if ((quant_rd < MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_RD)
     || (quant_rd > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_RD))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_wr_mult_regs_req.quant_rd = quant_rd;
    num += 2;
    buf += 2;
    len -= 2;

    /* rd_start_addr + quant_rd */
    end_addr = (uint32_t)rd_start_addr + (uint32_t)quant_rd;
    if (end_addr > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_RD)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    /* wr_start_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->rd_wr_mult_regs_req.wr_start_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    /* quant_wr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    quant_wr = ntohs(val16);
    if ((quant_wr < MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_WR)
     || (quant_wr > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_WR))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_wr_mult_regs_req.quant_wr = quant_wr;
    num += 2;
    buf += 2;
    len -= 2;

    /* wr_start_addr + quant_wr */
    end_addr = (uint32_t)wr_start_addr + (uint32_t)quant_wr;
    if (end_addr > MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_WR)
        return -MB_PDU_EXCEPT_ILLEGAL_ADDR;

    /* wr_byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    wr_byte_count = buf[0];
    if (wr_byte_count != quant_wr * 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_wr_mult_regs_req.wr_byte_count = wr_byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* wr_reg_val */
    if (len < wr_byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->rd_wr_mult_regs_req.wr_reg_val;
    memcpy(p, buf, wr_byte_count);
    for (i = 0; i < quant_wr; i++)
    {
        *p = ntohs(*p);
        p++;
    }
    num += wr_byte_count;
    buf += wr_byte_count;
    len -= wr_byte_count;

    return num;
}

static ssize_t mb_pdu_parse_rd_wr_mult_regs_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t *p = NULL;
    uint8_t byte_count = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    byte_count = buf[0];
    if ((byte_count & 0x01)
     || (byte_count > MB_PDU_RD_WR_MULT_REGS_MAX_RD_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_wr_mult_regs_resp.byte_count = byte_count;
    num += 1;
    buf += 1;
    len -= 1;

    /* rd_regs_val */
    if (len < byte_count)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->rd_wr_mult_regs_resp.rd_reg_val;
    memcpy(p, buf, byte_count);
    for (i = 0; i < byte_count; i += 2)
    {
        *p = ntohs(*p);
        p++;
    }
    num += byte_count;
    buf += byte_count;
    len -= byte_count;

    return num;
}

static ssize_t mb_pdu_parse_rd_fifo_q_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    ssize_t num = 0;

    /* fifo_ptr_addr */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    pdu->rd_fifo_q_req.fifo_ptr_addr = ntohs(val16);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

static ssize_t mb_pdu_parse_rd_fifo_q_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint16_t val16 = 0;
    uint16_t byte_count = 0;
    uint16_t fifo_count = 0;
    uint16_t num_bytes = 0;
    uint16_t *p = NULL;
    unsigned i = 0;
    ssize_t num = 0;

    /* byte_count */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    byte_count = ntohs(val16);
    if ((byte_count & 0x0001)
     || (byte_count > MB_PDU_RD_FIFO_Q_MAX_BYTE_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_fifo_q_resp.byte_count = byte_count;
    num += 2;
    buf += 2;
    len -= 2;

    /* fifo_count */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(&val16, buf, 2);
    fifo_count = ntohs(val16);
    if (((fifo_count + 1) * 2 != byte_count)
     || (fifo_count > MB_PDU_RD_FIFO_Q_MAX_FIFO_COUNT))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->rd_fifo_q_resp.fifo_count = fifo_count;
    num += 2;
    buf += 2;
    len -= 2;

    /* fifo_val_reg */
    num_bytes = fifo_count * 2;
    if (len < num_bytes)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    p = pdu->rd_fifo_q_resp.fifo_val_reg;
    memcpy(p, buf, num_bytes);
    for (i = 0; i < num_bytes; i += 2)
    {
        *p = ntohs(*p);
        p++;
    }
    num += num_bytes;
    buf += num_bytes;
    len -= num_bytes;

    return num;
}

static ssize_t mb_pdu_parse_enc_if_tran_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    ssize_t num = 0;

    /* mei_type */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->enc_if_trans_req.mei_type = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_data */
    memcpy(pdu->enc_if_trans_req.mei_data, buf, len);
    pdu->enc_if_trans_req.mei_data_len = len;
    num += len;
    buf += len;
    len -= len;

    return num;
}

static ssize_t mb_pdu_parse_enc_if_tran_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    ssize_t num = 0;

    /* mei_type */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->enc_if_trans_resp.mei_type = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* mei_data */
    memcpy(pdu->enc_if_trans_resp.mei_data, buf, len);
    pdu->enc_if_trans_resp.mei_data_len = len;
    num += len;
    buf += len;
    len -= len;

    return num;
}

static ssize_t mb_pdu_parse_err_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    uint8_t except_code = 0;
    ssize_t num = 0;

    /* except_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    except_code = buf[0];
    switch (except_code)
    {
    case MB_PDU_EXCEPT_ILLEGAL_FUNC:
    case MB_PDU_EXCEPT_ILLEGAL_ADDR:
    case MB_PDU_EXCEPT_ILLEGAL_VAL:
    case MB_PDU_EXCEPT_SERVER_DEV_FAIL:
    case MB_PDU_EXCEPT_ACK:
    case MB_PDU_EXCEPT_SERVER_DEV_BUSY:
    case MB_PDU_EXCEPT_MEM_PARITY_ERROR:
    case MB_PDU_EXCEPT_GATEWAY_PATH_UNAVAIL:
    case MB_PDU_EXCEPT_GATEWAY_TARGET_NO_RESP:
        break;
    default:
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    }
    pdu->err.except_code = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    return num;
}

ssize_t mb_pdu_parse_req(mb_pdu_t *pdu, const char *buf, size_t len)
{
    ssize_t num = 0;
    ssize_t ret = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_REQ;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->func_code = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* data */
    switch (pdu->func_code)
    {
    case MB_PDU_RD_COILS:
        ret = mb_pdu_parse_rd_coils_req(pdu, buf, len);
        break;
    case MB_PDU_RD_DISC_IPS:
        ret = mb_pdu_parse_rd_disc_ips_req(pdu, buf, len);
        break;
    case MB_PDU_RD_HOLD_REGS:
        ret = mb_pdu_parse_rd_hold_regs_req(pdu, buf, len);
        break;
    case MB_PDU_RD_IP_REGS:
        ret = mb_pdu_parse_rd_ip_regs_req(pdu, buf, len);
        break;
    case MB_PDU_WR_SING_COIL:
        ret = mb_pdu_parse_wr_sing_coil_req(pdu, buf, len);
        break;
    case MB_PDU_WR_SING_REG:
        ret = mb_pdu_parse_wr_sing_reg_req(pdu, buf, len);
        break;
    case MB_PDU_RD_EXCEPT_STAT:
        break;
    case MB_PDU_DIAG:
        ret = mb_pdu_parse_diag_req(pdu, buf, len);
        break;
    case MB_PDU_GET_COM_EV_CNTR:
        break;
    case MB_PDU_GET_COM_EV_LOG:
        break;
    case MB_PDU_WR_MULT_COILS:
        ret = mb_pdu_parse_wr_mult_coils_req(pdu, buf, len);
        break;
    case MB_PDU_WR_MULT_REGS:
        ret = mb_pdu_parse_wr_mult_regs_req(pdu, buf, len);
        break;
    case MB_PDU_REP_SERVER_ID:
        break;
    case MB_PDU_RD_FILE_REC:
        ret = mb_pdu_parse_rd_file_rec_req(pdu, buf, len);
        break;
    case MB_PDU_WR_FILE_REC:
        ret = mb_pdu_parse_wr_file_rec_req(pdu, buf, len);
        break;
    case MB_PDU_MASK_WR_REG:
        ret = mb_pdu_parse_mask_wr_reg_req(pdu, buf, len);
        break;
    case MB_PDU_RD_WR_MULT_REGS:
        ret = mb_pdu_parse_rd_wr_mult_regs_req(pdu,buf, len);
        break;
    case MB_PDU_RD_FIFO_Q:
        ret = mb_pdu_parse_rd_fifo_q_req(pdu, buf, len);
        break;
    case MB_PDU_ENC_IF_TRANS:
        ret = mb_pdu_parse_enc_if_tran_req(pdu, buf, len);
        break;
    default:
        ret = MB_PDU_EXCEPT_ILLEGAL_FUNC;
    }
    if (ret < 0)
        return ret;
    return num + ret;
}

ssize_t mb_pdu_parse_resp(mb_pdu_t *pdu, const char *buf, size_t len)
{
    ssize_t num = 0;
    ssize_t ret = 0;

    memset(pdu, 0, sizeof(mb_pdu_t));
    pdu->type = MB_PDU_RESP;

    /* func_code */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu->func_code = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* data */
    switch (pdu->func_code)
    {
    case MB_PDU_RD_COILS:
        ret = mb_pdu_parse_rd_coils_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_DISC_IPS:
        ret = mb_pdu_parse_rd_disc_ips_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_HOLD_REGS:
        ret = mb_pdu_parse_rd_hold_regs_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_IP_REGS:
        ret = mb_pdu_parse_rd_ip_regs_resp(pdu, buf, len);
        break;
    case MB_PDU_WR_SING_COIL:
        ret = mb_pdu_parse_wr_sing_coil_resp(pdu, buf, len);
        break;
    case MB_PDU_WR_SING_REG:
        ret = mb_pdu_parse_wr_sing_reg_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_EXCEPT_STAT:
        ret = mb_pdu_parse_rd_except_stat_resp(pdu, buf, len);
        break;
    case MB_PDU_DIAG:
        ret = mb_pdu_parse_diag_resp(pdu, buf, len);
        break;
    case MB_PDU_GET_COM_EV_CNTR:
        ret = mb_pdu_parse_get_com_ev_cntr_resp(pdu, buf, len);
        break;
    case MB_PDU_GET_COM_EV_LOG:
        ret = mb_pdu_parse_get_com_event_log_resp(pdu, buf, len);
        break;
    case MB_PDU_WR_MULT_COILS:
        ret = mb_pdu_parse_wr_mult_coils_resp(pdu, buf, len);
        break;
    case MB_PDU_WR_MULT_REGS:
        ret = mb_pdu_parse_wr_mult_regs_resp(pdu, buf, len);
        break;
    case MB_PDU_REP_SERVER_ID:
        ret = mb_pdu_parse_rep_server_id_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_FILE_REC:
        ret = mb_pdu_parse_rd_file_rec_resp(pdu, buf, len);
        break;
    case MB_PDU_WR_FILE_REC:
        ret = mb_pdu_parse_wr_file_rec_resp(pdu, buf, len);
        break;
    case MB_PDU_MASK_WR_REG:
        ret = mb_pdu_parse_mask_wr_reg_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_WR_MULT_REGS:
        ret = mb_pdu_parse_rd_wr_mult_regs_resp(pdu, buf, len);
        break;
    case MB_PDU_RD_FIFO_Q:
        ret = mb_pdu_parse_rd_fifo_q_resp(pdu, buf, len);
        break;
    case MB_PDU_ENC_IF_TRANS:
        ret = mb_pdu_parse_enc_if_tran_resp(pdu, buf, len);
        break;
    default:
        if (pdu->func_code >= 0x80)
            ret = mb_pdu_parse_err_resp(pdu, buf, len);
        else
            ret = MB_PDU_EXCEPT_ILLEGAL_FUNC;
    }
    if (ret < 0)
        return ret;
    return num + ret;
}
