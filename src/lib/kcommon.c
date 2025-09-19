#ifndef KCOMMON_C
#define KCOMMON_C

#include "./include/kcommon.h"

const char boot_module_magic[] = "KnudKernel";
const char HEXDIGITS[] = "0123456789ABCDEF";

int32_t StrCmp(const char* s_1, const char* s_2) {
    int32_t cmp;
    while (*s_1 && *s_2) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return *s_1 - *s_2;
}

int32_t StrNCmp(const char* s_1, const char* s_2, uint32_t n) {
    if (n == 0)
        return StrCmp(s_1, s_2);
    int32_t cmp;
    for (uint32_t i = 0; i < n; i++) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return cmp;
}

multiboot_mod* get_kernel(multiboot_info* m_info) {
    // TODO: cross compile clib
    if (m_info->mods_count == 0) {
        return NULL;
    }
    multiboot_mod* mod_iter = (multiboot_mod*)(uintptr_t)m_info->mods_addr;
    int32_t checksum;
    for (uint32_t i = 0; i < m_info->mods_count; i++) {
        char* mod_str = (char*)(uintptr_t)mod_iter->string;
        checksum = StrNCmp(boot_module_magic, mod_str, sizeof(boot_module_magic) - 1);
        if (checksum == 0) {
            return mod_iter;
        }
        mod_iter++;
    }
    return (checksum ? mod_iter : NULL);
}

uint32_t pop_reg32(uint8_t reg) {
    uint32_t ret;
    switch (reg) {
        case ESI:
            asm volatile("mov %0, esi"
                         :
                         : "m"(ret)
                         : "esi");
            break;
        case EDI:
            asm volatile("mov %0, edi"
                         :
                         : "m"(ret)
                         : "edi");
            break;

        case EBX:
            asm volatile("mov %0, ebx"
                         :
                         : "m"(ret)
                         : "ebx");
            break;
        default:
            break;
    }
    return ret;
}
#endif
