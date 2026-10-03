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

#include <gnu/multiboot2.h>
#include <multiboot2/utils.h>
#include <posix/posix_strings.h>

namespace multiboot2 {
bool memory_type_to_string(unsigned int type, char *buffer, size_t buffer_len)
{
    if (buffer == nullptr || buffer_len == 0) {
        return false;
    }

    switch (type) {
    case MULTIBOOT_MEMORY_AVAILABLE:
        strlcpy(buffer, "AVAILABLE", buffer_len);
        break;

    case MULTIBOOT_MEMORY_RESERVED:
        strlcpy(buffer, "RESERVED", buffer_len);
        break;

    case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
        strlcpy(buffer, "ACPI", buffer_len);
        break;

    case MULTIBOOT_MEMORY_NVS:
        strlcpy(buffer, "NVS", buffer_len);
        break;

    case MULTIBOOT_MEMORY_BADRAM:
        strlcpy(buffer, "BADRAM", buffer_len);
        break;

    default:
        strlcpy(buffer, "UNKNOWN", buffer_len);
        break;
    }

    return true;
}
} // namespace multiboot2