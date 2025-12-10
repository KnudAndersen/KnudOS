#include <types.h>
#include <params.h>
#include <memlayout.h>

#include <pmm.h>
#include <tty.h>
#include <io.h>
#include <vga.h>

struct tty global_tty;

void kernel_main(mb_info* mbi_paddr)
{
	init_tty(TTY_KIND_VGA, &global_tty);
	printf("hello!, yo %Xthis is somet text", 0x1234);
	// init_pmm(mbi_paddr, (void*)KSTACK_HI_NTH(0));
	asm volatile("hlt");

	return;
}
