ENTRY(jump_to_kernel)

VIRT = 0xFFFF800000000000;
PHYS = 0x300000;

SECTIONS {
    . = PHYS;
    .kernel_entry ALIGN (0x1000) : {
        *(*.kernel_entry)
    }
    . = VIRT + PHYS + 0x1000;
    __kernel_start = .;
    .text ALIGN (0x1000) : { 
       *(*.text) 
    }
    .rodata ALIGN (0x1000) : { 
       *(*.rodata) 
    }
    .data ALIGN (0x1000) : { 
       *(*.data) 
    }
    .bss ALIGN (0x1000) : { 
       *(*.bss) 
       *(*.COMMON)
    }
    __kernel_end = .;
}

