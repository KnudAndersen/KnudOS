#include <params.h>

// BOOT LAYOUT
#define BOOT_ID_MAP_PAGES (4ull * GiB / PAGE_SIZE)
#define EXTMEM_NC         (0x100000)
#define EXTMEM            (0x100000ULL)

// cannonical split for 48 bits of vaddr space
#define VIRTUAL_KERNEL_HI    (0xFFFFFFFFFFFFFFFFull)
#define VIRTUAL_KERNEL_LO    (0xFFFF800000000000ull)
#define VIRTUAL_USER_HI      (0x00007FFFFFFFFFFFull)
#define VIRTUAL_USER_LO      (0x0000000000000000ull)

#define VIRTUAL_KERNEL_LO_NC (0xFFFF800000000000)

// memory layout
#define KIMAGE_LO (KHHDM_HI + 1ull * MiB)          // offset for guard/misc
#define KHHDM_HI  (KHHDM_LO + 3ull * GiB)          // supports 32GiB of phys mem
#define KHHDM_LO  (VIRTUAL_KERNEL_LO + 1ull * MiB) // offset for guard/misc
/* when you add a // on this line, it can bug clangd lol */
// TODO: contribute and fix this bug?

#define KIMAGE_LO_NC (KHHDM_HI_NC + 1 * MiB)
#define KHHDM_HI_NC  (KHHDM_LO_NC + 3 * GiB)          // supports 32GiB of phys mem
#define KHHDM_LO_NC  (VIRTUAL_KERNEL_LO_NC + 1 * MiB) // offset for guard/misc

#define KSTACK_LO    (KIMAGE_LO + 1ull * MiB)
#define KSTACK_HI    (KSTACK_LO + KSTACK_MAX * (KSTACK_SIZE + PAGE_SIZE))

#define KSTACK_LO_NC (KIMAGE_LO_NC + MiB)
#define KSTACK_HI_NC (KSTACK_LO_NC + KSTACK_MAX * (KSTACK_SIZE + PAGE_SIZE))
// [kstack][page][kstack][page] <-- KSTACK_HI
#define KSTACK_LO_NTH(n)     (KSTACK_HI - (n + 1) * (KSTACK_SIZE + PAGE_SIZE))
#define KSTACK_LO_NTH_NC(n)  (KSTACK_HI_NC - (n + 1) * (KSTACK_SIZE + PAGE_SIZE))
#define KSTACK_HI_NTH(n)     (KSTACK_LO_NTH((n)) + KSTACK_SIZE)
#define KSTACK_HI_NTH_NC(n)  (KSTACK_LO_NTH_NC((n)) + KSTACK_SIZE)

#define KERNEL_ENTRY_STUB_NC (0x1000000)
