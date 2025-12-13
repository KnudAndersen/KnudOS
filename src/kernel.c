#include <types.h>
#include <params.h>
#include <memlayout.h>
#include <asm.h>

// MODULES
#include <pmm.h>
#include <tty.h>
#include <io.h>
#include <vga.h>
#include <vmm.h>
#include <cpuid.h>

static struct tty global_tty;
static vm_address_space addr_space;

void kernel_main(mb_info* info)
{
	check_cpuid_features();

	init_pmm(info);

	init_tty(TTY_KIND_VGA, &global_tty);
	init_io(&global_tty);
	// init_vmm(info, VMM_KERNEL, &addr_space);

	printf("hello!\n");
	halt_forever();
	return;
}
