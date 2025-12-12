#include <types.h>
#include <params.h>
#include <memlayout.h>
#include <kutils.h>

// MODULES
#include <pmm.h>
#include <tty.h>
#include <io.h>
#include <vga.h>
#include <vmm.h>

struct tty global_tty;

void kernel_main(mb_info* mbi_paddr, uintptr_t loader_end)
{
	// init_logging(&global_tty);

	init_tty(TTY_KIND_VGA, &global_tty);
	init_io(&global_tty);
	init_pmm(mbi_paddr, loader_end);
	init_vmm();
	halt_forever();
	return;
}
