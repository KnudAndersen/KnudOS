#define NO_INLINE
#include <memlayout.h>

ENTRY(kernel_main)

SECTIONS
{
    . = KERNEL_ENTRY_STUB_NC;
    .kernel_entry ALIGN (4K): { *(*.kernel_entry) }

    . = KIMAGE_LO_NC;
    .text ALIGN (4K): { *(*.text) }
    .rodata ALIGN (4K): { *(*.rodata) }
    .data ALIGN (4K): { *(*.data) }
    .bss ALIGN (4K): { *(*.bss) }
}

