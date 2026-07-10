#include "keyboard.h"
#include "console.h"
#include "io.h"

static int shift_pressed = 0;
static const char kbd_us_map[128] = {
    0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' ', 0};
static const char kbd_us_shift_map[128] = {
    0,   27,  '!',  '@',  '#',  '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ':',  '"',  '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  '<',  '>',  '?', 0,   '*', 0,   ' ', 0};

void keyboard_handler(void) {
  unsigned char status = inb(KEYBOARD_STATUS_PORT);
  if (status & 0x01) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode & 0x80) {
      unsigned char released_scancode = scancode & ~0x80;
      if (released_scancode == 0x2A || released_scancode == 0x36) {
        shift_pressed = 0;
      }
      return;
    } else {
      if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
      }
      if (scancode >= 128) {
        return;
      }
      char ascii =
          shift_pressed ? kbd_us_shift_map[scancode] : kbd_us_map[scancode];
      if (ascii != 0) {
        console_putchar(ascii);
      }
    }
  }
}