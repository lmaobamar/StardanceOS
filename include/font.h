#ifndef FONT_H
#define FONT_H

#define FONTDATAMAX 4096

#define VGA8x16_IDX  0

struct font_desc {
    int idx;
    const char *name;
    int width;
    int height;
    int charcount;
    const unsigned char *data;
    int pref;
};

extern const struct font_desc font_vga_8x16;

#endif
