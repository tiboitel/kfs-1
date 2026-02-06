#include "kernel/keyboard.h"
#include "kernel/terminal.h"

// Modifier keys state
static int	ctrl_pressed = 0;
static int	alt_pressed = 0;

// US QWERTY keymap (scancodes to ASCII)
static const char	keymap[128] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',	// 0x00-0x0E
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		// 0x0F-0x1C
	0, 											// 0x1D Ctrl
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',			// 0x1E-0x29
	0, 											// 0x2A Left shift
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 			// 0x2B-0x35
	0, 											// 0x36 Right shift
	'*',											// 0x37 Keypad *
	0, 											// 0x38 Alt
	' ', 											// 0x39 Space
	0, 											// 0x3A Caps lock
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 							// 0x3B-0x44 F1-F10
	0, 											// 0x45 Num lock
	0, 											// 0x46 Scroll lock
	0, 											// 0x47 Home
	0, 											// 0x48 Up arrow
	0, 											// 0x49 Page up
	'-',											// 0x4A Keypad -
	0, 											// 0x4B Left arrow
	0,											// 0x4C Keypad 5
	0, 											// 0x4D Right arrow
	'+',											// 0x4E Keypad +
	0, 											// 0x4F End
	0, 											// 0x50 Down arrow
	0, 											// 0x51 Page down
	0, 											// 0x52 Insert
	0, 											// 0x53 Delete
	0, 0, 0,										// 0x54-0x56
	0, 0 										// 0x57-0x58 F11, F12
};

// keyboard_init: Initialize keyboard driver
void	keyboard_init(void)
{
	// Nothing special to do in polling mode
}

// keyboard_poll: Check if key is available and handle it
void	keyboard_poll(void)
{
	uint8_t		scancode;
	char		ch;

	// Check if data is available (bit 0 of status port)
	if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
		return; // No data available

	// Read scancode from keyboard
	scancode = inb(KEYBOARD_DATA_PORT);

	// Handle Ctrl key press/release
	if (scancode == KEY_CTRL)
	{
		ctrl_pressed = 1;
		return;
	}
	else if (scancode == (KEY_CTRL | 0x80))
	{
		ctrl_pressed = 0;
		return;
	}

	// Handle Alt key press/release
	if (scancode == KEY_ALT)
	{
		alt_pressed = 1;
		return;
	}
	else if (scancode == (KEY_ALT | 0x80))
	{
		alt_pressed = 0;
		return;
	}

	// Only handle key press (bit 7 = 0)
	if (scancode & 0x80)
		return;

	// Handle Alt+arrows for screen switching
	if (alt_pressed)
	{
		if (scancode == KEY_LEFT)
		{
			int current = screen_get_current();
			if (current > 0)
				screen_switch(current - 1);
		}
		else if (scancode == KEY_RIGHT)
		{
			int current = screen_get_current();
			if (current < MAX_SCREENS - 1)
				screen_switch(current + 1);
		}
		return;
	}

	// Handle Ctrl+number for screen switching (backup method)
	if (ctrl_pressed)
	{
		char second_letter = keymap[scancode];
		if (second_letter == 'l')
		{
			terminal_clear();
			terminal_display_prompt(PROMPT);
		}
		if (second_letter == 'c')
		{
			ft_printf("^C\n");
			terminal_display_prompt(PROMPT);
		}
		if (second_letter == 'd')
		{
			terminal_panic();
		}
		terminal_remove_char_from_buffer();
		return;
	}

	// Handle arrow keys
	if (scancode == KEY_UP)
	{
		terminal_move_cursor_up();
		return;
	}
	if (scancode == KEY_DOWN)
	{
		terminal_move_cursor_down();
		return;
	}
	if (scancode == KEY_LEFT)
	{
		terminal_move_cursor_left();
		return;
	}
	if (scancode == KEY_RIGHT)
	{
		terminal_move_cursor_right();
		return;
	}
	if (scancode == KEY_DELETE)
	{
		terminal_delete();
		return;
	}

	// Convert scancode to ASCII
	ch = keymap[scancode];

	// Handle backspace
	if (ch == '\b')
	{
		terminal_backspace();
		terminal_remove_char_from_buffer();
		return;
	}

	// Handle Enter key - execute command
	if (ch == '\n')
	{
		terminal_putc('\n');
		terminal_execute_command();
		terminal_display_prompt(PROMPT);
		return;
	}
	// Print character if valid and add to buffer
	if (ch != 0)
	{
		terminal_putc(ch);
		terminal_add_char_to_buffer(ch);
	}
}
