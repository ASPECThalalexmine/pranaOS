/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "../../CursorManager.h"
#include "../../Desktop/Window.h"
#include <libfoundation/Logger.h>
#include <libg/Context.h>
#include <libg/PixelBitmap.h>
#include <libg/Point.h>
#include <vector>

namespace WinServer {

class ControlBar {
public:
    inline static ControlBar& the()
    {
        extern ControlBar* s_WinServer_ControlBar_the;
        return *s_WinServer_ControlBar_the;
    }

    ControlBar();

    static constexpr size_t height() { return 20; }
    static constexpr size_t padding() { return 4; }
    static constexpr size_t menubar_content_offset() { return 20; }

    size_t width() const { return m_bounds.width(); }
    LG::Rect& bounds() { return m_bounds; }
    const LG::Rect& bounds() const { return m_bounds; }
    const LG::Rect& control_button_bounds() const { return m_button_bounds; }

    [[gnu::always_inline]] inline void draw(LG::Context& ctx)
    {
        ctx.draw({ control_button_bounds().min_x(), control_button_bounds().min_y() }, m_menu_icon);
    }

private:
    LG::Rect m_bounds;
    LG::Rect m_button_bounds;
    LG::PixelBitmap m_menu_icon;
};

} // namespace WinServer