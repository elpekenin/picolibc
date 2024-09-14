/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2024, Synopsys Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "string_private.h"

/* C11 version; required by LLVM's C++11 library */
__errno_t
strerror_s(char *buf, rsize_t buflen, __errno_t errnum)
{
    int32_t result = 0;
    const char *msg = "";

    if (buf == NULL) {
        msg = "strerror_s: dest is NULL";
        goto handle_error;
    }

    if ((buflen == 0u) || (CHECK_RSIZE(buflen))) {
        msg = "strerror_s: dest buffer size is 0 or exceeds RSIZE_MAX";
        goto handle_error;
    }

    const char *cp = _strerror_r(errnum, 0, NULL);
    uint32_t len = strnlen_s(cp, MAX_ERROR_MSG);

    if (len < buflen) {
        (void)strncpy(buf, cp, MAX_ERROR_MSG);
    } else {
        /* Standard allows truncation of error message with '...' to
            indicate truncation. */
        (void)memcpy(buf, cp, (buflen - 1u));
        buf[(buflen - 1u)] = '\0';

        if (buflen > 3u) {
            (void)strncpy(&buf[(buflen - 4u)], "...", 4u);
        }

        result = ERANGE;
    }

    // Normal return path
    return result;

handle_error:
    if (__cur_handler != NULL) {
        __cur_handler(msg, NULL, -1);
    }

    return -1;
}
