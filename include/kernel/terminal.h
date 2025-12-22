#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_COLS 80
#define VGA_ROWS 25

void terminal_init(void);
void terminal_putc(char c);
void terminal_print(const char *str);

#endif
