#include <stdint.h>
volatile uint16_t* vga_text = (volatile uint16_t*)0xB8000;
#define VGA_BG_MASK   (0x7000)
#define VGA_FG_MASK   (0x0F00)
#define VGA_CHAR_MASK (0x00FF)
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint16_t vga_get_attr(char c, uint8_t fg, uint8_t bg) {
    return ((c << 0) & VGA_CHAR_MASK) | ((fg << 8) & VGA_FG_MASK) | ((bg << 12) & VGA_BG_MASK);
}

void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    for (int i = 0; i < 100; i++) {
        vga_text[i] = vga_get_attr('k', VGA_COLOR_RED, VGA_COLOR_BLACK);
    }
    while (1) {
        asm volatile("hlt");
    }
    return;
}
