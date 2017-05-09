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

#include <stdio.h>
#include <string.h>
#include "mb_rtu_adu.h"
#include "mb_test.h"

int print_cols = 93;

mb_test_result_t test_mb_rtu_adu_set(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_type_t type = MB_PDU_DEF;
    const uint8_t func_code = 0x04;
    const uint8_t data[] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8};
    int ret = 0;

    printf("%-*s", print_cols, "test   1: set RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set(&adu.pdu, type, func_code, data, sizeof(data));
    if (ret < 0)
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.type != type)
    {
        return FAIL;
    }
    if (memcmp((char *)adu.pdu.def.buf, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    if (adu.pdu.data_len != sizeof(data))
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_coils_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 0x0123;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x01, 0xab, 0xcd, 0x01, 0x23, 0x7a, 0xcc};

    printf("%-*s", print_cols, "test   2: set and format 'Read Coils' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_coils_req(&adu.pdu, start_addr, quant_coils);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (memcmp(buf, exp, sizeof(exp)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_coils_req_invalid_quant_coils1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test   3: set 'Read Coils' request RTU ADU with invalid quant_coils");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_coils_req(&adu.pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_coils_req_invalid_quant_coils2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 2001;
    int ret = 0;

    printf("%-*s", print_cols, "test   4: set 'Read Coils' request RTU ADU with invalid quant_coils");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_coils_req(&adu.pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_coils_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_coils = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test   5: set 'Read Coils' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_coils_req(&adu.pdu, start_addr, quant_coils);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_coils_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t byte_count = 3;
    const uint8_t coil_stat[] = {0xaa, 0x55, 0x01};
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x01, 0x03, 0xaa, 0x55, 0x01, 0x1c, 0xe3};

    printf("%-*s", print_cols, "test   6: set and format 'Read Coils' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_coils_resp(&adu.pdu, byte_count, coil_stat);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_coils_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t byte_count = 255;
    const uint8_t coil_stat[255] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test   7: set 'Read Coils' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_coils_resp(&adu.pdu, byte_count, coil_stat);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 0x0123;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x02, 0xab, 0xcd, 0x01, 0x23, 0x7a, 0x88};

    printf("%-*s", print_cols, "test   8: set and format 'Read Discrete Inputs' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_disc_ips_req(&adu.pdu, start_addr, quant_ips);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_req_invalid_num_ips1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test   9: set 'Read Discrete Inputs' request RTU ADU with invalid num_ips");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_disc_ips_req(&adu.pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_req_invalid_num_ips2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ips = 2001;
    int ret = 0;

    printf("%-*s", print_cols, "test  10: set 'Read Discrete Inputs' request RTU ADU with invalid num_ips");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_disc_ips_req(&adu.pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ips = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  11: set 'Read Discrete Inputs' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_disc_ips_req(&adu.pdu, start_addr, quant_ips);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t byte_count = 3;
    const uint8_t ip_stat[] = {0xaa, 0x55, 0x01};
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x02, 0x03, 0xaa, 0x55, 0x01, 0x1c, 0xa7};

    printf("%-*s", print_cols, "test  12: set and format 'Read Discrete Inputs' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_disc_ips_resp(&adu.pdu, byte_count, ip_stat);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_disc_ips_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 255;
    const uint8_t ip_stat[255] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  13: set 'Read Discrete Inputs' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_disc_ips_resp(&adu.pdu, byte_count, ip_stat);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x03, 0xab, 0xcd, 0x00, 0x02, 0x32, 0x74};

    printf("%-*s", print_cols, "test  14: set and format 'Read Holding Registers' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_hold_regs_req(&adu.pdu, start_addr, quant_regs);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_req_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  15: set 'Read Holding Registers' request RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_hold_regs_req(&adu.pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_req_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  16: set 'Read Holding Registers' request RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_hold_regs_req(&adu.pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  17: set 'Read Holding Registers' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_hold_regs_req(&adu.pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t byte_count = 4;
    const uint16_t hold_reg[] = {0x1234, 0x5678};
    ssize_t num = 0;
    char buf[9] = {0};
    char exp[] = {0x03, 0x03, 0x04, 0x12, 0x34, 0x56, 0x78, 0xc7, 0xa2};

    printf("%-*s", print_cols, "test  18: set and format 'Read Holding Registers' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_hold_regs_resp(&adu.pdu, byte_count, hold_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_hold_regs_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 252;
    const uint16_t hold_reg[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  19: set 'Read Holding Registers' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_hold_regs_resp(&adu.pdu, byte_count, hold_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t num_ip_regs = 0x0002;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x04, 0xab, 0xcd, 0x00, 0x02, 0xf2, 0xc1};

    printf("%-*s", print_cols, "test  20: set and format 'Read Input Registers' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_ip_regs_req(&adu.pdu, start_addr, num_ip_regs);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_req_invalid_quant_ip_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ip_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  21: set 'Read Input Registers' request RTU ADU with invalid quant_ip_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_ip_regs_req(&adu.pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_req_invalid_quant_ip_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ip_regs = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  22: set 'Read Input Registers' request RTU ADU with invalid quant_ip_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_ip_regs_req(&adu.pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ip_regs = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  23: set 'Read Input Registers' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_ip_regs_req(&adu.pdu, start_addr, quant_ip_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t byte_count = 4;
    const uint16_t ip_reg[] = {0x1234, 0x5678};
    ssize_t num = 0;
    char buf[9] = {0};
    char exp[] = {0x03, 0x04, 0x04, 0x12, 0x34, 0x56, 0x78, 0x70, 0xa3};

    printf("%-*s", print_cols, "test  24: set and format 'Read Input Registers' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_ip_regs_resp(&adu.pdu, byte_count, ip_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_ip_regs_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 252;
    const uint16_t ip_reg[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  25: set 'Read Input Registers' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_ip_regs_resp(&adu.pdu, byte_count, ip_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_sing_coil_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t op_addr = 0xabcd;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x05, 0xab, 0xcd, 0xff, 0x00, 0x03, 0x3c};

    printf("%-*s", print_cols, "test  26: set and format 'Write Single Coil' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_sing_coil_req(&adu.pdu, op_addr, op_val);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_sing_coil_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t op_addr = 0xabcd;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x05, 0xab, 0xcd, 0xff, 0x00, 0x03, 0x3c};

    printf("%-*s", print_cols, "test  27: set and format 'Write Single Coil' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_sing_coil_resp(&adu.pdu, op_addr, op_val);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_sing_reg_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t reg_addr = 0xabcd;
    const uint16_t reg_val = 0x1234;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x06, 0xab, 0xcd, 0x12, 0x34, 0x84, 0x34};

    printf("%-*s", print_cols, "test  28: set and format 'Write Single Register' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_sing_reg_req(&adu.pdu, reg_addr, reg_val);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_sing_reg_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t reg_addr = 0xabcd;
    const uint16_t reg_val = 0x1234;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x06, 0xab, 0xcd, 0x12, 0x34, 0x84, 0x34};

    printf("%-*s", print_cols, "test  29: set and format 'Write Single Register' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_sing_reg_resp(&adu.pdu, reg_addr, reg_val);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_except_stat_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    ssize_t num = 0;
    char buf[4] = {0};
    char exp[] = {0x03, 0x07, 0x82, 0x40};

    printf("%-*s", print_cols, "test  30: set and format 'Read Exception Status' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_rd_except_stat_req(&adu.pdu);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_except_stat_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t out_data = 0x6d;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x03, 0x07, 0x6d, 0x1d, 0x42};

    printf("%-*s", print_cols, "test  31: set and format 'Read Exception Status' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_rd_except_stat_resp(&adu.pdu, out_data);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_diag_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t sub_func = 0x0001;
    const uint16_t data[1] = {0xa537};
    const uint8_t num_data = 1;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x08, 0x00, 0x01, 0xa5, 0x37, 0xaf, 0x8a};

    printf("%-*s", print_cols, "test  32: set and format 'Diagnostics' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_diag_req(&adu.pdu, sub_func, data, num_data);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_diag_req_invalid_num_data(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t sub_func = 0x0001;
    const uint16_t data[126] = {0};
    const uint8_t num_data = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  33: set 'Diagnostics' request RTU ADU with invalid num_data");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_diag_req(&adu.pdu, sub_func, data, num_data);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_diag_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t sub_func = 0x0001;
    const uint16_t data[1] = {0xa537};
    const uint8_t num_data = 1;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x08, 0x00, 0x01, 0xa5, 0x37, 0xaf, 0x8a};

    printf("%-*s", print_cols, "test  34: set and format 'Diagnostics' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_diag_resp(&adu.pdu, sub_func, data, num_data);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_diag_resp_invalid_num_data(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t sub_func = 0x0001;
    const uint16_t data[126] = {0};
    const uint8_t num_data = 126;
    int ret = 0;

    printf("%-*s", print_cols, "test  35: set 'Diagnostics' response RTU ADU with invalid num_data");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_diag_resp(&adu.pdu, sub_func, data, num_data);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_get_com_ev_cntr_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    ssize_t num = 0;
    char buf[4] = {0};
    char exp[] = {0x03, 0x0b, 0x87, 0x40};

    printf("%-*s", print_cols, "test  36: set and format 'Get Comm Event Counter' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_get_com_ev_cntr_req(&adu.pdu);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_get_com_ev_cntr_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t status = 0xffff;
    const uint16_t ev_cnt = 0x0108;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x0b, 0xff, 0xff, 0x01, 0x08, 0x9b, 0xa5};

    printf("%-*s", print_cols, "test  37: set and format 'Get Comm Event Counter' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_get_com_ev_cntr_resp(&adu.pdu, status, ev_cnt);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_get_com_ev_log_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    ssize_t num = 0;
    char buf[4] = {0};
    char exp[] = {0x03, 0x0c, 0x45, 0x01};

    printf("%-*s", print_cols, "test  38: set and format 'Get Comm Event Log' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_get_com_ev_log_req(&adu.pdu);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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
    
mb_test_result_t test_mb_rtu_adu_get_com_ev_log_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[2] = {0x20, 0x00};
    const uint8_t num_events = 2;
    ssize_t num = 0;
    char buf[13] = {0};
    char exp[] = {0x03, 0x0c, 0x08, 0x00, 0x00, 0x01, 0x08, 0x01, 0x21, 0x20, 0x00, 0x79, 0x06};

    printf("%-*s", print_cols, "test  39: set and format 'Get Comm Event Log' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_get_com_ev_log_resp(&adu.pdu, status, ev_cnt, msg_cnt, event, num_events);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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
    
mb_test_result_t test_mb_rtu_adu_get_com_ev_log_resp_invalid_num_events(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[246] = {0};
    const uint8_t num_events = 246;
    int ret = 0;

    printf("%-*s", print_cols, "test  40: set 'Get Comm Event Log' response RTU ADU with invalid num_events");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_get_com_ev_log_resp(&adu.pdu, status, ev_cnt, msg_cnt, event, num_events);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_coils_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0x000b;
    const uint8_t op_val[] = {0xa5, 0x09};
    ssize_t num = 0;
    char buf[11] = {0};
    char exp[] = {0x03, 0x0f, 0xab, 0xcd, 0x00, 0x0b, 0x02, 0xa5, 0x09, 0x25, 0x4c};

    printf("%-*s", print_cols, "test  41: set and format 'Write Multiple Coils' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_wr_mult_coils_req(&adu.pdu, start_addr, quant_ops, op_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_mult_coils_req_invalid_quant_ops1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0;
    uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  42: set 'Write Multiple Coils' request RTU ADU with invalid quant_ops");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_req(&adu.pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}
    
mb_test_result_t test_mb_rtu_adu_wr_mult_coils_req_invalid_quant_ops2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 1969;
    const uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  43: set 'Write Multiple Coils' request RTU ADU with invalid quant_ops");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_req(&adu.pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}
    
mb_test_result_t test_mb_rtu_adu_wr_mult_coils_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ops = 1;
    const uint8_t op_val[] = {0x00};
    int ret = 0;

    printf("%-*s", print_cols, "test  44: set 'Write Multiple Coils' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_req(&adu.pdu, start_addr, quant_ops, op_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}
    
mb_test_result_t test_mb_rtu_adu_wr_mult_coils_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0x000b;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x0f, 0xab, 0xcd, 0x00, 0x0b, 0x35, 0xa4};

    printf("%-*s", print_cols, "test  45: set and format 'Write Multiple Coils' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_mult_coils_resp(&adu.pdu, start_addr, quant_ops);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_mult_coils_resp_invalid_quant_ops1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  46: set 'Write Multiple Coils' response RTU ADU with invalid quant_ops");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_resp(&adu.pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_coils_resp_invalid_quant_ops2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_ops = 1969;
    int ret = 0;

    printf("%-*s", print_cols, "test  47: set 'Write Multiple Coils' response RTU ADU with invalid quant_ops");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_resp(&adu.pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}
    
mb_test_result_t test_mb_rtu_adu_wr_mult_coils_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_ops = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  48: set 'Write Multiple Coils' response RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_coils_resp(&adu.pdu, start_addr, quant_ops);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    const uint8_t byte_count = 0x04;
    const uint16_t reg_val[] = {0xa519, 0xc5e2};
    ssize_t num = 0;
    char buf[13] = {0};
    char exp[] = {0x03, 0x10, 0xab, 0xcd, 0x00, 0x02, 0x04, 0xa5, 0x19, 0xc5, 0xe2, 0xfb, 0x9e};

    printf("%-*s", print_cols, "test  49: set and format 'Write Multiple Registers' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_wr_mult_regs_req(&adu.pdu, start_addr, quant_regs, byte_count, reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_req_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    const uint8_t byte_count = 0;
    const uint16_t reg_val[124] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  50: set 'Write Multiple Registers' request RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_req(&adu.pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_req_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 124;
    const uint8_t byte_count = 248;
    const uint16_t reg_val[124] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  51: set 'Write Multiple Registers' request RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_req(&adu.pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_req_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 2;
    const uint8_t byte_count = 3;
    const uint16_t reg_val[2] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  52: set 'Write Multiple Registers' request RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_req(&adu.pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 1;
    const uint8_t byte_count = 2;
    const uint16_t reg_val[1] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test  53: set 'Write Multiple Registers' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_req(&adu.pdu, start_addr, quant_regs, byte_count, reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[8] = {0};
    char exp[] = {0x03, 0x10, 0xab, 0xcd, 0x00, 0x02, 0xf1, 0xf1};

    printf("%-*s", print_cols, "test  54: set and format 'Write Multiple Registers' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_wr_mult_regs_resp(&adu.pdu, start_addr, quant_regs);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_resp_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  55: set 'Write Multiple Registers' response RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_resp(&adu.pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_resp_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_regs = 124;
    int ret = 0;

    printf("%-*s", print_cols, "test  56: set 'Write Multiple Registers' response RTU ADU with invalid quant_regs");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_mult_regs_resp(&adu.pdu, start_addr, quant_regs);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_mult_regs_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t start_addr = 0xffff;
    const uint16_t quant_regs = 0x0001;
    ssize_t num = 0;

    printf("%-*s", print_cols, "test  57: set and format 'Write Multiple Registers' response RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_wr_mult_regs_resp(&adu.pdu, start_addr, quant_regs);
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rep_server_id_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    ssize_t num = 0;
    char buf[4] = {0};
    char exp[] = {0x03, 0x11, 0x4c, 0xc1};

    printf("%-*s", print_cols, "test  58: set and format 'Report Server ID' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_rep_server_id_req(&adu.pdu);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rep_server_id_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 5;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    char buf[10] = {0};
    char exp[] = {0x03, 0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0xff, 0x97, 0x74};
    ssize_t num = 0;

    printf("%-*s", print_cols, "test  59: set and format 'Report Server ID' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rep_server_id_resp(&adu.pdu, byte_count, server_id, run_ind_status);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rep_server_id_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 0;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    int ret = 0;

    printf("%-*s", print_cols, "test  60: set and format 'Report Server ID' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rep_server_id_resp(&adu.pdu, byte_count, server_id, run_ind_status);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rep_server_id_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 252;
    const uint8_t server_id[253] = {0};
    const bool run_ind_status = true;
    int ret = 0;

    printf("%-*s", print_cols, "test  61: set and format 'Report Server ID' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rep_server_id_resp(&adu.pdu, byte_count, server_id, run_ind_status);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    ssize_t num = 0;
    char buf[19] = {0};
    char exp[] = {0x03, 0x14, 0x0e, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0x9d, 0x55};

    printf("%-*s", print_cols, "test  62: set and format 'Read File Record' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_num_sub_req1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[36] = {{0}};
    const size_t num_sub_req = 36;
    int ret = 0;

    printf("%-*s", print_cols, "test  63: set 'Read File Record' request RTU ADU with invalid num_sub_req");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_num_sub_req2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[0];
    const size_t num_sub_req = 0;
    int ret = 0;

    printf("%-*s", print_cols, "test  64: set 'Read File Record' request RTU ADU with invalid num_sub_req");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  65: set 'Read File Record' request RTU ADU with invalid ref_type");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  66: set 'Read File Record' request RTU ADU with invalid file_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                         [1] = {.file_num = 0x0003, .rec_num = 0x2710, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  67: set 'Read File Record' request RTU ADU with invalid rec_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001},
                                                         [1] = {.ref_type = 6, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  68: set 'Read File Record' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x0dfe, 0x0020}},
                                                          [1] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    ssize_t num = 0;
    char buf[17] = {0};
    char exp[] = {0x03, 0x14, 0x0c, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40, 0xa0, 0xfb};

    printf("%-*s", print_cols, "test  69: set and format 'Read File Record' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_num_sub_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[36] = {{0}};
    const size_t num_sub_req = 36;
    int ret = 0;

    printf("%-*s", print_cols, "test  70: set 'Read File Record' response RTU ADU with invalid num_sub_req");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_file_resp_len1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[1] = {{.file_resp_len = 0, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  71: set 'Read File Record' response RTU ADU with invalid file_resp_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_file_resp_len2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[1] = {{.file_resp_len = 247, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  72: set 'Read File Record' response RTU ADU with invalid file_resp_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 5, .rec_data = {0x0dfe, 0x0020}},
                                                          [1] = {.file_resp_len = 0x05, .ref_type = 6, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  73: set 'Read File Record' response RTU ADU with invalid ref_type");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 245, .ref_type = 6, .rec_data = {0}},
                                                          [1] = {.file_resp_len = 245, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  74: set 'Read File Record' response RTU ADU with invalid resp_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 1, .ref_type = 6, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  75: set 'Read File Record' response RTU ADU with invalid resp_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    char buf[18] = {0};
    char exp[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x6a, 0x57};

    printf("%-*s", print_cols, "test  76: set and format 'Write File Record' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_num_sub_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[28] = {{0}};
    const size_t num_sub_req = 28;
    int ret = 0;

    printf("%-*s", print_cols, "test  77: set 'Write File Record' request RTU ADU with invalid num_sub_req");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  78: set 'Write File Record' request RTU ADU with invalid ref_type");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  79: set 'Write File Record' request RTU ADU with invalid file_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x2710, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  80: set 'Write File Record' request RTU ADU with invalid rec_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_rec_len(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x007b, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  81: set 'Write File Record' request RTU ADU with invalid rec_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  82: set 'Write File Record' request RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_req_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0000, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  83: set 'Write File Record' request RTU ADU with invalid req_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_req_invalid_req_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[2] = {[0] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}},
                                                      [1] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  84: set 'Write File Record' request RTU ADU with invalid req_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_req(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    char buf[18] = {0};
    char exp[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x6a, 0x57};

    printf("%-*s", print_cols, "test  85: set and format 'Write File Record' resonse RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_num_sub_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[28] = {{0}};
    const size_t num_sub_req = 28;
    int ret = 0;

    printf("%-*s", print_cols, "test  86: set 'Write File Record' response RTU ADU with invalid num_sub_req");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 5, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  87: set 'Write File Record' response RTU ADU with invalid ref_type");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0000, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  88: set 'Write File Record' response RTU ADU with invalid file_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x2710, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  89: set 'Write File Record' response RTU ADU with invalid rec_num");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_rec_len(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x007b, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  90: set 'Write File Record' response RTU ADU with invalid rec_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x270f, .rec_len = 0x0001, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  91: set 'Write File Record' response RTU ADU with invalid end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 6, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0000, .rec_data = {0}}};
    const size_t num_sub_req = 1;
    int ret = 0;

    printf("%-*s", print_cols, "test  92: set 'Write File Record' response RTU ADU with invalid resp_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_wr_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[2] = {[0] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}},
                                                      [1] = {.ref_type = 6, .file_num = 0x0001, .rec_num = 0x0001, .rec_len = 0x0078, .rec_data = {0}}};
    const size_t num_sub_req = 2;
    int ret = 0;

    printf("%-*s", print_cols, "test  93: set 'Write File Record' response RTU ADU with invalid resp_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_wr_file_rec_resp(&adu.pdu, sub_req, num_sub_req);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_mask_wr_reg_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[10] = {0};
    char exp[] = {0x03, 0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25, 0x37, 0xe6};

    printf("%-*s", print_cols, "test  94: set and format 'Mask Write Register' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_mask_wr_reg_req(&adu.pdu, ref_addr, and_mask, or_mask);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_mask_wr_reg_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[10] = {0};
    char exp[] = {0x03, 0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25, 0x37, 0xe6};

    printf("%-*s", print_cols, "test  95: set and format 'Mask Write Register' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_mask_wr_reg_resp(&adu.pdu, ref_addr, and_mask, or_mask);
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    ssize_t num = 0;
    char buf[19] = {0};
    char exp[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0xe, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf1, 0xe7};

    printf("%-*s", print_cols, "test  96: set and format 'Read/Write Multiple Registers' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_rd1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x03;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  97: set 'Read/Write Multiple Registers' request RTU ADU with invalid quant_rd");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_rd2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 126;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x03;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  98: set 'Read/Write Multiple Registers' request RTU ADU with invalid quant_rd");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_rd_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0xffff;
    const uint16_t quant_rd = 0x0001;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    int ret = 0;

    printf("%-*s", print_cols, "test  99: set 'Read/Write Multiple Registers' request RTU ADU with invalid rd_end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_wr1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0;
    const uint16_t wr_reg_val[122] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 100: set 'Read/Write Multiple Registers' request RTU ADU with invalid quant_wr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_wr2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 122;
    const uint16_t wr_reg_val[122] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 101: set 'Read/Write Multiple Registers' request RTU ADU with invalid quant_wr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_wr_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0xffff;
    const uint16_t quant_wr = 0x0001;
    const uint16_t wr_reg_val[] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 102: set 'Read/Write Multiple Registers' request RTU ADU with invalid wr_end_addr");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_req(&adu.pdu, rd_start_addr, quant_rd, wr_start_addr, quant_wr, wr_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 0x0c;
    const uint16_t rd_reg_val[6] = {0x00fe, 0x0acd, 0x0001, 0x0003, 0x000d, 0x00ff};
    ssize_t num = 0;
    char buf[17] = {0};
    char exp[] = {0x03, 0x17, 0x0c, 0x00, 0xfe, 0x0a, 0xcd, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0d, 0x00, 0xff, 0x78, 0x9f};

    printf("%-*s", print_cols, "test 103: set and format 'Read/Write Multiple Registers' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_wr_mult_regs_resp(&adu.pdu, byte_count, rd_reg_val);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 252;
    const uint16_t rd_reg_val[126] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 104: set 'Read/Write Multiple Registers' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_resp(&adu.pdu, byte_count, rd_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_wr_mult_reg_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t byte_count = 1;
    const uint16_t rd_reg_val[1] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 105: set 'Read/Write Multiple Registers' response RTU ADU with invalid byte_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_wr_mult_regs_resp(&adu.pdu, byte_count, rd_reg_val);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_rd_fifo_q_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t fifo_ptr_addr = 0x04de;
    ssize_t num = 0;
    char buf[6] = {0};
    char exp[] = {0x03, 0x18, 0x04, 0xde, 0xff, 0x02};

    printf("%-*s", print_cols, "test 106: set and format 'Read FIFO Queue' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    mb_pdu_set_rd_fifo_q_req(&adu.pdu, fifo_ptr_addr);
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_fifo_q_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t fifo_count = 0x0002;
    const uint16_t fifo_val_reg[2] = {0x01be, 0x1284};
    ssize_t num = 0;
    char buf[12] = {0};
    char exp[] = {0x03, 0x18, 0x00, 0x06, 0x00, 0x02, 0x01, 0xbe, 0x12, 0x84, 0xd3, 0x58};

    printf("%-*s", print_cols, "test 107: set and format 'Read FIFO Queue' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_rd_fifo_q_resp(&adu.pdu, fifo_count, fifo_val_reg);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_rd_fifo_q_resp_invalid_fifo_count(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint16_t fifo_count = 32;
    const uint16_t fifo_val_reg[32] = {0};
    int ret = 0;

    printf("%-*s", print_cols, "test 108: set 'Read FIFO Queue' response RTU ADU with invalid fifo_count");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_rd_fifo_q_resp(&adu.pdu, fifo_count, fifo_val_reg);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_enc_if_trans_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[9] = {0};
    char exp[] = {0x03, 0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04, 0x15, 0x17};

    printf("%-*s", print_cols, "test 109: set and format 'Encapsulated Interface Transport' request RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_enc_if_trans_req(&adu.pdu, mei_type, mei_data, mei_data_len);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_req(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_enc_if_trans_req_invalid_mei_data_len(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[252] = {0};
    const uint8_t mei_data_len = 252;
    int ret = 0;

    printf("%-*s", print_cols, "test 110: set 'Encapsulated Interface Transport' request RTU ADU with invalid mei_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_enc_if_trans_req(&adu.pdu, mei_type, mei_data, mei_data_len);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_enc_if_trans_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[9] = {0};
    char exp[] = {0x03, 0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04, 0x15, 0x17};

    printf("%-*s", print_cols, "test 111: set and format 'Encapsulated Interface Transport' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_enc_if_trans_resp(&adu.pdu, mei_type, mei_data, mei_data_len);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_enc_if_trans_resp_invalid_mei_data_len(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[252] = {0};
    const uint8_t mei_data_len = 252;
    int ret = 0;

    printf("%-*s", print_cols, "test 112: set 'Encapsulated Interface Transport' response RTU ADU with invalid mei_data_len");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_enc_if_trans_resp(&adu.pdu, mei_type, mei_data, mei_data_len);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_err_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x02;
    ssize_t num = 0;
    char buf[5] = {0};
    char exp[] = {0x03, 0x81, 0x02, 0x51, 0x60};

    printf("%-*s", print_cols, "test 113: set and format 'Error' response RTU ADU");
    mb_rtu_adu_set_header(&adu, addr);
    num = mb_pdu_set_err_resp(&adu.pdu, func_code, except_code);
    if (num < 0)
    {
        return FAIL;
    }
    num = mb_rtu_adu_format_resp(&adu, buf, sizeof(buf));
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

mb_test_result_t test_mb_rtu_adu_err_resp_invalid_func_code(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x01;
    const uint8_t except_code = 0x02;
    int ret = 0;

    printf("%-*s", print_cols, "test 114: set and format 'Error' response RTU ADU with invalid func_code");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_err_resp(&adu.pdu, func_code, except_code);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_err_resp_invalid_except_code(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x07;
    int ret = 0;

    printf("%-*s", print_cols, "test 115: set and format 'Error' response RTU ADU with invalid except_code");
    mb_rtu_adu_set_header(&adu, addr);
    ret = mb_pdu_set_err_resp(&adu.pdu, func_code, except_code);
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x01;
    const mb_pdu_type_t type = MB_PDU_REQ;
    const uint16_t start_addr = 0xabcd;
    const uint16_t quant_coils = 0x0123;
    char buf[] = {0x03, 0x01, 0xab, 0xcd, 0x01, 0x23, 0x7a, 0xcc};
    ssize_t num = 0;

    printf("%-*s", print_cols, "test 116: parse 'Read Coils' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.type != type)
    {
        return FAIL;
    }
    if (adu.pdu.rd_coils_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_coils_req.quant_coils != quant_coils)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_req_invalid_quant_coils1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01, 0x00, 0x13, 0x00, 0x00, 0x2d, 0xcc};

    printf("%-*s", print_cols, "test 117: parse 'Read Coils' request RTU ADU with invalid quant_coils");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_req_invalid_quant_coils2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01, 0x00, 0x13, 0x07, 0xd1, 0x41, 0x0e};

    printf("%-*s", print_cols, "test 118: parse 'Read Coils' request RTU ADU with invalid quant_coils");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01, 0xff, 0xff, 0x00, 0x01, 0x0c, 0xfc};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 119: parse 'Read Coils' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x01;
    const uint8_t byte_count = 0x03;
    const uint8_t coil_stat[3] = {0xcd, 0x6b, 0x05};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01, 0x03, 0xcd, 0x6b, 0x05, 0x60, 0x43};

    printf("%-*s", print_cols, "test 120: parse 'Read Coils' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_coils_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_coils_resp.coil_stat, coil_stat, byte_count) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_coils_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x01, 0xfb, 0xd3, 0xc1};

    printf("%-*s", print_cols, "test 121: parse 'Read Coils' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x02;
    const uint16_t start_addr = 0x00c4;
    const uint16_t quant_ips = 0x0016;
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0x00, 0xc4, 0x00, 0x16, 0xdb, 0xb9};

    printf("%-*s", print_cols, "test 122: parse 'Read Discrete Inputs' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_disc_ips_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_disc_ips_req.quant_ips != quant_ips)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_quant_ips1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0x00, 0xc4, 0x00, 0x00, 0x15, 0x38};

    printf("%-*s", print_cols, "test 123: parse 'Read Discrete Inputs' request RTU ADU with invalid quant_ips");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_quant_ips2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0x00, 0xc4, 0x07, 0xd1, 0x79, 0xfa};

    printf("%-*s", print_cols, "test 124: parse 'Read Discrete Inputs' request RTU ADU with invalid quant_ips");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0xff, 0xff, 0x00, 0x01, 0x0c, 0xb8};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 125: parse 'Read Discrete Inputs' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x02;
    const uint8_t byte_count = 0x03;
    const uint8_t ip_stat[3] = {0xac, 0xdb, 0x35};
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0x03, 0xac, 0xdb, 0x35, 0x6a, 0x23};

    printf("%-*s", print_cols, "test 126: parse 'Read Discrete Inputs' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_disc_ips_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_disc_ips_resp.ip_stat, ip_stat, byte_count) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_disc_ips_resp_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x02, 0xfb, 0x23, 0xc1};

    printf("%-*s", print_cols, "test 127: parse 'Read Discrete Inputs' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x03;
    const uint16_t start_addr = 0x006b;
    const uint16_t quant_regs = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0x00, 0x6b, 0x00, 0x03, 0xf5, 0x75};

    printf("%-*s", print_cols, "test 128: parse 'Read Holding Registers' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_hold_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_hold_regs_req.quant_regs != quant_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0x00, 0x6b, 0x00, 0x00, 0xf4, 0x35};

    printf("%-*s", print_cols, "test 129: parse 'Read Holding Registers' request RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0x00, 0x6b, 0x00, 0x7e, 0xd4, 0xb5};

    printf("%-*s", print_cols, "test 130: parse 'Read Holding Registers' request RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0xff, 0xff, 0x00, 0x01, 0xcc, 0x85};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 131: parse 'Read Holding Registers' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x03;
    const uint8_t byte_count = 0x06;
    const uint16_t reg_val[3] = {0x022b, 0x0000, 0x0064};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0x06, 0x02, 0x2b, 0x00, 0x00, 0x00, 0x64, 0x1a, 0x1c};

    printf("%-*s", print_cols, "test 132: parse 'Read Holding Registers' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_hold_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_hold_regs_resp.reg_val, reg_val, sizeof(reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0x01, 0xf0, 0x40};

    printf("%-*s", print_cols, "test 133: parse 'Read Holding Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_hold_regs_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x03, 0xfc, 0x71, 0x81};

    printf("%-*s", print_cols, "test 134: parse 'Read Holding Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x04;
    const uint16_t start_addr = 0x0008;
    const uint16_t quant_ip_regs = 0x0001;
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0x00, 0x08, 0x00, 0x01, 0xea, 0xb1};

    printf("%-*s", print_cols, "test 135: parse 'Read Input Registers' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_ip_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_ip_regs_req.quant_ip_regs != quant_ip_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_quant_ip_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0x00, 0x08, 0x00, 0x00, 0x2a, 0x70};

    printf("%-*s", print_cols, "test 136: parse 'Read Input Registers' request RTU ADU with invalid quant_ip_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_quant_ip_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0x00, 0x08, 0x00, 0x7e, 0x0a, 0xf0};

    printf("%-*s", print_cols, "test 137: parse 'Read Input Registers' request RTU ADU with invalid quant_ip_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0xff, 0xff, 0x00, 0x01, 0x0c, 0x30};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 138: parse 'Read Input Registers' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x04;
    const uint8_t byte_count = 0x02;
    const uint16_t ip_reg[1] = {0x000a};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0x02, 0x00, 0x0a, 0xf7, 0x40};

    printf("%-*s", print_cols, "test 139: parse 'Read Input Registers' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_ip_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_ip_regs_resp.ip_reg, ip_reg, sizeof(ip_reg)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0x01, 0x00, 0x31, 0x40};

    printf("%-*s", print_cols, "test 140: parse 'Read Input Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_ip_regs_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x04, 0xfc, 0x41, 0x83};

    printf("%-*s", print_cols, "test 141: parse 'Read Input Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_req1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = false;
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0x00, 0x00, 0x09, 0x0c};

    printf("%-*s", print_cols, "test 142: parse 'Write Single Coil' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_req.op_addr != op_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_req.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_req2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0xff, 0x00, 0xf9, 0x4d};

    printf("%-*s", print_cols, "test 143: parse 'Write Single Coil' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_req.op_addr != op_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_req.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_req_invalid_op_val(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0x00, 0x01, 0xc9, 0xcd};

    printf("%-*s", print_cols, "test 144: parse 'Write Single Coil' request RTU ADU with invalid op_val");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_resp1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = false;
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0x00, 0x00, 0x09, 0x0c};

    printf("%-*s", print_cols, "test 145: parse 'Write Single Coil' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_resp.op_addr != op_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_resp.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_resp2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x05;
    const uint16_t op_addr = 0x00ac;
    const bool op_val = true;
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0xff, 0x00, 0xf9, 0x4d};

    printf("%-*s", print_cols, "test 146: parse 'Write Single Coil' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_resp.op_addr != op_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_coil_resp.op_val != op_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_coil_resp_invalid_op_val(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x05, 0x00, 0xac, 0x00, 0x01, 0xc9, 0xcd};

    printf("%-*s", print_cols, "test 147: parse 'Write Single Coil' response RTU ADU with invalid op_val");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_reg_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x06;
    const uint16_t reg_addr = 0x0001;
    const uint16_t reg_val = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x03, 0x06, 0x00, 0x01, 0x00, 0x03, 0xe9, 0x99};

    printf("%-*s", print_cols, "test 148: parse 'Write Single Register' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_reg_req.reg_addr != reg_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_reg_req.reg_val != reg_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_sing_reg_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x06;
    const uint16_t reg_addr = 0x0001;
    const uint16_t reg_val = 0x0003;
    ssize_t num = 0;
    char buf[] = {0x03, 0x06, 0x00, 0x01, 0x00, 0x03, 0xe9, 0x99};

    printf("%-*s", print_cols, "test 149: parse 'Write Single Register' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_reg_resp.reg_addr != reg_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_sing_reg_resp.reg_val != reg_val)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_except_stat_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x07;
    ssize_t num = 0;
    char buf[] = {0x03, 0x07, 0x82, 0x40};

    printf("%-*s", print_cols, "test 150: parse 'Read Exception Status' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_except_stat_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x07;
    const uint8_t out_data = 0x6d;
    ssize_t num = 0;
    char buf[] = {0x03, 0x07, 0x6d, 0x1d, 0x42};

    printf("%-*s", print_cols, "test 151: parse 'Read Exception Status' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_except_stat_resp.out_data != out_data)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_diag_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x08;
    const uint16_t sub_func = 0x0000;
    const uint16_t data[1] = {0xa537};
    ssize_t num = 0;
    char buf[] = {0x03, 0x08, 0x00, 0x00, 0xa5, 0x37, 0x6f, 0xdb};

    printf("%-*s", print_cols, "test 152: parse 'Diagnostics' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.diag_req.sub_func != sub_func)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.diag_req.data, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_diag_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x08;
    const uint16_t sub_func = 0x0000;
    const uint16_t data[1] = {0xa537};
    ssize_t num = 0;
    char buf[] = {0x03, 0x08, 0x00, 0x00, 0xa5, 0x37, 0x6f, 0xdb};

    printf("%-*s", print_cols, "test 153: parse 'Diagnostics' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.diag_resp.sub_func != sub_func)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.diag_resp.data, data, sizeof(data)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_diag_resp_invalid_len1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x08, 0x00, 0x00, 0xa5, 0x37, 0x00, 0x5b, 0x2f};  /* len not even */

    printf("%-*s", print_cols, "test 154: parse 'Diagnostics' response RTU ADU with invalid len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_diag_resp_invalid_len2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x08, 0x00, 0x00, 0xa2, 0x81};  /* len too short */

    printf("%-*s", print_cols, "test 155: parse 'Diagnostics' response RTU ADU with invalid len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_diag_resp_invalid_len3(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[255] = {0x03, 0x08, 0x00, 0x00};  /* len too large, note: CRC is not calculated for this one */

    printf("%-*s", print_cols, "test 156: parse 'Diagnostics' response RTU ADU with invalid len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_cntr_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0b;
    ssize_t num = 0;
    char buf[] = {0x03, 0x0b, 0x87, 0x40};

    printf("%-*s", print_cols, "test 157: parse 'Get Comm Event Counter' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_cntr_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0b;
    const uint16_t status = 0xffff;
    const uint16_t ev_cnt = 0x0108;
    ssize_t num = 0;
    char buf[] = {0x03, 0x0b, 0xff, 0xff, 0x01, 0x08, 0x9b, 0xa5};

    printf("%-*s", print_cols, "test 158: parse 'Get Comm Event Counter' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_cntr_resp.status != status)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_cntr_resp.ev_cnt != ev_cnt)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_log_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0c;
    ssize_t num = 0;
    char buf[] = {0x03, 0x0c, 0x45, 0x01};

    printf("%-*s", print_cols, "test 159: parse 'Get Comm Event Log' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_log_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0c;
    const uint8_t byte_count = 0x08;
    const uint16_t status = 0x0000;
    const uint16_t ev_cnt = 0x0108;
    const uint16_t msg_cnt = 0x0121;
    const uint8_t event[2] = {0x20, 0x00};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0c, 0x08, 0x00, 0x00, 0x01, 0x08, 0x01, 0x21, 0x20, 0x00, 0x79, 0x06};

    printf("%-*s", print_cols, "test 160: parse 'Get Comm Event Log' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_log_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_log_resp.status != status)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_log_resp.ev_cnt != ev_cnt)
    {
        return FAIL;
    }
    if (adu.pdu.get_com_ev_log_resp.msg_cnt != msg_cnt)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.get_com_ev_log_resp.event, event, sizeof(event)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_log_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0c, 0x02, 0x01, 0x05};  /* byte_count too short */

    printf("%-*s", print_cols, "test 161: parse 'Get Comm Event Log' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_get_com_ev_log_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[254] = {0x03, 0x0c, 0xfc};  /* byte_count too long, note: CRC is not calculated for this one */

    printf("%-*s", print_cols, "test 162: parse 'Get Comm Event Log' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0f;
    const uint16_t start_addr = 0x0013;
    const uint16_t quant_ops = 0x000a;
    const uint8_t byte_count = 0x02;
    const uint8_t op_val[2] = {0xcd, 0x01};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01, 0xab, 0x6b};

    printf("%-*s", print_cols, "test 163: parse 'Write Multiple Coils' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_coils_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_coils_req.quant_ops != quant_ops)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_coils_req.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.wr_mult_coils_req.op_val, op_val, sizeof(op_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_quant_ops1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x00, 0x00, 0x00, 0x7b, 0x2c};  /* quant_ops too small (and byte_count too small) */

    printf("%-*s", print_cols, "test 164: parse 'Write Multiple Coils' request RTU ADU with invalid quant_ops");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_quant_ops2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x07, 0xb1, 0xf7, 0x6c, 0xa8};  /* quant_ops too large (and byte_count too large) */

    printf("%-*s", print_cols, "test 165: parse 'Write Multiple Coils' request RTU ADU with invalid quant_ops");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x00, 0x01, 0x00, 0xeb, 0x2d};  /* quant_ops != byte_count */

    printf("%-*s", print_cols, "test 166: parse 'Write Multiple Coils' request RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0xff, 0xff, 0x00, 0x01, 0x01, 0x00, 0x55, 0xaf};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 167: parse 'Write Multiple Coils' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x0f;
    const uint16_t start_addr = 0x0013;
    const uint16_t quant_ops = 0x000a;
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x00, 0x0a, 0xeb, 0x25};

    printf("%-*s", print_cols, "test 168: parse 'Write Multiple Coils' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_coils_resp.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_coils_resp.quant_ops != quant_ops)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_quant_ops1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x00, 0x00, 0xec, 0xa5};  /* quant_ops too small */

    printf("%-*s", print_cols, "test 169: parse 'Write Multiple Coils' response RTU ADU with invalid quant_ops");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_quant_ops2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0x00, 0x13, 0x07, 0xb1, 0xa8, 0x67};  /* quant_ops too large */

    printf("%-*s", print_cols, "test 170: parse 'Write Multiple Coils' response RTU ADU with invalid quant_ops");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x0f, 0xff, 0xff, 0x00, 0x01, 0x01, 0xaf, 0xcc};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 171: parse 'Write Multiple Coils' response RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x10;
    const uint16_t start_addr = 0x0001;
    const uint16_t quant_regs = 0x0002;
    const uint8_t byte_count = 0x04;
    const uint16_t reg_val[2] = {0x000a, 0x0102};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0a, 0x01, 0x02, 0x88, 0x99};

    printf("%-*s", print_cols, "test 172: parse 'Write Multiple Registers' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_regs_req.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_regs_req.quant_regs != quant_regs)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_regs_req.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.wr_mult_regs_req.reg_val, reg_val, sizeof(reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x6c, 0x2b};  /* quant_regs too small (and byte_count too small) */

    printf("%-*s", print_cols, "test 173: parse 'Write Multiple Registers' request RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x7c, 0xf8, 0x2e, 0x0a};  /* quant_regs too large (and byte_count too large) */

    printf("%-*s", print_cols, "test 174: parse 'Write Multiple Registers' request RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_byte_count(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x02, 0x01, 0xcc, 0xeb};  /* byte_count != quant_regs */

    printf("%-*s", print_cols, "test 175: parse 'Write Multiple Registers' request RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0xff, 0xff, 0x00, 0x01, 0x02, 0x00, 0x00, 0x30, 0xa4};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 176: parse 'Write Multiple Registers' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x10;
    const uint16_t start_addr = 0x0001;
    const uint16_t quant_regs = 0x0002;
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x02, 0xea, 0x11};

    printf("%-*s", print_cols, "test 177: parse 'Write Multiple Registers' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_regs_resp.start_addr != start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.wr_mult_regs_resp.quant_regs != quant_regs)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_quant_regs1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0x00, 0x01, 0x00, 0x00, 0x2b, 0x90};  /* quant_regs too small */

    printf("%-*s", print_cols, "test 178: parse 'Write Multiple Registers' response RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_quant_regs2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x03, 0x10, 0x00, 0x01, 0x00, 0x7c, 0x2f, 0x97};  /* quant_regs too large */

    printf("%-*s", print_cols, "test 179: parse 'Write Multiple Registers' response RTU ADU with invalid quant_regs");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x10, 0xff, 0xff, 0x00, 0x01, 0x02, 0x00, 0x00, 0x30, 0xa4};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 180: parse 'Write Multiple Registers' response RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x11;
    ssize_t num = 0;
    char buf[] = {0x03, 0x11, 0x4c, 0xc1};

    printf("%-*s", print_cols, "test 181: parse 'Report server ID' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_resp1(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x11;
    const uint8_t byte_count = 0x05;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = false;
    ssize_t num = 0;
    char buf[] = {0x03, 0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0x00, 0xd7, 0x34};

    printf("%-*s", print_cols, "test 182: parse 'Report server ID' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rep_server_id_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rep_server_id_resp.server_id, server_id, sizeof(server_id)) != 0)
    {
        return FAIL;
    }
    if (adu.pdu.rep_server_id_resp.run_ind_status != run_ind_status)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_resp2(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x11;
    const uint8_t byte_count = 0x05;
    const uint8_t server_id[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    const bool run_ind_status = true;
    ssize_t num = 0;
    char buf[] = {0x03, 0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0xff, 0x97, 0x74};

    printf("%-*s", print_cols, "test 183: parse 'Report server ID' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rep_server_id_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rep_server_id_resp.server_id, server_id, sizeof(server_id)) != 0)
    {
        return FAIL;
    }
    if (adu.pdu.rep_server_id_resp.run_ind_status != run_ind_status)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    char buf[] = {0x03, 0x11, 0x00, 0x90, 0x8d};
    int ret = 0;

    printf("%-*s", print_cols, "test 184: parse 'Report server ID' response RTU ADU with invalid byte_count");
    ret = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    char buf[257] = {0x03, 0x11, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
                     0x20};
    int ret = 0;

    printf("%-*s", print_cols, "test 185: parse 'Report server ID' response RTU ADU with invalid byte_count");
    ret = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rep_server_id_resp_invalid_run_ind_status(void)
{
    mb_rtu_adu_t adu = {0};
    char buf[] = {0x03, 0x11, 0x05, 0xa1, 0xb2, 0xc3, 0xd4, 0x01, 0x09, 0xee};
    int ret = 0;

    printf("%-*s", print_cols, "test 186: parse 'Report server ID' response RTU ADU with invalid run_ind_status");
    ret = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (ret != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x14;
    const uint8_t byte_count = 0x0e;
    const mb_pdu_rd_file_rec_req_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0001, .rec_len = 0x0002},
                                                        [1] = {.ref_type = 0x06, .file_num = 0x0003, .rec_num = 0x0009, .rec_len = 0x0002}};
    const size_t num_sub_req = 2;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0e, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0x9d, 0x55};

    printf("%-*s", print_cols, "test 187: parse 'Read File Record' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_file_rec_req.byte_count != byte_count)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (adu.pdu.rd_file_rec_req.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (adu.pdu.rd_file_rec_req.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (adu.pdu.rd_file_rec_req.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (adu.pdu.rd_file_rec_req.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x06, 0xc2, 0x0e};  /* byte_count too small */

    printf("%-*s", print_cols, "test 188: parse 'Read File Record' request RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0xf6, 0x86, 0x0e};  /* byte_count too large */

    printf("%-*s", print_cols, "test 189: parse 'Read File Record' request RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0e, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0x51, 0xa7};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 190: parse 'Read File Record' request RTU ADU with invalid ref_type");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0x52, 0xa4};  /* file_num too small */

    printf("%-*s", print_cols, "test 191: parse 'Read File Record' request RTU ADU with invalid file_num");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0e, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x02, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0x79, 0xb4};  /* rec_num too large */

    printf("%-*s", print_cols, "test 192: parse 'Read File Record' request RTU ADU with invalid rec_num");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0e, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01, 0x06, 0x00, 0x03, 0x00, 0x09, 0x00, 0x02, 0xc9, 0xc5};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 193: parse 'Read File Record' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x14;
    const uint8_t resp_data_len = 0x0c;
    const mb_pdu_rd_file_rec_resp_sub_req_t sub_req[] = {[0] = {.file_resp_len = 0x05, .ref_type = 0x06, .rec_data = {0x0dfe, 0x0020}},
                                                         [1] = {.file_resp_len = 0x05, .ref_type = 0x06, .rec_data = {0x33cd, 0x0040}}};
    const size_t num_sub_req = 2;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0c, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40, 0xa0, 0xfb};

    printf("%-*s", print_cols, "test 194: parse 'Read File Record' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_file_rec_resp.resp_data_len != resp_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (adu.pdu.rd_file_rec_resp.sub_req[i].file_resp_len != sub_req[i].file_resp_len)
        {
            return FAIL;
        }
        if (adu.pdu.rd_file_rec_resp.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (memcmp(adu.pdu.rd_file_rec_resp.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].file_resp_len - 1) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x06, 0xc2, 0x0e};  /* resp_data_len too small */

    printf("%-*s", print_cols, "test 195: parse 'Read File Record' response RTU ADU with invalid resp_data_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0xf6, 0x86, 0x0e};  /* resp_data_len too large */

    printf("%-*s", print_cols, "test 196: parse 'Read File Record' response RTU ADU with invalid resp_data_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x0c, 0x05, 0x07, 0x0d, 0xfe, 0x00, 0x20, 0x05, 0x06, 0x33, 0xcd, 0x00, 0x40, 0x5c, 0xff};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 197: parse 'Read File Record' response RTU ADU with invalid ref_type");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x00, 0x61, 0x6e};  /* file_resp_len too small */

    printf("%-*s", print_cols, "test 198: parse 'Read File Record' response RTU ADU with invalid file_resp_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0xf7, 0xe7, 0x2f};  /* file_resp_len too large */

    printf("%-*s", print_cols, "test 199: parse 'Read File Record' response RTU ADU with invalid file_resp_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len3(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x14, 0x07, 0x05, 0x06, 0x0d, 0xfe, 0x00, 0x20, 0x08, 0xa7, 0x6f};  /* file_resp_len even */

    printf("%-*s", print_cols, "test 200: parse 'Read File Record' response RTU ADU with invalid file_resp_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x15;
    const uint8_t req_data_len = 0x0d;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    ssize_t num = 0;
    unsigned i = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x6a, 0x57};

    printf("%-*s", print_cols, "test 201: parse 'Write File Record' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_file_rec_req.req_data_len != req_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (adu.pdu.wr_file_rec_req.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_req.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_req.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_req.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
        if (memcmp(adu.pdu.wr_file_rec_req.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].rec_len * 2) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_req_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x07, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x7f, 0x15};  /* req_data_len too small */

    printf("%-*s", print_cols, "test 202: parse 'Write File Record' request RTU ADU with invalid req_data_len");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_req_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0xfc, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x88, 0x5b};  /* req_data_len too large */

    printf("%-*s", print_cols, "test 203: parse 'Write File Record' request RTU ADU with invalid req_data_len");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x05, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0xa9, 0x52};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 204: parse 'Write File Record' request RTU ADU with invalid ref_type");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x5a, 0x42};  /* file_num too small */

    printf("%-*s", print_cols, "test 205: parse 'Write File Record' request RTU ADU with invalid file_num");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x2a, 0x0c};  /* rec_num too large */

    printf("%-*s", print_cols, "test 206: parse 'Write File Record' request RTU ADU with invalid rec_num");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_req_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01, 0xb6, 0xdf};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 207: parse 'Write File Record' request RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x15;
    const uint8_t resp_data_len = 0x0d;
    const mb_pdu_wr_file_rec_sub_req_t sub_req[] = {[0] = {.ref_type = 0x06, .file_num = 0x0004, .rec_num = 0x0007, .rec_len = 0x0003, .rec_data = {0x06af, 0x04be, 0x100d}}};
    const size_t num_sub_req = 1;
    unsigned i = 0;
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x6a, 0x57};

    printf("%-*s", print_cols, "test 208: parse 'Write File Record' response RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.wr_file_rec_resp.resp_data_len != resp_data_len)
    {
        return FAIL;
    }
    for (i = 0; i < num_sub_req; i++)
    {
        if (adu.pdu.wr_file_rec_resp.sub_req[i].ref_type != sub_req[i].ref_type)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_resp.sub_req[i].file_num != sub_req[i].file_num)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_resp.sub_req[i].rec_num != sub_req[i].rec_num)
        {
            return FAIL;
        }
        if (adu.pdu.wr_file_rec_resp.sub_req[i].rec_len != sub_req[i].rec_len)
        {
            return FAIL;
        }
        if (memcmp(adu.pdu.wr_file_rec_resp.sub_req[i].rec_data, sub_req[i].rec_data, sub_req[i].rec_len * 2) != 0)
        {
            return FAIL;
        }
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_resp_data_len1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x07, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x7f, 0x15};

    printf("%-*s", print_cols, "test 209: parse 'Write File Record' response RTU ADU with invalid resp_data_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_resp_data_len2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0xfc, 0x06, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x88, 0x5b};

    printf("%-*s", print_cols, "test 210: parse 'Write File Record' response RTU ADU with invalid resp_data_len");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_ref_type(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x99, 0x47};  /* ref_type != 6 */

    printf("%-*s", print_cols, "test 211: parse 'Write File Record' response RTU ADU with invalid ref_type");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_file_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x5a, 0x42};

    printf("%-*s", print_cols, "test 212: parse 'Write File Record' response RTU ADU with invalid file_num");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_rec_num(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x10, 0x00, 0x03, 0x06, 0xaf, 0x04, 0xbe, 0x10, 0x0d, 0x2a, 0x02};

    printf("%-*s", print_cols, "test 213: parse 'Write File Record' response RTU ADU with invalid rec_num");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x15, 0x0d, 0x06, 0x00, 0x04, 0x27, 0x0f, 0x00, 0x01, 0xb6, 0xdf};  /* 0x270f + 0x0001 > 0x270f */

    printf("%-*s", print_cols, "test 214: parse 'Write File Record' response RTU ADU with invalid end_addr");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_mask_wr_reg_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x16;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[] = {0x03, 0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25, 0x37, 0xe6};

    printf("%-*s", print_cols, "test 215: parse 'Mask Write Register' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_req.ref_addr != ref_addr)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_req.and_mask != and_mask)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_req.or_mask != or_mask)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_mask_wr_reg_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x16;
    const uint16_t ref_addr = 0x0004;
    const uint16_t and_mask = 0x00f2;
    const uint16_t or_mask = 0x0025;
    ssize_t num = 0;
    char buf[] = {0x03, 0x16, 0x00, 0x04, 0x00, 0xf2, 0x00, 0x25, 0x37, 0xe6};

    printf("%-*s", print_cols, "test 216: parse 'Mask Write Register' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_resp.ref_addr != ref_addr)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_resp.and_mask != and_mask)
    {
        return FAIL;
    }
    if (adu.pdu.mask_wr_reg_resp.or_mask != or_mask)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x17;
    const uint16_t rd_start_addr = 0x0003;
    const uint16_t quant_rd = 0x0006;
    const uint16_t wr_start_addr = 0x000e;
    const uint16_t quant_wr = 0x0003;
    const uint8_t wr_byte_count = 0x06;
    const uint16_t wr_reg_val[] = {0x00ff, 0x00ff, 0x00ff};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf1, 0xe7};

    printf("%-*s", print_cols, "test 217: parse 'Read/Write Multiple Registers' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_req.rd_start_addr != rd_start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_req.quant_rd != quant_rd)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_req.wr_start_addr != wr_start_addr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_req.quant_wr != quant_wr)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_req.wr_byte_count != wr_byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_wr_mult_regs_req.wr_reg_val, wr_reg_val, quant_wr) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_rd1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf9, 0xef};  /* quant_rd too small */

    printf("%-*s", print_cols, "test 218: parse 'Read/Write Multiple Registers' request RTU ADU with invalid quant_rd");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_rd2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x7e, 0x00, 0x0e, 0x00, 0x03, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x51, 0x47};  /* quant_rd too large */

    printf("%-*s", print_cols, "test 219: parse 'Read/Write Multiple Registers' request RTU ADU with invalid quant_rd");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_rd_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xb2};  /* 0xffff + 0x0001 > 0xffff */

    printf("%-*s", print_cols, "test 220: parse 'Read/Write Multiple Registers' request RTU ADU with invalid rd_end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_ADDR)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_wr1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x00, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xe4, 0xa7};  /* quant_wr too small */

    printf("%-*s", print_cols, "test 221: parse 'Read/Write Multiple Registers' request RTU ADU with invalid quant_wr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_wr2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x7e, 0x06, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x4c, 0x21};  /* quant_wr too large */

    printf("%-*s", print_cols, "test 222: parse 'Read/Write Multiple Registers' request RTU ADU with invalid quant_wr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_wr_end_addr(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0xff, 0xff, 0x00, 0x01, 0x01, 0xcc, 0x1d};  /* 0xffff + 0x0001 */

    printf("%-*s", print_cols, "test 223: parse 'Read/Write Multiple Registers' request RTU ADU with invalid wr_end_addr");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_wr_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x05, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf1, 0xd4};  /* byte_count != 2 * quant_wr */

    printf("%-*s", print_cols, "test 224: parse 'Read/Write Multiple Registers' request RTU ADU with invalid wr_byte_count");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x17;
    const uint8_t byte_count = 0x0c;
    const uint16_t rd_reg_val[] = {0x00fe, 0x0acd, 0x0001, 0x0003, 0x000d, 0x00ff};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x0c, 0x00, 0xfe, 0x0a, 0xcd, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0d, 0x00, 0xff, 0x78, 0x9f};

    printf("%-*s", print_cols, "test 225: parse 'Read/Write Multiple Registers' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_wr_mult_regs_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_wr_mult_regs_resp.rd_reg_val, rd_reg_val, sizeof(rd_reg_val)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0x01, 0x00, 0xf4, 0xb1};  /* byte_count not even */

    printf("%-*s", print_cols, "test 226: parse 'Read/Write Multiple Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_wr_mult_regs_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x17, 0xfc, 0x71, 0x8e};  /* byte_count too large */

    printf("%-*s", print_cols, "test 227: parse 'Read/Write Multiple Registers' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_fifo_q_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x18;
    const uint16_t fifo_ptr_addr = 0x04de;
    ssize_t num = 0;
    char buf[] = {0x03, 0x18, 0x04, 0xde, 0xff, 0x02};

    printf("%-*s", print_cols, "test 228: parse 'Read FIFO Queue' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_fifo_q_req.fifo_ptr_addr != fifo_ptr_addr)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_fifo_q_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x18;
    const uint16_t byte_count = 0x0006;
    const uint16_t fifo_count = 0x0002;
    const uint16_t fifo_val_reg[] = {0x01b8, 0x1284};
    ssize_t num = 0;
    char buf[] = {0x03, 0x18, 0x00, 0x06, 0x00, 0x02, 0x01, 0xb8, 0x12, 0x84, 0xd2, 0xb8};

    printf("%-*s", print_cols, "test 229: parse 'Read FIFO Queue' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.rd_fifo_q_resp.byte_count != byte_count)
    {
        return FAIL;
    }
    if (adu.pdu.rd_fifo_q_resp.fifo_count != fifo_count)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.rd_fifo_q_resp.fifo_val_reg, fifo_val_reg, sizeof(fifo_val_reg)) != 0)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_byte_count1(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x18, 0x00, 0x01, 0xa7, 0x41};  /* byte_count not even */

    printf("%-*s", print_cols, "test 230: parse 'Read FIFO Queue' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_byte_count2(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x18, 0x00, 0x04, 0x00, 0x02, 0x01, 0xb8, 0x12, 0x84, 0x12, 0x9b};  /* byte_count does not match fifo_count */

    printf("%-*s", print_cols, "test 231: parse 'Read FIFO Queue' response RTU ADU with invalid byte_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_fifo_count(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x18, 0x00, 0x42, 0x00, 0x20, 0xe6, 0x81};  /* fifo_count too large */

    printf("%-*s", print_cols, "test 232: parse 'Read FIFO Queue' response RTU ADU with invalid fifo_count");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_enc_if_trans_req(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x2b;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[] = {0x03, 0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04, 0x15, 0x17};

    printf("%-*s", print_cols, "test 233: parse 'Encapsulated Interface Transport' request RTU ADU");
    num = mb_rtu_adu_parse_req(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.enc_if_trans_req.mei_type != mei_type)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.enc_if_trans_req.mei_data, mei_data, sizeof(mei_data)) != 0)
    {
        return FAIL;
    }
    if (adu.pdu.enc_if_trans_req.mei_data_len != mei_data_len)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_enc_if_trans_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x2b;
    const uint8_t mei_type = 0x0a;
    const uint8_t mei_data[] = {0x01, 0x02, 0x03, 0x04};
    const uint8_t mei_data_len = 4;
    ssize_t num = 0;
    char buf[] = {0x03, 0x2b, 0x0a, 0x01, 0x02, 0x03, 0x04, 0x15, 0x17};

    printf("%-*s", print_cols, "test 234: parse 'Encapsulated Interface Transport' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.enc_if_trans_resp.mei_type != mei_type)
    {
        return FAIL;
    }
    if (memcmp(adu.pdu.enc_if_trans_resp.mei_data, mei_data, sizeof(mei_data)) != 0)
    {
        return FAIL;
    }
    if (adu.pdu.enc_if_trans_resp.mei_data_len != mei_data_len)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_err_resp(void)
{
    mb_rtu_adu_t adu = {0};
    const uint8_t addr = 0x03;
    const uint8_t func_code = 0x81;
    const uint8_t except_code = 0x02;
    ssize_t num = 0;
    char buf[] = {0x03, 0x81, 0x02, 0x51, 0x60};

    printf("%-*s", print_cols, "test 235: parse 'Error' response RTU ADU");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != sizeof(buf))
    {
        return FAIL;
    }
    if (adu.addr != addr)
    {
        return FAIL;
    }
    if (adu.pdu.func_code != func_code)
    {
        return FAIL;
    }
    if (adu.pdu.err.except_code != except_code)
    {
        return FAIL;
    }
    return PASS;
}

mb_test_result_t test_mb_rtu_adu_parse_err_resp_invalid_except_code(void)
{
    mb_rtu_adu_t adu = {0};
    ssize_t num = 0;
    char buf[] = {0x03, 0x81, 0x07, 0x52, 0xa0};

    printf("%-*s", print_cols, "test 236: parse 'Error' response RTU ADU with invalid except_code");
    num = mb_rtu_adu_parse_resp(&adu, buf, sizeof(buf));
    if (num != -MB_PDU_EXCEPT_ILLEGAL_VAL)
    {
        return FAIL;
    }
    return PASS;
}

int main(void)
{
    mb_test_func_t func[] = {test_mb_rtu_adu_set,
                             test_mb_rtu_adu_rd_coils_req,
                             test_mb_rtu_adu_rd_coils_req_invalid_quant_coils1,
                             test_mb_rtu_adu_rd_coils_req_invalid_quant_coils2,
                             test_mb_rtu_adu_rd_coils_req_invalid_end_addr,
                             test_mb_rtu_adu_rd_coils_resp,
                             test_mb_rtu_adu_rd_coils_resp_invalid_byte_count,
                             test_mb_rtu_adu_rd_disc_ips_req,
                             test_mb_rtu_adu_rd_disc_ips_req_invalid_num_ips1,
                             test_mb_rtu_adu_rd_disc_ips_req_invalid_num_ips2,
                             test_mb_rtu_adu_rd_disc_ips_req_invalid_end_addr,
                             test_mb_rtu_adu_rd_disc_ips_resp,
                             test_mb_rtu_adu_rd_disc_ips_resp_invalid_byte_count,
                             test_mb_rtu_adu_rd_hold_regs_req,
                             test_mb_rtu_adu_rd_hold_regs_req_invalid_quant_regs1,
                             test_mb_rtu_adu_rd_hold_regs_req_invalid_quant_regs2,
                             test_mb_rtu_adu_rd_hold_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_rd_hold_regs_resp,
                             test_mb_rtu_adu_rd_hold_regs_resp_invalid_byte_count,
                             test_mb_rtu_adu_rd_ip_regs_req,
                             test_mb_rtu_adu_rd_ip_regs_req_invalid_quant_ip_regs1,
                             test_mb_rtu_adu_rd_ip_regs_req_invalid_quant_ip_regs2,
                             test_mb_rtu_adu_rd_ip_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_rd_ip_regs_resp,
                             test_mb_rtu_adu_rd_ip_regs_resp_invalid_byte_count,
                             test_mb_rtu_adu_wr_sing_coil_req,
                             test_mb_rtu_adu_wr_sing_coil_resp,
                             test_mb_rtu_adu_wr_sing_reg_req,
                             test_mb_rtu_adu_wr_sing_reg_resp,
                             test_mb_rtu_adu_rd_except_stat_req,
                             test_mb_rtu_adu_rd_except_stat_resp,
                             test_mb_rtu_adu_diag_req,
                             test_mb_rtu_adu_diag_req_invalid_num_data,
                             test_mb_rtu_adu_diag_resp,
                             test_mb_rtu_adu_diag_resp_invalid_num_data,
                             test_mb_rtu_adu_get_com_ev_cntr_req,
                             test_mb_rtu_adu_get_com_ev_cntr_resp,
                             test_mb_rtu_adu_get_com_ev_log_req,
                             test_mb_rtu_adu_get_com_ev_log_resp,
                             test_mb_rtu_adu_get_com_ev_log_resp_invalid_num_events,
                             test_mb_rtu_adu_wr_mult_coils_req,
                             test_mb_rtu_adu_wr_mult_coils_req_invalid_quant_ops1,
                             test_mb_rtu_adu_wr_mult_coils_req_invalid_quant_ops2,
                             test_mb_rtu_adu_wr_mult_coils_req_invalid_end_addr,
                             test_mb_rtu_adu_wr_mult_coils_resp,
                             test_mb_rtu_adu_wr_mult_coils_resp_invalid_quant_ops1,
                             test_mb_rtu_adu_wr_mult_coils_resp_invalid_quant_ops2,
                             test_mb_rtu_adu_wr_mult_coils_resp_invalid_end_addr,
                             test_mb_rtu_adu_wr_mult_regs_req,
                             test_mb_rtu_adu_wr_mult_regs_req_invalid_quant_regs1,
                             test_mb_rtu_adu_wr_mult_regs_req_invalid_quant_regs2,
                             test_mb_rtu_adu_wr_mult_regs_req_invalid_byte_count,
                             test_mb_rtu_adu_wr_mult_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_wr_mult_regs_resp,
                             test_mb_rtu_adu_wr_mult_regs_resp_invalid_quant_regs1,
                             test_mb_rtu_adu_wr_mult_regs_resp_invalid_quant_regs2,
                             test_mb_rtu_adu_wr_mult_regs_resp_invalid_end_addr,
                             test_mb_rtu_adu_rep_server_id_req,
                             test_mb_rtu_adu_rep_server_id_resp,
                             test_mb_rtu_adu_rep_server_id_resp_invalid_byte_count1,
                             test_mb_rtu_adu_rep_server_id_resp_invalid_byte_count2,
                             test_mb_rtu_adu_rd_file_rec_req,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_num_sub_req1,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_num_sub_req2,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_ref_type,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_file_num,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_rec_num,
                             test_mb_rtu_adu_rd_file_rec_req_invalid_end_addr,
                             test_mb_rtu_adu_rd_file_rec_resp,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_num_sub_req,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_file_resp_len1,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_file_resp_len2,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_ref_type,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_resp_data_len1,
                             test_mb_rtu_adu_rd_file_rec_resp_invalid_resp_data_len2,
                             test_mb_rtu_adu_wr_file_rec_req,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_num_sub_req,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_ref_type,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_file_num,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_rec_num,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_rec_len,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_end_addr,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_req_data_len1,
                             test_mb_rtu_adu_wr_file_rec_req_invalid_req_data_len2,
                             test_mb_rtu_adu_wr_file_rec_resp,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_num_sub_req,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_ref_type,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_file_num,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_rec_num,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_rec_len,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_end_addr,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_resp_data_len1,
                             test_mb_rtu_adu_wr_file_rec_resp_invalid_resp_data_len2,
                             test_mb_rtu_adu_mask_wr_reg_req,
                             test_mb_rtu_adu_mask_wr_reg_resp,
                             test_mb_rtu_adu_rd_wr_mult_reg_req,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_rd1,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_rd2,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_rd_end_addr,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_wr1,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_quant_wr2,
                             test_mb_rtu_adu_rd_wr_mult_reg_req_invalid_wr_end_addr,
                             test_mb_rtu_adu_rd_wr_mult_reg_resp,
                             test_mb_rtu_adu_rd_wr_mult_reg_resp_invalid_byte_count1,
                             test_mb_rtu_adu_rd_wr_mult_reg_resp_invalid_byte_count2,
                             test_mb_rtu_adu_rd_fifo_q_req,
                             test_mb_rtu_adu_rd_fifo_q_resp,
                             test_mb_rtu_adu_rd_fifo_q_resp_invalid_fifo_count,
                             test_mb_rtu_adu_enc_if_trans_req,
                             test_mb_rtu_adu_enc_if_trans_req_invalid_mei_data_len,
                             test_mb_rtu_adu_enc_if_trans_resp,
                             test_mb_rtu_adu_enc_if_trans_resp_invalid_mei_data_len,
                             test_mb_rtu_adu_err_resp,
                             test_mb_rtu_adu_err_resp_invalid_func_code,
                             test_mb_rtu_adu_err_resp_invalid_except_code,
                             test_mb_rtu_adu_parse_rd_coils_req,
                             test_mb_rtu_adu_parse_rd_coils_req_invalid_quant_coils1,
                             test_mb_rtu_adu_parse_rd_coils_req_invalid_quant_coils2,
                             test_mb_rtu_adu_parse_rd_coils_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_rd_coils_resp,
                             test_mb_rtu_adu_parse_rd_coils_resp_invalid_byte_count,
                             test_mb_rtu_adu_parse_rd_disc_ips_req,
                             test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_quant_ips1,
                             test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_quant_ips2,
                             test_mb_rtu_adu_parse_rd_disc_ips_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_rd_disc_ips_resp,
                             test_mb_rtu_adu_parse_rd_disc_ips_resp_invalid_byte_count,
                             test_mb_rtu_adu_parse_rd_hold_regs_req,
                             test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_quant_regs1,
                             test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_quant_regs2,
                             test_mb_rtu_adu_parse_rd_hold_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_rd_hold_regs_resp,
                             test_mb_rtu_adu_parse_rd_hold_regs_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rd_hold_regs_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_rd_ip_regs_req,
                             test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_quant_ip_regs1,
                             test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_quant_ip_regs2,
                             test_mb_rtu_adu_parse_rd_ip_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_rd_ip_regs_resp,
                             test_mb_rtu_adu_parse_rd_ip_regs_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rd_ip_regs_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_wr_sing_coil_req1,
                             test_mb_rtu_adu_parse_wr_sing_coil_req2,
                             test_mb_rtu_adu_parse_wr_sing_coil_req_invalid_op_val,
                             test_mb_rtu_adu_parse_wr_sing_coil_resp1,
                             test_mb_rtu_adu_parse_wr_sing_coil_resp2,
                             test_mb_rtu_adu_parse_wr_sing_coil_resp_invalid_op_val,
                             test_mb_rtu_adu_parse_wr_sing_reg_req,
                             test_mb_rtu_adu_parse_wr_sing_reg_resp,
                             test_mb_rtu_adu_parse_rd_except_stat_req,
                             test_mb_rtu_adu_parse_rd_except_stat_resp,
                             test_mb_rtu_adu_parse_diag_req,
                             test_mb_rtu_adu_parse_diag_resp,
                             test_mb_rtu_adu_parse_diag_resp_invalid_len1,
                             test_mb_rtu_adu_parse_diag_resp_invalid_len2,
                             test_mb_rtu_adu_parse_diag_resp_invalid_len3,
                             test_mb_rtu_adu_parse_get_com_ev_cntr_req,
                             test_mb_rtu_adu_parse_get_com_ev_cntr_resp,
                             test_mb_rtu_adu_parse_get_com_ev_log_req,
                             test_mb_rtu_adu_parse_get_com_ev_log_resp,
                             test_mb_rtu_adu_parse_get_com_ev_log_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_get_com_ev_log_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_wr_mult_coils_req,
                             test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_quant_ops1,
                             test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_quant_ops2,
                             test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_byte_count,
                             test_mb_rtu_adu_parse_wr_mult_coils_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_wr_mult_coils_resp,
                             test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_quant_ops1,
                             test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_quant_ops2,
                             test_mb_rtu_adu_parse_wr_mult_coils_resp_invalid_end_addr,
                             test_mb_rtu_adu_parse_wr_mult_regs_req,
                             test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_quant_regs1,
                             test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_quant_regs2,
                             test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_byte_count,
                             test_mb_rtu_adu_parse_wr_mult_regs_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_wr_mult_regs_resp,
                             test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_quant_regs1,
                             test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_quant_regs2,
                             test_mb_rtu_adu_parse_wr_mult_regs_resp_invalid_end_addr,
                             test_mb_rtu_adu_parse_rep_server_id_req,
                             test_mb_rtu_adu_parse_rep_server_id_resp1,
                             test_mb_rtu_adu_parse_rep_server_id_resp2,
                             test_mb_rtu_adu_parse_rep_server_id_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rep_server_id_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_rep_server_id_resp_invalid_run_ind_status,
                             test_mb_rtu_adu_parse_rd_file_rec_req,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_byte_count2,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_ref_type,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_file_num,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_rec_num,
                             test_mb_rtu_adu_parse_rd_file_rec_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_rd_file_rec_resp,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_resp_data_len1,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_resp_data_len2,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_ref_type,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len1,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len2,
                             test_mb_rtu_adu_parse_rd_file_rec_resp_invalid_file_resp_len3,
                             test_mb_rtu_adu_parse_wr_file_rec_req,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_req_data_len1,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_req_data_len2,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_ref_type,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_file_num,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_rec_num,
                             test_mb_rtu_adu_parse_wr_file_rec_req_invalid_end_addr,
                             test_mb_rtu_adu_parse_wr_file_rec_resp,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_resp_data_len1,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_resp_data_len2,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_ref_type,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_file_num,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_rec_num,
                             test_mb_rtu_adu_parse_wr_file_rec_resp_invalid_end_addr,
                             test_mb_rtu_adu_parse_mask_wr_reg_req,
                             test_mb_rtu_adu_parse_mask_wr_reg_resp,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_rd1,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_rd2,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_rd_end_addr,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_wr1,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_quant_wr2,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_wr_end_addr,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_req_invalid_wr_byte_count1,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_resp,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rd_wr_mult_regs_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_rd_fifo_q_req,
                             test_mb_rtu_adu_parse_rd_fifo_q_resp,
                             test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_byte_count1,
                             test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_byte_count2,
                             test_mb_rtu_adu_parse_rd_fifo_q_resp_invalid_fifo_count,
                             test_mb_rtu_adu_parse_enc_if_trans_req,
                             test_mb_rtu_adu_parse_enc_if_trans_resp,
                             test_mb_rtu_adu_parse_err_resp,
                             test_mb_rtu_adu_parse_err_resp_invalid_except_code
    };

    return mb_test_run(func, sizeof(func) / sizeof(func[0]));
}
