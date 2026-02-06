#include "kernel/terminal.h"
#include "kernel/io.h"
#include "kernel/vga.h"
#include "kernel/libc.h"
#include "kernel/keyboard.h"
#include "kernel/gdt.h"


//extern void			terminal_cursor_update(size_t row, size_t col);

extern uint8_t			color_get_current(void);

size_t				term_col = 0;
size_t				term_row = 0;

// Input tracking (for cursor movement limits)
size_t				input_start_row = 0;
size_t				input_start_col = 0;
size_t				input_end_col = 0;
size_t				input_end_row = 0;

// terminal_update_cursor: wrapper to update cursor at current position.
void				terminal_update_cursor(void)
{
	terminal_cursor_update(term_row, term_col);
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
	input_start_col = 0;
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
	uint8_t			term_color = color_get_current();

	// Block backspace if before input start position
	if (term_row < input_start_row || (term_row == input_start_row && term_col <= input_start_col))
		return;

	// Move cursor back
	if (term_col == 0)
	{
		term_row--;
		term_col = VGA_COLS - 1;
	}
	else
	{
		term_col--;
	}
	
	// Shift all characters after cursor position to the left
	size_t current_row = term_row;
	size_t current_col = term_col;
	
	while (current_row < input_end_row || (current_row == input_end_row && current_col < input_end_col))
	{
		size_t current_index = (VGA_COLS * current_row) + current_col;
		size_t next_col = current_col + 1;
		size_t next_row = current_row;
		
		if (next_col >= VGA_COLS)
		{
			next_col = 0;
			next_row++;
		}
		
		size_t next_index = (VGA_COLS * next_row) + next_col;
		
		// Copy next character to current position
		if (next_row <= input_end_row)
			vga_buffer[current_index] = vga_buffer[next_index];
		
		current_col = next_col;
		current_row = next_row;
	}
	
	// Clear the last character position
	size_t last_index = (VGA_COLS * input_end_row) + input_end_col;
	if (input_end_col > 0)
	{
		input_end_col--;
		last_index = (VGA_COLS * input_end_row) + input_end_col;
	}
	else if (input_end_row > 0)
	{
		input_end_row--;
		input_end_col = VGA_COLS - 1;
		last_index = (VGA_COLS * input_end_row) + input_end_col;
	}
	vga_buffer[last_index] = ((uint16_t)term_color << 8) | ' ';
	
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
	// Block movement if at or before prompt
	if (term_row < input_start_row)
		return;
	if (term_row == input_start_row && term_col <= input_start_col)
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

// terminal_display_prompt: display prompt and save position for input
void				terminal_display_prompt(const char *prompt)
{
	terminal_print(prompt);
	
	// Save current position as input start (after prompt)
	input_start_row = term_row;
	input_start_col = term_col;
	input_end_col = term_col;
	input_end_row = term_row;
}

// terminal_add_char_to_buffer: add character to command buffer
void				terminal_add_char_to_buffer(char c)
{
	extern t_screen		screens[MAX_SCREENS];
	extern int				current_screen;
	t_screen		*screen = &screens[current_screen];
	
	if (screen->cmd_len < CMD_BUFFER_SIZE - 1)
	{
		screen->cmd_buffer[screen->cmd_len] = c;
		screen->cmd_len++;
		screen->cmd_buffer[screen->cmd_len] = '\0';
	}
}

// terminal_remove_char_from_buffer: remove last character from buffer
void				terminal_remove_char_from_buffer(void)
{
	extern t_screen		screens[MAX_SCREENS];
	extern int		current_screen;
	t_screen		*screen = &screens[current_screen];
	
	if (screen->cmd_len > 0)
	{
		screen->cmd_len--;
		screen->cmd_buffer[screen->cmd_len] = '\0';
	}
}

// terminal_execute_command: process the command in the buffer
void				terminal_execute_command(void)
{
	extern t_screen		screens[MAX_SCREENS];
	extern int		current_screen;
	t_screen		*screen = &screens[current_screen];
	
	if (screen->cmd_len > 0)
	{
		terminal_handle_command(screen->cmd_buffer);
	}
	
	// Clear the buffer
	screen->cmd_len = 0;
	screen->cmd_buffer[0] = '\0';
}

void	terminal_reboot(void)
{
    outb(0x64, 0xFE);  // Envoie commande reset au port 0x64
}

void	terminal_poweroff(void)
{
	// QEMU/Bochs ACPI poweroff
	outw(0x604, 0x2000);
	outw(0xB004, 0x2000);

	// Fallback: halt the CPU if poweroff didn't work
	for (;;)
		__asm__ volatile ("hlt");
}
