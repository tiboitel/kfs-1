#include "kernel/terminal.h"

// External functions
extern void			cursor_update(size_t row, size_t col);
extern uint8_t			color_get_current(void);

// VGA buffer and cursor position
volatile uint16_t		*vga_buffer = (uint16_t *)0xB8000;
static size_t			term_col = 0;
static size_t			term_row = 0;

// Input tracking (for cursor movement limits)
static size_t			input_start_row = 0;
static size_t			input_end_col = 0;
static size_t			input_end_row = 0;

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
	
	// Initialize input tracking
	input_start_row = 0;
	input_end_col = 0;
	input_end_row = 0;
	
	terminal_enable_cursor(0, 15);
	terminal_update_cursor();
}

void				terminal_putc(char c)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	if (c == '\n')
	{
		term_col = 0;
		term_row++;
	}
	else
	{
		index = (VGA_COLS * term_row) + term_col;
		vga_buffer[index] = ((uint16_t)term_color << 8) | c;
		term_col++;
	}

	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row++;
	}

	if (term_row >= VGA_ROWS)
		terminal_scroll();
	
	// Update input end position
	input_end_col = term_col;
	input_end_row = term_row;
	
	terminal_update_cursor();
}

void				terminal_print(const char *str)
{
	size_t			i;

	for (i = 0; str[i] != '\0'; i++)
	{
		terminal_putc(str[i]);
	}
}

void				terminal_backspace(void)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	if (term_col == 0)
		return;

	term_col--;
	index = (VGA_COLS * term_row) + term_col;
	vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
	
	if (term_row == input_end_row && term_col < input_end_col)
		input_end_col = term_col;
	
	terminal_update_cursor();
}

void				terminal_delete(void)
{
	size_t			index;
	uint8_t			term_color = color_get_current();

	index = (VGA_COLS * term_row) + term_col;
	vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
	terminal_update_cursor();
}

void				terminal_move_cursor_left(void)
{
	if (term_row == input_start_row && term_col <= 0)
		return;
	
	if (term_col > 0)
	{
		term_col--;
	}
	else if (term_row > input_start_row)
	{
		term_row--;
		// Find last non-space character in previous line
		int col;
		for (col = VGA_COLS - 1; col >= 0; col--)
		{
			size_t index = (VGA_COLS * term_row) + col;
			if ((vga_buffer[index] & 0xFF) != ' ')
			{
				term_col = col + 1;
				break;
			}
		}
		if (col < 0)
			term_col = 0;
	}
	terminal_update_cursor();
}

void				terminal_move_cursor_right(void)
{
	if (term_row == input_end_row && term_col >= input_end_col)
		return;
	if (term_row > input_end_row)
		return;
	
	if (term_col < VGA_COLS - 1)
	{
		term_col++;
	}
	else if (term_row < input_end_row)
	{
		term_row++;
		term_col = 0;
	}
	terminal_update_cursor();
}

void				terminal_move_cursor_up(void)
{
	// Disabled: stay on current line
}

void				terminal_move_cursor_down(void)
{
	if (term_row < input_end_row && term_row < VGA_ROWS - 1)
	{
		term_row++;
		terminal_update_cursor();
	}
}
