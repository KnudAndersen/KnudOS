#ifndef LOAD_32_C
#define LOAD_32_C

#include "/usr/include/elf.h"
#include "gdt.h"
#include "paging.h"
#include <stdint.h>

/* x86-define */
#define CR0_PG         (1ULL << 31) /* paging enable */
#define CR4_PAE        (1ULL << 5)  /* physical addr extn */
#define IA32_EFER_ADDR (0xC0000080) /* model specific register for LM*/
#define IA32_EFER_LME  (1 << 8)     /* long mode enable */

typedef struct kernel_long_jump {
    uint32_t entry;
    uint16_t selector;
} __attribute__((packed)) ljmp_t;

extern uint32_t __loader_end__;
extern uint64_t* boot_pml4;

char boot_reserve[PAGE_RESERVE_SIZE] __attribute__((aligned(PAGE_SIZE)));
uint64_t reserve_off;

uint32_t long_mode_supported() {
    return 1;
};
extern gdtr32_t gdtr;

void init_cpu_state() {
    asm volatile("lgdt [%0]"
                 :
                 : "r"(&gdtr)
                 :);
    asm volatile("mov ax, %0\n\t"
                 "ltr ax"
                 :
                 : "r"((uint16_t)(GDT_TSS_LO * sizeof(uint64_t)))
                 : "ax");
    asm volatile("mov eax, cr0\n\t"
                 "and eax, %0\n\t"
                 "mov cr0, eax"
                 :
                 : "r"((uint32_t)(0xFFFFFFFF & ~(CR0_PG)))
                 : "eax");
    asm volatile("mov eax, cr4\n\t"
                 "or eax, %0\n\t"
                 "mov cr4, eax"
                 :
                 : "r"((uint32_t)CR4_PAE)
                 : "eax");
    asm volatile("mov cr3, %0"
                 :
                 : "r"((uint32_t)(uintptr_t)boot_pml4)
                 :);
    asm volatile("mov ecx, %0\n\t"
                 "rdmsr\n\t"
                 "or eax, %1\n\t"
                 "xor edx, edx\n\t"
                 "wrmsr"
                 :
                 : "r"((uint32_t)IA32_EFER_ADDR), "r"((uint32_t)IA32_EFER_LME)
                 : "eax", "ebx", "ecx");
    /* CR0.PG=1*/
    asm volatile("mov eax, cr0\n\t"
                 "or eax, %0\n\t"
                 "mov cr0, eax"
                 :
                 : "r"((uint32_t)CR0_PG)
                 : "eax");
    return;
}

uint32_t init_loader(uint32_t stack_top_addr) {
    if (!long_mode_supported()) {
        return 0;
    }
    init_gdt();
    init_tss(stack_top_addr);
    for (int k = 0; k < PAGE_RESERVE_SIZE; k++) {
        boot_reserve[k] = 0;
    }
    boot_pml4 = (uint64_t*)reserve_alloc_page();
    uint64_t page;
    for (page = 0; page < (uint64_t)&__loader_end__; page += PAGE_SIZE)
        map_memory(page, page, boot_pml4, 0, PAGE_DEFAULT);
    // do not enter long mode until we have set up the
    // virtual memory mapping of the kernel
    // init_cpu_state();
    // we will then jump to the kernel
    return 1;
}
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed)) multiboot_info;

typedef struct {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} __attribute__((packed)) multiboot_mod;

