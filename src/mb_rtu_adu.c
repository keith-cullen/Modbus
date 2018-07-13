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

static uint8_t mb_rtu_adu_crc_hi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static uint8_t mb_rtu_adu_crc_lo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static uint16_t mb_rtu_adu_calc_crc(const uint8_t *buf, size_t len)
{
    unsigned i = 0;
    uint8_t crc_hi = 0xff;
    uint8_t crc_lo = 0xff;
    uint8_t t = 0;

    for (i = 0; i < len; i++)
    {
        t = crc_lo ^ buf[i];
        crc_lo = crc_hi ^ mb_rtu_adu_crc_hi[t];
        crc_hi = mb_rtu_adu_crc_lo[t];
    }
    return ((uint16_t)crc_hi << 8) | (uint16_t)crc_lo;
}

int mb_rtu_adu_check_crc(const uint8_t *buf, size_t len)
{
    uint16_t exp = 0;
    uint16_t crc = 0;

    exp = mb_rtu_adu_calc_crc(buf, len - 2);
    crc = ((uint16_t)(uint8_t)buf[len - 1] << 8) | ((uint16_t)(uint8_t)buf[len - 2]);
    return crc == exp;
}

int mb_rtu_adu_valid_broadcast_req(mb_rtu_adu_t *adu)
{
    switch (adu->pdu.func_code)
    {
    case MB_PDU_WR_SING_COIL:
    case MB_PDU_WR_SING_REG:
    case MB_PDU_WR_MULT_COILS:
    case MB_PDU_WR_MULT_REGS:
    case MB_PDU_WR_FILE_REC:
    case MB_PDU_MASK_WR_REG:
        return 1;
    case MB_PDU_DIAG:
        switch (adu->pdu.diag_req.sub_func)
        {
        case MB_PDU_CLEAR_COUNTERS:
            return 1;
        }
    }
    return 0;
}

void mb_rtu_adu_set_header(mb_rtu_adu_t *adu, uint8_t addr)
{
    adu->addr = addr;
}

ssize_t mb_rtu_adu_format_req(mb_rtu_adu_t *adu, char *buf, size_t len)
{
    const uint8_t *start = (const uint8_t *)buf;
    uint16_t crc = 0;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    /* addr */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = adu->addr;
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    pdu_num = mb_pdu_format_req(&adu->pdu, buf, len);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    /* crc */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    crc = mb_rtu_adu_calc_crc(start, num);
    buf[0] = (uint8_t)(crc);
    buf[1] = (uint8_t)(crc >> 8);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

ssize_t mb_rtu_adu_format_resp(mb_rtu_adu_t *adu, char *buf, size_t len)
{
    const uint8_t *start = (const uint8_t *)buf;
    uint16_t crc = 0;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    /* addr */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    buf[0] = adu->addr;
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    pdu_num = mb_pdu_format_resp(&adu->pdu, buf, len);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    /* crc */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    crc = mb_rtu_adu_calc_crc(start, num);
    buf[0] = (uint8_t)(crc);
    buf[1] = (uint8_t)(crc >> 8);
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

ssize_t mb_rtu_adu_parse_req(mb_rtu_adu_t *adu, const char *buf, size_t len)
{
    const uint8_t *start = (const uint8_t *)buf;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    memset(adu, 0, sizeof(mb_rtu_adu_t));

    /* addr */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->addr = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu_num = mb_pdu_parse_req(&adu->pdu, buf, len - 2);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    /* crc */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (!mb_rtu_adu_check_crc(start, num + 2))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

ssize_t mb_rtu_adu_parse_resp(mb_rtu_adu_t *adu, const char *buf, size_t len)
{
    const uint8_t *start = (const uint8_t *)buf;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    memset(adu, 0, sizeof(mb_rtu_adu_t));

    /* addr */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->addr = buf[0];
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    pdu_num = mb_pdu_parse_resp(&adu->pdu, buf, len - 2);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    /* crc */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    if (!mb_rtu_adu_check_crc(start, num + 2))
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    num += 2;
    buf += 2;
    len -= 2;

    return num;
}

int mb_rtu_adu_to_str(mb_rtu_adu_t *adu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t data_len = 0;
    uint8_t *data = NULL;
    int count = 0;
    int num = 0;

    num = snprintf(buf, len, "{addr: 0x%02x", adu->addr);
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;

    num = snprintf(buf, len, ", func_code: 0x%02x", adu->pdu.func_code);
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;

    data = adu->pdu.def.buf + 1;  /* skip over func_code in pdu */
    data_len = adu->pdu.data_len;

    if (data_len > 0)
    {
        num = snprintf(buf, len, ", data: [");
        buf += num;
        len = (num > len) ? 0 : len - num;
        count += num;

        for (i = 0; i < data_len; i++)
        {
            if (i == 0)
                num = snprintf(buf, len, "0x%02x", data[i]);
            else
                num = snprintf(buf, len, " 0x%02x", data[i]);
            buf += num;
            len = (num > len) ? 0 : len - num;
            count += num;
        }

        num = snprintf(buf, len, "]");
        buf += num;
        len = (num > len) ? 0 : len - num;
        count += num;
    }
    num = snprintf(buf, len, "}");
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;
    return count;
}
