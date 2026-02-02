#include "kernel/terminal.h"
#include "kernel/io.h"

// External VGA buffer
extern volatile uint16_t	*vga_buffer;

// Screen management
static t_screen				screens[MAX_SCREENS];
static int					current_screen = 0;

// External terminal state (need to get/set from terminal.c)
extern size_t				term_col;
extern size_t				term_row;
extern size_t				input_start_row;
extern size_t				input_end_row;
extern size_t				input_end_col;

void				screen_init(void)
{
	int				i;

	// Initialize all screens
	for (i = 0; i < MAX_SCREENS; i++)
	{
		screens[i].cursor_row = 0;
		screens[i].cursor_col = 0;
		screens[i].color = 0x0F; // White on black
		screens[i].input_start_row = 0;
		screens[i].input_end_row = 0;
		screens[i].input_end_col = 0;
		screens[i].is_initialized = 0; // Mark as not initialized
		
		// Clear buffer
		for (int j = 0; j < VGA_ROWS * VGA_COLS; j++)
			screens[i].buffer[j] = 0x0F20; // White space
	}
	
	current_screen = 0;
	screens[0].is_initialized = 1; // Screen 0 is initialized at boot
}

void				screen_save_current(void)
{
	int				i;
	
	// Save VGA buffer to current screen
	for (i = 0; i < VGA_ROWS * VGA_COLS; i++)
		screens[current_screen].buffer[i] = vga_buffer[i];
	
	// Save cursor position
	screens[current_screen].cursor_row = term_row;
	screens[current_screen].cursor_col = term_col;
	
	// Save input tracking
	screens[current_screen].input_start_row = input_start_row;
	screens[current_screen].input_end_row = input_end_row;
	screens[current_screen].input_end_col = input_end_col;
}

void				screen_load(int screen_id)
{
	int			i;
	
	if (screen_id < 0 || screen_id >= MAX_SCREENS)
		return;

	// Load buffer from screen to VGA
	for (i = 0; i < VGA_ROWS * VGA_COLS; i++)
		vga_buffer[i] = screens[screen_id].buffer[i];
	
	// Restore cursor position
	term_row = screens[screen_id].cursor_row;
	term_col = screens[screen_id].cursor_col;
	
	// Restore input tracking
	input_start_row = screens[screen_id].input_start_row;
	input_end_row = screens[screen_id].input_end_row;
	input_end_col = screens[screen_id].input_end_col;
	
	// Update cursor on screen
	terminal_update_cursor();
	
	// If screen not initialized, display welcome message
	if (!screens[screen_id].is_initialized)
	{
		ft_printf("Screen %d\n", screen_id);
		terminal_display_prompt(PROMPT);
		screens[screen_id].is_initialized = 1;
	}
}

void				screen_switch(int screen_id)
{
	 
	if (screen_id < 0 || screen_id >= MAX_SCREENS)
		return;
	
	if (screen_id == current_screen)
		return;
	
	// Save current screen
	screen_save_current();
	
	// Switch
	current_screen = screen_id;
	
	// Load new screen
	screen_load(screen_id);
}

int				screen_get_current(void)
{
	return current_screen;
}
