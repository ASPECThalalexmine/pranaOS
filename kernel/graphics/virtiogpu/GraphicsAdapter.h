/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/graphics/virtiogpu/Console.h>
#include <kernel/graphics/virtiogpu/FrameBufferDevice.h>
#include <kernel/graphics/virtiogpu/GPU.h>

namespace Kernel::Graphics::VirtIOGPU {

class GraphicsAdapter final
    : public GraphicsDevice
    , public PCI::DeviceController {
    BASE_MAKE_ETERNAL

public:
    static NonnullRefPtr<GraphicsAdapter> initialize(PCI::Address);

    virtual bool framebuffer_devices_initialized() const override { return m_created_framebuffer_devices; }

private:
    explicit GraphicsAdapter(PCI::Address base_address);

    virtual void initialize_framebuffer_devices() override;
    virtual Type type() const override { return Type::Raw; }

    virtual void enable_consoles() override;
    virtual void disable_consoles() override;

    virtual bool modesetting_capable() const override { return false; }
    virtual bool double_framebuffering_capable() const override { return false; }

    virtual bool try_to_set_resolution(size_t, size_t, size_t) override { return false; }
    virtual bool set_y_offset(size_t, size_t) override { return false; }

    RefPtr<GPU> m_gpu_device;
    bool m_created_framebuffer_devices { false };
};

}