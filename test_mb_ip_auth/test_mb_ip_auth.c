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

#include <stddef.h>
#include <stdio.h>
#include "mb_ip_auth.h"
#include "mb_test.h"

int print_cols = 93;

mb_test_result_t test_mb_ip_auth_addr(void)
{
    mb_ip_auth_list_t auth = {0};
    struct in_addr addr1 = {0};
    struct in_addr addr2 = {0};
    const char *str1 = "127.0.0.1";
    const char *str2 = "10.10.10.10";
    int ret = 0;

    printf("%-*s", print_cols, "test 1: add addresses and authorise addresses");

    ret = inet_pton(AF_INET, str1, &addr1);
    if (ret <= 0)
    {
        return FAIL;
    }
    ret = inet_pton(AF_INET, str2, &addr2);
    if (ret <= 0)
    {
        return FAIL;
    }

    mb_ip_auth_list_create(&auth);

    /* check for first address */
    ret = mb_ip_auth_list_check_addr(&auth, &addr1);
    if (ret != 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* add first address */
    ret = mb_ip_auth_list_add_addr(&auth, &addr1);
    if (ret < 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for first address */
    ret = mb_ip_auth_list_check_addr(&auth, &addr1);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for second address */
    ret = mb_ip_auth_list_check_addr(&auth, &addr2);
    if (ret != 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* add second address */
    ret = mb_ip_auth_list_add_addr(&auth, &addr2);
    if (ret < 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for first address */
    ret = mb_ip_auth_list_check_addr(&auth, &addr1);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for second address */
    ret = mb_ip_auth_list_check_addr(&auth, &addr2);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    mb_ip_auth_list_destroy(&auth);

    return PASS;
}

mb_test_result_t test_mb_ip_auth_str(void)
{
    mb_ip_auth_list_t auth = {0};
    const char *str1 = "127.0.0.1";
    const char *str2 = "10.10.10.10";
    int ret = 0;

    printf("%-*s", print_cols, "test 2: add address strings and authorise addresses");

    mb_ip_auth_list_create(&auth);

    /* check for first address */
    ret = mb_ip_auth_list_check_str(&auth, str1);
    if (ret != 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* add first address */
    ret = mb_ip_auth_list_add_str(&auth, str1);
    if (ret < 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for first address */
    ret = mb_ip_auth_list_check_str(&auth, str1);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for second address */
    ret = mb_ip_auth_list_check_str(&auth, str2);
    if (ret != 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* add second address */
    ret = mb_ip_auth_list_add_str(&auth, str2);
    if (ret < 0)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for first address */
    ret = mb_ip_auth_list_check_str(&auth, str1);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    /* check for second address */
    ret = mb_ip_auth_list_check_str(&auth, str2);
    if (ret != 1)
    {
        mb_ip_auth_list_destroy(&auth);
        return FAIL;
    }

    mb_ip_auth_list_destroy(&auth);

    return PASS;
}

int main(void)
{
    mb_test_func_t func[] = {test_mb_ip_auth_addr,
                             test_mb_ip_auth_str};

    return mb_test_run(func, sizeof(func) / sizeof(func[0]));
}
