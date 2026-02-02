#include "kernel/keyboard.h"
#include "kernel/terminal.h"

// US QWERTY keymap (scancodes to ASCII)
static const char keymap[128] = {
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

// Arrow key scancodes
#define KEY_UP		0x48
#define KEY_DOWN	0x50
#define KEY_LEFT	0x4B
#define KEY_RIGHT	0x4D
#define KEY_DELETE	0x53

// keyboard_init: Initialize keyboard driver
void			keyboard_init(void)
{
	// Nothing special to do in polling mode
}

// keyboard_poll: Check if key is available and handle it
void			keyboard_poll(void)
{
	uint8_t		scancode;
	char		ch;

	// Check if data is available (bit 0 of status port)
	if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
		return; // No data available

	// Read scancode from keyboard
	scancode = inb(KEYBOARD_DATA_PORT);

	// Only handle key press (bit 7 = 0)
	if (scancode & 0x80)
		return;

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
		return;
	}

	// Print character if valid
	if (ch != 0)
		terminal_putc(ch);
}
