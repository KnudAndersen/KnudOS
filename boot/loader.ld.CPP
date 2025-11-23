#define NO_INLINE
#include <memlayout.h>
ENTRY(boot_entry)

SECTIONS
{
    . = EXTMEM_NC;
    .multiboot ALIGN (4K) : {*(*.multiboot) }
    .text ALIGN (4K): { *(*.text) }
    .rodata ALIGN (4K): { *(*.rodata) }
    .data ALIGN (4K): { *(*.data) }
    .bss ALIGN (4K): { *(*.bss) }
    __linker_loader_end = .;
}
