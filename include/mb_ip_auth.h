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

#ifndef MB_IP_AUTH_H
#define MB_IP_AUTH_H

#include <arpa/inet.h>

typedef struct mb_ip_auth_node
{
    struct in_addr addr;
    struct mb_ip_auth_node *next;
}
mb_ip_auth_node_t;

typedef struct
{
    mb_ip_auth_node_t *first;
    mb_ip_auth_node_t *last;
}
mb_ip_auth_list_t;

int mb_ip_auth_node_new(mb_ip_auth_node_t **node, const struct in_addr *addr);
void mb_ip_auth_node_delete(mb_ip_auth_node_t *node);

void mb_ip_auth_list_create(mb_ip_auth_list_t *list);
void mb_ip_auth_list_destroy(mb_ip_auth_list_t *list);
int mb_ip_auth_list_add_addr(mb_ip_auth_list_t *list, const struct in_addr *addr);
int mb_ip_auth_list_add_str(mb_ip_auth_list_t *list, const char *str);
int mb_ip_auth_list_check_addr(mb_ip_auth_list_t *list, const struct in_addr *addr);
int mb_ip_auth_list_check_str(mb_ip_auth_list_t *list, const char *str);

#endif
