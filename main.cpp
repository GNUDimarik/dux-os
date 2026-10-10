#include "kernel/console.h"
#include "multiboot2/utils.h"
#include <asm/cpu.h>
#include <dux/kernel/printk.h>
#include <gnu/multiboot2.h>
#include <malloc.h>
#include <multiboot2/Parser.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Macros.  */

/* Some screen stuff.  */
/* The number of columns.  */
#define COLUMNS 80
/* The number of lines.  */
#define LINES 24
/* The attribute of an character.  */
#define ATTRIBUTE 7
/* The video memory address.  */
#define VIDEO 0xB8000

constexpr uintptr_t kPageSize = 4096;

using namespace dux::kernel;

__BEGIN_DECLS
extern char __kernel_start[];
extern char __kernel_end[];

/*static uintptr_t address_of(const void *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}*/

constexpr size_t align_down(size_t value, size_t alignment)
{
    return value & ~(alignment - 1);
}

#define BUFFER_SIZE 128

[[noreturn]]
void main(uint32_t magic, uint32_t addr)
{
    const uintptr_t kernel_end = reinterpret_cast<uintptr_t>(__kernel_end);
    const uintptr_t first_free = (kernel_end + 7) & ~static_cast<uintptr_t>(7);
    dux::kernel::console_init(reinterpret_cast<char *>(VIDEO), COLUMNS, LINES);
    size_t heap_size{0};

    if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
        const auto kernel_begin = reinterpret_cast<uintptr_t>(__kernel_start);
        const auto kernel_end = reinterpret_cast<uintptr_t>(__kernel_end);

        struct multiboot_tag *tag;
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        for (tag = reinterpret_cast<multiboot_tag *>(addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = reinterpret_cast<multiboot_tag *>(
                 reinterpret_cast<multiboot_uint8_t *>(tag) + ((tag->size + 7) & ~7))) {
            switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP:
                for (multiboot_memory_map_t *mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                     reinterpret_cast<multiboot_uint8_t *>(mmap)
                     < (multiboot_uint8_t *) tag + tag->size;
                     mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                                        + ((struct multiboot_tag_mmap *) tag)
                                                              ->entry_size)) {
                    multiboot2::memory_type_to_string(mmap->type, buffer, BUFFER_SIZE);
                    const unsigned long long bytes = static_cast<unsigned long long>(mmap->len);

                    const unsigned long long mb = bytes / (1024ULL * 1024ULL);

                    const unsigned long long mb_fraction = ((bytes % (1024ULL * 1024ULL)) * 100ULL)
                                                           / (1024ULL * 1024ULL);

                    printk(
                        "base_addr = 0x%llx, length = %llu.%02llu MB, type = %s\n",
                        static_cast<unsigned long long>(mmap->addr),
                        mb,
                        mb_fraction,
                        buffer);

                    const uint64_t region_begin = mmap->addr;
                    const uint64_t region_end = mmap->addr + mmap->len;

                    if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE && first_free >= region_begin
                        && first_free < region_end) {
                        heap_size = static_cast<size_t>(region_end - first_free)
                                    & ~static_cast<size_t>(7);
                    }
                }
                break;
            }
        }

        printk(
            "kernel_start = 0x%llx\n"
            "kernel_end   = 0x%llx\n"
            "first_free   = 0x%llx\n"
            "heap_size = %lu bytes\n",
            static_cast<unsigned long long>(kernel_begin),
            static_cast<unsigned long long>(kernel_end),
            static_cast<unsigned long long>(first_free),
            heap_size);

        if (heap_size > 0) {
            libstdc_allocator_initialize(reinterpret_cast<void *>(first_free), heap_size);
            char *string = new char[BUFFER_SIZE];
            printk("string is %s\n", string);
            //libstdc_dump_memory();
            multiboot2::Parser parser(addr);
            int index = 0;

            for (const auto &i : parser) {
                multiboot2::tag_type_to_string(i.type, string, BUFFER_SIZE);
                printk("tag[%d] type '%s'\n", index++, string);
            }

            delete[] string;
        } else {
            panic("No memory");
        }
    } else {
        panic("DUX requires Multiboot2 header");
    }

    for (;;) {
        cpu_halt();
    }
}

__END_DECLS
