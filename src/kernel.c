#include <types.h>
#include <params.h>
#include <memlayout.h>
#include <asm.h>
#include <string.h>

// MODULES
#include <pmm.h>
#include <tty.h>
#include <io.h>
#include <vga.h>
#include <vmm.h>
#include <cpuid.h>

static struct tty global_tty;
static struct vm_address_space addr_space;
struct mb_preserved multiboot;

void preserve_multiboot(struct mb_info* src, struct mb_preserved* dst_container)
{
	struct mb_info* dst = &dst_container->info;
	struct mb_mmap_entry* mmap_dst = dst_container->mmap;
	struct mb_mmap_entry* mmap_src = (void*)(uintptr_t)src->mmap_addr;
	struct mb_module* mod_dst = dst_container->mods;
	struct mb_module* mod_src = (void*)(uintptr_t)src->mods_addr;

	memcpy(dst, src, sizeof(struct mb_info));
	memcpy(mmap_dst, mmap_src, src->mmap_length);
	memcpy(mod_dst, mod_src, src->mods_count * sizeof(struct mb_module));
	// no longer in use
	src->mmap_addr = 0;
	src->mods_addr = 0;
}

void kernel_main(struct mb_info* info_ptr, uintptr_t loader_end)
{
	preserve_multiboot(info_ptr, &multiboot);
	check_cpuid_features();

	// TODO: early logging -> need to make tty/io not depend on pmm
	// Idea: add a "exclude region" feature to the boot identity map removal func
	// 	 not that elegant though... figure this out later
	init_pmm(&multiboot, loader_end);
	init_vmm(&multiboot, VMM_KERNEL, &addr_space);
	init_tty(TTY_KIND_VGA, &global_tty);
	init_io(&global_tty);

	halt_forever();
	return;
}
