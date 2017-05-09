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

#ifndef MB_PDU_H
#define MB_PDU_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/*  protocol data unit
 *
 *  0           1      N <= 253
 *  +-----------+------+
 *  | func_code | data |
 *  +-----------+------+
 */

#define MB_PDU_PROTO_ID                           0
#define MB_PDU_MAX_DATA_LEN                       252
#define MB_PDU_RD_COILS_MAX_ADDR                  0x0000ffff
#define MB_PDU_RD_COILS_MIN_QUANT_COILS           1
#define MB_PDU_RD_COILS_MAX_QUANT_COILS           2000
#define MB_PDU_RD_COILS_MAX_BYTE_COUNT            250     /* MAX_QUANT_COILS / 8 */
#define MB_PDU_RD_DISC_IPS_MAX_ADDR               0x0000ffff
#define MB_PDU_RD_DISC_IPS_MIN_QUANT_IPS          1
#define MB_PDU_RD_DISC_IPS_MAX_QUANT_IPS          2000
#define MB_PDU_RD_DISC_IPS_MAX_BYTE_COUNT         250     /* MAX_QUANT_IPS / 8 */
#define MB_PDU_RD_HOLD_REGS_MAX_ADDR              0x0000ffff
#define MB_PDU_RD_HOLD_REGS_MIN_QUANT_REGS        1
#define MB_PDU_RD_HOLD_REGS_MAX_QUANT_REGS        125
#define MB_PDU_RD_HOLD_REGS_MAX_BYTE_COUNT        250     /* MAX_QUANT_REGS * 2 */
#define MB_PDU_RD_IP_REGS_MAX_ADDR                0x0000ffff
#define MB_PDU_RD_IP_REGS_MIN_QUANT_IP_REGS       1
#define MB_PDU_RD_IP_REGS_MAX_QUANT_IP_REGS       125
#define MB_PDU_RD_IP_REGS_MAX_BYTE_COUNT          230     /* MAX_QUANT_IP_REGS * 2 */
#define MB_PDU_WR_SING_COIL_OFF_VAL               0x0000
#define MB_PDU_WR_SING_COIL_ON_VAL                0xff00
#define MB_PDU_DIAG_MAX_NUM_DATA                  125     /* (MAX_DATA_LEN - 2) / 2 */
#define MB_PDU_GET_COM_EV_LOG_MIN_BYTE_COUNT      6
#define MB_PDU_GET_COM_EV_LOG_MAX_NUM_EVENTS      245     /* MAX_DATA_LEN - 7  */
#define MB_PDU_WR_MULT_COILS_MAX_ADDR             0x0000ffff
#define MB_PDU_WR_MULT_COILS_MIN_QUANT_OPS        1
#define MB_PDU_WR_MULT_COILS_MAX_QUANT_OPS        1968
#define MB_PDU_WR_MULT_COILS_MAX_BYTE_COUNT       246
#define MB_PDU_WR_MULT_REGS_MAX_ADDR              0x0000ffff
#define MB_PDU_WR_MULT_REGS_MIN_QUANT_REGS        1
#define MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS        123
#define MB_PDU_REP_SERVER_ID_MIN_BYTE_COUNT       1
#define MB_PDU_REP_SERVER_ID_MAX_BYTE_COUNT       251
#define MB_PDU_REP_SERVER_ID_MAX_DATA_LEN         250
#define MB_PDU_FILE_REC_REF_TYPE                  0x06
#define MB_PDU_FILE_REC_MIN_FILE_NUM              1
#define MB_PDU_FILE_REC_MAX_REC_NUM               0x270f
#define MB_PDU_RD_FILE_REC_REQ_SUB_REQ_NUM_BYTES  7
#define MB_PDU_RD_FILE_REC_MAX_NUM_SUB_REQ        35
#define MB_PDU_RD_FILE_REC_MIN_BYTE_COUNT         7
#define MB_PDU_RD_FILE_REC_MAX_BYTE_COUNT         245
#define MB_PDU_RD_FILE_REC_MIN_RESP_DATA_LEN      7
#define MB_PDU_RD_FILE_REC_MAX_RESP_DATA_LEN      245
#define MB_PDU_RD_FILE_REC_MIN_FILE_RESP_LEN      1
#define MB_PDU_RD_FILE_REC_MAX_FILE_RESP_LEN      245
#define MB_PDU_RD_FILE_REC_MAX_NUM_REC_DATA       122     /* (MAX_FILE_RESP_LEN - 1) / 2 */
#define MB_PDU_WR_FILE_REC_MAX_NUM_SUB_REQ        27
#define MB_PDU_WR_FILE_REC_MAX_REC_LEN            122
#define MB_PDU_WR_FILE_REC_MIN_REQ_DATA_LEN       9
#define MB_PDU_WR_FILE_REC_MAX_REQ_DATA_LEN       251
#define MB_PDU_WR_FILE_REC_MIN_RESP_DATA_LEN      9
#define MB_PDU_WR_FILE_REC_MAX_RESP_DATA_LEN      251
#define MB_PDU_RD_WR_MULT_REGS_MAX_ADDR           0x0000ffff
#define MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_RD       1
#define MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_RD       125
#define MB_PDU_RD_WR_MULT_REGS_MAX_RD_BYTE_COUNT  250     /* (MAX_QUANT_RD * 2) */
#define MB_PDU_RD_WR_MULT_REGS_MIN_QUANT_WR       1
#define MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_WR       121
#define MB_PDU_RD_WR_MULT_REGS_MAX_WR_BYTE_COUNT  242     /* (MAX_QUANT_WR * 2) */
#define MB_PDU_RD_FIFO_Q_MAX_FIFO_COUNT           31
#define MB_PDU_RD_FIFO_Q_MAX_BYTE_COUNT           64      /* (MAX_FIFO_COUNT + 1) * 2 */
#define MB_PDU_ENC_IF_TRANS_MAX_MEI_DATA_LEN      251     /* MAX_DATA_LEN - 1 */

