/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "config.h"
#include "drivers/ata.h"
#include "drivers/display.h"
#include "drivers/drive_desc.h"
#include "drivers/elf_lite.h"
#include "drivers/ext2_lite.h"
#include "drivers/fs_desc.h"
#include "mem/mem_map.h"
#include "mem/vm.h"
#include "types.h"

int get_load_disk(drive_desc_t* drive_desc)
{
    init_ata(0x1F0, 1);
    if (indentify_ata_device(drive_desc) == 0) {
        printh((uint32_t)drive_desc->read);
        return 0;
    }
    return -1;
}

int get_fs_of_disk(drive_desc_t* drive_desc, fs_desc_t* fs_desc)
{
    if (ext2_lite_init(drive_desc, fs_desc) == 0) {
        return 0;
    }
    return -1;
}

void stage2(mem_desc_t* mem_desc)
{
    clean_screen();
    printf("STAGE2\n");
    drive_desc_t drive_desc;
    fs_desc_t fs_desc;

    if (get_load_disk(&drive_desc) != 0) {
        printf("E1");
        while (1) { }
    }

    if (get_fs_of_disk(&drive_desc, &fs_desc) != 0) {
        printf("E2");
        while (1) { }
    }

    printf("P3\n");

    uint32_t kernel_size;
    printd(elf_load_kernel(&drive_desc, &fs_desc, KERNEL_PATH, &kernel_size));

    mem_desc->kernel_size = kernel_size / 1024 + 1;

    vm_setup();

    asm volatile("mov %cr0, %eax");
    asm volatile("or $0x80000000, %eax");
    asm volatile("mov %eax, %cr0");

    asm volatile("add $0xffc00000, %ebp");
    asm volatile("add $0xffc00000, %esp");

    asm volatile("push %0"
                 :
                 : "r"(mem_desc));
    asm volatile("mov $0xc0000000, %eax");
    asm volatile("call *%eax");
    while (1) { }
}