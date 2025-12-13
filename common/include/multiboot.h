#ifndef COMMON_MULTIBOOT_H
#define COMMON_MULTIBOOT_H

#include <types.h>

// I have bits 0 & 1 of flags set
typedef struct {
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	u32 syms[4];
	u32 mmap_length;
	u32 mmap_addr;
	u32 drives_length;
	u32 drives_addr;
	u32 config_table;
	u32 boot_loader_name;
	u32 apm_table;
	u32 vbe_control_info;
	u32 vbe_mode_info;
	u16 vbe_mode;
	u16 vbe_interface_seg;
	u16 vbe_interface_off;
	u16 vbe_interface_len;
	u64 framebuffer_addr;
	u32 framebuffer_pitch;
	u32 framebuffer_width;
	u32 framebuffer_height;
	u8 framebuffer_bpp;
	u8 framebuffer_type;
	u8 color_info[5];
} mb_info;

#define MBI_FLAG_MEM       (1 << 0)
#define MBI_FLAG_CMDLINE   (1 << 2)
#define MBI_FLAG_MODS      (1 << 3)
#define MBI_FLAG_MMAP      (1 << 6)
#define MBI_HAS_MEM(info)  (!!((info)->flags & MBI_FLAG_MEM))
#define MBI_HAS_MODS(info) (!!((info)->flags & MBI_FLAG_MODS))
#define MBI_HAS_MMAP(info) (!!((info)->flags & MBI_FLAG_MMAP))

typedef struct {
	u32 size;
	u64 base_addr;
	u64 length;
	u32 type;
} PACK mb_mmap_entry;

#define MB_MMAP_AVAILABLE (1)
#define MB_MMAP_ACPI      (3)
#define MB_MMAP_RESERVED  (4)
#define MB_MMAP_DEFECTIVE (5)

typedef struct {
	u32 mod_start;
	u32 mod_end;
	u32 string;
	u32 reserved;
} PACK mb_module;

#endif
