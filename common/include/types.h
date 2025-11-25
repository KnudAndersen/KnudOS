#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <params.h>
// level 0 types
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;
typedef uintptr_t UPTR;

typedef uint64_t size_t;
// level 1 types

typedef struct page_t {
	U8 bytes[PAGE_SIZE];
} page_t;

typedef struct page_table_t {
	U64 entries[PTE_NUM];
} page_table_t;

typedef struct mb_mod {
	U32 mod_start;
	U32 mod_end;
	U32 str;
	U32 reserved;
} mb_mod;

#endif
