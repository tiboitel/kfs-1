#include "kernel/terminal.h"

// External reference to VGA buffer
extern uint16_t *vga_buffer;

// Current terminal color (global state)
static uint8_t			current_color = 0x0F; // White on black by default

// vga_color_entry: create a VGA color byte from foreground and background colors.
uint8_t				vga_color_entry(enum vga_color fg, enum vga_color bg)
{
	return fg | (bg << 4);
}

// color_get_current: get the current terminal color.
uint8_t				color_get_current(void)
{
	return current_color;
}

// color_set_current: set the current terminal color.
void				color_set_current(uint8_t color)
{
	current_color = color;
}

// terminal_setcolor: change the current terminal color (alias for compatibility).
void				terminal_setcolor(uint8_t color)
{
	color_set_current(color);
}

// terminal_repaint: change the color of ALL characters on screen WITHOUT erasing content.
// Keeps existing characters, only updates their color attributes.
void				terminal_repaint(uint8_t color)
{
	size_t			index;
	
	for (int col = 0; col < VGA_COLS; col++)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			index = (VGA_COLS * row) + col;
			// Extract character (low byte), replace color (high byte)
			char ch = vga_buffer[index] & 0xFF;
			vga_buffer[index] = ((uint16_t)color << 8) | ch;
		}
	}
}

