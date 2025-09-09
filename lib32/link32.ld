ENTRY(loader_main)

SECTIONS 
{
    . = 1M;
    .multiboot ALIGN (4K) : { *(*.multiboot) }
    __mboot_end__ = .;
    .text ALIGN (4K): { *(*.text*) }
    __text_end__ = .;
    .rodata ALIGN (4K): { *(*.rodata*) }
    __rodata_end__ = .;
    .data ALIGN (4K): { *(*.data*) }
    __data_end__ = .;
    .bss ALIGN (4K): { *(*.bss*) }
    __loader_end__ = .; 
}
