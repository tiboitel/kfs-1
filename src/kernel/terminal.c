#include "kernel/terminal.h"

// External functions from cursor.c
extern void			cursor_update(size_t row, size_t col);

// External functions from color.c
extern uint8_t			color_get_current(void);

volatile uint16_t		*vga_buffer = (uint16_t *)0xB8000;

static size_t			term_col = 0;
static size_t			term_row = 0;

// terminal_update_cursor: wrapper to update cursor at current position.
void				terminal_update_cursor(void)
{
	cursor_update(term_row, term_col);
}

// terminal_clear: clear the entire screen with the current color.
void				terminal_clear(void)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	term_col = 0;
	term_row = 0;
	
	for (int col = 0; col < VGA_COLS; col++)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			index = (VGA_COLS * row) + col;	
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
		}
	}
	terminal_update_cursor();
}

// terminal_scroll: scroll the screen up by one line.
void				terminal_scroll(void)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	// Copy each line to the previous line (shift up)
	for (int row = 0; row < VGA_ROWS - 1; row++)
	{
		for (int col = 0; col < VGA_COLS; col++)
		{
			index = (VGA_COLS * row) + col;
			vga_buffer[index] = vga_buffer[index + VGA_COLS];
		}
	}

	// Clear the last line
	for (int col = 0; col < VGA_COLS; col++)
	{
		index = (VGA_COLS * (VGA_ROWS - 1)) + col;
		vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
	}

	// Move cursor to start of last line
	term_row = VGA_ROWS - 1;
	term_col = 0;
}

// term_init: initiates the terminal by clearing it.
void				terminal_init()
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	for (int col = 0; col < VGA_COLS; col++)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			index = (VGA_COLS * row) + col;	
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
		}
	}
	
	// Enable cursor (scanlines 0-15 for block cursor)
	terminal_enable_cursor(0, 15);
	terminal_update_cursor();
}


	/* term_putc: place single character on the screen.
	*  With scrolling support.
	*  When bottom of screen is reached, screen scrolls up.
*/
void				terminal_putc(char c)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	index = 0;
	switch (c)
	{
		// Newline character return column to 0 and increment row.
		case '\n':
			{
				term_col = 0;
				term_row++;
				break;
			}
			// Characters get displayed and increment column.
		default:
			{
				// Calculate vga_buffer index.
				index = (VGA_COLS * term_row) + term_col;
				vga_buffer[index] = ((uint16_t)term_color << 8) | c;
				term_col++;
				break;
			}
	}

	// If we get past last colummn, reset the column to 0
	// and increment the row to a new line.
	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row++;
	}

	// If we get past last row, scroll the screen
	if (term_row >= VGA_ROWS)
	{
		terminal_scroll();
	}
	
	// Update cursor position
	terminal_update_cursor();
}

// term_print: print a single string onto the screen.
void				terminal_print(const char *str)
{
	size_t			i;

	for (i = 0; str[i] != '\0'; i++)
	{
		terminal_putc(str[i]);
	}
}
