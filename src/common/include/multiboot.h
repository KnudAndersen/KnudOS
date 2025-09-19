#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MMAP_AVAILABLE   (1)
#define MULTIBOOT_MMAP_RESERVED    (2) /* TODO: confirm */
#define MULTIBOOT_MMAP_ACPI        (3)
#define MULTIBOOT_MMAP_HIBERNATION (4)
#define MULTIBOOT_MMAP_DEFECTIVE   (5)

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

typedef struct multiboot_mmap_ent {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) mb_mmap_ent;
#endif
