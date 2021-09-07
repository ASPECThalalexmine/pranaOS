/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "../Components/Base/BaseWindow.h"
#include "../Components/MenuBar/MenuItem.h"
#include "../Connection.h"
#include "WindowFrame.h"
#include <libfoundation/SharedBuffer.h>
#include <libg/PixelBitmap.h>
#include <libg/Rect.h>
#include <sys/types.h>
#include <utility>

namespace WinServer::Desktop {

class Window : public BaseWindow {
public:
    Window(int connection_id, int id, const CreateWindowMessage& msg);
    Window(Window&& win);

    inline WindowFrame& frame() { return m_frame; }
    inline const WindowFrame& frame() const { return m_frame; }

    inline void set_name(const std::string& name)
    {
        frame().set_app_name(name);
        menubar_content()[0].set_title(name);
    }

    void make_frame();
    void make_frameless();

    inline void set_icon(LG::string&& name)
    {
        m_icon_path = std::move(name);
        m_frame.reload_icon();
    }

    inline void set_icon(const LG::string& name)
    {
        m_icon_path = name;
        m_frame.reload_icon();
    }

    inline const LG::CornerMask& corner_mask() const { return m_corner_mask; }

    inline const LG::string& icon_path() const { return m_icon_path; }

    inline std::vector<MenuDir>& menubar_content() { return m_menubar_content; }
    inline const std::vector<MenuDir>& menubar_content() const { return m_menubar_content; }

    virtual void did_size_change(const LG::Size& size) override;

    inline void set_color(const LG::Color& clr) { m_frame.set_color(clr); }
    inline LG::Color& color() { return m_frame.color(); }
    inline const LG::Color& color() const { return m_frame.color(); }

private:
    void recalc_bounds(const LG::Size& size);

    WindowFrame m_frame;
    LG::CornerMask m_corner_mask { LG::CornerMask::SystemRadius, LG::CornerMask::NonMasked, LG::CornerMask::Masked };
    LG::string m_icon_path {};
    std::vector<MenuDir> m_menubar_content;
};

} // namespace WinServer