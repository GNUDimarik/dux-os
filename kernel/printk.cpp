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
#include <dux/kernel/printk.h>

static constexpr char kPanicMessage[] = "Kernel panic: ";
static constexpr size_t kBufferSize = 1024;

namespace dux::kernel {

int vprintk(const char *fmt, va_list args)
{
    char buffer[kBufferSize];
    const int ret = vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (ret > 0) {
        console_write(buffer, ret < kBufferSize ? ret : kBufferSize - 1);
    }

    return ret;
}

int printk(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    const int result = vprintk(fmt, ap);
    va_end(ap);
    return result;
}

[[noreturn]] void panic(const char *fmt, ...)
{
    console_write(kPanicMessage, sizeof(kPanicMessage) - 1);
    va_list ap;
    va_start(ap, fmt);
    vprintk(fmt, ap);
    va_end(ap);

    for (;;) {
    }
}

} // namespace dux::kernel