typedef enum
{
    MB_PDU_EXCEPT_ILLEGAL_FUNC = 1,
    MB_PDU_EXCEPT_ILLEGAL_ADDR = 2,
    MB_PDU_EXCEPT_ILLEGAL_VAL = 3,
    MB_PDU_EXCEPT_SERVER_DEV_FAIL = 4,
    MB_PDU_EXCEPT_ACK = 5,
    MB_PDU_EXCEPT_SERVER_DEV_BUSY = 6,
    MB_PDU_EXCEPT_MEM_PARITY_ERROR = 8,
    MB_PDU_EXCEPT_GATEWAY_PATH_UNAVAIL = 10,
    MB_PDU_EXCEPT_GATEWAY_TARGET_NO_RESP = 11
}
mb_pdu_except_code_t;

typedef enum
{
    MB_PDU_DEF = 0,
    MB_PDU_REQ,
    MB_PDU_RESP,
    MB_PDU_ERR
}
mb_pdu_type_t;

/* Modbus Application Protocol Specification V1.1b3 */
typedef enum
{
    MB_PDU_RD_COILS = 0x01,                   /* Read Coils */
    MB_PDU_RD_DISC_IPS = 0x02,                /* Read Discrete Inputs */
    MB_PDU_RD_HOLD_REGS = 0x03,               /* Read Holding Registers */
    MB_PDU_RD_IP_REGS = 0x04,                 /* Read Input Register */
    MB_PDU_WR_SING_COIL = 0x05,               /* Write Single Coil */
    MB_PDU_WR_SING_REG = 0x06,                /* Write Single Register */
    MB_PDU_RD_EXCEPT_STAT = 0x07,             /* Read Exception Status */
    MB_PDU_DIAG = 0x08,                       /* Diagnostic */
    MB_PDU_GET_COM_EV_CNTR = 0x0b,            /* Get Com event counter */
    MB_PDU_GET_COM_EV_LOG = 0x0c,             /* Get Com Event Log */
    MB_PDU_WR_MULT_COILS = 0x0f,              /* Write Multiple Coils */
    MB_PDU_WR_MULT_REGS = 0x10,               /* Write Multiple Registers */
    MB_PDU_REP_SERVER_ID = 0x11,              /* Report Server ID */
    MB_PDU_RD_FILE_REC = 0x14,                /* Read File Record */
    MB_PDU_WR_FILE_REC = 0x15,                /* Write File Record */
    MB_PDU_MASK_WR_REG = 0x16,                /* Mask Write Register */
    MB_PDU_RD_WR_MULT_REGS = 0x17,            /* Read/Write Multiple Registers */
    MB_PDU_RD_FIFO_Q = 0x18,                  /* Read FIFO queue */
    MB_PDU_ENC_IF_TRANS = 0x2b                /* Encapsulated Interface Transport */
}
mb_pdu_func_code_t;

