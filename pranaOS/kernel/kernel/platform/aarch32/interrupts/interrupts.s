/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

.extern _start
.extern undefined_handler
.extern svc_handler
.extern prefetch_abort_handler
.extern data_abort_handler
.extern irq_handler
.extern fast_irq_handler

.section ".interrupt_vector_table"
.global trap_return
.global vector_table
vector_table:
    b       reset_handler
    b       undefined_handler_isp
    b       svc_isp
    b       prefetch_abort_isp
    b       data_abort_isp
    nop
    b       irq_isp
vector_FIQ:
    b       vector_FIQ
    
prefetch_abort_isp:
    ldr     r1, =.prefetch_abort_handler_addr
    ldr     r0, [r1]
    blx     r0

irq_isp:
    subs    lr, lr, #4

    stmfd   sp!, {r1-r5}
    mrs     r1, spsr
    mrs     r2, sp_usr
    mrs     r3, lr_usr
    mov     r4, sp
    add     sp, sp, #20
    mov     r5, lr
    cps	    #0x13
   
    mov     lr, r5
    stmfd   sp!, {r6-r12,lr}

    mov     r6, r0
    ldr     r7, [r4]
    add     r4, r4, #4
    ldr     r8, [r4]
    add     r4, r4, #4
    ldr     r9, [r4]
    add     r4, r4, #4
    ldr     r10, [r4]
    add     r4, r4, #4
    ldr     r11, [r4]

    stmfd   sp!, {r6-r11}
    stmfd   sp!, {r1-r3}

    ldr     r0, =.irq_handler_addr
    ldr     r1, [r0]
    mov     r0, sp
    blx     r1

    ldmfd   sp!, {r0-r2}
    msr     spsr, r0
    msr     sp_usr, r1
    msr     lr_usr, r2
    
    ldmfd   sp!, {r0-r12,lr}
    subs    pc, lr, #0
    nop

svc_isp:
    stmfd   sp!, {r0-r12,lr}
    mrs     r0, spsr
    mrs     r1, sp_usr
    mrs     r2, lr_usr
    stmfd   sp!, {r0-r2}

    ldr     r0, =.svc_handler_addr
    ldr     r1, [r0]
    mov     r0, sp
    blx     r1

trap_return:
    ldmfd   sp!, {r0-r2}
    msr     spsr, r0
    msr     sp_usr, r1
    msr     lr_usr, r2
    
    ldmfd   sp!, {r0-r12,lr}
    subs    pc, lr, #0
    nop

undefined_handler_isp:
    subs    lr, lr, #4
    stmfd   sp!, {r0-r12,lr}
    mrs     r0, spsr
    mrs     r1, sp_usr
    mrs     r2, lr_usr
    stmfd   sp!, {r0-r2}

    ldr     r0, =.undefined_handler_addr
    ldr     r1, [r0]
    mov     r0, sp
    blx     r1

    ldmfd   sp!, {r0-r2}
    msr     spsr, r0
    msr     sp_usr, r1
    msr     lr_usr, r2

    ldmfd   sp!, {r0-r12,lr}
    subs    pc, lr, #0
    nop

data_abort_isp:
    subs    lr, lr, #8
    stmfd   sp!, {r0-r12,lr}
    mrs     r0, spsr
    mrs     r1, sp_usr
    mrs     r2, lr_usr
    stmfd   sp!, {r0-r2}

    ldr     r0, =.data_abort_handler_addr
    ldr     r1, [r0]
    mov     r0, sp
    blx     r1

    ldmfd   sp!, {r0-r2}
    msr     spsr, r0
    msr     sp_usr, r1
    msr     lr_usr, r2
    
    ldmfd   sp!, {r0-r12,lr}
    subs    pc, lr, #0
    nop

halt_cpu:
    wfi 
    b       halt_cpu

reset_handler:
    mrc     p15, #0, r0, c0, c0, #5 
    and     r0, r0, #3 
    cmp     r0, #0 
    beq     _reset_cpu0 

    cmp     r0, #1 
    beq     _reset_cpu1 

    cmp     r0, #2 
    beq     _reset_cpu2 

    cmp     r0, #3 
    beq     _reset_cpu3 

    ldr     pc,=halt_cpu

_reset_cpu0:
    push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =vector_table

    mcr     P15, 0, r0, c12, c0, 0

    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}
    ldr     pc, =_start

_reset_cpu1:
    push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =vector_table

    mcr     P15, 0, r0, c12, c0, 0

    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}
    ldr     pc, =_start

_reset_cpu2:
    push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =vector_table

    mcr     P15, 0, r0, c12, c0, 0

    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}
    ldr     pc, =_start

_reset_cpu3:
    push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =vector_table

    mcr     P15, 0, r0, c12, c0, 0

    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}
    ldr     pc, =_start

.svc_handler_addr:
    .word svc_handler

.irq_handler_addr:
    .word irq_handler

.undefined_handler_addr:
    .word undefined_handler

.data_abort_handler_addr:
    .word data_abort_handler

.fast_irq_handler_addr:
    .word fast_irq_handler

.prefetch_abort_handler_addr:
    .word prefetch_abort_handler

.section ".text"
.global set_svc_stack
set_svc_stack:
    mov     r1, sp
    mov     r2, lr
    cps	    #0x13       
    mov	    sp, r0
    cps	    #0x1F		
    mov     sp, r1
    bx      r2

.global set_irq_stack
set_irq_stack:
    mov     r1, sp
    mov     r2, lr
    cps	    #0x12       
    mov	    sp, r0
    cps	    #0x1F		
    mov     sp, r1
    bx      r2

.global set_abort_stack
set_abort_stack:
    mov     r1, sp
    mov     r2, lr
    cps	    #0x17       
    mov	    sp, r0
    cps	    #0x1F		
    mov     sp, r1
    bx      r2

.global set_undefined_stack
set_undefined_stack:
    mov     r1, sp
    mov     r2, lr
    cps	    #0x1B       
    mov	    sp, r0
    cps	    #0x1F		
    mov     sp, r1
    bx      r2