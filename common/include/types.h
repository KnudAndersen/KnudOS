#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <params.h>
// level 0 types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint64_t size_t;

// level 1 types

typedef struct page_t {
	u8 bytes[PAGE_SIZE];
} page_t;

typedef struct page_table_t {
	u64 entries[PTE_NUM];
} page_table_t;

typedef struct mb_mod {
	u32 mod_start;
	u32 mod_end;
	u32 str;
	u32 reserved;
} mb_mod;

#endif
