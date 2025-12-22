#include "kernel/terminal.h"

volatile uint16_t		*vga_buffer = (uint16_t *)0xB8000;

static size_t			term_col = 0;
static size_t			term_row = 0;
// term_color: black background, white foreground.
uint8_t				term_color = 0x0F;

// term_init: initiates the terminal by clearing it.
void				terminal_init()
{
	size_t			index;

	for (int col = 0; col < VGA_COLS; col++)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			index = (VGA_COLS * row) + col;	
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
		}
	}
}


/* term_putc: place single character on the screen.
*  No scrolling implemented yet.
*  When bottom of screen is reached,
*  output wraps to the top.
*/
void				terminal_putc(char c)
{
	size_t			index;

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

	// If we get past last row, reset the column to 0
	// and increment the row to get to a new line.
	if (term_row >= VGA_ROWS)
	{
		term_row = 0;
	}
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
