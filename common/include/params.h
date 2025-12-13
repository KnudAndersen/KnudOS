#ifndef PARAMS_H
#define PARAMS_H

// HARDWARE/MEMORY
#define MAX_PHYS_RAM  (8ull * GiB)
#define MAX_CPU_CORES (8)

// X86 REGISTERS
#define CR4_PGE        (1 << 7)
#define CR4_PAE        (1ull << 5)
#define CR0_PG         (1 << 31)
#define CR3_PDBR_MASK  (0xFFFFFFFFFFFFF000)
#define CR3_PDBR_SHIFT (12)
#define MSR_EFER       (0xC0000080)
#define EFER_LME       (1 << 8)

// GENERAL
#define KiB  (1024)
#define MiB  (1024 * KiB)
#define GiB  (1024 * MiB)
#define NULL ((void*)0)

// GLOBAL METHODS
#define MAKE_PTR(ptr)      ((void*)(uintptr_t)(ptr))
#define CEIL_DIV(val, div) (((val) + (div) - 1) / (div))

// X86 GENERAL
#define PAGE_SIZE (4 * KiB)
#define PTE_NUM   (512)

// BOOT STUB
#define MULTIBOOT_MAGIC    0x1BADB002
#define MULTIBOOT_RESPONSE 0x2BADB002
#define MULTIBOOT_FLAGS    (MULTIBOOT_ALIGN | MULTIBOOT_MMAP)
#define MULTIBOOT_ALIGN    (1 << 0)
#define MULTIBOOT_MMAP     (1 << 1)

// extra identity map length for reserving early pages, e.g. reserving page tables for
// higher half direct map in src/vmm.c
#define BOOT_IDENTITY_MAP_EXTRA (20 * MiB)
// this number controls the number of pages in the boot page resreve in loader image
// it will need to grow if BOOT_IDENTITY_MAP_EXTRA is increased obviously
#define BOOT_PAGES_MAX (3ull * 4096ull)
// COMPILER MACROS
#define ALIGN_C(val) __attribute__((aligned(val)))
#define PACK         __attribute__((packed))

// NOTE: NO_INLINE for cpp -> avoid linker failures
#ifndef NO_INLINE
#endif

#endif
