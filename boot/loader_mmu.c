#ifndef LOADER_MMU_C
#define LOADER_MMU_C

#include <mmu.h>
#include <types.h>
#include <loader_mmu.h>
#include <utils.h>
#include <memlayout.h>
#include <multiboot.h>
#include <asm.h>
#include "/usr/include/elf.h"

#define OPT_ELF_FLAGS  (1 << 0)
#define OPT_X86_FLAGS  (1 << 1)
#define OPT_PHYS_ALLOC (1 << 2)

struct tss tss ALIGN_C(16);
struct gdt gdt ALIGN_C(16);

extern u64 __linker_loader_end;

Elf64_Addr get_kernel_entry(struct mb_info* info)
{
	Elf64_Addr ret;
	mb_mod* iter = MAKE_PTR(info->mods_addr);
	Elf64_Ehdr* header = MAKE_PTR(iter->mod_start);
	ret = header->e_entry;
	return ret;
}

static void gdt_set_tss_entry(u32 lim, u64 base, u8 access, u8 flag)
{
	lim &= 0xFFFFF;
	flag &= 0xF;

	volatile tss64_desc* p = &gdt.tss_descriptor;

	*p = (tss64_desc){
		.base_15_0 = (base >> 0) & 0xFFFF,
		.base_23_16 = (base >> 16) & 0xFF,
		.base_31_24 = (base >> 24) & 0xFF,
		.base_63_32 = (base >> 32) & 0xFFFFFFFF,
		.lim_15_0 = (lim >> 0) & 0xFFFF,
		.flag_lim_19_16 = ((u16)flag << 4) | ((lim >> 16) & 0xF),
		.access = access,
		.reserved = 0,
	};
}

static void gdt_set_entry(u32 num, u32 lim, u32 base, u8 access, u8 flag)
{
	lim &= 0xFFFFF;
	flag &= 0xF;

	volatile seg_desc* p = &gdt.descriptors[num];

	*p = (seg_desc){
		.base_15_0 = (base >> 0) & 0xFFFF,
		.base_23_16 = (base >> 16) & 0xFF,
		.base_31_24 = (base >> 24) & 0xFF,
		.lim_15_0 = (lim >> 0) & 0xFFFF,
		.flag_lim_19_16 = ((lim >> 16) & 0xF) | (flag << 4),
		.access = access,
	};
}

static void* alloc_boot_page()
{
	static page_t boot_pages[BOOT_PAGES_MAX] ALIGN_C(PAGE_SIZE);
	static u64 unused = 0;
	if (unused >= BOOT_PAGES_MAX) {
		while (1) {
			asm volatile("hlt");
		}
		halt_forever();
		return NULL;
	}
	page_t* ret = &boot_pages[unused++];
	loader_memset(ret, 0, sizeof(page_t));
	return ret;
}

static u64 elf_to_x86_flags(u64 flags)
{
	u64 ret = 0;
	if (flags & PF_X) {
		ret &= ~(X86_EXEC_DISABLE);
	}
	if (flags & PF_W || flags & PF_R) {
		ret |= X86_READ_WRITE;
	}
	return ret;
}

static page_table_t* get_next_table(page_table_t* parent, u64 index)
{
	page_table_t* next;
	if (!(parent->entries[index] & X86_PRESENT)) {
		next = alloc_boot_page();
		parent->entries[index] = (u64)next | X86_PRESENT | X86_READ_WRITE;
	} else {
		next = (page_table_t*)(parent->entries[index] & X86_ADDR_MASK);
	}
	return next;
}

