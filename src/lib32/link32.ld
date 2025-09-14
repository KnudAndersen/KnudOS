ENTRY(boot_start)


SECTIONS 
{
    . = 1M;
    .multiboot ALIGN (4K) : { *(*.multiboot) }
    .text ALIGN (4K): { *(*.text*) }
    .rodata ALIGN (4K): { *(*.rodata*) }
    .data ALIGN (4K): { *(*.data*) }
    .bss ALIGN (4K): { *(*.bss*) }
    . = 3M;
    .boot_reserve ALIGN (4K) : {
        *(*.boot_reserve)
    }
    __loader_end__ = .; 
}
