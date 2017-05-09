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
#include <stdarg.h>
#include "mb_log.h"

static mb_log_level_t mb_log_level = MB_LOG_DEF_LEVEL;

void mb_log_set_level(mb_log_level_t level)
{
    switch (level) {
    case MB_LOG_WARN:
    case MB_LOG_NOTICE:
    case MB_LOG_INFO:
    case MB_LOG_DEBUG:
        mb_log_level = level;
        break;
    default:
        mb_log_level = MB_LOG_DEF_LEVEL;
    }
}

mb_log_level_t mb_log_get_level(void)
{
    return mb_log_level;
}

void mb_log_error(const char *msg, ...)
{
    va_list arg_list;

    va_start(arg_list, msg);
    if (MB_LOG_ERROR <= mb_log_level)
    {
        printf("Error  : ");
        vprintf(msg, arg_list);
        printf("\n");
    }
    va_end(arg_list);
}

void mb_log_warn(const char *msg, ...)
{
    va_list arg_list;

    va_start(arg_list, msg);
    if (MB_LOG_WARN <= mb_log_level)
    {
        printf("Warning: ");
        vprintf(msg, arg_list);
        printf("\n");
    }
    va_end(arg_list);
}

void mb_log_notice(const char *msg, ...)
{
    va_list arg_list;

    va_start(arg_list, msg);
    if (MB_LOG_NOTICE <= mb_log_level)
    {
        printf("Notice : ");
        vprintf(msg, arg_list);
        printf("\n");
    }
    va_end(arg_list);
}

void mb_log_info(const char *msg, ...)
{
    va_list arg_list;

    va_start(arg_list, msg);
    if (MB_LOG_INFO <= mb_log_level)
    {
        printf("Info   : ");
        vprintf(msg, arg_list);
        printf("\n");
    }
    va_end(arg_list);
}

void mb_log_debug(const char *msg, ...)
{
    va_list arg_list;

    va_start(arg_list, msg);
    if (MB_LOG_DEBUG <= mb_log_level)
    {
        printf("Debug  : ");
        vprintf(msg, arg_list);
        printf("\n");
    }
    va_end(arg_list);
}
