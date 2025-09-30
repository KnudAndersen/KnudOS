#ifndef KERNEL_C
#define KERNEL_C

#include "../common/include/gsyms.h"
#include "./lib/include/interrupt.h"
#include "./lib/include/io.h"
#include "./lib/include/kalloc.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/paging.h"
#include "./lib/include/pmm.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"
tty_t tty0 = (tty_t){0};

void init_kernel(multiboot_info* m_info, uint64_t* pml4) {
    /* TODO:
     * set up IDT and interrupt routing
     * ACPI tables parser
     * reserve map ACPI region and other MMIO like APIC
     * set up VMM, add support for specific physaddrs for MMIO
     *
     * Bonus:
     * formatted io/logging
     * stop demand paging kstack (set fixed size, e.g. linux recently updated to 8KiB)
     */
    /* GRAPHICS */
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG, TTY_DEFAULT_BG, 0, 0);
    vga_tty_render(&tty0);
    /* MEMORY MANAGEMENT */
    pmm_init(m_info);
    kheap_init(pml4);
    /* INTERRUPTS */
    init_idt();
}

void kernel_main(uint64_t __multiboot, uint64_t __reserve, uint64_t __reserve_off,
                 uint64_t __boot_pml4) {

    multiboot_info* m_info = (multiboot_info*)(uint64_t)__multiboot;
    init_kernel(m_info, (uint64_t*)__boot_pml4);

    __KERNEL_PANIC__;
    return;
}
#endif
