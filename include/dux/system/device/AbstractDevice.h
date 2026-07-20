/*
* The MIT License (MIT)
 *
 * Copyright (c) 2026 Dmitry Adzhiev <dmitry.adjiev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef DUX_ABSTRACTDEVICE_H
#define DUX_ABSTRACTDEVICE_H

#include <dux/system/device/IDevice.h>
#include <errno.h>

namespace dux::system::device {
template<typename _Interface>
class AbstractDevice : public _Interface
{
public:
    AbstractDevice(DeviceId id, const char *name)
        : id_(id)
        , name_(name)
    {}
    [[nodiscard]] DeviceId id() const noexcept override { return id_; }
    const char *name() const noexcept override { return name_; }
    int ioctl(unsigned long request, void *argument) override { return -ENOTSUP; }
    int flush() override { return 0; }

private:
    DeviceId id_;
    const char *name_;
};
} // namespace dux::system::device

#endif //DUX_ABSTRACTDEVICE_H