static void boot_map_page(u64 virt, u64 phys, u64 flags, u8 option, void* cr3)
{
	flags = (option & OPT_ELF_FLAGS) ? elf_to_x86_flags(flags) :
	        (option & OPT_X86_FLAGS) ? flags :
	                                   0;
	if (flags == 0) {
		halt_forever();
	}
	u64 vaddr = (u64)virt & ~(PAGE_SIZE - 1);
	u64 paddr = (u64)phys & ~(PAGE_SIZE - 1);

	u32 pt_index = (vaddr >> 12) & 0x1FF;
	u32 pdt_index = (vaddr >> 21) & 0x1FF;
	u32 pdpt_index = (vaddr >> 30) & 0x1FF;
	u32 pml4_index = (vaddr >> 39) & 0x1FF;

	page_table_t* pml4 = (page_table_t*)((u64)cr3 & X86_ADDR_MASK);
	page_table_t* pdpt = get_next_table(pml4, pml4_index);
	page_table_t* pdt = get_next_table(pdpt, pdpt_index);
	page_table_t* pt = get_next_table(pdt, pdt_index);

	pt->entries[pt_index] = (u64)paddr | flags | X86_PRESENT;
}

static void map_page_range(u64 virt_base, u64 phys_base, u64 bytes, u64 flags, u8 option, void* cr3)
{
	u64 phys, virt;
	u32 num_pages = CEIL_DIV(bytes, PAGE_SIZE);

	for (u32 pg = 0; pg < num_pages; ++pg) {
		phys = (option & OPT_PHYS_ALLOC) ? (u64)alloc_boot_page() :
		                                   phys_base + pg * PAGE_SIZE;
		virt = virt_base + pg * PAGE_SIZE;
		boot_map_page(virt, phys, flags, option, cr3);
	}
}
static void elf_load_segment(Elf64_Ehdr* header, Elf64_Phdr* segment, void* cr3)
{
	u64 file_start, file_size, mem_size, pg_off;
	u64 phys_addr, virt_addr;

	file_start = (u64)header;
	file_size = CEIL_DIV(segment->p_filesz, PAGE_SIZE) * PAGE_SIZE;
	mem_size = CEIL_DIV(segment->p_memsz, PAGE_SIZE) * PAGE_SIZE;

	for (pg_off = 0; pg_off < file_size; pg_off += PAGE_SIZE) {
		phys_addr = file_start + segment->p_offset + pg_off;
		virt_addr = segment->p_vaddr + pg_off;
		boot_map_page(virt_addr, phys_addr, segment->p_flags, OPT_ELF_FLAGS, cr3);
	}

	if (segment->p_filesz < file_size) {
		u64 pg_end = segment->p_filesz % PAGE_SIZE;
		u64 pg_start = file_size - PAGE_SIZE;
		u8* pg = (u8*)MAKE_PTR(file_start + segment->p_offset + pg_start);
		loader_memset(pg + pg_end, 0, PAGE_SIZE - pg_end);
	}

	for (pg_off = file_size; pg_off < mem_size; pg_off += PAGE_SIZE) {
		phys_addr = (u64)alloc_boot_page();
		virt_addr = segment->p_vaddr + pg_off;
		boot_map_page(virt_addr, phys_addr, segment->p_flags, OPT_ELF_FLAGS, cr3);
	}
}

// TODO: make a common "for each mmap entry" function / macro
void identity_map_available_ram(struct mb_info* info, void* cr3)
{
	//	u64 bytes = (u64)&__linker_loader_end + BOOT_IDENTITY_MAP_EXTRA;
	//	u64 flags = X86_READ_WRITE | X86_PRESENT;
	char* ptr = (char*)(uintptr_t)info->mmap_addr;
	char* end = ptr + info->mmap_length;
	while (ptr < end) {
		struct mb_mmap_entry* entry = (struct mb_mmap_entry*)ptr;

		if (entry->type == MB_MMAP_AVAILABLE) {
			u64 virt_base, phys_base, flags;
			virt_base = phys_base = entry->base_addr;
			flags = X86_PRESENT | X86_READ_WRITE;
			// TODO: consolidate this with the 64-bit page_table.c API's
			map_page_range(virt_base, phys_base, entry->length, flags, OPT_X86_FLAGS,
			               cr3);
		}

		ptr += entry->size + sizeof(entry->size);
	}
}

