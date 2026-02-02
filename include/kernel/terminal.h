#ifndef TERMINAL_H
# define TERMINAL_H

# include <stddef.h>
# include <stdint.h>

# define VGA_COLS 80
# define VGA_ROWS 25
# define MAX_SCREENS 100

# define PROMPT "root@kfs:~$ "

// VGA color constants
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

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

// utils
size_t	ft_strlen(const char *str);
size_t strncmp(const char *s1, const char *s2, size_t n);

#endif