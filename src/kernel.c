#include <types.h>
#include <params.h>
#include <memlayout.h>

#include <pmm.h>
#include <tty.h>
#include <io.h>
#include <vga.h>
#include <asm.h>

struct tty global_tty;

void kernel_main(mb_info* mbi_paddr, uintptr_t loader_end)
{
	init_tty(TTY_KIND_VGA, &global_tty);
	init_io(&global_tty);
	// init_logging(&global_tty);
	init_pmm(mbi_paddr, loader_end);

	while (1)
		asm volatile("hlt");
	return;
}
