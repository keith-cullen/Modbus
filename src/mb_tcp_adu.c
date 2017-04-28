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
#include <arpa/inet.h>
#include "mb_tcp_adu.h"

void mb_tcp_adu_set_header(mb_tcp_adu_t *adu, uint16_t trans_id, uint16_t proto_id, uint8_t unit_id)
{
    adu->trans_id = trans_id;
    adu->proto_id = proto_id;
    adu->unit_id = unit_id;
}

ssize_t mb_tcp_adu_format_req(mb_tcp_adu_t *adu, char *buf, size_t len)
{
    uint16_t *len_field = NULL;
    uint16_t val16 = 0;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    /* trans_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(adu->trans_id);
    memcpy(buf, &val16, sizeof(val16));
    num += 2;
    buf += 2;
    len -= 2;

    /* proto_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(adu->proto_id);
    memcpy(buf, &val16, sizeof(val16));
    num += 2;
    buf += 2;
    len -= 2;

    /* len */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    len_field = (uint16_t *)buf;
    num += 2;
    buf += 2;
    len -= 2;

    /* unit_id */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, &adu->unit_id, sizeof(adu->unit_id));
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

    /* len */
    adu->len = sizeof(uint8_t) + pdu_num;
    val16 = htons(adu->len);
    memcpy(len_field, &val16, sizeof(val16));

    return num;
}

/* also used to format an error response */
ssize_t mb_tcp_adu_format_resp(mb_tcp_adu_t *adu, char *buf, size_t len)
{
    uint16_t *len_field = NULL;
    uint16_t val16 = 0;
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    /* trans_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(adu->trans_id);
    memcpy(buf, &val16, sizeof(val16));
    num += 2;
    buf += 2;
    len -= 2;

    /* proto_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    val16 = htons(adu->proto_id);
    memcpy(buf, &val16, sizeof(val16));
    num += 2;
    buf += 2;
    len -= 2;

    /* len */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    len_field = (uint16_t *)buf;
    num += 2;
    buf += 2;
    len -= 2;

    /* unit_id */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    memcpy(buf, &adu->unit_id, sizeof(adu->unit_id));
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

    /* len */
    adu->len = sizeof(uint8_t) + pdu_num;
    val16 = htons(adu->len);
    memcpy(len_field, &val16, sizeof(val16));

    return num;
}

ssize_t mb_tcp_adu_parse_req(mb_tcp_adu_t *adu, const char *buf, size_t len)
{
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    memset(adu, 0, sizeof(mb_tcp_adu_t));

    /* trans_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->trans_id = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* proto_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->proto_id = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* len */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->len = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* unit_id */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->unit_id = *buf;
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    pdu_num = mb_pdu_parse_req(&adu->pdu, buf, len);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    if (adu->len != sizeof(uint8_t) + pdu_num)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;

    return num;
}

ssize_t mb_tcp_adu_parse_resp(mb_tcp_adu_t *adu, const char *buf, size_t len)
{
    ssize_t pdu_num = 0;
    ssize_t num = 0;

    memset(adu, 0, sizeof(mb_tcp_adu_t));

    /* trans_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->trans_id = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* proto_id */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->proto_id = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* len */
    if (len < 2)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->len = ntohs(*(uint16_t *)buf);
    num += 2;
    buf += 2;
    len -= 2;

    /* unit_id */
    if (len < 1)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;
    adu->unit_id = *buf;
    num += 1;
    buf += 1;
    len -= 1;

    /* pdu */
    pdu_num = mb_pdu_parse_resp(&adu->pdu, buf, len);
    if (pdu_num < 0)
        return pdu_num;
    num += pdu_num;
    buf += pdu_num;
    len -= pdu_num;

    if (adu->len != sizeof(uint8_t) + pdu_num)
        return -MB_PDU_EXCEPT_ILLEGAL_VAL;

    return num;
}

int mb_tcp_adu_to_str(mb_tcp_adu_t *adu, char *buf, size_t len)
{
    unsigned i = 0;
    uint16_t data_len = 0;
    uint8_t *data = NULL;
    int count = 0;
    int num = 0;

    num = snprintf(buf, len, "{trans_id: 0x%04x", adu->trans_id);
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;

    num = snprintf(buf, len, ", proto_id: 0x%04x", adu->proto_id);
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;

    num = snprintf(buf, len, ", len: 0x%04x", adu->len);
    buf += num;
    len = (num > len) ? 0 : len - num;
    count += num;

    num = snprintf(buf, len, ", unit_id: 0x%02x", adu->unit_id);
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
