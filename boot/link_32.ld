ENTRY(boot_start)

SECTIONS 
{
    . = 1M;
    .multiboot ALIGN (4K) : { *(*.multiboot) }
    .text ALIGN (4K): { *(*.text) }
    .rodata ALIGN (4K): { *(*.rodata) }
    .data ALIGN (4K): { *(*.data) }
    .bss ALIGN (4K): { *(*.bss) }
    __loader_end__ = .; 
}
