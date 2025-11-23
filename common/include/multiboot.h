#include <types.h>

// I have bits 0 & 1 of flags set
typedef struct mb_info {
	U32 flags;
	U32 mem_lower;
	U32 mem_upper;
	U32 boot_device;
	U32 cmdline;
	U32 mods_count;
	U32 mods_addr;
	U32 syms[4];
	U32 mmap_length;
	U32 mmap_addr;
	U32 drives_length;
	U32 drives_addr;
	U32 config_table;
	U32 boot_loader_name;
	U32 apm_table;
	U32 vbe_control_info;
	U32 vbe_mode_info;
	U16 vbe_mode;
	U16 vbe_interface_seg;
	U16 vbe_interface_off;
	U16 vbe_interface_len;
	U64 framebuffer_addr;
	U32 framebuffer_pitch;
	U32 framebuffer_width;
	U32 framebuffer_height;
	U8 framebuffer_bpp;
	U8 framebuffer_type;
	U8 color_info[5];
} mb_info;

#define MBI_FLAG_MEM       (1 << 0)
#define MBI_FLAG_CMDLINE   (1 << 2)
#define MBI_FLAG_MODS      (1 << 3)
#define MBI_FLAG_MMAP      (1 << 6)

#define MBI_HAS_MEM(info)  (!!((info)->flags & MBI_FLAG_MEM))
#define MBI_HAS_MODS(info) (!!((info)->flags & MBI_FLAG_MODS))
#define MBI_HAS_MMAP(info) (!!((info)->flags & MBI_FLAG_MMAP))
