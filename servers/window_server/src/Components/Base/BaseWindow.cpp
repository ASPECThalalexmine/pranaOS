/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "BaseWindow.h"
#include "../../WindowManager.h"
#include <utility>

namespace WinServer {

BaseWindow::BaseWindow(int connection_id, int id, const CreateWindowMessage& msg)
    : m_id(id)
    , m_connection_id(connection_id)
    , m_type((WindowType)msg.type())
    , m_buffer(msg.buffer_id())
    , m_content_bitmap()
    , m_bounds(0, 0, 0, 0)
    , m_content_bounds(0, 0, 0, 0)
{
}

BaseWindow::BaseWindow(BaseWindow&& win)
    : m_id(win.m_id)
    , m_connection_id(win.m_connection_id)
    , m_buffer(win.m_buffer)
    , m_content_bitmap(std::move(win.m_content_bitmap))
    , m_bounds(win.m_bounds)
    , m_content_bounds(win.m_content_bounds)
{
}

} // namespace WinServer