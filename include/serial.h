#ifndef SERIAL_H
#define SERIAL_H

#define COM1_PORT 0x3F8

void init_serial(void);
void serial_putchar(char c);
void serial_print(const char *str);

#endif