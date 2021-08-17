/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libg/Color.h>
#include <libui/Context.h>
#include <libui/ScrollView.h>
#include <utility>

namespace UI {

ScrollView::ScrollView(View* superview, const LG::Rect& frame)
    : View(superview, frame)
{
}

ScrollView::ScrollView(View* superview, Window* window, const LG::Rect& frame)
    : View(superview, window, frame)
{
}

// void ScrollView::display(const LG::Rect& rect)
// {
//     Context ctx(*this);
//     ctx.add_clip(rect);

// }

void ScrollView::mouse_wheel_event(int wheel_data)
{
    m_content_offset.offset_by(0, wheel_data * 10);
    set_needs_display();
}

std::optional<LG::Point<int>> ScrollView::subview_location(const View& subview) const
{
    auto frame_origin = subview.frame().origin();
    frame_origin.offset_by(m_content_offset);
    return frame_origin;
}

std::optional<View*> ScrollView::subview_at(const LG::Point<int>& point) const
{
    for (int i = subviews().size() - 1; i >= 0; --i) {
        auto frame = subviews()[i]->frame();
        frame.offset_by(m_content_offset);
        if (frame.contains(point)) {
            return subviews()[i];
        }
    }
    return {};
}

void ScrollView::receive_mouse_move_event(MouseEvent& event)
{
    auto location = LG::Point<int>(event.x(), event.y());
    if (!is_hovered()) {
        hover_begin(location);
    }

    foreach_subview([&](View& subview) -> bool {
        auto frame = subview.frame();
        frame.offset_by(m_content_offset);
        bool event_hits_subview = frame.contains(event.x(), event.y());
        if (subview.is_hovered() && !event_hits_subview) {
            LG::Point<int> point(event.x(), event.y());
            point.offset_by(-frame.origin());
            MouseLeaveEvent mle(point.x(), point.y());
            subview.receive_mouse_leave_event(mle);
        } else if (event_hits_subview) {
            LG::Point<int> point(event.x(), event.y());
            point.offset_by(-frame.origin());
            MouseEvent me(point.x(), point.y());
            subview.receive_mouse_move_event(me);
        }
        return true;
    });

    mouse_moved(location);
    Responder::receive_mouse_move_event(event);
}

void ScrollView::receive_display_event(DisplayEvent& event)
{
    event.bounds().intersect(bounds());
    display(event.bounds());
    foreach_subview([&](View& subview) -> bool {
        auto bounds = event.bounds();
        auto frame = subview.frame();
        frame.offset_by(m_content_offset);
        bounds.intersect(frame);
        if (!bounds.empty()) {
            graphics_push_context(Context(subview, frame, Context::RelativeToCurrentContext::Yes));
            bounds.origin().offset_by(-frame.origin());
            DisplayEvent own_event(bounds);
            subview.receive_display_event(own_event);
            graphics_pop_context();
        }
        return true;
    });
    did_display(event.bounds());

    if (!has_superview()) {
        // Only superview sends invalidate_message to server.
        bool success = send_invalidate_message_to_server(event.bounds());
    }

    Responder::receive_display_event(event);
}

void ScrollView::recalc_content_props()
{
    int max_width = 0;
    int max_height = 0;

    for (auto* view : subviews()) {
        max_width = std::max(max_width, view->frame().max_x());
        max_height = std::max(max_height, view->frame().max_y());
    }

    m_content_size.set_width(max_width);
    m_content_size.set_height(max_height);
}

} // namespace UI