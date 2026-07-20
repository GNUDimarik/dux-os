OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)

SECTIONS
{
    /*
     * Загружаем ядро начиная с физического адреса 1 MiB.
     *
     * Нижняя память традиционно занята BIOS, таблицами,
     * VGA и другими структурами платформы.
     */
    . = 1M;

    __kernel_start = .;

    /*
     * Multiboot2 header должен находиться как можно ближе
     * к началу ELF-файла.
     *
     * KEEP запрещает линкеру удалить секцию при использовании
     * --gc-sections.
     */
    .multiboot ALIGN(8) :
    {
        KEEP(*(.multiboot))
    }

    /*
     * Исполняемый код.
     */
    .text ALIGN(4K) :
    {
        __text_start = .;

        *(.text)
        *(.text.*)

        __text_end = .;
    }

    /*
     * Константные данные.
     */
    .rodata ALIGN(4K) :
    {
        __rodata_start = .;

        *(.rodata)
        *(.rodata.*)

        __rodata_end = .;
    }

    /*
     * Инициализированные изменяемые данные.
     */
    .data ALIGN(4K) :
    {
        __data_start = .;

        *(.data)
        *(.data.*)

        __data_end = .;
    }

    /*
     * Неинициализированные данные.
     */
    .bss ALIGN(4K) :
    {
        __bss_start = .;

        *(COMMON)
        *(.bss)
        *(.bss.*)

        __bss_end = .;
    }

    __kernel_end = .;

    /*
     * Не нужные ядру служебные секции.
     */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.note*)
        *(.gcc_except_table)
    }
}