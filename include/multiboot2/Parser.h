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

#ifndef DUX_PARSER_H
#define DUX_PARSER_H

#include <gnu/multiboot2.h>
#include <stdint.h>

namespace multiboot2 {

constexpr uint32_t kTagHeaderSize = 8;

namespace detail {

constexpr uint32_t kAlignment = 8;

class Iterator
{
public:
    explicit Iterator(multiboot_tag *tag)
        : tag_(tag)
    {}

    const multiboot_tag &operator*() const { return *tag_; }
    const multiboot_tag *operator->() const { return tag_; }
    Iterator &operator++()
    {
        if (tag_ && tag_->size >= kTagHeaderSize && tag_->type != MULTIBOOT_TAG_TYPE_END) {
            tag_ = reinterpret_cast<multiboot_tag *>(
                reinterpret_cast<char *>(tag_) + ((tag_->size + 7) & ~7));
        }

        return *this;
    }

    bool operator==(const Iterator &other) const { return tag_ == other.tag_; }

private:
    multiboot_tag *tag_;
};

} // namespace detail

struct Multiboot2Info {
    uint32_t total_size;
    uint32_t reserved;
};

class Parser
{
public:
    using iterator = detail::Iterator;
    using const_iterator = const detail::Iterator;

    explicit Parser(void *addr);
    explicit Parser(uint32_t addr);
    iterator begin();
    iterator end();
    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator end() const;
private:
    [[nodiscard]] const Multiboot2Info* getInfo() const;
    [[nodiscard]] multiboot_tag* firstTag() const;
    [[nodiscard]] multiboot_tag* lastTag() const;
    uintptr_t base_{};
};

} // namespace multiboot2

#endif //DUX_PARSER_H
