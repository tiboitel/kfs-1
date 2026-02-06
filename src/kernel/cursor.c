#include "kernel/terminal.h"

// I/O port operations for cursor control
static inline void	outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t	inb(uint16_t port)
{
	uint8_t			ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

// terminal_enable_cursor: enable and configure the VGA cursor.
void	terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// terminal_disable_cursor: disable the VGA cursor.
void	terminal_disable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

// cursor_update: update cursor position at specific row and column.
void	terminal_cursor_update(size_t row, size_t col)
{
	uint16_t		pos = row * VGA_COLS + col;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

