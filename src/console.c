#include "console.h"
#include "font.h"
#include "framebuffer.h"
#include "serial.h"

static int text_col = 0;
static int text_row = 0;
static unsigned char text_color = 15;

void init_console(void) {
  text_col = 0;
  text_row = 0;
  text_color = 15;
  console_clear();
}

void console_set_color(unsigned char color) { text_color = color; }

void console_get_position(int *col, int *row) {
  if (col)
    *col = text_col;
  if (row)
    *row = text_row;
}

void console_set_position(int col, int row) {
  int max_cols = SCREEN_WIDTH / font_vga_8x16.width;
  int max_rows = SCREEN_HEIGHT / font_vga_8x16.height;

  if (col >= 0 && col < max_cols)
    text_col = col;
  if (row >= 0 && row < max_rows)
    text_row = row;
}

void console_clear(void) {
  fb_clear(0); // * 0 is black
  text_col = 0;
  text_row = 0;
}

void console_putchar(char c) {
  serial_putchar(c);

  unsigned char fg = text_color;
  unsigned char bg = 0;

  int max_cols = SCREEN_WIDTH / font_vga_8x16.width;
  int max_rows = SCREEN_HEIGHT / font_vga_8x16.height;

  if (c == 10) { // * LF (line feed)
    text_col = 0;
    text_row++;
  } else if (c == 13) { // * CR (carriage return)
    text_col = 0;
  } else if (c == 9) { // * TAB
    text_col = (text_col + 4) & ~3;
  } else {
    unsigned int offset = (unsigned char)c * font_vga_8x16.height;
    int px = text_col * font_vga_8x16.width;
    int py = text_row * font_vga_8x16.height;

    for (int row = 0; row < font_vga_8x16.height; row++) {
      unsigned char bitmask = font_vga_8x16.data[offset + row];

      for (int col = 0; col < font_vga_8x16.width; col++) {
        if (bitmask & (0x80 >> col)) {
          fb_putpixel(px + col, py + row, fg);
        } else {
          fb_putpixel(px + col, py + row, bg);
        }
      }
    }

    text_col++;
    if (text_col >= max_cols) {
      text_col = 0;
      text_row++;
    }
  }

  if (text_row >= max_rows) {
    // TODO: implement scroll
    text_row = max_rows - 1;
  }
}

void console_print(const char *str) {
  while (*str) {
    console_putchar(*str);
    str++;
  }
}

static void kprint_int(int value, int base) {
  char buf[32];
  int i = 0;
  unsigned int uvalue = value;

  if (base == 10 && value < 0) {
    console_putchar('-');
    uvalue = -value;
  }

  do {
    unsigned int rem = uvalue % base;
    buf[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
    uvalue /= base;
  } while (uvalue > 0);

  while (i > 0) {
    console_putchar(buf[--i]);
  }
}

void kprintf(const char *fmt, ...) {
  __builtin_va_list args;
  __builtin_va_start(args, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'c': {
        char c = (char)__builtin_va_arg(args, int);
        console_putchar(c);
        break;
      }
      case 's': {
        const char *s = __builtin_va_arg(args, const char *);
        console_print(s);
        break;
      }
      case 'd': {
        int d = __builtin_va_arg(args, int);
        kprint_int(d, 10);
        break;
      }
      case 'x': {
        int x = __builtin_va_arg(args, int);
        kprint_int(x, 16);
        break;
      }
      case '%': {
        console_putchar('%');
        break;
      }
      default: {
        console_putchar('%');
        console_putchar(*fmt);
        break;
      }
      }
    } else {
      console_putchar(*fmt);
    }
    fmt++;
  }
  __builtin_va_end(args);
}