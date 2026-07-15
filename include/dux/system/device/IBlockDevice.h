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

#ifndef DUX_SYSTEM_DEVICE_IBLOCKDEVICE_H
#define DUX_SYSTEM_DEVICE_IBLOCKDEVICE_H

#include <dux/system/device/IDevice.h>
#include <dux/types.h>
#include <stddef.h>

namespace dux::system::device
{
class IBlockDevice : public IDevice
{
public:
    DeviceType type() const noexcept final
    {
        return DeviceType::kBlock;
    }

    virtual size_t blockSize() const noexcept = 0;
    virtual uint64_t blockCount() const noexcept = 0;

    virtual ssize_t readBlocks(
        uint64_t firstBlock,
        size_t blockCount,
        void *buffer) = 0;

    virtual ssize_t writeBlocks(
        uint64_t firstBlock,
        size_t blockCount,
        const void *buffer) = 0;

    virtual int flush() = 0;
};
}

#endif //DUX_SYSTEM_DEVICE_IBLOCKDEVICE_H