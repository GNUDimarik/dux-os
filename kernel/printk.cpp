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

#include <asm/cpu.h>
#include <dux/irq_save_lock.h>
#include <dux/kernel/console.h>
#include <dux/kernel/printk.h>
#include <dux/spin_lock.h>
#include <string.h>

namespace dux::kernel {

namespace {
static constexpr char kPanicMessage[] = "Kernel panic: ";
static constexpr int kBufferSize = 1024;
static constexpr char kErrorMessage[] = "Error: ";
dux::spin_lock global_lock;
} // namespace

static int vprintk_unlocked(const char *fmt, va_list args)
{
    char buffer[kBufferSize];
    const int ret = vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (ret > 0) {
        console_write(buffer, ret < kBufferSize ? ret : kBufferSize - 1);
    }

    return ret;
}

int vprintk(const char *fmt, va_list args)
{
    irq_save_lock l(global_lock);
    return vprintk_unlocked(fmt, args);
}

int printk(const char *fmt, ...)
{
    irq_save_lock l(global_lock);
    va_list ap;
    va_start(ap, fmt);
    const int result = vprintk_unlocked(fmt, ap);
    va_end(ap);
    return result;
}

int printk(const char *tag, const char *fmt, ...)
{
    irq_save_lock l(global_lock);
    int result = strlen(tag);
    console_write(tag, result);
    va_list ap;
    va_start(ap, fmt);
    result += vprintk_unlocked(fmt, ap);
    va_end(ap);
    return result;
}

int printk_error(const char *tag, const char *fmt, ...)
{
    irq_save_lock l(global_lock);
    console_write(kErrorMessage, sizeof(kErrorMessage) - 1);
    int result = sizeof(kErrorMessage) - 1;
    const auto tag_len = strlen(tag);
    console_write(tag, tag_len);
    result += tag_len;
    va_list ap;
    va_start(ap, fmt);
    result += vprintk_unlocked(fmt, ap);
    va_end(ap);

    return result;
}

[[noreturn]] void panic(const char *fmt, ...)
{
    irq_save_lock l(global_lock);
    console_write(kPanicMessage, sizeof(kPanicMessage) - 1);
    va_list ap;
    va_start(ap, fmt);
    vprintk_unlocked(fmt, ap);
    va_end(ap);

    for (;;) {
        cpu_halt();
    }
}

} // namespace dux::kernel