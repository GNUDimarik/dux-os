OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)

SECTIONS
{
    /* Текущая схема: VMA == LMA, ядро загружается с 1 MiB. */
    . = 1M;
    __kernel_start = .;

    /* Заголовок Multiboot2, если boot.S выделяет отдельную секцию. */
    .multiboot ALIGN(8) :
    {
        KEEP(*(.multiboot))
        KEEP(*(.multiboot.*))
    }

    .text ALIGN(4K) :
    {
        __text_start = .;

        /*
         * Совместимость с boot.S, где Multiboot2 header лежит в .text.
         * Загрузочный объект помещаем раньше остального кода.
         */
        KEEP(*boot.S.obj(.text .text.*))
        KEEP(*boot.S.o(.text .text.*))

        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)

        __text_end = .;
    }

    .rodata ALIGN(4K) :
    {
        __rodata_start = .;

        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)

        __rodata_end = .;
    }

    .data ALIGN(4K) :
    {
        __data_start = .;

        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)

        __data_end = .;
    }

    /* Таблица функций динамической инициализации C++-объектов. */
    .init_array ALIGN(4) :
    {
        __init_array_start = .;

        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))

        __init_array_end = .;
    }

    /* Таблица финализации, если компилятор её сформирует. */
    .fini_array ALIGN(4) :
    {
        __fini_array_start = .;

        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
        KEEP(*(.fini_array))

        __fini_array_end = .;
    }

    /* Неинициализированные данные, включая стек из .comm. */
    .bss ALIGN(4K) (NOLOAD) :
    {
        __bss_start = .;

        *(COMMON)
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)

        __bss_end = .;
    }

    /* Резервируем последнюю занятую ядром страницу целиком. */
    . = ALIGN(4K);
    __kernel_end = .;

    /* Для текущего ядра без исключений и stack unwinding. */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.eh_frame.*)
        *(.eh_frame_hdr)
        *(.gcc_except_table)
        *(.gcc_except_table.*)
        *(.note*)
    }
}