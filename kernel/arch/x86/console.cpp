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
#include <errno.h>
#include <string.h>

namespace {

using namespace dux::kernel;

char *g_base = nullptr;
int g_pos = 0;
int g_width = 0;
int g_height = 0;
int g_background = static_cast<int>(Color::kDarkGray);
int g_char_attr = static_cast<int>(Color::kWhite);

int size()
{
    return g_width * g_height;
}

void put(int pos, int c, int attr)
{
    g_base[pos * 2] = static_cast<char>(c);
    g_base[pos * 2 + 1] = static_cast<char>(attr);
}

void scroll()
{
    memmove(g_base, g_base + g_width * 2, (g_height - 1) * g_width * 2);
    const int first = (g_height - 1) * g_width;

    for (int i = first; i < size(); ++i) {
        put(i, ' ', g_background);
    }

    g_pos = first;
}

int put_char(int c, int attr)
{
    if (!g_base) {
        return g_pos;
    }

    if (c == '\n') {
        g_pos += g_width - g_pos % g_width;
    } else {
        put(g_pos++, c, attr);
    }

    if (g_pos >= size()) {
        scroll();
    }

    return g_pos;
}

} // namespace

namespace dux::kernel {

int console_init(char *base, int width, int height)
{
    g_base = base;
    g_width = width;
    g_height = height;
    return g_pos;
}

int console_write(const char *str, int len)
{
    while (*str) {
        put_char(*str++, g_char_attr);
    }

    return g_pos;
}

int console_clear(int attr)
{
    if (!g_base) {
        return -EINVAL;
    }

    for (int i = 0; i < size(); ++i) {
        put(i, ' ', attr);
    }

    g_pos = 0;
    return g_pos;
}
} // namespace dux::kernel