/* Modbus over Serial Line Specification and Implementation Guide V1.02 */
/* Modbus Application Protocol Specification V1.1b3 */
typedef enum
{
    MB_PDU_QUERY_DATA = 0x00,                 /* Return Query Data */
    MB_PDU_CLEAR_COUNTERS = 0x0a,             /* Clear Counters and Diagnostic Register */
    MB_PDU_BUS_MSG_COUNT = 0x0b,              /* Return Bus Message Count */
    MB_PDU_BUS_COM_ERR_COUNT = 0x0c,          /* Return Bus Communication Error Count */
    MB_PDU_SLAVE_EXCEP_ERR_COUNT = 0x0d,      /* Slave Exception Error Count */
    MB_PDU_SLAVE_MSG_COUNT = 0x0e,            /* Slave Message Count */
    MB_PDU_SLAVE_NO_RESP_COUNT = 0x0f         /* Slave No Response Count */
}
mb_pdu_diag_sub_func_code_t;

typedef struct
{
    uint8_t buf[MB_PDU_MAX_DATA_LEN];
}
mb_pdu_def_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_coils;
}
mb_pdu_rd_coils_req_t;

typedef struct
{
    uint8_t byte_count;
    uint8_t coil_stat[MB_PDU_RD_COILS_MAX_BYTE_COUNT];
}
mb_pdu_rd_coils_resp_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_ips;
}
mb_pdu_rd_disc_ips_req_t;

typedef struct
{
    uint8_t byte_count;
    uint8_t ip_stat[MB_PDU_RD_DISC_IPS_MAX_BYTE_COUNT];
}
mb_pdu_rd_disc_ips_resp_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_regs;
}
mb_pdu_rd_hold_regs_req_t;

typedef struct
{
    uint8_t byte_count;
    uint16_t reg_val[MB_PDU_RD_HOLD_REGS_MAX_QUANT_REGS];
}
mb_pdu_rd_hold_regs_resp_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_ip_regs;
}
mb_pdu_rd_ip_regs_req_t;

typedef struct
{
    uint8_t byte_count;
    uint16_t ip_reg[MB_PDU_RD_IP_REGS_MAX_QUANT_IP_REGS];
}
mb_pdu_rd_ip_regs_resp_t;

typedef struct
{
    uint16_t op_addr;
    bool op_val;
}
mb_pdu_wr_sing_coil_t;

typedef struct
{
    uint16_t reg_addr;
    uint16_t reg_val;
}
mb_pdu_wr_sing_reg_t;

typedef struct
{
}
mb_pdu_rd_except_stat_req_t;

typedef struct
{
    uint8_t out_data;
}
mb_pdu_rd_except_stat_resp_t;

typedef struct
{
    uint16_t sub_func;
    uint16_t data[MB_PDU_DIAG_MAX_NUM_DATA];
    uint8_t num_data;
}
mb_pdu_diag_t;

typedef struct
{
}
mb_pdu_get_com_ev_cntr_req_t;

typedef struct
{
    uint16_t status;
    uint16_t ev_cnt;
}
mb_pdu_get_com_ev_cntr_resp_t;

typedef struct
{
}
mb_pdu_get_com_ev_log_req_t;

typedef struct
{
    uint8_t byte_count;
    uint16_t status;
    uint16_t ev_cnt;
    uint16_t msg_cnt;
    uint8_t event[MB_PDU_GET_COM_EV_LOG_MAX_NUM_EVENTS];
}
mb_pdu_get_com_ev_log_resp_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_ops;
    uint8_t byte_count;
    uint8_t op_val[MB_PDU_WR_MULT_COILS_MAX_BYTE_COUNT];
}
mb_pdu_wr_mult_coils_req_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_ops;
}
mb_pdu_wr_mult_coils_resp_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_regs;
    uint8_t byte_count;
    uint16_t reg_val[MB_PDU_WR_MULT_REGS_MAX_QUANT_REGS];
}
mb_pdu_wr_mult_regs_req_t;

