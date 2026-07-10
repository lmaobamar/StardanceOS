#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

extern unsigned int fb_width;
extern unsigned int fb_height;
extern unsigned int fb_pitch;
extern unsigned char fb_bpp;

#define SCREEN_WIDTH  fb_width
#define SCREEN_HEIGHT fb_height

struct multiboot_tag_framebuffer {
    unsigned int type;
    unsigned int size;
    unsigned long long address;
    unsigned int pitch;
    unsigned int width;
    unsigned int height;
    unsigned char bpp;
    unsigned char type_id;
    unsigned short reserved;
} __attribute__((packed));

void init_framebuffer(unsigned int addr);
void fb_putpixel(int x, int y, unsigned char color);
void fb_clear(unsigned char color);

#endif