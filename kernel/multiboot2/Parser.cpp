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

#include <multiboot2/Parser.h>

namespace multiboot2 {

Parser::Parser(void *addr)
    : base_(reinterpret_cast<uintptr_t>(addr))
{}

Parser::Parser(uint32_t addr)
    : base_(reinterpret_cast<uintptr_t>(addr))
{}
Parser::iterator Parser::begin()
{
    return iterator{firstTag()};
}

Parser::iterator Parser::end()
{
    return iterator{lastTag()};
}

Parser::const_iterator Parser::begin() const
{
    return const_iterator{firstTag()};
}

Parser::const_iterator Parser::end() const
{
    return const_iterator{lastTag()};
}

const Multiboot2Info *Parser::getInfo() const
{
    return reinterpret_cast<Multiboot2Info *>(base_);
}

multiboot_tag *Parser::firstTag() const
{
    return reinterpret_cast<multiboot_tag *>(base_ + sizeof(Multiboot2Info));
}

multiboot_tag *Parser::lastTag() const
{
    return reinterpret_cast<multiboot_tag *>(base_ + getInfo()->total_size - sizeof(multiboot_tag));
}

} // namespace multiboot2
// namespace multiboot2