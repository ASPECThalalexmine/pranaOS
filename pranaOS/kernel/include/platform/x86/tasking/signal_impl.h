/*
 * Copyright (c) 2021, NukeWilliams
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_X86_TASKING_SIGNAL_IMPL_H
#define _KERNEL_PLATFORM_X86_TASKING_SIGNAL_IMPL_H

// includes
#include <libkernel/types.h>
struct thread;

int signal_impl_prepare_stack(struct thread* thread, int signo, uint32_t old_sp, uint32_t magic);
int signal_impl_restore_stack(struct thread* thread, uint32_t* old_sp, uint32_t* magic);

#endif 