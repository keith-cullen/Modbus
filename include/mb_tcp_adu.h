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

#ifndef MB_TCP_ADU_H
#define MB_TCP_ADU_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "mb_pdu.h"

/*  TCP application data unit
 *
 *  0          2          4     6         7           8      N <= 260
 *  +----------+----------+-----+---------+-----------+------+
 *  | trans_id | proto_id | len | unit_id | func_code | data |
 *  +----------+----------+-----+---------+-----------+------+
 *                              |<-------------------------->|
 *                                           len
 */

#define MB_TCP_ADU_MAX_LEN        260
#define MB_TCP_ADU_HEADER_LEN     7
#define MB_TCP_ADU_LEN_OFF        4
#define MB_TCP_ADU_DATA_LEN_DIFF  2    /* difference between the len field in a tcp adu and the len field in an pdu */

/* Application Protocol Data Unit */
typedef struct
{
    uint16_t trans_id;
    uint16_t proto_id;
    uint16_t len;
    uint8_t unit_id;
    mb_pdu_t pdu;
}
mb_tcp_adu_t;

void mb_tcp_adu_set_header(mb_tcp_adu_t *adu, uint16_t trans_id, uint16_t proto_id, uint8_t unit_id);
ssize_t mb_tcp_adu_format_req(mb_tcp_adu_t *adu, char *buf, size_t len);
ssize_t mb_tcp_adu_format_resp(mb_tcp_adu_t *adu, char *buf, size_t len);
ssize_t mb_tcp_adu_parse_req(mb_tcp_adu_t *adu, const char *buf, size_t len);
ssize_t mb_tcp_adu_parse_resp(mb_tcp_adu_t *adu, const char *buf, size_t len);
int mb_tcp_adu_to_str(mb_tcp_adu_t *adu, char *buf, size_t len);

#endif
