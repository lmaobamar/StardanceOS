#ifndef CONSOLE_H
#define CONSOLE_H

void init_console(void);
void console_putchar(char c);
void console_print(const char *str);
void console_set_color(unsigned char color);

void kprintf(const char *fmt, ...);

void console_clear(void);
void console_get_position(int *col, int *row);
void console_set_position(int col, int row);

#endif