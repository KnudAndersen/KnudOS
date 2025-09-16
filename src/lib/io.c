#ifndef IO_C
#define IO_C

#include "./include/io.h"

extern tty_t tty0;

inline void tprintc(tty_t* term, char c) {
    if (term == 0) {
        while (1) {
            asm volatile("hlt");
        }
    }
    if (c == '\t') {
        for (uint8_t i = 0; i < TTY_TAB_WIDTH; i++)
            kprintc(' ');
        return;
    }
    if (term->col_i == term->cols - 1 || c == '\n') {
        term->col_i = 1;
        term->row_i++;
        if (term->row_i == term->rows - 1) {
            tty_scroll(term, TTY_SCROLL_UP, 1);
            term->row_i = term->rows - 2;
        }
    }
    if (c != '\n' && c != '\t') {
        term->data[term->row_i][term->col_i++] =
            vga_get_attr(c, term->default_fg, term->default_bg);
        vga_tty_render(&tty0);
    }
}

void kprintc(char c) {
    tprintc(&tty0, c);
}
void kprints(char* str) {
    uint32_t i = 0;
    while (str[i]) {
        kprintc(str[i]);
        i++;
    }
}
void tprints(tty_t* term, char* str) {
    uint32_t i = 0;
    while (str[i]) {
        tprintc(term, str[i]);
        i++;
    }
}
static inline void tprint_dec(tty_t* term, uint64_t val, uint8_t shift, uint8_t mod, uint8_t size) {
    if (val == 0) {
        kprints("0");
        return;
    }
    char out[2 * size + 1];
    out[2 * size] = '\0';
    int32_t i = 2 * size - 1;
    while (val) {
        out[i] = HEXDIGITS[val % mod];
        i--;
        val /= shift;
    }
    tprints(term, out + (i + 1));
}

static inline void tprint_pow2(tty_t* term, uint64_t val, uint8_t shift, uint8_t mod,
                               uint8_t size) {
    if (val == 0) {
        kprints("0");
        return;
    }
    char out[2 * size + 1];
    out[2 * size] = '\0';
    int32_t i = 2 * size - 1;
    while (val) {
        out[i] = HEXDIGITS[val & mod];
        i--;
        val >>= shift;
    }
    tprints(term, out + (i + 1));
}

void kprintb(uint8_t val) {
    tprint_dec(&tty0, val, DECMOD, DECMOD, sizeof(val));
}

void kprintd(uint32_t val) {
    tprint_dec(&tty0, val, DECMOD, DECMOD, sizeof(val));
}

void kprintl(uint64_t val) {
    tprint_dec(&tty0, val, DECMOD, DECMOD, sizeof(val));
}

void kprintbx(uint8_t val) {
    tprint_pow2(&tty0, val, HEXSHIFT, HEXMOD, sizeof(val));
}

void kprintdx(uint32_t val) {
    tprint_pow2(&tty0, val, HEXSHIFT, HEXMOD, sizeof(val));
}

void kprintlx(uint64_t val) {
    tprint_pow2(&tty0, val, HEXSHIFT, HEXMOD, sizeof(val));
}

void tprintb(tty_t* term, uint8_t val) {
    tprint_dec(term, val, DECMOD, DECMOD, sizeof(val));
}

void tprintd(tty_t* term, uint32_t val) {
    tprint_dec(term, val, DECMOD, DECMOD, sizeof(val));
}

void tprintl(tty_t* term, uint64_t val) {
    tprint_dec(term, val, DECMOD, DECMOD, sizeof(val));
}

void tprintbx(tty_t* term, uint8_t val) {
    tprint_pow2(term, val, HEXSHIFT, HEXMOD, sizeof(val));
}

void tprintdx(tty_t* term, uint32_t val) {
    tprint_pow2(term, val, HEXSHIFT, HEXMOD, sizeof(val));
}

void tprintlx(tty_t* term, uint64_t val) {
    tprint_pow2(term, val, HEXSHIFT, HEXMOD, sizeof(val));
}

#endif
