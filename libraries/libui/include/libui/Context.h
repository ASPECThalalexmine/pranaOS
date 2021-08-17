/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include <libg/Context.h>
#include <libui/ContextManager.h>
#include <libui/View.h>
#include <libui/Window.h>

namespace UI {
class Context : public LG::Context {
public:
    enum RelativeToCurrentContext {
        Yes
    };

    explicit Context(LG::PixelBitmap& bitmap)
        : LG::Context(bitmap)
    {
    }

    explicit Context(View& view)
        : Context(view.window()->bitmap())
    {
        auto frame = view.frame_in_window();
        add_clip(frame);
        set_draw_offset(frame.origin());
    }

    Context(View& view, const LG::Rect& frame)
        : Context(view.window()->bitmap())
    {
        add_clip(frame);
        set_draw_offset(frame.origin());
    }

    Context(View& view, RelativeToCurrentContext)
        : Context(view.window()->bitmap())
    {
        auto context_frame = view.frame();
        context_frame.offset_by(graphics_current_context().draw_offset());
        add_clip(context_frame);
        set_draw_offset(context_frame.origin());
    }

    Context(View& view, const LG::Rect& frame, RelativeToCurrentContext)
        : Context(view.window()->bitmap())
    {
        auto context_frame = frame;
        context_frame.offset_by(graphics_current_context().draw_offset());
        add_clip(context_frame);
        set_draw_offset(context_frame.origin());
    }
};
} // namespace UI