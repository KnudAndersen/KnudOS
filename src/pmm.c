
#include <pmm.h>
#include <io.h>
#include <memlayout.h>
#include <asm.h>

#define PMM_MARK_USED (0)
#define PMM_MARK_FREE (1)

#define PMM_ROWS      (MAX_PHYS_RAM / (PAGE_SIZE * PMM_COLS))
#define PMM_COLS      (8ull * sizeof(u64))

u64 pmm_bitmap[PMM_ROWS] = { 0 };

static void pmm_set_used(uintptr_t page);
static void pmm_set_unused(uintptr_t page);
static void pmm_free_range(uintptr_t base, size_t size);
static void pmm_free_range(uintptr_t base, size_t size);
static int pmm_is_free(size_t row, size_t col);

// DESC: allocates a page of physical memory
uintptr_t pmm_alloc()
{
	// QUESTION: would it unironicaly be cleaner to use goto here
	size_t i, j;
	uintptr_t found = 0;
	for (j = 0; j < PMM_ROWS; j++) {
		for (i = 0; i < PMM_COLS; i++) {
			if (pmm_is_free(j, i)) {
				found = PAGE_SIZE * (j * PMM_COLS + i);
				break;
			}
		}
		if (found)
			break;
	}

	// out of physical memory
	if (!found) {
		while (1) {
			asm volatile("hlt");
		}
	}

	pmm_set_used(found);
	return found;
}

void pmm_free(uintptr_t page)
{
	pmm_set_unused(page);
}

static int pmm_is_free(size_t row, size_t col)
{
	if (row * PMM_COLS + col >= MAX_PHYS_RAM / PAGE_SIZE)
		return 0;

	u64 bit = 1ull << (PMM_COLS - 1 - col);
	return !(pmm_bitmap[row] & bit);
}

static void pmm_set_used(uintptr_t page)
{
	if (page >= MAX_PHYS_RAM)
		return;

	u64 page_index = page / PAGE_SIZE;
	u64 col = page_index % PMM_COLS;

	u64 bit = 1ull << (PMM_COLS - 1 - col);
	pmm_bitmap[page_index / PMM_COLS] |= bit;
}

static void pmm_set_unused(uintptr_t page)
{
	if (page >= MAX_PHYS_RAM)
		return;
	u64 page_index = page / PAGE_SIZE;
	u64 col = page_index % PMM_COLS;
	if (page_index / PMM_COLS >= PMM_ROWS) {
		size_t left = page_index / PMM_COLS;
		size_t right = PMM_ROWS;
		while (1)
			asm volatile("hlt");
	}
	u64 bit = 1ull << (PMM_COLS - 1 - col);
	pmm_bitmap[page_index / PMM_COLS] &= ~bit;
}

static void pmm_reserve_range(uintptr_t base, size_t size)
{
	base &= ~(PAGE_SIZE - 1);
	size = CEIL_DIV(size, PAGE_SIZE) * PAGE_SIZE;

	for (size_t off = 0; off < size; off += PAGE_SIZE)
		pmm_set_used(base + off);
}

static void pmm_free_range(uintptr_t base, size_t size)
{
	base &= ~(PAGE_SIZE - 1);
	size = CEIL_DIV(size, PAGE_SIZE) * PAGE_SIZE;

	for (size_t off = 0; off < size; off += PAGE_SIZE)
		pmm_set_unused(base + off);
}

static void free_available_ram(mb_info* info)
{
	char* ptr = (char*)(uintptr_t)info->mmap_addr;
	char* end = ptr + info->mmap_length;

	while (ptr < end) {
		mb_mmap_entry* entry = (mb_mmap_entry*)ptr;

		if (entry->type == MB_MMAP_AVAILABLE)
			pmm_free_range(entry->base_addr, entry->length);

		ptr += entry->size + sizeof(entry->size);
	}
}

static void reserve_modules(mb_info* info)
{
	if (!info->mods_count)
		return;

	mb_module* modules = (void*)(uintptr_t)info->mods_addr;

	for (unsigned int i = 0; i < info->mods_count; i++)
		pmm_reserve_range(modules[i].mod_start, modules[i].mod_end - modules[i].mod_start);
}

static void reserve_loader(uintptr_t loader_end)
{
	uintptr_t start = EXTMEM;
	// size_t size = 0;
	size_t size = (size_t)loader_end - (size_t)start;
	pmm_reserve_range(start, size);
}

/*
 * INITIALIZES PMM.
 * Reserves the physical memory of:
 * [X] multiboot "reserved" memory
 * [X] 32-bit loader (contains page tables, first kstack)
 * [X] all grub modules
 */

void init_pmm(mb_info* info, uintptr_t loader_end)
{
	pmm_reserve_range(0, MAX_PHYS_RAM);
	free_available_ram(info);
	pmm_reserve_range(0, EXTMEM);
	reserve_loader(loader_end);
	reserve_modules(info);
}
