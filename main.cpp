#include "kernel/console.h"
#include <asm/cpu.h>
#include <dux/kernel/printk.h>
#include <gnu/multiboot2.h>
#include <stdint.h>

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

extern "C" {
extern char __kernel_start[];
extern char __kernel_end[];
}

static uintptr_t address_of(const void *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

constexpr uintptr_t kPageSize = 4096;

using dux::kernel::printk;

__BEGIN_DECLS
extern char __kernel_start[];
extern char __kernel_end[];

[[noreturn]]
void main(uint32_t magic, uint32_t addr)
{
    dux::kernel::console_init(reinterpret_cast<char *>(VIDEO), COLUMNS, LINES);
    printk("Hello world\n");
    /*if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
        const uintptr_t kernel_begin = reinterpret_cast<uintptr_t>(__kernel_start);

        const uintptr_t kernel_end = reinterpret_cast<uintptr_t>(__kernel_end);

        const uintptr_t kernel_size = kernel_end - kernel_begin;
        struct multiboot_tag *tag;
        unsigned size;

        for (tag = (struct multiboot_tag *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
            switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP:

                break;

            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:

                break;
            }
        }

        dux::kernel::console_init(reinterpret_cast<char *>(VIDEO), COLUMNS, LINES);
    }*/

    for (;;) {
        cpu_halt();
    }
}

__END_DECLS