void load_elf_modules(struct mb_info* info, void* cr3)
{
	mb_mod* iter = MAKE_PTR(info->mods_addr);
	Elf64_Ehdr* header = MAKE_PTR(iter->mod_start);

	if (header->e_phentsize != sizeof(Elf64_Phdr))
		halt_forever();

	Elf64_Half seg_num = header->e_phnum;
	Elf64_Phdr* segment = MAKE_PTR(iter->mod_start + header->e_phoff);

	for (u32 seg = 0; seg < seg_num; seg++) {
		if (segment->p_type == PT_LOAD)
			elf_load_segment(header, segment, cr3);
		else
			halt_forever();
		segment++;
	}
}

void init_kernel(struct mb_info* info, void** cr3)
{
	// TODO check if valid elf

	*cr3 = alloc_boot_page();
	if (*cr3 == NULL)
		halt_forever();

	identity_map_available_ram(info, *cr3);

	// map init CPU's stack
	map_page_range(KSTACK_NTH_LO(0), 0, KSTACK_SIZE, X86_READ_WRITE | X86_PRESENT,
	               OPT_X86_FLAGS | OPT_PHYS_ALLOC, *cr3);

	if (info->mods_count != 1)
		halt_forever();

	load_elf_modules(info, *cr3);
}

void init_gdt()
{
	// set up GDT for IA-32E
	loader_memset(&gdt, 0, sizeof(gdt));
	gdt_set_entry(SEG_NULL, 0, 0, 0, 0);
	gdt_set_entry(SEG_KCODE, S_LIM, S_BASE, SA_CODE_DEF, SF_DEF);
	gdt_set_entry(SEG_KDATA, S_LIM, S_BASE, SA_DATA_DEF, SF_DEF);
	gdt_set_entry(SEG_UCODE, S_LIM, S_BASE, SA_CODE_DEF | SA_USER, SF_DEF);
	gdt_set_entry(SEG_UDATA, S_LIM, S_BASE, SA_DATA_DEF | SA_USER, SF_DEF);
	gdt_set_tss_entry(sizeof(struct tss) - 1, (u64)&tss, TA_DEF, 0);
}

void init_tss(void* stack_top)
{
	// TODO
	// set up TSS for TA-32E
	loader_memset(&tss, 0, sizeof(tss));
	tss.rsp0 = (u64)stack_top;
}

/*/  
 *  INITIALIZE REGISTERS FOR IA-32E (X86 LONG MODE).
*/
void init_ia32e_registers(void* cr3)
{
	// TODO: check if CPU supports 64 bits
	// not that urgent, as the kernel will not work anyway in leg/compat mode

	u64 tmp;
	u64 pdbr_addr = (u64)cr3;

	struct gdtr reg;
	u16 tss_selector = SEG_TSS_BASE << 3;

	load_gdt_register(&reg, &gdt);

	load_task_register(tss_selector);

	read_ctrl_reg(CR0, &tmp);
	tmp &= ~(CR0_PG);
	write_ctrl_reg(CR0, tmp);
	read_ctrl_reg(CR4, &tmp);
	tmp |= CR4_PAE;
	write_ctrl_reg(CR4, tmp);

	read_ctrl_reg(CR3, &tmp);
	tmp |= (pdbr_addr & ~(0xFFF));
	write_ctrl_reg(CR3, tmp);

	read_msr_register(MSR_EFER, &tmp);
	tmp |= EFER_LME;
	write_msr_register(MSR_EFER, tmp);

	read_ctrl_reg(CR0, &tmp);
	tmp |= CR0_PG;
	write_ctrl_reg(CR0, tmp);
}

void init_jump_pointer(struct far_ptr* ret_ptr)
{
	ret_ptr->sel = SEG_KCODE * 0x8;
	ret_ptr->location = (u32)KERNEL_ENTRY_STUB_NC;
}

#endif
