/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "../shared/Connections/WSConnection.h"
#include "Compositor.h"

namespace WinServer {

class WindowServerDecoder : public BaseWindowServerDecoder {
public:
    WindowServerDecoder() = default;
    ~WindowServerDecoder() = default;

    using BaseWindowServerDecoder::handle;
    virtual std::unique_ptr<Message> handle(const GreetMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const CreateWindowMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const DestroyWindowMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const SetBarStyleMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const SetTitleMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const SetBufferMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const InvalidateMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MenuBarCreateMenuMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MenuBarCreateItemMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const AskBringToFrontMessage& msg) override;
};

} // namespace WinServer