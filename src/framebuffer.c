#include "framebuffer.h"

static unsigned char *fb_ptr = 0;

unsigned int fb_width = 320;
unsigned int fb_height = 200;
unsigned int fb_pitch = 320;
unsigned char fb_bpp = 8;

void init_framebuffer(unsigned int addr) {
    unsigned int total_size = *(unsigned int *)addr;
    unsigned char *tag_ptr = (unsigned char *)(addr + 8);
    unsigned char *end = (unsigned char *)addr + total_size;

    while ((unsigned int)(tag_ptr + 8) < (unsigned int)end) {
        struct multiboot_tag_framebuffer *tag = (struct multiboot_tag_framebuffer *)tag_ptr;
        if (tag->type == 0)
            break;
        if (tag->type == 8) {
            fb_ptr = (unsigned char *)(unsigned int)tag->address;
            fb_width = tag->width;
            fb_height = tag->height;
            fb_pitch = tag->pitch;
            fb_bpp = tag->bpp;
            break;
        }
        tag_ptr += ((tag->size + 7) & ~7);
    }

    fb_clear(0);
}

void fb_putpixel(int x, int y, unsigned char color) {
    if (fb_ptr == 0)
        return;
    if (x < 0 || (unsigned int)x >= fb_width || y < 0 || (unsigned int)y >= fb_height)
        return;
    if (fb_bpp == 32) {
        static const unsigned int vga_palette[16] = {
            0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
            0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
            0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
            0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF,
        };
        unsigned int offset = y * fb_pitch + x * 4;
        *(unsigned int *)(fb_ptr + offset) = vga_palette[color & 0x0F];
    } else {
        fb_ptr[y * fb_pitch + x] = color;
    }
}

void fb_clear(unsigned char color) {
    if (fb_ptr == 0)
        return;
    unsigned int fb_size = fb_pitch * fb_height;
    for (unsigned int i = 0; i < fb_size; i++) {
        fb_ptr[i] = color;
    }
}
