
#include <pmm.h>
#include <io.h>
#include <memlayout.h>
#include <asm.h>
#include <string.h>

#define PMM_ROWS                ((MAX_PHYS_RAM) / (PAGE_SIZE * PMM_COLS))
#define PMM_COLS                (8ull * sizeof(u64))
#define PMM_BITMAP_MAX_PADDR    (PMM_ROWS * PMM_COLS)

#define PMM_MAX_AVAILABLE_ZONES (3ull)

struct bitmap {
	u64 data[PMM_ROWS];
	uintptr_t start;
	uintptr_t end;
};

struct bitmap_container {
	struct bitmap maps[PMM_MAX_AVAILABLE_ZONES];
};

struct bitmap_container bitmaps;

static void bitmap_set_used(struct bitmap* bitmap, uintptr_t page);
static void bitmap_set_unused(struct bitmap* bitmap, uintptr_t page);
static int bitmap_find_free_page(struct bitmap* bitmap, size_t* ret_row, size_t* ret_col);
static int bitmap_is_free(struct bitmap* bitmap, size_t row, size_t col);

static int pmm_reserve_range(uintptr_t base, size_t size);
static int pmm_free_range(uintptr_t base, size_t size);

// TODO: use hinting to optimize this
uintptr_t pmm_alloc()
{
	int found = 0;
	uintptr_t result = 0;
	size_t row, col;
	struct bitmap* bitmap;
	size_t bitmap_index;
	for (bitmap_index = 0; bitmap_index < PMM_MAX_AVAILABLE_ZONES; bitmap_index++) {
		bitmap = &bitmaps.maps[bitmap_index];
		if (bitmap_find_free_page(bitmap, &row, &col)) {
			found = 1;
			result = bitmap->start + PAGE_SIZE * (row * PMM_COLS + col);
			break;
		}
	}

	// out of physical memory
	if (!found)
		halt_forever();

	// mark
	bitmap_set_used(bitmap, result);

	return result;
}

int pmm_free(uintptr_t page)
{
	struct bitmap* bitmap;
	for (size_t bitmap_index = 0; bitmap_index < PMM_MAX_AVAILABLE_ZONES; bitmap_index++) {
		bitmap = &bitmaps.maps[bitmap_index];
		if (bitmap->start <= page && page < bitmap->end) {
			bitmap_set_unused(bitmap, page);
			return 1;
		}
	}
	return 0;
}

static int bitmap_is_free(struct bitmap* bitmap, size_t row, size_t col)
{
	if (row * PMM_COLS + col >= MAX_PHYS_RAM / PAGE_SIZE)
		return 0;

	u64 bit = 1ull << (PMM_COLS - 1 - col);
	return !(bitmap->data[row] & bit);
}

static int bitmap_find_free_page(struct bitmap* bitmap, size_t* ret_row, size_t* ret_col)
{
	for (size_t row = 0; row < PMM_ROWS; row++) {
		for (size_t col = 0; col < PMM_COLS; col++) {
			if (bitmap_is_free(bitmap, row, col)) {
				*ret_row = row;
				*ret_col = col;
				return 1;
			}
		}
	}
	return 0;
}

static void bitmap_set_used(struct bitmap* bitmap, uintptr_t page)
{
	if (bitmap->start > page || page > bitmap->end || page - bitmap->start >= MAX_PHYS_RAM)
		halt_forever();

	u64 page_index = (page - bitmap->start) / PAGE_SIZE;
	u64 col = page_index % PMM_COLS;

	u64 bit = 1ull << (PMM_COLS - 1 - col);
	bitmap->data[page_index / PMM_COLS] |= bit;
}

static void bitmap_set_unused(struct bitmap* bitmap, uintptr_t page)
{
	if (bitmap->start > page || page > bitmap->end || page - bitmap->start >= MAX_PHYS_RAM)
		halt_forever();

	u64 page_index = (page - bitmap->start) / PAGE_SIZE;
	u64 col = page_index % PMM_COLS;

	u64 bit = 1ull << (PMM_COLS - 1 - col);
	bitmap->data[page_index / PMM_COLS] &= ~bit;
}

static int bitmap_reserve_range(struct bitmap* bitmap, uintptr_t base, size_t size)
{
	if (bitmap->start <= base && base < bitmap->end) {
		for (size_t off = 0; off < size; off += PAGE_SIZE)
			bitmap_set_used(bitmap, base + off);

		return 1;
	}
	return 0;
}

static int pmm_reserve_range(uintptr_t base, size_t size)
{
	base &= ~(PAGE_SIZE - 1);
	struct bitmap* bitmap;
	for (size_t bitmap_index = 0; bitmap_index < PMM_MAX_AVAILABLE_ZONES; bitmap_index++) {
		bitmap = &bitmaps.maps[bitmap_index];
		if (bitmap_reserve_range(bitmap, base, size))
			return 1;
	}
	return 0;
}

static int bitmap_free_range(struct bitmap* bitmap, uintptr_t base, size_t size)
{
	if (bitmap->start <= base && base < bitmap->end) {
		for (size_t off = 0; off < size; off += PAGE_SIZE)
			bitmap_set_unused(bitmap, base + off);

		return 1;
	}
	return 0;
}

static int pmm_free_range(uintptr_t base, size_t size)
{
	base &= ~(PAGE_SIZE - 1);
	size = CEIL_DIV(size, PAGE_SIZE) * PAGE_SIZE;

	struct bitmap* bitmap;

	for (size_t bitmap_index = 0; bitmap_index < PMM_MAX_AVAILABLE_ZONES; bitmap_index++) {
		bitmap = &bitmaps.maps[bitmap_index];

		if (bitmap_free_range(bitmap, base, size))
			return 1;
	}

	return 0;
}

void initialize_container(struct bitmap_container* bitmaps)
{
	memset(bitmaps, 0, sizeof(struct bitmap_container));
	struct bitmap* bitmap;
	size_t bitmap_index;

	for (bitmap_index = 0; bitmap_index < PMM_MAX_AVAILABLE_ZONES; bitmap_index++) {
		bitmap = &bitmaps->maps[bitmap_index];
		memset(bitmap->data, 0xFF, sizeof(bitmap->data));
	}
}

void reserve_available_ram(struct bitmap_container* bitmaps, mb_info* info)
{
	uintptr_t zone_index = 0;
	struct bitmap* bitmap;

	char* ptr = (char*)(uintptr_t)info->mmap_addr;
	char* end = ptr + info->mmap_length;

	while (ptr < end) {
		mb_mmap_entry* entry = (mb_mmap_entry*)ptr;
		if (entry->type == MB_MMAP_AVAILABLE) {
			if (zone_index >= PMM_MAX_AVAILABLE_ZONES)
				halt_forever();

			bitmap = &bitmaps->maps[zone_index];
			bitmap->start = entry->base_addr;
			bitmap->end = entry->base_addr + entry->length;
			if (!bitmap_free_range(bitmap, entry->base_addr, entry->length))
				halt_forever();

			zone_index++;
		}
		ptr += entry->size + sizeof(entry->size);
	}
}
void init_pmm(mb_info* info)
{
	initialize_container(&bitmaps);
	reserve_available_ram(&bitmaps, info);
}
