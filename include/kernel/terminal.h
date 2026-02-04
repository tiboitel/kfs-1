#ifndef TERMINAL_H
# define TERMINAL_H

# include <stddef.h>
# include <stdint.h>
# include "vga.h"

# define MAX_SCREENS 100
# define PROMPT "root@kfs:~$ "
# define CMD_BUFFER_SIZE 256

// Screen structure for virtual screens
typedef struct s_screen {
	uint16_t	buffer[VGA_ROWS * VGA_COLS];
	size_t		cursor_row;
	size_t		cursor_col;
	uint8_t		color;
	size_t		input_start_row;
	size_t		input_end_row;
	size_t		input_end_col;
	int			is_initialized;
	char		cmd_buffer[CMD_BUFFER_SIZE];
	size_t		cmd_len;
} t_screen;

// Terminal functions
void terminal_init(void);
void terminal_putc(char c);
void terminal_print(const char *str);
void terminal_display_prompt(const char *prompt);

void terminal_clear(void);
void terminal_repaint(uint8_t color);
void terminal_scroll(void);
void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void terminal_disable_cursor(void);
void terminal_update_cursor(void);
uint8_t vga_color_entry(enum vga_color fg, enum vga_color bg);
void terminal_setcolor(uint8_t color);

// New functions for keyboard support
void terminal_backspace(void);
void terminal_delete(void);
void terminal_move_cursor_left(void);
void terminal_move_cursor_right(void);
void terminal_move_cursor_up(void);
void terminal_move_cursor_down(void);

// Virtual screens
void screen_init(void);
void screen_switch(int screen_id);
void screen_save_current(void);
void screen_load(int screen_id);
int screen_get_current(void);

// Command handling
void terminal_add_char_to_buffer(char c);
void terminal_remove_char_from_buffer(void);
void terminal_execute_command(void);
void terminal_handle_command(char *str);

// System control
void terminal_reboot(void);

#endif