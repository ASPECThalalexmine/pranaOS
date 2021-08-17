/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platform/generic/system.h>
#include <platform/x86/irq_handler.h>
#include <tasking/cpu.h>
#include <tasking/tasking.h>

static inline void irq_redirect(uint8_t int_no)
{
    void (*func)() = (void*)handlers[int_no];
    func();
}

void irq_handler(trapframe_t* tf)
{
    system_disable_interrupts();
    cpu_enter_kernel_space();

    if (tf->int_no >= IRQ_SLAVE_OFFSET) {
        port_byte_out(0xA0, 0x20);
    }
    port_byte_out(0x20, 0x20);

    if (likely(RUNNING_THREAD)) {
        if (RUNNING_THREAD->process->is_kthread) {
            RUNNING_THREAD->tf = tf;
        }
    }

    irq_redirect(tf->int_no);
    /* We are leaving interrupt, and later interrupts will be on,
       when flags are restored */
    cpu_leave_kernel_space();
    system_enable_interrupts_only_counter();
}

void irq_empty_handler()
{
    return;
}