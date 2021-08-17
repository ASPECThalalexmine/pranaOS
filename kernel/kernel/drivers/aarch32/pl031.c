/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <drivers/aarch32/pl031.h>
#include <libkernel/bits/errno.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <libkernel/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>
#include <tasking/tasking.h>
#include <libkernel/libkern.h>

// #define DEBUG_PL031

static zone_t mapped_zone;
static volatile pl031_registers_t* registers = (pl031_registers_t*)PL031_BASE;

static inline int _pl031_map_itself()
{
    mapped_zone = zoner_new_zone(sizeof(pl031_registers_t));
    vmm_map_page(mapped_zone.start, PL031_BASE, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    registers = (pl031_registers_t*)mapped_zone.ptr;
    return 0;
}

void pl031_install()
{
    if (_pl031_map_itself()) {
#ifdef DEBUG_PL031
        log_error("PL031: Can't map itself!");
#endif
        return;
    }
}

uint32_t pl031_read_rtc()
{
    return registers->data;
}
