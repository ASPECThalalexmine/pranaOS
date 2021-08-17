/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_PLATFORM_X86_IRQ_HANDLER_H
#define _KERNEL_PLATFORM_X86_IRQ_HANDLER_H

#include <drivers/x86/display.h>
#include <platform/x86/idt.h>

void irq_handler(trapframe_t *tf);
void irq_empty_handler();

#endif