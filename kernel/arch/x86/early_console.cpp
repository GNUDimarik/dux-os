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

#include <dux/kernel/console.h>
#include <stdint.h>

namespace {

constexpr uintptr_t kVgaBufferAddress = 0xB8000;
constexpr uint16_t kVgaWidth = 80;
constexpr uint8_t kDefaultColor = 0x0F;

volatile uint16_t *const kVgaBuffer = reinterpret_cast<volatile uint16_t *>(kVgaBufferAddress);

constexpr uint16_t make_vga_entry(char character, uint8_t color)
{
    return static_cast<uint16_t>(
        static_cast<uint8_t>(character) | static_cast<uint16_t>(color) << 8);
}

} // namespace

namespace dux::kernel {
int console_write(const char *str, size_t len)
{
    uint16_t position = 0;

    while (*str != '\0') {
        kVgaBuffer[position++] = make_vga_entry(*str++, kDefaultColor);
    }

    return position;
}
}