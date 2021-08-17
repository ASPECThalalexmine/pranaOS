/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <drivers/x86/fpu.h>
#include <libkernel/kassert.h>
#include <libkernel/log.h>
#include <mem/vmm/vmm.h>
#include <platform/generic/registers.h>
#include <platform/generic/system.h>
#include <platform/x86/isr_handler.h>
#include <tasking/cpu.h>
#include <tasking/dump.h>
#include <tasking/sched.h>
#include <tasking/tasking.h>
#include <tasking/thread.h>

#define ERR_BUF_SIZE 64
static char err_buf[ERR_BUF_SIZE];

static const char* exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(trapframe_t* tf)
{
    system_disable_interrupts();
    cpu_enter_kernel_space();

    proc_t* p = NULL;
    if (likely(RUNNING_THREAD)) {
        p = RUNNING_THREAD->process;
        if (RUNNING_THREAD->process->is_kthread) {
            RUNNING_THREAD->tf = tf;
        }
    }

    if (tf->int_no == 0) {
        if (!p) {
            log_warn("Crash: division by zero in %d tid\n", RUNNING_THREAD->tid);
            dump_and_kill(p);
        } else {
            snprintf(err_buf, ERR_BUF_SIZE, "Kernel trap at %x, type %d=%s", tf->eip, tf->int_no, &exception_messages[tf->int_no]);
            kpanic_tf(err_buf, tf);
        }
    } else if (tf->int_no == 1) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 2) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 3) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 4) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 5) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 6) {
        if (!p) {
            snprintf(err_buf, ERR_BUF_SIZE, "Kernel trap at %x, type %d=%s", tf->eip, tf->int_no, &exception_messages[tf->int_no]);
            kpanic_tf(err_buf, tf);
        } else {
            log_warn("Crash: invalid opcode in %d tid\n", RUNNING_THREAD->tid);
            dump_and_kill(p);
        }
    } else if (tf->int_no == 7) {
        fpu_handler();
    } else if (tf->int_no == 8) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 9) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 10) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 11) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 12) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 13) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else if (tf->int_no == 14) {
        int res = vmm_page_fault_handler(tf->err, read_cr2());
        if (res == SHOULD_CRASH) {
            if (!p) {
                snprintf(err_buf, ERR_BUF_SIZE, "Kernel trap at %x, type %d=%s", tf->eip, tf->int_no, &exception_messages[tf->int_no]);
                kpanic_tf(err_buf, tf);
            } else {
                log_warn("Crash: pf err %d at %x: %d pid, %x eip\n", tf->err, read_cr2(), p->pid, tf->eip);
                dump_and_kill(p);
            }
        }
    } else if (tf->int_no == 15) {
        log_error("Int w/o handler: %d: %s: %d", tf->int_no, exception_messages[tf->int_no], tf->err);
        system_stop();
    } else {
        log_error("Int w/o handler: %d: %d", tf->int_no, tf->err);
        system_stop();
    }

    /* We are leaving interrupt, and later interrupts will be on,
       when flags are restored */
    cpu_leave_kernel_space();
    system_enable_interrupts_only_counter();
}
