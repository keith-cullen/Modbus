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

#include <stdio.h>
#include <string.h>
#include "mb_pdu.h"
#include "mb_test.h"

int print_cols = 93;

mb_test_result_t test_mb_pdu_set(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_type_t type = MB_PDU_DEF;
    const uint8_t func_code = 0x04;
    const uint8_t data[] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8};
    int ret = 0;

    printf("%-*s", print_cols, "test   1: set PDU");
    ret = mb_pdu_set(&pdu, type, func_code, data, sizeof(data));
    if (ret < 0)
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.type != type)
    {
        return FAIL;
    }
    if (memcmp((char *)pdu.def.buf, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    if (pdu.data_len != sizeof(data))
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 0x0123;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x01, 0xab, 0xcd, 0x01, 0x23};

    printf("%-*s", print_cols, "test   2: set and format 'Read Coils' request PDU");
    num = mb_pdu_set_rd_coils_req(&pdu, start_addr, quant_coils);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_req_invalid_quant_coils1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test   3: set 'Read Coils' request PDU with invalid quant_coils");
    ret = mb_pdu_set_rd_coils_req(&pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_req_invalid_quant_coils2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 2001;
    int ret = 0;

    printf("%-*s", print_cols, "test   4: set 'Read Coils' request PDU with invalid quant_coils");
    ret = mb_pdu_set_rd_coils_req(&pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_coils = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test   5: set 'Read Coils' request PDU with invalid end_addr");
    ret = mb_pdu_set_rd_coils_req(&pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t byte_count = 3;
    const uint8_t coil_stat[] = {0xaa, 0x55, 0x01};
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x01, 0x03, 0xaa, 0x55, 0x01};

    printf("%-*s", print_cols, "test   6: set and format 'Read Coils' response PDU");
    num = mb_pdu_set_rd_coils_resp(&pdu, byte_count, coil_stat);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_coils_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 255;
    const uint8_t coil_stat[255] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test   7: set 'Read Coils' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_coils_resp(&pdu, byte_count, coil_stat);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 0x0123;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x02, 0xab, 0xcd, 0x01, 0x23};

    printf("%-*s", print_cols, "test   8: set and format 'Read Discrete Inputs' request PDU");
    num = mb_pdu_set_rd_disc_ips_req(&pdu, start_addr, quant_ips);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_req_invalid_num_ips1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test   9: set 'Read Discrete Inputs' request PDU with invalid num_ips");
    ret = mb_pdu_set_rd_disc_ips_req(&pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_req_invalid_num_ips2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 2001;
    int ret = 0;

    printf("%-*s", print_cols, "test  10: set 'Read Discrete Inputs' request PDU with invalid num_ips");
    ret = mb_pdu_set_rd_disc_ips_req(&pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ips = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  11: set 'Read Discrete Inputs' request PDU with invalid end_addr");
    ret = mb_pdu_set_rd_disc_ips_req(&pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t byte_count = 3;
    const uint8_t ip_stat[] = {0xaa, 0x55, 0x01};
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x02, 0x03, 0xaa, 0x55, 0x01};

    printf("%-*s", print_cols, "test  12: set and format 'Read Discrete Inputs' response PDU");
    num = mb_pdu_set_rd_disc_ips_resp(&pdu, byte_count, ip_stat);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_disc_ips_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 255;
    const uint8_t ip_stat[255] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  13: set 'Read Discrete Inputs' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_disc_ips_resp(&pdu, byte_count, ip_stat);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x03, 0xab, 0xcd, 0x00, 0x02};

    printf("%-*s", print_cols, "test  14: set and format 'Read Holding Registers' request PDU");
    num = mb_pdu_set_rd_hold_regs_req(&pdu, start_addr, quant_regs);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_req_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  15: set 'Read Holding Registers' request PDU with invalid quant_regs");
    ret = mb_pdu_set_rd_hold_regs_req(&pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_req_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  16: set 'Read Holding Registers' request PDU with invalid quant_regs");
    ret = mb_pdu_set_rd_hold_regs_req(&pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  17: set 'Read Holding Registers' request PDU with invalid end_addr");
    ret = mb_pdu_set_rd_hold_regs_req(&pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t byte_count = 4;
    const uint16_t hold_reg[] = {0x1234, 0x5678};
    ssize_t num = 0;
    char buf[6] = {0};
    char exp[] = {0x03, 0x04, 0x12, 0x34, 0x56, 0x78};

    printf("%-*s", print_cols, "test  18: set and format 'Read Holding Registers' response PDU");
    num = mb_pdu_set_rd_hold_regs_resp(&pdu, byte_count, hold_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_hold_regs_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 252;
    const uint16_t hold_reg[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  19: set 'Read Holding Registers' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_hold_regs_resp(&pdu, byte_count, hold_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t num_ip_regs = 0x0002;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x04, 0xab, 0xcd, 0x00, 0x02};

    printf("%-*s", print_cols, "test  20: set and format 'Read Input Registers' request PDU");
    num = mb_pdu_set_rd_ip_regs_req(&pdu, start_addr, num_ip_regs);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_req_invalid_quant_ip_regs1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ip_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  21: set 'Read Input Registers' request PDU with invalid quant_ip_regs");
    ret = mb_pdu_set_rd_ip_regs_req(&pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_req_invalid_quant_ip_regs2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ip_regs = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  22: set 'Read Input Registers' request PDU with invalid quant_ip_regs");
    ret = mb_pdu_set_rd_ip_regs_req(&pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ip_regs = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  23: set 'Read Input Registers' request PDU with invalid end_addr");
    ret = mb_pdu_set_rd_ip_regs_req(&pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t byte_count = 4;
    const uint16_t ip_reg[] = {0x1234, 0x5678};
    ssize_t num = 0;
    char buf[6] = {0};
    char exp[] = {0x04, 0x04, 0x12, 0x34, 0x56, 0x78};

    printf("%-*s", print_cols, "test  24: set and format 'Read Input Registers' response PDU");
    num = mb_pdu_set_rd_ip_regs_resp(&pdu, byte_count, ip_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_ip_regs_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 252;
    const uint16_t ip_reg[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  25: set 'Read Input Registers' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_ip_regs_resp(&pdu, byte_count, ip_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_sing_coil_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t op_addr = 0xabcd;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x05, 0xab, 0xcd, 0xff, 0x00};

    printf("%-*s", print_cols, "test  26: set and format 'Write Single Coil' request PDU");
    mb_pdu_set_wr_sing_coil_req(&pdu, op_addr, op_val);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_sing_coil_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t op_addr = 0xabcd;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x05, 0xab, 0xcd, 0xff, 0x00};

    printf("%-*s", print_cols, "test  27: set and format 'Write Single Coil' response PDU");
    mb_pdu_set_wr_sing_coil_resp(&pdu, op_addr, op_val);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_sing_reg_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t reg_addr = 0xabcd;
    const uint16_t reg_val = 0x1234;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x06, 0xab, 0xcd, 0x12, 0x34};

    printf("%-*s", print_cols, "test  28: set and format 'Write Single Register' request PDU");
    mb_pdu_set_wr_sing_reg_req(&pdu, reg_addr, reg_val);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_sing_reg_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t reg_addr = 0xabcd;
    const uint16_t reg_val = 0x1234;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x06, 0xab, 0xcd, 0x12, 0x34};

    printf("%-*s", print_cols, "test  29: set and format 'Write Single Register' response PDU");
    mb_pdu_set_wr_sing_reg_resp(&pdu, reg_addr, reg_val);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_except_stat_req(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[1] = {0};
    char exp[] = {0x07};

    printf("%-*s", print_cols, "test  30: set and format 'Read Exception Status' request PDU");
    mb_pdu_set_rd_except_stat_req(&pdu);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_except_stat_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t out_data = 0x6d;
    ssize_t num = 0;
    char buf[2] = {0};
    char exp[] = {0x07, 0x6d};

    printf("%-*s", print_cols, "test  31: set and format 'Read Exception Status' response PDU");
    mb_pdu_set_rd_except_stat_resp(&pdu, out_data);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_diag_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t sub_func = 0x0001;
    const uint16_t data[1] = {0xa537};
    const uint8_t num_data = 1;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x08, 0x00, 0x01, 0xa5, 0x37};

    printf("%-*s", print_cols, "test  32: set and format 'Diagnostics' request PDU");
    num = mb_pdu_set_diag_req(&pdu, sub_func, data, num_data);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_diag_req_invalid_num_data(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t sub_func = 0x0001;
    const uint16_t data[126] = {0};
    const uint8_t num_data = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  33: set 'Diagnostics' request PDU with invalid num_data");
    ret = mb_pdu_set_diag_req(&pdu, sub_func, data, num_data);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_diag_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t sub_func = 0x0001;
    const uint16_t data[1] = {0xa537};
    const uint8_t num_data = 1;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x08, 0x00, 0x01, 0xa5, 0x37};

    printf("%-*s", print_cols, "test  34: set and format 'Diagnostics' response PDU");
    num = mb_pdu_set_diag_resp(&pdu, sub_func, data, num_data);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_diag_resp_invalid_num_data(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t sub_func = 0x0001;
    const uint16_t data[126] = {0};
    const uint8_t num_data = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  35: set 'Diagnostics' response PDU with invalid num_data");
    ret = mb_pdu_set_diag_resp(&pdu, sub_func, data, num_data);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_get_com_ev_cntr_req(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[1] = {0};
    char exp[] = {0x0b};

    printf("%-*s", print_cols, "test  36: set and format 'Get Comm Event Counter' request PDU");
    mb_pdu_set_get_com_ev_cntr_req(&pdu);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_get_com_ev_cntr_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t status = 0xffff;
    const uint16_t ev_cnt = 0x0108;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x0b, 0xff, 0xff, 0x01, 0x08};

    printf("%-*s", print_cols, "test  37: set and format 'Get Comm Event Counter' response PDU");
    mb_pdu_set_get_com_ev_cntr_resp(&pdu, status, ev_cnt);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_get_com_ev_log_req(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[1] = {0};
    char exp[] = {0x0c};

    printf("%-*s", print_cols, "test  38: set and format 'Get Comm Event Log' request PDU");
    mb_pdu_set_get_com_ev_log_req(&pdu);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_get_com_ev_log_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[2] = {0x20, 0x00};
    const uint8_t num_events = 2;
    ssize_t num = 0;
    char buf[10] = {0};
    char exp[] = {0x0c, 0x08, 0x00, 0x00, 0x01, 0x08, 0x01, 0x21, 0x20, 0x00};

    printf("%-*s", print_cols, "test  39: set and format 'Get Comm Event Log' response PDU");
    num = mb_pdu_set_get_com_ev_log_resp(&pdu, status, ev_cnt, msg_cnt, event, num_events);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_get_com_ev_log_resp_invalid_num_events(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[246] = {0};
    const uint8_t num_events = 246;
    int ret = 0;

    printf("%-*s", print_cols, "test  40: set 'Get Comm Event Log' response PDU with invalid num_events");
    ret = mb_pdu_set_get_com_ev_log_resp(&pdu, status, ev_cnt, msg_cnt, event, num_events);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0x000b;
    const uint8_t op_val[] = {0xa5, 0x09};
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x0f, 0xab, 0xcd, 0x00, 0x0b, 0x02, 0xa5, 0x09};

    printf("%-*s", print_cols, "test  41: set and format 'Write Multiple Coils' request PDU");
    num = mb_pdu_set_wr_mult_coils_req(&pdu, start_addr, quant_ops, op_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_req_invalid_quant_ops1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0;
    uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  42: set 'Write Multiple Coils' request PDU with invalid quant_ops");
    ret = mb_pdu_set_wr_mult_coils_req(&pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_req_invalid_quant_ops2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 1969;
    const uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  43: set 'Write Multiple Coils' request PDU with invalid quant_ops");
    ret = mb_pdu_set_wr_mult_coils_req(&pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ops = 1;
    const uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  44: set 'Write Multiple Coils' request PDU with invalid end_addr");
    ret = mb_pdu_set_wr_mult_coils_req(&pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0x000b;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x0f, 0xab, 0xcd, 0x00, 0x0b};

    printf("%-*s", print_cols, "test  45: set and format 'Write Multiple Coils' response PDU");
    mb_pdu_set_wr_mult_coils_resp(&pdu, start_addr, quant_ops);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_resp_invalid_quant_ops1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  46: set 'Write Multiple Coils' response PDU with invalid quant_ops");
    ret = mb_pdu_set_wr_mult_coils_resp(&pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_resp_invalid_quant_ops2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 1969;
    int ret = 0;

    printf("%-*s", print_cols, "test  47: set 'Write Multiple Coils' response PDU with invalid quant_ops");
    ret = mb_pdu_set_wr_mult_coils_resp(&pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_coils_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ops = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  48: set 'Write Multiple Coils' response PDU with invalid end_addr");
    ret = mb_pdu_set_wr_mult_coils_resp(&pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    const uint8_t byte_count = 0x04;
    const uint16_t reg_val[] = {0xa519, 0xc5e2};
    ssize_t num = 0;
    char buf[10] = {0};
    char exp[] = {0x10, 0xab, 0xcd, 0x00, 0x02, 0x04, 0xa5, 0x19, 0xc5, 0xe2};

    printf("%-*s", print_cols, "test  49: set and format 'Write Multiple Registers' request PDU");
    num = mb_pdu_set_wr_mult_regs_req(&pdu, start_addr, quant_regs, byte_count, reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_req_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    const uint8_t byte_count = 0;
    const uint16_t reg_val[124] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  50: set 'Write Multiple Registers' request PDU with invalid quant_regs");
    ret = mb_pdu_set_wr_mult_regs_req(&pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_req_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 124;
    const uint8_t byte_count = 248;
    const uint16_t reg_val[124] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  51: set 'Write Multiple Registers' request PDU with invalid quant_regs");
    ret = mb_pdu_set_wr_mult_regs_req(&pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_req_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 2;
    const uint8_t byte_count = 3;
    const uint16_t reg_val[2] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  52: set 'Write Multiple Registers' request PDU with invalid byte_count");
    ret = mb_pdu_set_wr_mult_regs_req(&pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 1;
    const uint8_t byte_count = 2;
    const uint16_t reg_val[1] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  53: set 'Write Multiple Registers' request PDU with invalid end_addr");
    ret = mb_pdu_set_wr_mult_regs_req(&pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x10, 0xab, 0xcd, 0x00, 0x02};

    printf("%-*s", print_cols, "test  54: set and format 'Write Multiple Registers' response PDU");
    mb_pdu_set_wr_mult_regs_resp(&pdu, start_addr, quant_regs);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_resp_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  55: set 'Write Multiple Registers' response PDU with invalid quant_regs");
    ret = mb_pdu_set_wr_mult_regs_resp(&pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_resp_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 124;
    int ret = 0;

    printf("%-*s", print_cols, "test  56: set 'Write Multiple Registers' response PDU with invalid quant_regs");
    ret = mb_pdu_set_wr_mult_regs_resp(&pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_mult_regs_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 0x0001;
    ssize_t num = 0;

    printf("%-*s", print_cols, "test  57: set and format 'Write Multiple Registers' response PDU with invalid end_addr");
    num = mb_pdu_set_wr_mult_regs_resp(&pdu, start_addr, quant_regs);
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rep_server_id_req(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[1] = {0};
    char exp[] = {0x11};

    printf("%-*s", print_cols, "test  58: set and format 'Report Server ID' request PDU");
    mb_pdu_set_rep_server_id_req(&pdu);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rep_server_id_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 5;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    char buf[7] = {0};
    char exp[] = {0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0xff};
    ssize_t num = 0;

    printf("%-*s", print_cols, "test  59: set and format 'Report Server ID' response PDU");
    num = mb_pdu_set_rep_server_id_resp(&pdu, byte_count, server_id, run_ind_status);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rep_server_id_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 0;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    int ret = 0;

    printf("%-*s", print_cols, "test  60: set and format 'Report Server ID' response PDU with invalid byte_count");
    ret = mb_pdu_set_rep_server_id_resp(&pdu, byte_count, server_id, run_ind_status);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rep_server_id_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 252;
    const uint8_t server_id[253] = {0};
    const bool run_ind_status = true;
    int ret = 0;

    printf("%-*s", print_cols, "test  61: set and format 'Report Server ID' response PDU with invalid byte_count");
    ret = mb_pdu_set_rep_server_id_resp(&pdu, byte_count, server_id, run_ind_status);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    ssize_t num = 0;
    char buf[16] = {0};
    char exp[] = {0x14, 0x0e, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};

    printf("%-*s", print_cols, "test  62: set and format 'Read File Record' request PDU");
    num = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_num_sub_req1(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[36] = {{0}};
    const size_t num_sub_req = 36;
    int ret = 0;

    printf("%-*s", print_cols, "test  63: set 'Read File Record' request PDU with invalid num_sub_req");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_num_sub_req2(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[0];
    const size_t num_sub_req = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  64: set 'Read File Record' request PDU with invalid num_sub_req");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  65: set 'Read File Record' request PDU with invalid ref_type");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  66: set 'Read File Record' request PDU with invalid file_num");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.file_num = 0x0003, .rec_num = 0x2710, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  67: set 'Read File Record' request PDU with invalid rec_num");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  68: set 'Read File Record' request PDU with invalid end_addr");
    ret = mb_pdu_set_rd_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x0dfe, 0x0020}},
                                                          [1] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    ssize_t num = 0;
    char buf[14] = {0};
    char exp[] = {0x14, 0x0c, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40};

    printf("%-*s", print_cols, "test  69: set and format 'Read File Record' response PDU");
    num = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_num_sub_req(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[36] = {{0}};
    const size_t num_sub_req = 36;
    int ret = 0;

    printf("%-*s", print_cols, "test  70: set 'Read File Record' response PDU with invalid num_sub_req");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_file_resp_len1(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[1] = {{.file_resp_len = 0, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  71: set 'Read File Record' response PDU with invalid file_resp_len");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_file_resp_len2(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[1] = {{.file_resp_len = 247, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  72: set 'Read File Record' response PDU with invalid file_resp_len");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 5, .rec_data = {0x0dfe, 0x0020}},
                                                          [1] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  73: set 'Read File Record' response PDU with invalid ref_type");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 245, .ref_type = 6, .rec_data = {0}},
                                                          [1] = {.file_resp_len = 245, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  74: set 'Read File Record' response PDU with invalid resp_data_len");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 1, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  75: set 'Read File Record' response PDU with invalid resp_data_len");
    ret = mb_pdu_set_rd_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    char buf[15] = {0};
    char exp[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};

    printf("%-*s", print_cols, "test  76: set and format 'Write File Record' request PDU");
    num = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_num_sub_req(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[28] = {{0}};
    const size_t num_sub_req = 28;
    int ret = 0;

    printf("%-*s", print_cols, "test  77: set 'Write File Record' request PDU with invalid num_sub_req");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  78: set 'Write File Record' request PDU with invalid ref_type");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  79: set 'Write File Record' request PDU with invalid file_num");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x2710, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  80: set 'Write File Record' request PDU with invalid rec_num");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_rec_len(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x007b, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  81: set 'Write File Record' request PDU with invalid rec_len");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  82: set 'Write File Record' request PDU with invalid end_addr");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_req_data_len1(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0000, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  83: set 'Write File Record' request PDU with invalid req_data_len");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_req_invalid_req_data_len2(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[2] = {[0] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}},
                                                      [1] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  84: set 'Write File Record' request PDU with invalid req_data_len");
    ret = mb_pdu_set_wr_file_rec_req(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    char buf[15] = {0};
    char exp[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x00d};

    printf("%-*s", print_cols, "test  85: set and format 'Write File Record' resonse PDU");
    num = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_num_sub_req(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[28] = {{0}};
    const size_t num_sub_req = 28;
    int ret = 0;

    printf("%-*s", print_cols, "test  86: set 'Write File Record' response PDU with invalid num_sub_req");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  87: set 'Write File Record' response PDU with invalid ref_type");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  88: set 'Write File Record' response PDU with invalid file_num");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x2710, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  89: set 'Write File Record' response PDU with invalid rec_num");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_rec_len(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x007b, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  90: set 'Write File Record' response PDU with invalid rec_len");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  91: set 'Write File Record' response PDU with invalid end_addr");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0000, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  92: set 'Write File Record' response PDU with invalid resp_data_len");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_wr_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_pdu_t pdu = {0};
    const mb_pdu_wr_file_rec_sub_req_t sub_req[2] = {[0] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}},
                                                      [1] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  93: set 'Write File Record' response PDU with invalid resp_data_len");
    ret = mb_pdu_set_wr_file_rec_resp(&pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_mask_wr_reg_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[7] = {0};
    char exp[] = {0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25};

    printf("%-*s", print_cols, "test  94: set and format 'Mask Write Register' request PDU");
    mb_pdu_set_mask_wr_reg_req(&pdu, ref_addr, and_mask, or_mask);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_mask_wr_reg_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[7] = {0};
    char exp[] = {0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25};

    printf("%-*s", print_cols, "test  95: set and format 'Mask Write Register' response PDU");
    mb_pdu_set_mask_wr_reg_resp(&pdu, ref_addr, and_mask, or_mask);
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    ssize_t num = 0;
    char buf[16] = {0};
    char exp[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0xe, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};

    printf("%-*s", print_cols, "test  96: set and format 'Read/Write Multiple Registers' request PDU");
    num = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_rd1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x03;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  97: set 'Read/Write Multiple Registers' request PDU with invalid quant_rd");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_rd2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 126;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x03;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  98: set 'Read/Write Multiple Registers' request PDU with invalid quant_rd");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_rd_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0xffff;
    const uint16_t quant_rd = 0x0001;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  99: set 'Read/Write Multiple Registers' request PDU with invalid rd_end_addr");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_wr1(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0;
    const uint16_t wr_reg_val[122] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 100: set 'Read/Write Multiple Registers' request PDU with invalid quant_wr");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_wr2(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 122;
    const uint16_t wr_reg_val[122] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 101: set 'Read/Write Multiple Registers' request PDU with invalid quant_wr");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_req_invalid_wr_end_addr(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0xffff;
    const uint16_t quant_wr = 0x0001;
    const uint16_t wr_reg_val[] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 102: set 'Read/Write Multiple Registers' request PDU with invalid wr_end_addr");
    ret = mb_pdu_set_rd_wr_mult_regs_req(&pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 0x0c;
    const uint16_t rd_reg_val[6] = {0x00fe, 0x0acd, 0x0001, 0x0003, 0x000d, 0x00ff};
    ssize_t num = 0;
    char buf[14] = {0};
    char exp[] = {0x17, 0x0c, 0x00, 0xfe, 0x0a, 0xcd, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0d, 0x00, 0xff};

    printf("%-*s", print_cols, "test 103: set and format 'Read/Write Multiple Registers' response PDU");
    num = mb_pdu_set_rd_wr_mult_regs_resp(&pdu, byte_count, rd_reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 252;
    const uint16_t rd_reg_val[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 104: set 'Read/Write Multiple Registers' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_wr_mult_regs_resp(&pdu, byte_count, rd_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_wr_mult_reg_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t byte_count = 1;
    const uint16_t rd_reg_val[1] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 105: set 'Read/Write Multiple Registers' response PDU with invalid byte_count");
    ret = mb_pdu_set_rd_wr_mult_regs_resp(&pdu, byte_count, rd_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_fifo_q_req(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t fifo_ptr_addr = 0x04de;
    ssize_t num = 0;
    char buf[3] = {0};
    char exp[] = {0x18, 0x04, 0xde};

    printf("%-*s", print_cols, "test 106: set and format 'Read FIFO Queue' request PDU");
    mb_pdu_set_rd_fifo_q_req(&pdu, fifo_ptr_addr);
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_fifo_q_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t fifo_count = 0x0002;
    const uint16_t fifo_val_reg[2] = {0x01be, 0x1284};
    ssize_t num = 0;
    char buf[9] = {0};
    char exp[] = {0x18, 0x00, 0x06, 0x00, 0x02, 0x01, 0xbe, 0x12, 0x84};

    printf("%-*s", print_cols, "test 107: set and format 'Read FIFO Queue' response PDU");
    num = mb_pdu_set_rd_fifo_q_resp(&pdu, fifo_count, fifo_val_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_rd_fifo_q_resp_invalid_fifo_count(void)
{
    mb_pdu_t pdu = {0};
    const uint16_t fifo_count = 32;
    const uint16_t fifo_val_reg[32] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 108: set 'Read FIFO Queue' response PDU with invalid fifo_count");
    ret = mb_pdu_set_rd_fifo_q_resp(&pdu, fifo_count, fifo_val_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_enc_if_trans_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[6] = {0};
    char exp[] = {0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04};

    printf("%-*s", print_cols, "test 109: set and format 'Encapsulated Interface Transport' request PDU");
    num = mb_pdu_set_enc_if_trans_req(&pdu, mei_type, mei_data, mei_data_len);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_enc_if_trans_req_invalid_mei_data_len(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[252] = {0};
    const uint8_t mei_data_len = 252;
    int ret = 0;

    printf("%-*s", print_cols, "test 110: set 'Encapsulated Interface Transport' request PDU with invalid mei_data_len");
    ret = mb_pdu_set_enc_if_trans_req(&pdu, mei_type, mei_data, mei_data_len);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_enc_if_trans_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[6] = {0};
    char exp[] = {0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04};

    printf("%-*s", print_cols, "test 111: set and format 'Encapsulated Interface Transport' response PDU");
    num = mb_pdu_set_enc_if_trans_resp(&pdu, mei_type, mei_data, mei_data_len);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_enc_if_trans_resp_invalid_mei_data_len(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[252] = {0};
    const uint8_t mei_data_len = 252;
    int ret = 0;

    printf("%-*s", print_cols, "test 112: set 'Encapsulated Interface Transport' response PDU with invalid mei_data_len");
    ret = mb_pdu_set_enc_if_trans_resp(&pdu, mei_type, mei_data, mei_data_len);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_err_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x02;
    ssize_t num = 0;
    char buf[2] = {0};
    char exp[] = {0x81, 0x02};

    printf("%-*s", print_cols, "test 113: set and format 'Error' response PDU");
    num = mb_pdu_set_err_resp(&pdu, func_code, except_code);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_pdu_format_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(exp))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(buf)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_err_resp_invalid_func_code(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x01;
    const uint8_t except_code = 0x02;
    int ret = 0;

    printf("%-*s", print_cols, "test 114: set and format 'Error' response PDU with invalid func_code");
    ret = mb_pdu_set_err_resp(&pdu, func_code, except_code);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_err_resp_invalid_except_code(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x07;
    int ret = 0;

    printf("%-*s", print_cols, "test 115: set and format 'Error' response PDU with invalid except_code");
    ret = mb_pdu_set_err_resp(&pdu, func_code, except_code);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x01;
    const uint16_t start_addr = 0x0013;
    const uint16_t quant_coils = 0x0013;
    ssize_t num = 0;
    char buf[] = {0x01, 0x00, 0x13, 0x00, 0x13};

    printf("%-*s", print_cols, "test 116: parse 'Read Coils' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_coils_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_coils_req.quant_coils != quant_coils)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_req_invalid_quant_coils1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x01, 0x00, 0x13, 0x00, 0x00};

    printf("%-*s", print_cols, "test 117: parse 'Read Coils' request PDU with invalid quant_coils");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_req_invalid_quant_coils2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x01, 0x00, 0x13, 0x07, 0xd1};

    printf("%-*s", print_cols, "test 118: parse 'Read Coils' request PDU with invalid quant_coils");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x01, 0xff, 0xff, 0x00, 0x01};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 119: parse 'Read Coils' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x01;
    const uint8_t byte_count = 0x03;
    const uint8_t coil_stat[3] = {0xcd, 0x6b, 0x05};
    ssize_t num = 0;
    char buf[] = {0x01, 0x03, 0xcd, 0x6b, 0x05};

    printf("%-*s", print_cols, "test 120: parse 'Read Coils' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_coils_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_coils_resp.coil_stat, coil_stat, byte_count) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_coils_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x01, 0xfb};

    printf("%-*s", print_cols, "test 121: parse 'Read Coils' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x02;
    const uint16_t start_addr = 0x00c4;
    const uint16_t quant_ips = 0x0016;
    ssize_t num = 0;
    char buf[] = {0x02, 0x00, 0xc4, 0x00, 0x16};

    printf("%-*s", print_cols, "test 122: parse 'Read Discrete Inputs' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_disc_ips_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_disc_ips_req.quant_ips != quant_ips)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_req_invalid_quant_ips1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x02, 0x00, 0xc4, 0x00, 0x00};

    printf("%-*s", print_cols, "test 123: parse 'Read Discrete Inputs' request PDU with invalid quant_ips");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_req_invalid_quant_ips2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x02, 0x00, 0xc4, 0x07, 0xd1};

    printf("%-*s", print_cols, "test 124: parse 'Read Discrete Inputs' request PDU with invalid quant_ips");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x02, 0xff, 0xff, 0x00, 0x01};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 125: parse 'Read Discrete Inputs' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x02;
    const uint8_t byte_count = 0x03;
    const uint8_t ip_stat[3] = {0xac, 0xdb, 0x35};
    ssize_t num = 0;
    char buf[] = {0x02, 0x03, 0xac, 0xdb, 0x35};

    printf("%-*s", print_cols, "test 126: parse 'Read Discrete Inputs' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_disc_ips_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_disc_ips_resp.ip_stat, ip_stat, byte_count) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_disc_ips_resp_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x02, 0xfb};

    printf("%-*s", print_cols, "test 127: parse 'Read Discrete Inputs' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x03;
    const uint16_t start_addr = 0x006b;
    const uint16_t quant_regs = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x03, 0x00, 0x6b, 0x00, 0x03};

    printf("%-*s", print_cols, "test 128: parse 'Read Holding Registers' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_hold_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_hold_regs_req.quant_regs != quant_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_req_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x00, 0x6b, 0x00, 0x00};

    printf("%-*s", print_cols, "test 129: parse 'Read Holding Registers' request PDU with invalid quant_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_req_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x00, 0x6b, 0x00, 0x7e};

    printf("%-*s", print_cols, "test 130: parse 'Read Holding Registers' request PDU with invalid quant_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0xff, 0xff, 0x00, 0x01};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 131: parse 'Read Holding Registers' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x03;
    const uint8_t byte_count = 0x06;
    const uint16_t reg_val[3] = {0x022b, 0x0000, 0x0064};
    ssize_t num = 0;
    char buf[] = {0x03, 0x06, 0x02, 0x2b, 0x00, 0x00, 0x00, 0x64};

    printf("%-*s", print_cols, "test 132: parse 'Read Holding Registers' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_hold_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_hold_regs_resp.reg_val, reg_val, sizeof(reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01};

    printf("%-*s", print_cols, "test 133: parse 'Read Holding Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_hold_regs_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0xfc};

    printf("%-*s", print_cols, "test 134: parse 'Read Holding Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x04;
    const uint16_t start_addr = 0x0008;
    const uint16_t quant_ip_regs = 0x0001;
    ssize_t num = 0;
    char buf[] = {0x04, 0x00, 0x08, 0x00, 0x01};

    printf("%-*s", print_cols, "test 135: parse 'Read Input Registers' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_ip_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_ip_regs_req.quant_ip_regs != quant_ip_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_req_invalid_quant_ip_regs1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x04, 0x00, 0x08, 0x00, 0x00};

    printf("%-*s", print_cols, "test 136: parse 'Read Input Registers' request PDU with invalid quant_ip_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_req_invalid_quant_ip_regs2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x04, 0x00, 0x08, 0x00, 0x7e};

    printf("%-*s", print_cols, "test 137: parse 'Read Input Registers' request PDU with invalid quant_ip_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x04, 0xff, 0xff, 0x00, 0x01};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 138: parse 'Read Input Registers' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x04;
    const uint8_t byte_count = 0x02;
    const uint16_t ip_reg[1] = {0x000a};
    ssize_t num = 0;
    char buf[] = {0x04, 0x02, 0x00, 0x0a};

    printf("%-*s", print_cols, "test 139: parse 'Read Input Registers' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_ip_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_ip_regs_resp.ip_reg, ip_reg, sizeof(ip_reg)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x04, 0x01, 0x00};

    printf("%-*s", print_cols, "test 140: parse 'Read Input Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_ip_regs_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x04, 0xfc};

    printf("%-*s", print_cols, "test 141: parse 'Read Input Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_req1(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = false;
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0x00, 0x00};

    printf("%-*s", print_cols, "test 142: parse 'Write Single Coil' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_req.op_addr != op_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_req.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_req2(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0xff, 0x00};

    printf("%-*s", print_cols, "test 143: parse 'Write Single Coil' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_req.op_addr != op_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_req.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_req_invalid_op_val(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0x00, 0x01};

    printf("%-*s", print_cols, "test 144: parse 'Write Single Coil' request PDU with invalid op_val");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_resp1(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = false;
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0x00, 0x00};

    printf("%-*s", print_cols, "test 145: parse 'Write Single Coil' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_resp.op_addr != op_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_resp.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_resp2(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0xff, 0x00};

    printf("%-*s", print_cols, "test 146: parse 'Write Single Coil' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_resp.op_addr != op_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_coil_resp.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_coil_resp_invalid_op_val(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x05, 0x00, 0xac, 0x00, 0x01};

    printf("%-*s", print_cols, "test 147: parse 'Write Single Coil' response PDU with invalid op_val");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_reg_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x06;
    const uint16_t reg_addr = 0x0001;
    const uint16_t reg_val = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x06, 0x00, 0x01, 0x00, 0x03};

    printf("%-*s", print_cols, "test 148: parse 'Write Single Register' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_reg_req.reg_addr != reg_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_reg_req.reg_val != reg_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_sing_reg_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x06;
    const uint16_t reg_addr = 0x0001;
    const uint16_t reg_val = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x06, 0x00, 0x01, 0x00, 0x03};

    printf("%-*s", print_cols, "test 149: parse 'Write Single Register' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_sing_reg_resp.reg_addr != reg_addr)
    {
        return FAIL;
    }
    if (pdu.wr_sing_reg_resp.reg_val != reg_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_except_stat_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x07;
    ssize_t num = 0;
    char buf[] = {0x07};

    printf("%-*s", print_cols, "test 150: parse 'Read Exception Status' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_except_stat_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x07;
    const uint8_t out_data = 0x6d;
    ssize_t num = 0;
    char buf[] = {0x07, 0x6d};

    printf("%-*s", print_cols, "test 151: parse 'Read Exception Status' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_except_stat_resp.out_data != out_data)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_diag_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x08;
    const uint16_t sub_func = 0x0000;
    const uint16_t data[1] = {0xa537};
    const uint16_t num_data = 1;
    ssize_t num = 0;
    char buf[] = {0x08, 0x00, 0x00, 0xa5, 0x37};

    printf("%-*s", print_cols, "test 152: parse 'Diagnostics' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.diag_req.sub_func != sub_func)
    {
        return FAIL;
    }
    if (memcmp(pdu.diag_req.data, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    if (pdu.diag_req.num_data != num_data)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_diag_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x08;
    const uint16_t sub_func = 0x0000;
    const uint16_t data[1] = {0xa537};
    const uint16_t num_data = 1;
    ssize_t num = 0;
    char buf[] = {0x08, 0x00, 0x00, 0xa5, 0x37};

    printf("%-*s", print_cols, "test 153: parse 'Diagnostics' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.diag_resp.sub_func != sub_func)
    {
        return FAIL;
    }
    if (memcmp(pdu.diag_resp.data, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    if (pdu.diag_resp.num_data != num_data)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_diag_resp_invalid_len1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x08, 0x00, 0x00, 0xa5, 0x37, 0x00};  /* len not even */

    printf("%-*s", print_cols, "test 154: parse 'Diagnostics' response PDU with invalid len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_diag_resp_invalid_len2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x08, 0x00, 0x00};  /* len too short */

    printf("%-*s", print_cols, "test 155: parse 'Diagnostics' response PDU with invalid len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_diag_resp_invalid_len3(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[255] = {0x08, 0x00, 0x00};  /* len too large */

    printf("%-*s", print_cols, "test 156: parse 'Diagnostics' response PDU with invalid len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_cntr_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0b;
    ssize_t num = 0;
    char buf[] = {0x0b};

    printf("%-*s", print_cols, "test 157: parse 'Get Comm Event Counter' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_cntr_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0b;
    const uint16_t status = 0xffff;
    const uint16_t ev_cnt = 0x0108;
    ssize_t num = 0;
    char buf[] = {0x0b, 0xff, 0xff, 0x01, 0x08};

    printf("%-*s", print_cols, "test 158: parse 'Get Comm Event Counter' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_cntr_resp.status != status)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_cntr_resp.ev_cnt != ev_cnt)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_log_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0c;
    ssize_t num = 0;
    char buf[] = {0x0c};

    printf("%-*s", print_cols, "test 159: parse 'Get Comm Event Log' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_log_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0c;
    const uint8_t byte_count = 0x08;
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[2] = {0x20, 0x00};
    ssize_t num = 0;
    char buf[] = {0x0c, 0x08, 0x00, 0x00, 0x01, 0x08, 0x01, 0x21, 0x20, 0x00};

    printf("%-*s", print_cols, "test 160: parse 'Get Comm Event Log' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_log_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_log_resp.status != status)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_log_resp.ev_cnt != ev_cnt)
    {
        return FAIL;
    }
    if (pdu.get_com_ev_log_resp.msg_cnt != msg_cnt)
    {
        return FAIL;
    }
    if (memcmp(pdu.get_com_ev_log_resp.event, event, sizeof(event)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_log_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0c, 0x02};  /* byte_count too short */

    printf("%-*s", print_cols, "test 161: parse 'Get Comm Event Log' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_get_com_ev_log_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[254] = {0x0c, 0xfc};  /* byte_count too long */

    printf("%-*s", print_cols, "test 162: parse 'Get Comm Event Log' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0f;
    const uint16_t start_addr = 0x0013;
    const uint16_t quant_ops = 0x000a;
    const uint8_t byte_count = 0x02;
    const uint8_t op_val[2] = {0xcd, 0x01};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01};

    printf("%-*s", print_cols, "test 163: parse 'Write Multiple Coils' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_mult_coils_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.wr_mult_coils_req.quant_ops != quant_ops)
    {
        return FAIL;
    }
    if (pdu.wr_mult_coils_req.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.wr_mult_coils_req.op_val, op_val, sizeof(op_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_req_invalid_quant_ops1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x00, 0x00, 0x00};  /* quant_ops too small (and byte_count too small) */

    printf("%-*s", print_cols, "test 164: parse 'Write Multiple Coils' request PDU with invalid quant_ops");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_req_invalid_quant_ops2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x07, 0xb1, 0xf7};  /* quant_ops too large (and byte_count too large) */

    printf("%-*s", print_cols, "test 165: parse 'Write Multiple Coils' request PDU with invalid quant_ops");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_req_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x00, 0x01, 0x00};  /* quant_ops != byte_count */

    printf("%-*s", print_cols, "test 166: parse 'Write Multiple Coils' request PDU with invalid byte_count");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0xff, 0xff, 0x00, 0x01, 0x01, 0x00};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 167: parse 'Write Multiple Coils' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x0f;
    const uint16_t start_addr = 0x0013;
    const uint16_t quant_ops = 0x000a;
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x00, 0x0a};

    printf("%-*s", print_cols, "test 168: parse 'Write Multiple Coils' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_mult_coils_resp.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.wr_mult_coils_resp.quant_ops != quant_ops)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_resp_invalid_quant_ops1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x00, 0x00};  /* quant_ops too small */

    printf("%-*s", print_cols, "test 169: parse 'Write Multiple Coils' response PDU with invalid quant_ops");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_resp_invalid_quant_ops2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0x00, 0x13, 0x07, 0xb1};  /* quant_ops too large */

    printf("%-*s", print_cols, "test 170: parse 'Write Multiple Coils' response PDU with invalid quant_ops");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_coils_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x0f, 0xff, 0xff, 0x00, 0x01, 0x01};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 171: parse 'Write Multiple Coils' response PDU with invalid end_addr");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x10;
    const uint16_t start_addr = 0x0001;
    const uint16_t quant_regs = 0x0002;
    const uint8_t byte_count = 0x04;
    const uint16_t reg_val[2] = {0x000a, 0x0102};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0a, 0x01, 0x02};

    printf("%-*s", print_cols, "test 172: parse 'Write Multiple Registers' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_mult_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.wr_mult_regs_req.quant_regs != quant_regs)
    {
        return FAIL;
    }
    if (pdu.wr_mult_regs_req.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.wr_mult_regs_req.reg_val, reg_val, sizeof(reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_req_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x00, 0x00};  /* quant_regs too small (and byte_count too small) */

    printf("%-*s", print_cols, "test 173: parse 'Write Multiple Registers' request PDU with invalid quant_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_req_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x7c, 0xf8};  /* quant_regs too large (and byte_count too large) */

    printf("%-*s", print_cols, "test 174: parse 'Write Multiple Registers' request PDU with invalid quant_regs");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_req_invalid_byte_count(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x02, 0x01};  /* byte_count != quant_regs */

    printf("%-*s", print_cols, "test 175: parse 'Write Multiple Registers' request PDU with invalid byte_count");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0xff, 0xff, 0x00, 0x01, 0x02, 0x00, 0x00};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 176: parse 'Write Multiple Registers' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x10;
    const uint16_t start_addr = 0x0001;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x02};

    printf("%-*s", print_cols, "test 177: parse 'Write Multiple Registers' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_mult_regs_resp.start_addr != start_addr)
    {
        return FAIL;
    }
    if (pdu.wr_mult_regs_resp.quant_regs != quant_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_resp_invalid_quant_regs1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x00};  /* quant_regs too small */

    printf("%-*s", print_cols, "test 178: parse 'Write Multiple Registers' response PDU with invalid quant_regs");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_resp_invalid_quant_regs2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0x00, 0x01, 0x00, 0x7c};  /* quant_regs too large */

    printf("%-*s", print_cols, "test 179: parse 'Write Multiple Registers' response PDU with invalid quant_regs");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_mult_regs_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x10, 0xff, 0xff, 0x00, 0x01, 0x02, 0x00, 0x00};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 180: parse 'Write Multiple Registers' response PDU with invalid end_addr");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x11;
    ssize_t num = 0;
    char buf[] = {0x11};

    printf("%-*s", print_cols, "test 181: parse 'Report server ID' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_resp1(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x11;
    const uint8_t byte_count = 0x05;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = false;
    ssize_t num = 0;
    char buf[] = {0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0x00};

    printf("%-*s", print_cols, "test 182: parse 'Report server ID' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rep_server_id_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rep_server_id_resp.server_id, server_id, sizeof(server_id)) != 0)
    {
        return FAIL;
    }
    if (pdu.rep_server_id_resp.run_ind_status != run_ind_status)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_resp2(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x11;
    const uint8_t byte_count = 0x05;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    ssize_t num = 0;
    char buf[] = {0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0xff};

    printf("%-*s", print_cols, "test 183: parse 'Report server ID' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rep_server_id_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rep_server_id_resp.server_id, server_id, sizeof(server_id)) != 0)
    {
        return FAIL;
    }
    if (pdu.rep_server_id_resp.run_ind_status != run_ind_status)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    char buf[] = {0x11, 0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test 184: parse 'Report server ID' response PDU with invalid byte_count");
    ret = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    char buf[254] = {0x11, 0xfc, 0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test 185: parse 'Report server ID' response PDU with invalid byte_count");
    ret = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rep_server_id_resp_invalid_run_ind_status(void)
{
    mb_pdu_t pdu = {0};
    char buf[] = {0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0x01};
    int ret = 0;

    printf("%-*s", print_cols, "test 186: parse 'Report server ID' response PDU with invalid run_ind_status");
    ret = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x14;
    const uint8_t byte_count = 0x0e;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                        [1] = {.ref_type = 0x06, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x14, 0x0e, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};

    printf("%-*s", print_cols, "test 187: parse 'Read File Record' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_file_rec_req.byte_count != byte_count)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (pdu.rd_file_rec_req.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (pdu.rd_file_rec_req.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (pdu.rd_file_rec_req.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (pdu.rd_file_rec_req.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x06};  /* byte_count too small */

    printf("%-*s", print_cols, "test 188: parse 'Read File Record' request PDU with invalid byte_count");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0xf6};  /* byte_count too large */

    printf("%-*s", print_cols, "test 189: parse 'Read File Record' request PDU with invalid byte_count");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x0e, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 190: parse 'Read File Record' request PDU with invalid ref_type");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};  /* file_num too small */

    printf("%-*s", print_cols, "test 191: parse 'Read File Record' request PDU with invalid file_num");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x0e, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};  /* rec_num too large */

    printf("%-*s", print_cols, "test 192: parse 'Read File Record' request PDU with invalid rec_num");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x0e, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 193: parse 'Read File Record' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x14;
    const uint8_t resp_data_len = 0x0c;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 0x06, .rec_data = {0x0dfe, 0x0020}},
                                                         [1] = {.file_resp_len = 0x05, .ref_type = 0x06, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x14, 0x0c, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40};

    printf("%-*s", print_cols, "test 194: parse 'Read File Record' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_file_rec_resp.resp_data_len != resp_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (pdu.rd_file_rec_resp.sub_req[i].file_resp_len != sub_req[i].file_resp_len)
        {
            return FAIL;
        }
        if (pdu.rd_file_rec_resp.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (memcmp(pdu.rd_file_rec_resp.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].file_resp_len - 1) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x06};  /* resp_data_len too small */

    printf("%-*s", print_cols, "test 195: parse 'Read File Record' response PDU with invalid resp_data_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0xf6};  /* resp_data_len too large */

    printf("%-*s", print_cols, "test 196: parse 'Read File Record' response PDU with invalid resp_data_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x0c, 0x05, 0x07, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 197: parse 'Read File Record' response PDU with invalid ref_type");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x00};  /* file_resp_len too small */

    printf("%-*s", print_cols, "test 198: parse 'Read File Record' response PDU with invalid file_resp_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0xf7};  /* file_resp_len too large */

    printf("%-*s", print_cols, "test 199: parse 'Read File Record' response PDU with invalid file_resp_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len3(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x08};  /* file_resp_len even */

    printf("%-*s", print_cols, "test 200: parse 'Read File Record' response PDU with invalid file_resp_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x15;
    const uint8_t req_data_len = 0x0d;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    unsigned i = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};

    printf("%-*s", print_cols, "test 201: parse 'Write File Record' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_file_rec_req.req_data_len != req_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (pdu.wr_file_rec_req.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_req.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_req.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_req.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
        if (memcmp(pdu.wr_file_rec_req.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].rec_len * 2) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_req_data_len1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x07, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00};  /* req_data_len too small */

    printf("%-*s", print_cols, "test 202: parse 'Write File Record' request PDU with invalid req_data_len");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_req_data_len2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0xfc, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00};  /* req_data_len too large */

    printf("%-*s", print_cols, "test 203: parse 'Write File Record' request PDU with invalid req_data_len");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x05, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 204: parse 'Write File Record' request PDU with invalid ref_type");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};  /* file_num too small */

    printf("%-*s", print_cols, "test 205: parse 'Write File Record' request PDU with invalid file_num");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};  /* rec_num too large */

    printf("%-*s", print_cols, "test 206: parse 'Write File Record' request PDU with invalid rec_num");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_req_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 207: parse 'Write File Record' request PDU with invalid end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x15;
    const uint8_t resp_data_len = 0x0d;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};

    printf("%-*s", print_cols, "test 208: parse 'Write File Record' response PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.wr_file_rec_resp.resp_data_len != resp_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (pdu.wr_file_rec_resp.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_resp.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_resp.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (pdu.wr_file_rec_resp.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
        if (memcmp(pdu.wr_file_rec_resp.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].rec_len * 2) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x07, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00};

    printf("%-*s", print_cols, "test 209: parse 'Write File Record' response PDU with invalid resp_data_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0xfc, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00};

    printf("%-*s", print_cols, "test 210: parse 'Write File Record' response PDU with invalid resp_data_len");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_ref_type(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 211: parse 'Write File Record' response PDU with invalid ref_type");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_file_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};

    printf("%-*s", print_cols, "test 212: parse 'Write File Record' response PDU with invalid file_num");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_rec_num(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d};

    printf("%-*s", print_cols, "test 213: parse 'Write File Record' response PDU with invalid rec_num");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_wr_file_rec_resp_invalid_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 214: parse 'Write File Record' response PDU with invalid end_addr");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_mask_wr_reg_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x16;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[] = {0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25};

    printf("%-*s", print_cols, "test 215: parse 'Mask Write Register' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_req.ref_addr != ref_addr)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_req.and_mask != and_mask)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_req.or_mask != or_mask)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_mask_wr_reg_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x16;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[] = {0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25};

    printf("%-*s", print_cols, "test 216: parse 'Mask Write Register' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_resp.ref_addr != ref_addr)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_resp.and_mask != and_mask)
    {
        return FAIL;
    }
    if (pdu.mask_wr_reg_resp.or_mask != or_mask)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x17;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint8_t wr_byte_count = 0x06;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};

    printf("%-*s", print_cols, "test 217: parse 'Read/Write Multiple Registers' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_req.rd_start_addr != rd_start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_req.quant_rd != quant_rd)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_req.wr_start_addr != wr_start_addr)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_req.quant_wr != quant_wr)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_req.wr_byte_count != wr_byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_wr_mult_regs_req.wr_reg_val, wr_reg_val, quant_wr) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_rd1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};  /* quant_rd too small */

    printf("%-*s", print_cols, "test 218: parse 'Read/Write Multiple Registers' request PDU with invalid quant_rd");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_rd2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x7e, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};  /* quant_rd too large */

    printf("%-*s", print_cols, "test 219: parse 'Read/Write Multiple Registers' request PDU with invalid quant_rd");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_rd_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 220: parse 'Read/Write Multiple Registers' request PDU with invalid rd_end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_wr1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x00, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};  /* quant_wr too small */

    printf("%-*s", print_cols, "test 221: parse 'Read/Write Multiple Registers' request PDU with invalid quant_wr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_wr2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x7e, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};  /* quant_wr too large */

    printf("%-*s", print_cols, "test 222: parse 'Read/Write Multiple Registers' request PDU with invalid quant_wr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_wr_end_addr(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0xff, 0xff, 0x00, 0x01, 0x01};  /* 0xffff + 0x0001 */

    printf("%-*s", print_cols, "test 223: parse 'Read/Write Multiple Registers' request PDU with invalid wr_end_addr");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_wr_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x05, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};  /* byte_count != 2 * quant_wr */

    printf("%-*s", print_cols, "test 224: parse 'Read/Write Multiple Registers' request PDU with invalid wr_byte_count");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x17;
    const uint8_t byte_count = 0x0c;
    const uint16_t rd_reg_val[] = {0x00fe, 0x0acd, 0x0001, 0x0003, 0x000d, 0x00ff};
    ssize_t num = 0;
    char buf[] = {0x17, 0x0c, 0x00, 0xfe, 0x0a, 0xcd, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0d, 0x00, 0xff};

    printf("%-*s", print_cols, "test 225: parse 'Read/Write Multiple Registers' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_wr_mult_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_wr_mult_regs_resp.rd_reg_val, rd_reg_val, sizeof(rd_reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0x01, 0x00};  /* byte_count not even */

    printf("%-*s", print_cols, "test 226: parse 'Read/Write Multiple Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_wr_mult_regs_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x17, 0xfc};  /* byte_count too large */

    printf("%-*s", print_cols, "test 227: parse 'Read/Write Multiple Registers' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_fifo_q_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x18;
    const uint16_t fifo_ptr_addr = 0x04de;
    ssize_t num = 0;
    char buf[] = {0x18, 0x04, 0xde};

    printf("%-*s", print_cols, "test 228: parse 'Read FIFO Queue' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_fifo_q_req.fifo_ptr_addr != fifo_ptr_addr)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_fifo_q_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x18;
    const uint16_t byte_count = 0x0006;
    const uint16_t fifo_count = 0x0002;
    const uint16_t fifo_val_reg[] = {0x01b8, 0x1284};
    ssize_t num = 0;
    char buf[] = {0x18, 0x00, 0x06, 0x00, 0x02, 0x01, 0xb8, 0x12, 0x84};

    printf("%-*s", print_cols, "test 229: parse 'Read FIFO Queue' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.rd_fifo_q_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (pdu.rd_fifo_q_resp.fifo_count != fifo_count)
    {
        return FAIL;
    }
    if (memcmp(pdu.rd_fifo_q_resp.fifo_val_reg, fifo_val_reg, sizeof(fifo_val_reg)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_fifo_q_resp_invalid_byte_count1(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x18, 0x00, 0x01};  /* byte_count not even */

    printf("%-*s", print_cols, "test 230: parse 'Read FIFO Queue' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_fifo_q_resp_invalid_byte_count2(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x18, 0x00, 0x04, 0x00, 0x02, 0x01, 0xb8, 0x12, 0x84};  /* byte_count does not match fifo_count */

    printf("%-*s", print_cols, "test 231: parse 'Read FIFO Queue' response PDU with invalid byte_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_rd_fifo_q_resp_invalid_fifo_count(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x18, 0x00, 0x42, 0x00, 0x20};  /* fifo_count too large */

    printf("%-*s", print_cols, "test 232: parse 'Read FIFO Queue' response PDU with invalid fifo_count");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_enc_if_trans_req(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x2b;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[] = {0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04};

    printf("%-*s", print_cols, "test 233: parse 'Encapsulated Interface Transport' request PDU");
    num = mb_pdu_parse_req(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.enc_if_trans_req.mei_type != mei_type)
    {
        return FAIL;
    }
    if (memcmp(pdu.enc_if_trans_req.mei_data, mei_data, sizeof(mei_data)) != 0)
    {
        return FAIL;
    }
    if (pdu.enc_if_trans_req.mei_data_len != mei_data_len)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_enc_if_trans_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x2b;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[] = {0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04};

    printf("%-*s", print_cols, "test 234: parse 'Encapsulated Interface Transport' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.enc_if_trans_resp.mei_type != mei_type)
    {
        return FAIL;
    }
    if (memcmp(pdu.enc_if_trans_resp.mei_data, mei_data, sizeof(mei_data)) != 0)
    {
        return FAIL;
    }
    if (pdu.enc_if_trans_resp.mei_data_len != mei_data_len)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_err_resp(void)
{
    mb_pdu_t pdu = {0};
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x02;
    ssize_t num = 0;
    char buf[] = {0x81, 0x02};

    printf("%-*s", print_cols, "test 235: parse 'Error' response PDU");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (pdu.err.except_code != except_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_pdu_parse_err_resp_invalid_except_code(void)
{
    mb_pdu_t pdu = {0};
    ssize_t num = 0;
    char buf[] = {0x81, 0x07};

    printf("%-*s", print_cols, "test 236: parse 'Error' response PDU with invalid except_code");
    num = mb_pdu_parse_resp(&pdu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

int main(void)
{
    mb_test_func_t func[] = {test_mb_pdu_set,
                             test_mb_pdu_rd_coils_req,
                             test_mb_pdu_rd_coils_req_invalid_quant_coils1,
                             test_mb_pdu_rd_coils_req_invalid_quant_coils2,
                             test_mb_pdu_rd_coils_req_invalid_end_addr,
                             test_mb_pdu_rd_coils_resp,
                             test_mb_pdu_rd_coils_resp_invalid_byte_count,
                             test_mb_pdu_rd_disc_ips_req,
                             test_mb_pdu_rd_disc_ips_req_invalid_num_ips1,
                             test_mb_pdu_rd_disc_ips_req_invalid_num_ips2,
                             test_mb_pdu_rd_disc_ips_req_invalid_end_addr,
                             test_mb_pdu_rd_disc_ips_resp,
                             test_mb_pdu_rd_disc_ips_resp_invalid_byte_count,
                             test_mb_pdu_rd_hold_regs_req,
                             test_mb_pdu_rd_hold_regs_req_invalid_quant_regs1,
                             test_mb_pdu_rd_hold_regs_req_invalid_quant_regs2,
                             test_mb_pdu_rd_hold_regs_req_invalid_end_addr,
                             test_mb_pdu_rd_hold_regs_resp,
                             test_mb_pdu_rd_hold_regs_resp_invalid_byte_count,
                             test_mb_pdu_rd_ip_regs_req,
                             test_mb_pdu_rd_ip_regs_req_invalid_quant_ip_regs1,
                             test_mb_pdu_rd_ip_regs_req_invalid_quant_ip_regs2,
                             test_mb_pdu_rd_ip_regs_req_invalid_end_addr,
                             test_mb_pdu_rd_ip_regs_resp,
                             test_mb_pdu_rd_ip_regs_resp_invalid_byte_count,
                             test_mb_pdu_wr_sing_coil_req,
                             test_mb_pdu_wr_sing_coil_resp,
                             test_mb_pdu_wr_sing_reg_req,
                             test_mb_pdu_wr_sing_reg_resp,
                             test_mb_pdu_rd_except_stat_req,
                             test_mb_pdu_rd_except_stat_resp,
                             test_mb_pdu_diag_req,
                             test_mb_pdu_diag_req_invalid_num_data,
                             test_mb_pdu_diag_resp,
                             test_mb_pdu_diag_resp_invalid_num_data,
                             test_mb_pdu_get_com_ev_cntr_req,
                             test_mb_pdu_get_com_ev_cntr_resp,
                             test_mb_pdu_get_com_ev_log_req,
                             test_mb_pdu_get_com_ev_log_resp,
                             test_mb_pdu_get_com_ev_log_resp_invalid_num_events,
                             test_mb_pdu_wr_mult_coils_req,
                             test_mb_pdu_wr_mult_coils_req_invalid_quant_ops1,
                             test_mb_pdu_wr_mult_coils_req_invalid_quant_ops2,
                             test_mb_pdu_wr_mult_coils_req_invalid_end_addr,
                             test_mb_pdu_wr_mult_coils_resp,
                             test_mb_pdu_wr_mult_coils_resp_invalid_quant_ops1,
                             test_mb_pdu_wr_mult_coils_resp_invalid_quant_ops2,
                             test_mb_pdu_wr_mult_coils_resp_invalid_end_addr,
                             test_mb_pdu_wr_mult_regs_req,
                             test_mb_pdu_wr_mult_regs_req_invalid_quant_regs1,
                             test_mb_pdu_wr_mult_regs_req_invalid_quant_regs2,
                             test_mb_pdu_wr_mult_regs_req_invalid_byte_count,
                             test_mb_pdu_wr_mult_regs_req_invalid_end_addr,
                             test_mb_pdu_wr_mult_regs_resp,
                             test_mb_pdu_wr_mult_regs_resp_invalid_quant_regs1,
                             test_mb_pdu_wr_mult_regs_resp_invalid_quant_regs2,
                             test_mb_pdu_wr_mult_regs_resp_invalid_end_addr,
                             test_mb_pdu_rep_server_id_req,
                             test_mb_pdu_rep_server_id_resp,
                             test_mb_pdu_rep_server_id_resp_invalid_byte_count1,
                             test_mb_pdu_rep_server_id_resp_invalid_byte_count2,
                             test_mb_pdu_rd_file_rec_req,
                             test_mb_pdu_rd_file_rec_req_invalid_num_sub_req1,
                             test_mb_pdu_rd_file_rec_req_invalid_num_sub_req2,
                             test_mb_pdu_rd_file_rec_req_invalid_ref_type,
                             test_mb_pdu_rd_file_rec_req_invalid_file_num,
                             test_mb_pdu_rd_file_rec_req_invalid_rec_num,
                             test_mb_pdu_rd_file_rec_req_invalid_end_addr,
                             test_mb_pdu_rd_file_rec_resp,
                             test_mb_pdu_rd_file_rec_resp_invalid_num_sub_req,
                             test_mb_pdu_rd_file_rec_resp_invalid_file_resp_len1,
                             test_mb_pdu_rd_file_rec_resp_invalid_file_resp_len2,
                             test_mb_pdu_rd_file_rec_resp_invalid_ref_type,
                             test_mb_pdu_rd_file_rec_resp_invalid_resp_data_len1,
                             test_mb_pdu_rd_file_rec_resp_invalid_resp_data_len2,
                             test_mb_pdu_wr_file_rec_req,
                             test_mb_pdu_wr_file_rec_req_invalid_num_sub_req,
                             test_mb_pdu_wr_file_rec_req_invalid_ref_type,
                             test_mb_pdu_wr_file_rec_req_invalid_file_num,
                             test_mb_pdu_wr_file_rec_req_invalid_rec_num,
                             test_mb_pdu_wr_file_rec_req_invalid_rec_len,
                             test_mb_pdu_wr_file_rec_req_invalid_end_addr,
                             test_mb_pdu_wr_file_rec_req_invalid_req_data_len1,
                             test_mb_pdu_wr_file_rec_req_invalid_req_data_len2,
                             test_mb_pdu_wr_file_rec_resp,
                             test_mb_pdu_wr_file_rec_resp_invalid_num_sub_req,
                             test_mb_pdu_wr_file_rec_resp_invalid_ref_type,
                             test_mb_pdu_wr_file_rec_resp_invalid_file_num,
                             test_mb_pdu_wr_file_rec_resp_invalid_rec_num,
                             test_mb_pdu_wr_file_rec_resp_invalid_rec_len,
                             test_mb_pdu_wr_file_rec_resp_invalid_end_addr,
                             test_mb_pdu_wr_file_rec_resp_invalid_resp_data_len1,
                             test_mb_pdu_wr_file_rec_resp_invalid_resp_data_len2,
                             test_mb_pdu_mask_wr_reg_req,
                             test_mb_pdu_mask_wr_reg_resp,
                             test_mb_pdu_rd_wr_mult_reg_req,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_rd1,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_rd2,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_rd_end_addr,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_wr1,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_quant_wr2,
                             test_mb_pdu_rd_wr_mult_reg_req_invalid_wr_end_addr,
                             test_mb_pdu_rd_wr_mult_reg_resp,
                             test_mb_pdu_rd_wr_mult_reg_resp_invalid_byte_count1,
                             test_mb_pdu_rd_wr_mult_reg_resp_invalid_byte_count2,
                             test_mb_pdu_rd_fifo_q_req,
                             test_mb_pdu_rd_fifo_q_resp,
                             test_mb_pdu_rd_fifo_q_resp_invalid_fifo_count,
                             test_mb_pdu_enc_if_trans_req,
                             test_mb_pdu_enc_if_trans_req_invalid_mei_data_len,
                             test_mb_pdu_enc_if_trans_resp,
                             test_mb_pdu_enc_if_trans_resp_invalid_mei_data_len,
                             test_mb_pdu_err_resp,
                             test_mb_pdu_err_resp_invalid_func_code,
                             test_mb_pdu_err_resp_invalid_except_code,
                             test_mb_pdu_parse_rd_coils_req,
                             test_mb_pdu_parse_rd_coils_req_invalid_quant_coils1,
                             test_mb_pdu_parse_rd_coils_req_invalid_quant_coils2,
                             test_mb_pdu_parse_rd_coils_req_invalid_end_addr,
                             test_mb_pdu_parse_rd_coils_resp,
                             test_mb_pdu_parse_rd_coils_resp_invalid_byte_count,
                             test_mb_pdu_parse_rd_disc_ips_req,
                             test_mb_pdu_parse_rd_disc_ips_req_invalid_quant_ips1,
                             test_mb_pdu_parse_rd_disc_ips_req_invalid_quant_ips2,
                             test_mb_pdu_parse_rd_disc_ips_req_invalid_end_addr,
                             test_mb_pdu_parse_rd_disc_ips_resp,
                             test_mb_pdu_parse_rd_disc_ips_resp_invalid_byte_count,
                             test_mb_pdu_parse_rd_hold_regs_req,
                             test_mb_pdu_parse_rd_hold_regs_req_invalid_quant_regs1,
                             test_mb_pdu_parse_rd_hold_regs_req_invalid_quant_regs2,
                             test_mb_pdu_parse_rd_hold_regs_req_invalid_end_addr,
                             test_mb_pdu_parse_rd_hold_regs_resp,
                             test_mb_pdu_parse_rd_hold_regs_resp_invalid_byte_count1,
                             test_mb_pdu_parse_rd_hold_regs_resp_invalid_byte_count2,
                             test_mb_pdu_parse_rd_ip_regs_req,
                             test_mb_pdu_parse_rd_ip_regs_req_invalid_quant_ip_regs1,
                             test_mb_pdu_parse_rd_ip_regs_req_invalid_quant_ip_regs2,
                             test_mb_pdu_parse_rd_ip_regs_req_invalid_end_addr,
                             test_mb_pdu_parse_rd_ip_regs_resp,
                             test_mb_pdu_parse_rd_ip_regs_resp_invalid_byte_count1,
                             test_mb_pdu_parse_rd_ip_regs_resp_invalid_byte_count2,
                             test_mb_pdu_parse_wr_sing_coil_req1,
                             test_mb_pdu_parse_wr_sing_coil_req2,
                             test_mb_pdu_parse_wr_sing_coil_req_invalid_op_val,
                             test_mb_pdu_parse_wr_sing_coil_resp1,
                             test_mb_pdu_parse_wr_sing_coil_resp2,
                             test_mb_pdu_parse_wr_sing_coil_resp_invalid_op_val,
                             test_mb_pdu_parse_wr_sing_reg_req,
                             test_mb_pdu_parse_wr_sing_reg_resp,
                             test_mb_pdu_parse_rd_except_stat_req,
                             test_mb_pdu_parse_rd_except_stat_resp,
                             test_mb_pdu_parse_diag_req,
                             test_mb_pdu_parse_diag_resp,
                             test_mb_pdu_parse_diag_resp_invalid_len1,
                             test_mb_pdu_parse_diag_resp_invalid_len2,
                             test_mb_pdu_parse_diag_resp_invalid_len3,
                             test_mb_pdu_parse_get_com_ev_cntr_req,
                             test_mb_pdu_parse_get_com_ev_cntr_resp,
                             test_mb_pdu_parse_get_com_ev_log_req,
                             test_mb_pdu_parse_get_com_ev_log_resp,
                             test_mb_pdu_parse_get_com_ev_log_resp_invalid_byte_count1,
                             test_mb_pdu_parse_get_com_ev_log_resp_invalid_byte_count2,
                             test_mb_pdu_parse_wr_mult_coils_req,
                             test_mb_pdu_parse_wr_mult_coils_req_invalid_quant_ops1,
                             test_mb_pdu_parse_wr_mult_coils_req_invalid_quant_ops2,
                             test_mb_pdu_parse_wr_mult_coils_req_invalid_byte_count,
                             test_mb_pdu_parse_wr_mult_coils_req_invalid_end_addr,
                             test_mb_pdu_parse_wr_mult_coils_resp,
                             test_mb_pdu_parse_wr_mult_coils_resp_invalid_quant_ops1,
                             test_mb_pdu_parse_wr_mult_coils_resp_invalid_quant_ops2,
                             test_mb_pdu_parse_wr_mult_coils_resp_invalid_end_addr,
                             test_mb_pdu_parse_wr_mult_regs_req,
                             test_mb_pdu_parse_wr_mult_regs_req_invalid_quant_regs1,
                             test_mb_pdu_parse_wr_mult_regs_req_invalid_quant_regs2,
                             test_mb_pdu_parse_wr_mult_regs_req_invalid_byte_count,
                             test_mb_pdu_parse_wr_mult_regs_req_invalid_end_addr,
                             test_mb_pdu_parse_wr_mult_regs_resp,
                             test_mb_pdu_parse_wr_mult_regs_resp_invalid_quant_regs1,
                             test_mb_pdu_parse_wr_mult_regs_resp_invalid_quant_regs2,
                             test_mb_pdu_parse_wr_mult_regs_resp_invalid_end_addr,
                             test_mb_pdu_parse_rep_server_id_req,
                             test_mb_pdu_parse_rep_server_id_resp1,
                             test_mb_pdu_parse_rep_server_id_resp2,
                             test_mb_pdu_parse_rep_server_id_resp_invalid_byte_count1,
                             test_mb_pdu_parse_rep_server_id_resp_invalid_byte_count2,
                             test_mb_pdu_parse_rep_server_id_resp_invalid_run_ind_status,
                             test_mb_pdu_parse_rd_file_rec_req,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_byte_count1,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_byte_count2,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_ref_type,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_file_num,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_rec_num,
                             test_mb_pdu_parse_rd_file_rec_req_invalid_end_addr,
                             test_mb_pdu_parse_rd_file_rec_resp,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_resp_data_len1,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_resp_data_len2,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_ref_type,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len1,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len2,
                             test_mb_pdu_parse_rd_file_rec_resp_invalid_file_resp_len3,
                             test_mb_pdu_parse_wr_file_rec_req,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_req_data_len1,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_req_data_len2,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_ref_type,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_file_num,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_rec_num,
                             test_mb_pdu_parse_wr_file_rec_req_invalid_end_addr,
                             test_mb_pdu_parse_wr_file_rec_resp,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_resp_data_len1,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_resp_data_len2,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_ref_type,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_file_num,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_rec_num,
                             test_mb_pdu_parse_wr_file_rec_resp_invalid_end_addr,
                             test_mb_pdu_parse_mask_wr_reg_req,
                             test_mb_pdu_parse_mask_wr_reg_resp,
                             test_mb_pdu_parse_rd_wr_mult_regs_req,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_rd1,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_rd2,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_rd_end_addr,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_wr1,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_quant_wr2,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_wr_end_addr,
                             test_mb_pdu_parse_rd_wr_mult_regs_req_invalid_wr_byte_count1,
                             test_mb_pdu_parse_rd_wr_mult_regs_resp,
                             test_mb_pdu_parse_rd_wr_mult_regs_resp_invalid_byte_count1,
                             test_mb_pdu_parse_rd_wr_mult_regs_resp_invalid_byte_count2,
                             test_mb_pdu_parse_rd_fifo_q_req,
                             test_mb_pdu_parse_rd_fifo_q_resp,
                             test_mb_pdu_parse_rd_fifo_q_resp_invalid_byte_count1,
                             test_mb_pdu_parse_rd_fifo_q_resp_invalid_byte_count2,
                             test_mb_pdu_parse_rd_fifo_q_resp_invalid_fifo_count,
                             test_mb_pdu_parse_enc_if_trans_req,
                             test_mb_pdu_parse_enc_if_trans_resp,
                             test_mb_pdu_parse_err_resp,
                             test_mb_pdu_parse_err_resp_invalid_except_code};

    return mb_test_run(func, sizeof(func) / sizeof(func[0]));
}
