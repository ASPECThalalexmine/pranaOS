/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libkernel/bits/errno.h>
#include <libkernel/libkern.h>
#include <libkernel/log.h>
#include <platform/generic/syscalls/params.h>
#include <platform/generic/tasking/trapframe.h>
#include <syscalls/handlers.h>
#include <time/time_manager.h>

void sys_clock_gettime(trapframe_t* tf)
{
    clockid_t clk_id = param1;
    timespec_t* u_ts = (timespec_t*)param2;

    switch (clk_id) {
    case CLOCK_MONOTONIC:
        u_ts->tv_sec = timeman_seconds_since_boot();
        u_ts->tv_nsec = timeman_get_ticks_from_last_second() * (1000000000 / timeman_ticks_per_second());
        break;
    case CLOCK_REALTIME:
        u_ts->tv_sec = timeman_now();
        u_ts->tv_nsec = timeman_get_ticks_from_last_second() * (1000000000 / timeman_ticks_per_second());
        break;
    default:
        return_with_val(-EINVAL);
    }
    return_with_val(0);
}

void sys_gettimeofday(trapframe_t* tf)
{
    timeval_t* tv = (timeval_t*)param1;
    timezone_t* tz = (timezone_t*)param2;

    if (!tv || !tz) {
        return_with_val(-EINVAL);
    }

    tv->tv_sec = timeman_now();
    tv->tv_usec = timeman_get_ticks_from_last_second() * (1000000 / timeman_ticks_per_second());

    tz->tz_dsttime = DST_NONE;
    tz->tz_minuteswest = 0;

    return_with_val(0);
}