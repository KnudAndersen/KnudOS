#ifndef INTERRUPT_C
#define INTERRUPT_C
#include "./include/kalloc.h"

#define IDT_ENT_SIZE (8)
#define IDT_NUM      (256)

typedef struct idtr_selector {
    uint16_t sel;
    uint64_t base;
} __attribute__((packed)) idtr_selector;

static inline void set_idrt(uint64_t idt_addr) {
    idtr_selector idtr;
    idtr.sel = 0x8 * 0;
    idtr.base = idt_addr;
}

void init_idt() {
    void* idt = kmalloc(IDT_ENT_SIZE * IDT_NUM);
}

#endif
