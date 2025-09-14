ENTRY(kernel_main)
VIRT = 0xFFFF800000000000;
PHYS = 0x300000;

SECTIONS {
    . = PHYS - 0x1000;
    .cba ALIGN (0x1000) : {
        *(*.cba)
    }
    . = VIRT + PHYS;
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
}

