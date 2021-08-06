/*
 * Copyright (c) 2021, OliviaCE
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_X86_KEYBOARD_H
#define _KERNEL_DRIVERS_X86_KEYBOARD_H

// includes
#include <drivers/driver_manager.h>
#include <drivers/generic/keyboard.h>

void keyboard_handler();
bool kbdriver_install();
void kbdriver_run();

uint32_t kbdriver_get_last_key();
void kbdriver_discard_last_key();

#endif
