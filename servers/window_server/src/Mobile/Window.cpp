/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Window.h"
#include "../WindowManager.h"
#include <utility>

namespace WinServer::Mobile {

Window::Window(int connection_id, int id, const CreateWindowMessage& msg)
    : BaseWindow(connection_id, id, msg)
{
    m_bounds = LG::Rect(0, 0, msg.width(), msg.height());
    m_content_bounds = LG::Rect(0, 0, msg.width(), msg.height());
    m_content_bitmap = LG::PixelBitmap(m_buffer.data(), content_bounds().width(), content_bounds().height());
}

Window::Window(Window&& win)
    : BaseWindow(std::move(win))
{
}

} // namespace WinServer