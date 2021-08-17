/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_PLATFORM_X86_TASKING_SWITCHVM_H
#define _KERNEL_PLATFORM_X86_TASKING_SWITCHVM_H

#include <libkern/types.h>
#include <tasking/tasking.h>

void switchuvm(thread_t* thread);

#endif