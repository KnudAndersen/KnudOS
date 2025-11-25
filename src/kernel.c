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

	init_pmm(mbi_paddr, (void*)KSTACK_HI_NTH(0));

	for (U32 i = 0; i < VGA_COLS * VGA_ROWS; i++) {
		PrintF("x");
	}
	for (U32 i = 0; i < 5; i++) {
		PrintF("X");
	}
	//init_pmm(mbi_paddr, (void*)KSTACK_HI_NTH(0));

	asm volatile("hlt");

	return;
}
