/*
 * Copyright (c) 2021, OliviaCE
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_TARGET_CORTEX_A15_MEMMAP_H
#define _KERNEL_PLATFORM_AARCH32_TARGET_CORTEX_A15_MEMMAP_H

#define KERNEL_PM_BASE 0x80100000
#define KERNEL_BASE 0xc0000000
#define KMALLOC_BASE (KERNEL_BASE + 0x400000)

extern struct memory_map* arm_memmap;

#endif
