# include "kernel/terminal.h"
# include "kernel/keyboard.h"

#if defined(_linux_)
#error "This code must be compiled with a cross-compiler"
#endif

// kernel_main: main function.
void				kernel_main()
{
	// Initiate the terminal.
	terminal_init();

	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_clear();
	
	// Initialize keyboard (polling mode)
	keyboard_init();
	terminal_print("42\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
	terminal_print("Bonus Implemented...\n\n\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_setcolor(vga_color_entry(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
	terminal_print("- Use Arrow Keys to move cursor. Backspace and Delete supported.\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));

	// Infinite loop - poll keyboard
	while (1)
	{
		keyboard_poll();
	}
}
