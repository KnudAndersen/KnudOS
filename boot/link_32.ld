ENTRY(boot_start)

SECTIONS 
{
    . = 1M;
    .multiboot ALIGN (4K) : { *(*.multiboot) }
    .text ALIGN (4K): { *(*.text) }
    .rodata ALIGN (4K): { *(*.rodata) }
    .data ALIGN (4K): { *(*.data) }
    __boot_reserve_start__ = .;
    .boot_reserve ALIGN(4K) : { *(*.boot_reserve)}
    __boot_reserve_end__ = .;
    .bss ALIGN (4K): { *(*.bss) }
    __loader_end__ = .; 
}
