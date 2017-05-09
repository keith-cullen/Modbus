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

#ifndef MB_RTU_ADU_H
#define MB_RTU_ADU_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "mb_pdu.h"

/*  RTU application data unit
 *
 *  0         1           2      N    N+2
 *  +---------+-----------+------+-----+
 *  | address | func_code | data | CRC |
 *  +---------+-----------+------+-----+
 *                        |<---->|
 *                        0 to 252
 *  N   <= 254
 *  N+2 <= 256
 */

#define MB_RTU_ADU_MIN_LEN           3
#define MB_RTU_ADU_MAX_LEN           256
#define MB_RTU_ADU_BROADCAST_ADDR    0
#define MB_RTU_ADU_MIN_UNICAST_ADDR  1
#define MB_RTU_ADU_MAX_UNICAST_ADDR  247

/* Application Protocol Data Unit */
typedef struct
{
    uint8_t addr;
    mb_pdu_t pdu;
}
mb_rtu_adu_t;

int mb_rtu_adu_check_crc(const uint8_t *buf, size_t len);
int mb_rtu_adu_valid_broadcast_req(mb_rtu_adu_t *adu);
void mb_rtu_adu_set_header(mb_rtu_adu_t *adu, uint8_t addr);
ssize_t mb_rtu_adu_format_req(mb_rtu_adu_t *adu, char *buf, size_t len);
ssize_t mb_rtu_adu_format_resp(mb_rtu_adu_t *adu, char *buf, size_t len);
ssize_t mb_rtu_adu_parse_req(mb_rtu_adu_t *adu, const char *buf, size_t len);
ssize_t mb_rtu_adu_parse_resp(mb_rtu_adu_t *adu, const char *buf, size_t len);
int mb_rtu_adu_to_str(mb_rtu_adu_t *adu, char *buf, size_t len);

#endif
