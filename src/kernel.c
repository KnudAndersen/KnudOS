#include <types.h>
#include <params.h>
#include <memlayout.h>

#include <pmm.h>
#include <tty.h>
#include <io.h>

void kernel_main(mb_info* mbi_paddr)
{
	struct tty tty;
	init_tty(TTY_KIND_VGA, &tty);

	PrintF(&tty, "hello whats up?");

	//init_pmm(mbi_paddr, (void*)KSTACK_HI_NTH(0));

	asm volatile("hlt");

	return;
}
