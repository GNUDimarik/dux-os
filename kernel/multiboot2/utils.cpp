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

bool tag_type_to_string(unsigned int type, char *buffer, size_t buffer_len)
{
    if (buffer == nullptr || buffer_len == 0) {
        return false;
    }

    switch (type) {
    case MULTIBOOT_TAG_TYPE_END:
        strlcpy(buffer, "END", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_CMDLINE:
        strlcpy(buffer, "CMDLINE", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        strlcpy(buffer, "BOOT_LOADER_NAME", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_MODULE:
        strlcpy(buffer, "MODULE", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        strlcpy(buffer, "BASIC_MEMINFO", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_BOOTDEV:
        strlcpy(buffer, "BOOTDEV", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_MMAP:
        strlcpy(buffer, "MMAP", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_VBE:
        strlcpy(buffer, "VBE", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        strlcpy(buffer, "FRAMEBUFFER", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
        strlcpy(buffer, "ELF_SECTIONS", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_APM:
        strlcpy(buffer, "APM", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI32:
        strlcpy(buffer, "EFI32", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI64:
        strlcpy(buffer, "EFI64", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_SMBIOS:
        strlcpy(buffer, "SMBIOS", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_ACPI_OLD:
        strlcpy(buffer, "ACPI_OLD", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_ACPI_NEW:
        strlcpy(buffer, "ACPI_NEW", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_NETWORK:
        strlcpy(buffer, "NETWORK", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI_MMAP:
        strlcpy(buffer, "EFI_MMAP", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI_BS:
        strlcpy(buffer, "EFI_BS", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI32_IH:
        strlcpy(buffer, "EFI32_IH", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_EFI64_IH:
        strlcpy(buffer, "EFI64_IH", buffer_len);
        break;

    case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
        strlcpy(buffer, "LOAD_BASE_ADDR", buffer_len);
        break;

    default:
        strlcpy(buffer, "UNKNOWN", buffer_len);
        break;
    }

    return true;
}
} // namespace multiboot2