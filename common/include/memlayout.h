#include <params.h>

#define PADDING (1 * MiB)
#define GUARD   (PAGE_SIZE)

// BOOT EXTENDED MEMORY
#define EXTMEM (0x100000ull)

// VIRTUAL ADDRESS SPACE SPLIT (48-BITS)
#define KERNEL_SPACE_HI (0xFFFFFFFFFFFFFFFFull)
#define KERNEL_SPACE_LO (0xFFFF800000000000ull)
#define USER_SPACE_HI   (0x00007FFFFFFFFFFFull)
#define USER_SPACE_LO   (0x0000000000000000ull)

// HIGHER HALF DIRECT MAP
#define HHDM_LO   (KERNEL_SPACE_LO + PADDING)
#define HHDM_HI   (HHDM_LO + MAX_PHYS_RAM)
#define HHDM_VOFF (HHDM_LO)

// KERNEL IMAGE (VIRTUAL, PHYSICAL)
#define KERNEL_IMAGE_LO (HHDM_HI + PADDING)

// KERNEL STACKS
// KSTACK_REGION_LO -> [GUARD][STACK|GUARD][STACK|GUARD]...[STACK|GUARD] <- KSTACK_REGION_HI
// KSTACK_NTH_LO(0) ----------^       (lowest byte of stack inclusive)
// KSTACK_NTH_HI(0) ----------------^ (highest byte of stack exclusive, so first byte after stack)

#define KSTACK_MAX_NUM   (2 * MAX_CPU_CORES)
#define KSTACK_REGION_LO (KERNEL_IMAGE_LO + GUARD)
#define KSTACK_SIZE      (8ull * KiB)
#define KSTACK_SLOT      (KSTACK_SIZE + GUARD)
#define KSTACK_NTH_LO(n) (KSTACK_REGION_LO + (n) * KSTACK_SLOT)
#define KSTACK_NTH_HI(n) (KSTACK_NTH_LO(n) + KSTACK_SIZE)

// NO CAST MACROS, NC = NO CASE
// tedious to maintain, but useful for applying CPP to LD script among other filetypes
#define MAX_PHYS_RAM_NC      (8 * 1024 * 1024 * 1024)
#define EXTMEM_NC            (0x100000)
#define KERNEL_SPACE_LO_NC   (0xFFFF800000000000)
#define KERNEL_ENTRY_STUB_NC (0x1000000)
#define PAGE_SIZE_NC         (4096)
#define PADDING_NC           (1024 * 1024)
#define KSTACK_SIZE_NC       (8 * 1024)

#define GUARD_NC             (PAGE_SIZE_NC)
#define HHDM_LO_NC           (KERNEL_SPACE_LO_NC + PADDING_NC)
#define HHDM_HI_NC           (HHDM_LO_NC + MAX_PHYS_RAM_NC)
#define KSTACK_REGION_LO_NC  (KERNEL_IMAGE_LO_NC + GUARD_NC)
#define KERNEL_IMAGE_LO_NC   (HHDM_HI_NC + PADDING_NC)
#define KSTACK_SLOT_NC       (KSTACK_SIZE_NC + GUARD_NC)
#define KSTACK_NTH_LO_NC(n)  (KSTACK_REGION_LO_NC + (n) * KSTACK_SLOT_NC)
#define KSTACK_NTH_HI_NC(n)  (KSTACK_NTH_LO_NC(n) + KSTACK_SIZE_NC)
