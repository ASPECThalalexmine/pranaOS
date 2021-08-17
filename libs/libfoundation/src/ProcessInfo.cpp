/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cstring>
#include <iostream>
#include <libfoundation/Logger.h>
#include <libfoundation/ProcessInfo.h>
#include <memory>
#include <unistd.h>

namespace LFoundation {

ProcessInfo* s_LFoundation_ProcessInfo_the = nullptr;

ProcessInfo::ProcessInfo(int argc, char** argv)
{
    s_LFoundation_ProcessInfo_the = this;

    // Parse argv[0] to get a process name.
    int process_name_start = 0;
    for (int i = 0; i < strlen(argv[0]) - 1; i++) {
        if (argv[0][i] == '/') {
            process_name_start = i + 1;
        }
    }
    m_process_name = std::string(&argv[0][process_name_start]);

    for (int i = 1; i < argc; i++) {
        m_args.push_back(std::string(argv[i]));
    }
}

} // namespace LFoundation