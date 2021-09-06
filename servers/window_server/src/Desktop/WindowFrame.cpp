/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowFrame.h"
#include "../Colors.h"
#include "../Components/Elements/Button.h"
#include "../WindowManager.h"
#include "Window.h"
#include <libg/Font.h>
#include <libg/ImageLoaders/PNGLoader.h>
#include <libg/Rect.h>
#include <utility>

#define CONTROL_PANEL_CLOSE 0x0
#define CONTROL_PANEL_MAXIMIZE 0x1
#define CONTROL_PANEL_MINIMIZE 0x2

namespace WinServer::Desktop {

static const uint32_t s_close_button_glyph_data[10] = {
    0b1100000011,
    0b1110000111,
    0b0111001110,
    0b0011111100,
    0b0001111000,
    0b0001111000,
    0b0011111100,
    0b0111001110,
    0b1110000111,
    0b1100000011,
};

static const uint32_t s_maximise_button_glyph_data[10] = {
    0b1111111111,
    0b1111111111,
    0b1100000011,
    0b1100000011,
    0b1100000011,
    0b1100000011,
    0b1100000011,
    0b1100000011,
    0b1111111111,
    0b1111111111
};

static const uint32_t s_minimise_button_glyph_data[10] = {
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b0000000000,
    0b1111111111,
    0b1111111111
};

WindowFrame::WindowFrame(Window& window)
    : m_window(window)
    , m_window_control_buttons()
    , m_control_panel_buttons()
{
    auto* close = new Button();
    close->set_icon(LG::GlyphBitmap(s_close_button_glyph_data, 10, 10));
    auto* maximize = new Button();
    maximize->set_icon(LG::GlyphBitmap(s_maximise_button_glyph_data, 10, 10));
    auto* minimize = new Button();
    minimize->set_icon(LG::GlyphBitmap(s_minimise_button_glyph_data, 10, 10));

    m_window_control_buttons.push_back(close);
    m_window_control_buttons.push_back(maximize);
    m_window_control_buttons.push_back(minimize);
}

WindowFrame::WindowFrame(Window& window, std::vector<Button*>&& control_panel_buttons, std::vector<Button*>&& window_control_buttons)
    : m_window(window)
    , m_window_control_buttons(std::move(window_control_buttons))
    , m_control_panel_buttons(std::move(control_panel_buttons))
{
}

void WindowFrame::set_app_name(const std::string& title)
{
    if (m_control_panel_buttons.size() > 0) {
        m_control_panel_buttons[0]->set_title(title);
    } else {
        auto* new_control = new Button();
        new_control->set_title(title);
        m_control_panel_buttons.push_back(new_control);
    }
    WinServer::Compositor::the().invalidate(bounds());
}

void WindowFrame::set_app_name(std::string&& title)
{
    if (m_control_panel_buttons.size() > 0) {
        m_control_panel_buttons[0]->set_title(title);
    } else {
        auto* new_control = new Button();
        new_control->set_title(title);
        new_control->set_font(LG::Font::system_bold_font());
        m_control_panel_buttons.push_back(new_control);
    }
    WinServer::Compositor::the().invalidate(bounds());
}

void WindowFrame::add_control(const std::string& title)
{
    auto* new_control = new Button();
    new_control->set_title(title);
    m_control_panel_buttons.push_back(new_control);
    WinServer::Compositor::the().invalidate(bounds());
}

void WindowFrame::draw(LG::Context& ctx)
{
    if (!visible()) {
        return;
    }
    int x = m_window.bounds().min_x();
    int y = m_window.bounds().min_y();
    size_t width = m_window.bounds().width();
    size_t height = m_window.bounds().height();

    int right_x = x + width - right_border_size();
    int bottom_y = y + height - bottom_border_size();

    // Drawing frame and shadings
    ctx.set_fill_color(color());
    ctx.fill_rounded(LG::Rect(x + left_border_size(), y + std_top_border_frame_size(), width - 2 * left_border_size(), top_border_size() - std_top_border_frame_size()), LG::CornerMask(4, true, false));
    if (active()) {
        ctx.set_fill_color(Color::Shadow);

        auto shading_rect = LG::Rect(x + left_border_size(), y + std_top_border_frame_size(), width - 2 * left_border_size(), height - std_top_border_frame_size() - std_bottom_border_size());
        ctx.draw_box_shading(shading_rect, LG::Shading(LG::Shading::Type::Box, 0, LG::Shading::SystemSpread), LG::CornerMask(LG::CornerMask::SystemRadius));
    }

    // Drawing labels, icons.
    // Drawing positions are calculated using a start of the frame.
    if (active()) {
        ctx.set_fill_color(LG::Color::LightSystemText);
    } else {
        ctx.set_fill_color(Color::InactiveText);
    }
    ctx.draw({ x + spacing(), y + icon_y_offset() }, icon());

    constexpr int start_controls_offset = icon_width() + 2 * spacing();
    int start_controls = x + start_controls_offset;
    for (int i = 0; i < m_control_panel_buttons.size(); i++) {
        m_control_panel_buttons[i]->display(ctx, { start_controls, y + text_y_offset() });
        start_controls += spacing() + m_control_panel_buttons[i]->bounds().width();
    }

    int start_buttons = right_x - spacing() - m_window_control_buttons[0]->bounds().width();
    for (int i = 0; i < m_window_control_buttons.size(); i++) {
        m_window_control_buttons[i]->display(ctx, { start_buttons, y + button_y_offset() });
        start_buttons += -spacing() - m_window_control_buttons[i]->bounds().width();
    }
}

void WindowFrame::invalidate(WinServer::Compositor& compositor) const
{
    if (!visible()) {
        return;
    }
    int x = m_window.bounds().min_x();
    int y = m_window.bounds().min_y();
    size_t width = m_window.bounds().width();
    size_t height = m_window.bounds().height();
    int right_x = x + width - right_border_size();
    int bottom_y = y + height - bottom_border_size();
    compositor.invalidate(LG::Rect(x, y, width, top_border_size()));
    compositor.invalidate(LG::Rect(x, y, left_border_size(), height));
    compositor.invalidate(LG::Rect(right_x, y, right_border_size(), height));
    compositor.invalidate(LG::Rect(x, bottom_y, width, bottom_border_size()));
}

void WindowFrame::receive_tap_event(const LG::Point<int>& tap)
{
    // Calculating buttons' positions
    size_t width = m_window.bounds().width();
    int right_x = width - right_border_size();
    int start_buttons = right_x - spacing() - m_window_control_buttons[0]->bounds().width();
    for (int button_id = 0; button_id < m_window_control_buttons.size(); button_id++) {
        auto button_frame = m_window_control_buttons[button_id]->bounds();
        button_frame.offset_by(start_buttons, button_y_offset());
        if (button_frame.contains(tap)) {
            handle_control_panel_tap(button_id);
            return;
        }
        start_buttons += -spacing() - button_frame.width();
    }
}

const LG::Rect WindowFrame::bounds() const
{
    const auto& bounds = m_window.bounds();
    return LG::Rect(bounds.min_x(), bounds.min_y(), bounds.width(), top_border_size());
}

void WindowFrame::handle_control_panel_tap(int button_id)
{
    auto& wm = WindowManager::the();
    switch (button_id) {
    case CONTROL_PANEL_CLOSE:
        wm.close_window(m_window);
        break;
    case CONTROL_PANEL_MINIMIZE:
        wm.minimize_window(m_window);
        break;
    default:
        break;
    }
}

void WindowFrame::reload_icon()
{
    LG::PNG::PNGLoader loader;
    m_icon = loader.load_from_file(m_window.icon_path() + "/12x12.png");
}

} // namespace WinServer