typedef struct
{
    uint16_t start_addr;
    uint16_t quant_regs;
}
mb_pdu_wr_mult_regs_resp_t;

typedef struct
{
}
mb_pdu_rep_server_id_req_t;

typedef struct
{
    uint8_t byte_count;
    uint8_t server_id[MB_PDU_REP_SERVER_ID_MAX_DATA_LEN - 1];
    bool run_ind_status;
}
mb_pdu_rep_server_id_resp_t;

typedef struct
{
    uint8_t ref_type;
    uint16_t file_num;
    uint16_t rec_num;
    uint16_t rec_len;
}
mb_pdu_rd_file_rec_req_sub_req_t;

typedef struct
{
    uint8_t byte_count;
    mb_pdu_rd_file_rec_req_sub_req_t sub_req[MB_PDU_RD_FILE_REC_MAX_NUM_SUB_REQ];
}
mb_pdu_rd_file_rec_req_t;

typedef struct
{
    uint8_t file_resp_len;
    uint8_t ref_type;
    uint16_t rec_data[MB_PDU_RD_FILE_REC_MAX_NUM_REC_DATA];
}
mb_pdu_rd_file_rec_resp_sub_req_t;

typedef struct
{
    uint8_t resp_data_len;
    mb_pdu_rd_file_rec_resp_sub_req_t sub_req[MB_PDU_RD_FILE_REC_MAX_NUM_SUB_REQ];
}
mb_pdu_rd_file_rec_resp_t;

typedef struct
{
    uint8_t ref_type;
    uint16_t file_num;
    uint16_t rec_num;
    uint16_t rec_len;
    uint16_t rec_data[MB_PDU_WR_FILE_REC_MAX_REC_LEN];
}
mb_pdu_wr_file_rec_sub_req_t;

typedef struct
{
    uint8_t req_data_len;
    mb_pdu_wr_file_rec_sub_req_t sub_req[MB_PDU_WR_FILE_REC_MAX_NUM_SUB_REQ];
}
mb_pdu_wr_file_rec_req_t;

typedef struct
{
    uint8_t resp_data_len;
    mb_pdu_wr_file_rec_sub_req_t sub_req[MB_PDU_WR_FILE_REC_MAX_NUM_SUB_REQ];
}
mb_pdu_wr_file_rec_resp_t;

typedef struct
{
    uint16_t ref_addr;
    uint16_t and_mask;
    uint16_t or_mask;
}
mb_pdu_mask_wr_reg_t;

typedef struct
{
    uint16_t rd_start_addr;
    uint16_t quant_rd;
    uint16_t wr_start_addr;
    uint16_t quant_wr;
    uint8_t wr_byte_count;
    uint16_t wr_reg_val[MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_WR];
}
mb_pdu_rd_wr_mult_regs_req_t;

typedef struct
{
    uint8_t byte_count;
    uint16_t rd_reg_val[MB_PDU_RD_WR_MULT_REGS_MAX_QUANT_RD];
}
mb_pdu_rd_wr_mult_regs_resp_t;

typedef struct
{
    uint16_t fifo_ptr_addr;
}
mb_pdu_rd_fifo_q_req_t;

typedef struct
{
    uint16_t byte_count;
    uint16_t fifo_count;
    uint16_t fifo_val_reg[MB_PDU_RD_FIFO_Q_MAX_FIFO_COUNT];
}
mb_pdu_rd_fifo_q_resp_t;

typedef struct
{
    uint8_t mei_type;
    uint8_t mei_data[MB_PDU_ENC_IF_TRANS_MAX_MEI_DATA_LEN];
    uint8_t mei_data_len;
}
mb_pdu_enc_if_trans_t;

typedef struct
{
    uint8_t except_code;
}
mb_pdu_err_t;

