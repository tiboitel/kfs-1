#include "kernel/terminal.h"
#include "kernel/libc.h"
#include "kernel/io.h"
#include "kernel/vga.h"

// Current terminal color (global state)
uint8_t current_color = 0x0F; // White on black by default
// VGA buffer and cursor position
uint16_t *vga_buffer = (uint16_t *)0xB8000;

//---------------------------------------
// vga_color_entry: create a VGA color byte from foreground and background colors.
uint8_t	vga_color_entry(enum vga_color fg, enum vga_color bg)
{
	return fg | (bg << 4);
}

// color_get_current: get the current terminal color.
uint8_t	color_get_current(void)
{
	return current_color;
}

// color_set_current: set the current terminal color.
void	color_set_current(uint8_t color)
{
	current_color = color;
}

// terminal_setcolor: change the current terminal color (alias for compatibility).
void	terminal_setcolor(uint8_t color)
{
	color_set_current(color);
}

// terminal_repaint: change the color of ALL characters on screen WITHOUT erasing content.
// Keeps existing characters, only updates their color attributes.
void	terminal_repaint(uint8_t color)
{
	extern t_screen	screens[MAX_SCREENS];
	int 		i;
	size_t		index;

	color_set_current(color);

	// Update VGA buffer (current screen)
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
	
	// Update all screen buffers
	for (i = 0; i < MAX_SCREENS; i++)
	{
		screens[i].color = color;
		for (int j = 0; j < VGA_ROWS * VGA_COLS; j++)
		{
			char ch = screens[i].buffer[j] & 0xFF;
			screens[i].buffer[j] = ((uint16_t)color << 8) | ch;
		}
	}
}

// handle set color CLI
void	handle_setcolor(char *color)
{
	if (ft_strncmp(color, "red", 3) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_RED));
	}
	else if (ft_strncmp(color, "green", 5) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
	}
	else if (ft_strncmp(color, "blue", 4) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLUE));
	}
	else if (ft_strncmp(color, "white", 5) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
	}
	else if (ft_strncmp(color, "black", 5) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	}
	else if (ft_strncmp(color, "brown", 5) == 0) {
		terminal_repaint(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BROWN));
	}
	else {
		ft_printf("Unknown color: %s\n", color);
		return ;
	}
}
