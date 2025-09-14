ENTRY(kernel_main)

VIRT = 0x800000000000;
PHYS = 0x300000;

SECTIONS {
    . = 
    .cba ALIGN (0x1000) : {
        *(*.cba)
    }
    . = VIRT + PHYS;
    .text ALIGN (0x1000) : AT(ADDR(.text) - VIRT) { 
       *(*.text) 
    }
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata)-VIRT) { 
       *(*.rodata) 
    }
    .data ALIGN (0x1000) : AT(ADDR(.data)-VIRT) { 
       *(*.data) 
    }
    .bss ALIGN (0x1000) : AT(ADDR(.bss)-VIRT) { 
       *(*.bss) 
       *(*.COMMON)
    }
}