typedef struct
{
    uint8_t func_code;
    mb_pdu_type_t type;
    union
    {
        mb_pdu_def_t def;
        mb_pdu_rd_coils_req_t rd_coils_req;
        mb_pdu_rd_coils_resp_t rd_coils_resp;
        mb_pdu_rd_disc_ips_req_t rd_disc_ips_req;
        mb_pdu_rd_disc_ips_resp_t rd_disc_ips_resp;
        mb_pdu_rd_hold_regs_req_t rd_hold_regs_req;
        mb_pdu_rd_hold_regs_resp_t rd_hold_regs_resp;
        mb_pdu_rd_ip_regs_req_t rd_ip_regs_req;
        mb_pdu_rd_ip_regs_resp_t rd_ip_regs_resp;
        mb_pdu_wr_sing_coil_t wr_sing_coil_req;
        mb_pdu_wr_sing_coil_t wr_sing_coil_resp;
        mb_pdu_wr_sing_reg_t wr_sing_reg_req;
        mb_pdu_wr_sing_reg_t wr_sing_reg_resp;
        mb_pdu_rd_except_stat_req_t rd_except_stat_req;
        mb_pdu_rd_except_stat_resp_t rd_except_stat_resp;
        mb_pdu_diag_t diag_req;
        mb_pdu_diag_t diag_resp;
        mb_pdu_get_com_ev_cntr_req_t get_com_ev_cntr_req;
        mb_pdu_get_com_ev_cntr_resp_t get_com_ev_cntr_resp;
        mb_pdu_get_com_ev_log_req_t get_com_ev_log_req;
        mb_pdu_get_com_ev_log_resp_t get_com_ev_log_resp;
        mb_pdu_wr_mult_coils_req_t wr_mult_coils_req;
        mb_pdu_wr_mult_coils_resp_t wr_mult_coils_resp;
        mb_pdu_wr_mult_regs_req_t wr_mult_regs_req;
        mb_pdu_wr_mult_regs_resp_t wr_mult_regs_resp;
        mb_pdu_rep_server_id_req_t rep_server_id_req;
        mb_pdu_rep_server_id_resp_t rep_server_id_resp;
        mb_pdu_rd_file_rec_req_t rd_file_rec_req;
        mb_pdu_rd_file_rec_resp_t rd_file_rec_resp;
        mb_pdu_wr_file_rec_req_t wr_file_rec_req;
        mb_pdu_wr_file_rec_resp_t wr_file_rec_resp;
        mb_pdu_mask_wr_reg_t mask_wr_reg_req;
        mb_pdu_mask_wr_reg_t mask_wr_reg_resp;
        mb_pdu_rd_wr_mult_regs_req_t rd_wr_mult_regs_req;
        mb_pdu_rd_wr_mult_regs_resp_t rd_wr_mult_regs_resp;
        mb_pdu_rd_fifo_q_req_t rd_fifo_q_req;
        mb_pdu_rd_fifo_q_resp_t rd_fifo_q_resp;
        mb_pdu_enc_if_trans_t enc_if_trans_req;
        mb_pdu_enc_if_trans_t enc_if_trans_resp;
        mb_pdu_err_t err;
    };
    uint16_t data_len;
}
mb_pdu_t;

