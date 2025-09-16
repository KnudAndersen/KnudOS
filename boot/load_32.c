#ifndef LOAD_32_C
#define LOAD_32_C

#include "../src/common/include/elf.h"
#include "../src/common/include/multiboot.h"
#include "./include/gdt_32.h"
#include "./include/paging_32.h"

typedef struct kernel_long_jump {
    uint32_t entry;
    uint16_t selector;
} __attribute__((packed)) ljmp_t;

extern uint32_t __loader_end__;
extern uint64_t* boot_pml4;
extern gdtr32_t gdtr;
extern gdt_desc gdt[GDT_NUM_ENT] __attribute__((aligned(8)));

static const char boot_module_magic[] = "KnudKernel";
char boot_reserve[PAGE_RESERVE_SIZE] __attribute__((aligned(PAGE_SIZE)));
uint64_t reserve_off = 0;

uint32_t long_mode_supported() {
    // TODO copy code from archive
    return 1;
};

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
    return 1;
}

int valid = 1;
int is_valid_elf(Elf64_Ehdr* base) {
    return (base->e_ident[EI_MAG0] == ELFMAG0) && (base->e_ident[EI_MAG1] == ELFMAG1) &&
        (base->e_ident[EI_MAG2] == ELFMAG2) && (base->e_ident[EI_MAG3] == ELFMAG3);
}
void Memcpy(void* dst, void* src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
}

Elf64_Addr load_elf(Elf64_Ehdr* base) {
    /* NOTE: assumes identity map applied to reserve memory, always true in loading*/
    /* NOTE: assumes pages are zero initialized */
    /* NOTE: assumes tha reserve memory is identity mapped */
    Elf64_Phdr* ph = (Elf64_Phdr*)((char*)base + base->e_phoff);
    for (Elf64_Half i = 0; i < base->e_phnum; i++) {
        if (ph->p_type == PT_LOAD && (ph->p_vaddr % PAGE_SIZE)) {
            __KERNEL_PANIC__;
        } else if (ph->p_type == PT_LOAD) {
            uint32_t phys_base = (uintptr_t)base + ph->p_offset;
            void* page_alloc;
            uint64_t flags = PAGE_PRESENT;
            /* TODO: fix execute disable. needs to be set in MSR */
            // flags |= (ph->p_flags & PF_X) ? 0 : PAGE_EXEC_DISABLE;
            flags |= ((ph->p_flags & PF_W) || (ph->p_flags & PF_R)) ? PAGE_READ_WRITE : 0;
            const uint32_t num_full_pg = ph->p_filesz / PAGE_SIZE;
            const uint32_t bytes_partial_pg = ph->p_filesz % PAGE_SIZE;
            const uint32_t num_partial_pg = (bytes_partial_pg) ? 1 : 0;
            const uint64_t num_zero_page = CEIL_DIV(ph->p_memsz - ph->p_filesz, PAGE_SIZE);
            for (uint32_t j = 0; j < num_full_pg; j++) {
                map_memory(ph->p_vaddr + j * PAGE_SIZE, phys_base + j * PAGE_SIZE, boot_pml4, 0,
                           flags);
            }
            if (bytes_partial_pg) {
                page_alloc = reserve_alloc_page();
                Memcpy(page_alloc, (void*)(phys_base + num_full_pg * PAGE_SIZE), bytes_partial_pg);
                map_memory(ph->p_vaddr + num_full_pg * PAGE_SIZE, (uintptr_t)page_alloc, boot_pml4,
                           0, flags);
            }
            for (uint32_t j = 0; j < num_zero_page; j++) {
                page_alloc = reserve_alloc_page();
                map_memory(ph->p_vaddr + (j + num_partial_pg) * PAGE_SIZE, (uintptr_t)page_alloc,
                           boot_pml4, 0, flags);
            };
        }
        ph++;
    }
    return base->e_entry;
}

#define KERNEL_INVALID_ENTRY (-1)
Elf64_Addr load_kernel(void* m_base) {
    multiboot_info* m_info = (multiboot_info*)m_base;
    multiboot_mod* mod_iter = (multiboot_mod*)(uintptr_t)m_info->mods_addr;
    Elf64_Addr ret = KERNEL_INVALID_ENTRY;
    Elf64_Ehdr* bin;
    int32_t checksum;
    for (uint32_t i = 0; i < m_info->mods_count; i++) {
        char* mod_str = (char*)(uintptr_t)mod_iter->string;
        checksum = StrNCmp(boot_module_magic, mod_str, sizeof(boot_module_magic) - 1);
        if (checksum == 0) {
            for (uint64_t pg = mod_iter->mod_start; pg < mod_iter->mod_end; pg += PAGE_SIZE) {
                map_memory(pg, pg, boot_pml4, 0, PAGE_DEFAULT);
            }
            bin = (Elf64_Ehdr*)(uintptr_t)mod_iter->mod_start;
            if (!is_valid_elf(bin)) {
                for (uint64_t pg = mod_iter->mod_start; pg < mod_iter->mod_end; pg += PAGE_SIZE) {
                    // TODO
                    unmap_memory(pg, pg, boot_pml4, 0, PAGE_DEFAULT);
                }
            } else {
                ret = load_elf(bin);
                break;
            }
        }
        mod_iter++;
    }
    return ret;
}

ljmp_t mem;
uint32_t loader_main(uint32_t stack_top, uint32_t stack_bot) {
    if (!init_loader(stack_top)) {
        __KERNEL_PANIC__;
    }
    void* mboot_paddr = (void*)(uintptr_t)get_ebx();
    Elf64_Addr entry_addr;
    if ((entry_addr = load_kernel(mboot_paddr)) == KERNEL_INVALID_ENTRY) {
        __KERNEL_PANIC__;
    }
    init_cpu_state();
    mem.selector = GDT_KERN_CODE * sizeof(*gdt);
    mem.entry = (uint32_t)entry_addr;
    return (uint32_t)(uintptr_t)&mem;
}

#endif
