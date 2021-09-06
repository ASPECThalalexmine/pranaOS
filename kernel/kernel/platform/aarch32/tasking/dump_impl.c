/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libkern/log.h>
#include <mem/vmm/vmm.h>
#include <platform/generic/system.h>
#include <platform/x86/tasking/dump_impl.h>

void dump_regs(dump_data_t* dump_data)
{
}

void dump_backtrace(dump_data_t* dump_data)
{
}

int dump_impl(dump_data_t* dump_data)
{
    char buf[64];
    trapframe_t* tf = dump_data->p->main_thread->tf;
    snprintf(buf, 64, "Dump not supported for arm target yet!\n");
    dump_data->writer(buf);
    return 0;
}

int dump_kernel_impl(dump_data_t* dump_data, const char* err_desc)
{
    return 0;
}

int dump_kernel_impl_from_tf(dump_data_t* dump_data, const char* err_desc, trapframe_t* tf)
{
    return 0;
}
