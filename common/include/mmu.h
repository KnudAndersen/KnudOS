#ifndef MMU_H
#define MMU_H

#include <types.h>
#include <params.h>

// X86 SEGMENTS
// (figure 3-8)
typedef struct seg_desc {
	u16 lim_15_0;
	u16 base_15_0;
	u8 base_23_16;
	u8 access;
	u8 flag_lim_19_16;
	u8 base_31_24;
} PACK seg_desc;

typedef enum {
	SEG_NULL,
	SEG_KCODE, // kernel
	SEG_KDATA,
	SEG_UCODE, // user
	SEG_UDATA,
	SEG_TSS_BASE,
} x86_segment_def;

// DEFAULT ACCESS/FLAG
#define SA_CODE_DEF (SA_PRESENT | SA_EXEC | SA_READ | SA_CODE)
#define SA_CODE_DEF (SA_PRESENT | SA_EXEC | SA_READ | SA_CODE)
#define SA_DATA_DEF (SA_PRESENT | SA_WRITE | SA_DATA)
#define SF_DEF      (SF_LONGMODE | SF_GRAN)

#define S_LIM       (0xFFFFF)
#define S_BASE      (0xFFFFFFFF)

// X86 SGMT DESCRIPTOR ACCESS BYTE
// SEE FIGURE 3-8 INTEL SDM VOL 3A
#define SA_PRESENT  (1 << 7) // present
#define SA_USER     (3 << 5) // user
#define SA_CODE     (1 << 4) // code or data
#define SA_DATA     (1 << 4) // code or data
#define SA_EXEC     (1 << 3) // exec
#define SA_DOWN     (1 << 2) // (data only)
#define SA_CONF     (1 << 2) // (code only)
#define SA_WRITE    (1 << 1) // (data only)
#define SA_READ     (1 << 1) // (code only)

#define SA_DATA_DEF (SA_PRESENT | SA_WRITE | SA_DATA)
#define SF_DEF      (SF_LONGMODE | SF_GRAN)

#define S_LIM       (0xFFFFF)
#define S_BASE      (0xFFFFFFFF)

// X86 SGMT DESCRIPTOR ACCESS BYTE
// SEE FIGURE 3-8 INTEL SDM VOL 3A
#define SA_PRESENT (1 << 7) // present
#define SA_USER    (3 << 5) // user
#define SA_CODE    (1 << 4) // code or data
#define SA_DATA    (1 << 4) // code or data
#define SA_EXEC    (1 << 3) // exec
#define SA_DOWN    (1 << 2) // (data only)
#define SA_CONF    (1 << 2) // (code only)
#define SA_WRITE   (1 << 1) // (data only)
#define SA_READ    (1 << 1) // (code only)

// X86 SGMT DESCRIPTOR FLAG NIBBLE
#define SF_LONGMODE (1 << 1)
#define SF_GRAN     (1 << 3)

// X86 64-BIT TSS DESCRIPTOR
// (figure 7-4)
typedef struct tss64_desc {
	u16 lim_15_0;
	u16 base_15_0;
	u8 base_23_16;
	u8 access;
	u8 flag_lim_19_16;
	u8 base_31_24;
	u32 base_63_32;
	u32 reserved;
} PACK tss64_desc;

// X86 IA-32e SYSTEM ACCESS BYTE
// TYPE
#define TA_TSS64_AVL (0x9)
#define TA_TSS64_BSY (0xB)
#define TA_CGATE64   (0xC)
#define TA_IGATE64   (0xE)
#define TA_TGATE64   (0xF)
// ELSE
#define TA_USER    (3 << 5)
#define TA_PRESENT (1 << 7)

// X86 IA-32e SYSTEM FLAG NIBBLE
#define TF_AVL  (1 << 0)
#define TF_GRAN (1 << 3)

#define TA_DEF  (TA_TSS64_AVL | TA_PRESENT)
#define TF_DEF  (TF_AVL /* | TF_GRAN */)

struct tss {
	u32 reserved1;
	u64 rsp0;
	u64 rsp1;
	u64 rsp2;
	u64 reserved2;
	u64 ist1;
	u64 ist2;
	u64 ist3;
	u64 ist4;
	u64 ist5;
	u64 ist6;
	u64 ist7;
	u64 reserved3;
	u16 reserved4;
	u16 iomap_baseaddr;
} PACK;
// X86 GDT

#define GDT_NUM_DESC (5)
struct gdt {
	seg_desc descriptors[GDT_NUM_DESC];
	tss64_desc tss_descriptor;
} PACK;

#define SEG_SEL_RPL(n) (((n) & 0x3) << 0)
#define SEG_SEL_LDT    (1 << 2)
#define SEG_SEL_IDX(n) ((n) << 3)

struct gdtr {
	u16 limit;
	u32 base;
} PACK;

// X86 PAGING (FOR IA32E)
// section 4.5
// TODO: convert the prefix to specify x86-64
#define X86_PRESENT      (1ull << 0)
#define X86_READ_WRITE   (1ull << 1)
#define X86_USER         (1ull << 2)
#define X86_EXEC_DISABLE (1ull << 63)
#define X86_ADDR_MASK    (0x000FFFFFFFFFF000ULL)
#endif
