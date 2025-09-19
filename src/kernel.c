#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/io.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/memory.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

tty_t tty0 = (tty_t){0};
void init_kernel(multiboot_info* m_info) {
    /* TODO:
     * map kstack
     * map ACPI region and other MMIO like APIC
     * pass down multiboot info from boostrap
     * initialize pmm accounting for all in-use pages (use multiboot mmap)
     * set up paging again using the pmm and HHDM
     * vmm/kheap
     * formatted io/logging
     */
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG, TTY_DEFAULT_BG, 0, 0);
    vga_tty_render(&tty0);
    pmm_init(m_info);
}
void kernel_main() {

    /* ************************************ */
    /* TREAT AS KNUD OS ABI, DO NOT MODIFY! */
    uint32_t __multiboot = pop_reg32(ESI);
    /* ************************************ */

    multiboot_info* m_info = (multiboot_info*)(uint64_t)__multiboot;
    init_kernel(m_info);
    init_kheap();
    void* test = kmalloc(0x69);
    kprint_heap();
    while (1) {
        asm volatile("hlt");
    }
    return;
}
#endif