int mb_pdu_set(mb_pdu_t *pdu, mb_pdu_type_t type, uint8_t func_code, const uint8_t *data, uint16_t data_len);
int mb_pdu_set_rd_coils_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_coils);
int mb_pdu_set_rd_coils_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *coil_stat);
int mb_pdu_set_rd_disc_ips_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ips);
int mb_pdu_set_rd_disc_ips_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *ip_stat);
int mb_pdu_set_rd_hold_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs);
int mb_pdu_set_rd_hold_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *reg_val);
int mb_pdu_set_rd_ip_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ip_regs);
int mb_pdu_set_rd_ip_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *ip_reg);
void mb_pdu_set_wr_sing_coil_req(mb_pdu_t *pdu, uint16_t op_addr, bool op_val);
void mb_pdu_set_wr_sing_coil_resp(mb_pdu_t *pdu, uint16_t op_addr, bool op_val);
void mb_pdu_set_wr_sing_reg_req(mb_pdu_t *pdu, uint16_t reg_addr, uint16_t reg_val);
void mb_pdu_set_wr_sing_reg_resp(mb_pdu_t *pdu, uint16_t reg_addr, uint16_t reg_val);
void mb_pdu_set_rd_except_stat_req(mb_pdu_t *pdu);
void mb_pdu_set_rd_except_stat_resp(mb_pdu_t *pdu, uint8_t out_data);
int mb_pdu_set_diag_req(mb_pdu_t *pdu, uint16_t sub_func, const uint16_t *data, uint8_t num_data);
int mb_pdu_set_diag_resp(mb_pdu_t *pdu, uint16_t sub_func, const uint16_t *data, uint8_t num_data);
void mb_pdu_set_get_com_ev_cntr_req(mb_pdu_t *pdu);
void mb_pdu_set_get_com_ev_cntr_resp(mb_pdu_t *pdu, uint16_t status, uint16_t ev_cnt);
void mb_pdu_set_get_com_ev_log_req(mb_pdu_t *pdu);
int mb_pdu_set_get_com_ev_log_resp(mb_pdu_t *pdu, uint16_t status, uint16_t ev_cnt, uint16_t msg_cnt, const uint8_t *event, uint8_t num_events);
int mb_pdu_set_wr_mult_coils_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ops, const uint8_t *op_val);
int mb_pdu_set_wr_mult_coils_resp(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_ops);
int mb_pdu_set_wr_mult_regs_req(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs, uint8_t byte_count, const uint16_t *reg_val);
int mb_pdu_set_wr_mult_regs_resp(mb_pdu_t *pdu, uint16_t start_addr, uint16_t quant_regs);
void mb_pdu_set_rep_server_id_req(mb_pdu_t *pdu);
int mb_pdu_set_rep_server_id_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint8_t *server_id, bool run_ind_status);
int mb_pdu_set_rd_file_rec_req(mb_pdu_t *pdu, const mb_pdu_rd_file_rec_req_sub_req_t *sub_req, size_t num_sub_req);
int mb_pdu_set_rd_file_rec_resp(mb_pdu_t *pdu, const mb_pdu_rd_file_rec_resp_sub_req_t *sub_req, size_t num_sub_req);
int mb_pdu_set_wr_file_rec_req(mb_pdu_t *pdu, const mb_pdu_wr_file_rec_sub_req_t *sub_req, size_t num_sub_req);
int mb_pdu_set_wr_file_rec_resp(mb_pdu_t *pdu, const mb_pdu_wr_file_rec_sub_req_t *sub_req, size_t num_sub_req);
void mb_pdu_set_mask_wr_reg_req(mb_pdu_t *pdu, uint16_t ref_addr, uint16_t and_mask, uint16_t or_mask);
void mb_pdu_set_mask_wr_reg_resp(mb_pdu_t *pdu, uint16_t ref_addr, uint16_t and_mask, uint16_t or_mask);
int mb_pdu_set_rd_wr_mult_regs_req(mb_pdu_t *pdu, uint16_t rd_start_addr, uint16_t quant_rd, uint16_t wr_start_addr, uint16_t quant_wr, const uint16_t *wr_reg_val);
int mb_pdu_set_rd_wr_mult_regs_resp(mb_pdu_t *pdu, uint8_t byte_count, const uint16_t *rd_reg_val);
void mb_pdu_set_rd_fifo_q_req(mb_pdu_t *pdu, uint16_t fifo_ptr_addr);
int mb_pdu_set_rd_fifo_q_resp(mb_pdu_t *pdu, uint16_t fifo_count, const uint16_t *fifo_val_reg);
int mb_pdu_set_enc_if_trans_req(mb_pdu_t *pdu, uint8_t mei_type, const uint8_t *mei_data, uint8_t mei_data_len);
int mb_pdu_set_enc_if_trans_resp(mb_pdu_t *pdu, uint8_t mei_type, const uint8_t *mei_data, uint8_t mei_data_len);
int mb_pdu_set_err_resp(mb_pdu_t *pdu, uint8_t func_code, uint8_t except_code);

ssize_t mb_pdu_format_req(mb_pdu_t *pdu, char *buf, size_t len);
ssize_t mb_pdu_format_resp(mb_pdu_t *pdu, char *buf, size_t len);

ssize_t mb_pdu_parse_req(mb_pdu_t *pdu, const char *buf, size_t len);
ssize_t mb_pdu_parse_resp(mb_pdu_t *pdu, const char *buf, size_t len);

#endif
