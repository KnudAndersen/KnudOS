#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/kalloc.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/memory_manager.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

void kernel_main(uint64_t __multiboot, uint64_t __reserve,
		 uint64_t __reserve_off, uint64_t __boot_pml4)
{
	Init_MemoryManager((multiboot_info *)__multiboot);

	Init_Kernel_Heap((uint64_t *)__boot_pml4);

	tty_t *tty_0 = (tty_t *)kmalloc(sizeof(tty_t));

	Init_TTY(tty_0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG,
		 TTY_DEFAULT_BG, 0, 0);

	vga_tty_render(tty_0);

	__KERNEL_PANIC__;
	return;
}
#endif
