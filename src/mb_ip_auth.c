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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mb_ip_auth.h"

int mb_ip_auth_node_new(mb_ip_auth_node_t **node, const struct in_addr *addr)
{
    *node = (mb_ip_auth_node_t *)malloc(sizeof(mb_ip_auth_node_t));
    if (*node == NULL)
    {
        return -ENOMEM;
    }
    memcpy(&(*node)->addr, addr, sizeof(struct in_addr));
    (*node)->next = NULL;
    return 0;
}

void mb_ip_auth_node_delete(mb_ip_auth_node_t *node)
{
    free(node);
}

void mb_ip_auth_list_create(mb_ip_auth_list_t *list)
{
    memset(list, 0, sizeof(mb_ip_auth_list_t));
}

void mb_ip_auth_list_destroy(mb_ip_auth_list_t *list)
{
    mb_ip_auth_node_t *prev = NULL;
    mb_ip_auth_node_t *node = NULL;

    node = list->first;
    while (node != NULL)
    {
        prev = node;
        node = node->next;
        mb_ip_auth_node_delete(prev);
    }
}

int mb_ip_auth_list_add_addr(mb_ip_auth_list_t *list, const struct in_addr *addr)
{
    mb_ip_auth_node_t *node = NULL;
    int ret = 0;

    ret = mb_ip_auth_node_new(&node, addr);
    if (ret < 0)
    {
        return ret;
    }
    if (list->first == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        list->last = node;
    }
    return 0;
}

int mb_ip_auth_list_add_str(mb_ip_auth_list_t *list, const char *str)
{
    mb_ip_auth_node_t *node = NULL;
    struct in_addr addr = {0};
    int ret = 0;

    ret = inet_pton(AF_INET, str, &addr);
    if (ret < 0)
    {
        return -errno;
    }
    if (ret == 0)
    {
        return -EINVAL;
    }
    ret = mb_ip_auth_node_new(&node, &addr);
    if (ret < 0)
    {
        return ret;
    }
    if (list->first == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        list->last = node;
    }
    return 0;
}

int mb_ip_auth_list_check_addr(mb_ip_auth_list_t *list, const struct in_addr *addr)
{
    mb_ip_auth_node_t *node = NULL;

    node = list->first;
    while (node != NULL)
    {
        if (memcmp(addr, &node->addr, sizeof(struct in_addr)) == 0)
        {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

int mb_ip_auth_list_check_str(mb_ip_auth_list_t *list, const char *str)
{
    mb_ip_auth_node_t *node = NULL;
    struct in_addr addr = {0};
    int ret = 0;

    ret = inet_pton(AF_INET, str, &addr);
    if (ret < 0)
    {
        return -errno;
    }
    if (ret == 0)
    {
        return -EINVAL;
    }
    node = list->first;
    while (node != NULL)
    {
        if (memcmp(&addr, &node->addr, sizeof(struct in_addr)) == 0)
        {
            return 1;
        }
        node = node->next;
    }
    return 0;
}
