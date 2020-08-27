ENTRY(_start)

SECTIONS
{
    . = 1M;

    .text : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    .data : ALIGN(4K)
    {
        start_ctors = .;
        KEEP(*(.ctors));
        KEEP(*(SORT_BY_INIT_PRIORITY(.ctors.*)));
        end_ctors = .;
        *(.data)
    }

    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }

    /* Define a symbol that we can use to get the end of kernel code */
    end = .;
}
