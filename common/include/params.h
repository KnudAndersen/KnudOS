#ifndef PARAMS_H
#define PARAMS_H

// X86 REGISTERS
#define CR4_PGE        (1 << 7)
#define CR4_PAE        (1ULL << 5)
#define CR0_PG         (1 << 31)
#define CR3_PDBR_MASK  (0xFFFFFFFFFFFFF000)
#define CR3_PDBR_SHIFT (12)
#define MSR_EFER       (0xC0000080)
#define EFER_LME       (1 << 8)

// KERNEL GENERAL
#define KSTACK_MAX     (20)
#define KSTACK_SIZE    (4 * KiB)
#define KSTACK_SIZE_NC (4096)
#define KiB            (1024)
#define MiB            (1024 * KiB)
#define GiB            (1024 * MiB)
#define NULL           ((void*)0)

// GLOBAL METHODS
#define MAKE_PTR(ptr)      ((void*)(UPTR)(ptr))
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

// COMPILER MACROS
#define ALIGN_C(val) __attribute__((aligned(val)))
#define PACK         __attribute__((packed))

// NOTE: NO_INLINE for cpp -> avoid linker failures
#ifndef NO_INLINE
#endif

#endif