multiboot_info tmp;
multiboot_mod md;
static const char string[] = "KnudKernel";
int valid = 1;
/* x86 define execute disable */
#define PAGE_EXEC_DISABLE (1ULL << 63)
#define PAGE_WRITE        (1ULL << 1)
int load_elf(Elf64_Ehdr* base) {
    valid = valid && (base->e_ident[EI_MAG0] == ELFMAG0);
    valid = valid && (base->e_ident[EI_MAG1] == ELFMAG1);
    valid = valid && (base->e_ident[EI_MAG2] == ELFMAG2);
    Elf64_Phdr* ph = (Elf64_Phdr*)((char*)base + base->e_phoff);
    if (!valid) {
        return valid;
    }
    for (Elf64_Half i = 0; i < base->e_phnum; i++) {
        if (ph->p_type == PT_LOAD && ph->p_vaddr % PAGE_SIZE) {
            __KERNEL_PANIC__;
        } else if (ph->p_type == PT_LOAD) {
            char* phys_base = (char*)base + ph->p_offset;
            void* zero_page;
            uint64_t flags = PAGE_PRESENT;
            flags |= (ph->p_flags & PF_X) ? 0 : PAGE_EXEC_DISABLE;
            flags |= (ph->p_flags & PF_W) ? PAGE_WRITE : 0;
            uint64_t page_i, page_b;
            uint64_t file_full_pages = ph->p_filesz / PAGE_SIZE;
            uint64_t file_full_bytes = file_full_pages * PAGE_SIZE;
            uint64_t file_partial_bytes = ph->p_filesz % PAGE_SIZE;
            uint64_t zero_bytes = (ph->p_memsz - ph->p_filesz);
            uint64_t zero_pages = CEIL_DIV(zero_bytes, PAGE_SIZE);
            for (page_i = 0; page_i < file_full_pages; page_i++) {
                page_b = page_i * PAGE_SIZE;
                map_memory(ph->p_vaddr + page_b, (uint64_t)phys_base + page_b, boot_pml4, 0, flags);
            }
            if (file_partial_bytes) {
                zero_page = reserve_alloc_page();
                for (uint64_t k = 0; k < file_partial_bytes; k++) {
                    ((char*)zero_page)[k] = phys_base[file_full_bytes + k];
                }
                page_b = file_full_pages * PAGE_SIZE;
                map_memory(ph->p_vaddr + page_b, (uint64_t)zero_page, boot_pml4, 0, flags);
                if (zero_bytes) {
                    zero_bytes -= file_partial_bytes;
                    zero_pages = CEIL_DIV(zero_bytes, PAGE_SIZE);
                }
            }
            if (zero_bytes) {
                for (page_i = 0; page_i < zero_pages; page_i++) {
                    zero_page = reserve_alloc_page();
                    page_b = (file_full_pages + page_i) * PAGE_SIZE;
                    map_memory(ph->p_vaddr + page_b, (uint64_t)zero_page, boot_pml4, 0, flags);
                }
            }
        }
        __KERNEL_PANIC__;
        ph++;
    }
    return valid;
}

static const char kernel_magic[] = "KnudKernel";

uint32_t load_kernel(void* m_base) {
    multiboot_info* m_info = (multiboot_info*)m_base;
    multiboot_mod* mod_iter = (multiboot_mod*)(uintptr_t)m_info->mods_addr;
    for (uint32_t i = 0; i < m_info->mods_count; i++) {
        char* mod_str = (char*)(uintptr_t)mod_iter->string;
        int is_kernel = 1;
        for (int j = 0; mod_str[j] && (j < sizeof(kernel_magic)); j++) {
            if (mod_str[j] != kernel_magic[j]) {
                is_kernel = 0;
                break;
            }
        }
        if (is_kernel) {
            for (uint64_t pg = mod_iter->mod_start; pg < mod_iter->mod_end; pg += PAGE_SIZE)
                map_memory(pg, pg, boot_pml4, 0, PAGE_DEFAULT);
            load_elf((Elf64_Ehdr*)(uintptr_t)mod_iter->mod_start);
            break;
        }
        mod_iter++;
    }
    __KERNEL_PANIC__;
    return 0;
}

uint32_t loader_main(uint32_t boot_stack) {
    if (!init_loader(boot_stack)) {
        __KERNEL_PANIC__;
    }
    void* mboot_paddr = (void*)(uintptr_t)get_ebx();

    if (!load_kernel(mboot_paddr)) {
        __KERNEL_PANIC__;
    }
    return 0;
}

#endif
