# include "kernel/terminal.h"

#if defined(_linux_)
#error "This code must be compiled with a cross-compiler"
#endif

// kernel_main: main function.
void				kernel_main()
{
	// Initiate the terminal.
	terminal_init();


	terminal_setcolor(vga_color_entry(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
	// Clear entire screen with this color
	terminal_clear();
	
	// Write text (will be black on red)
	terminal_print("42\n\n\n");

	// Set color: black text on red background
	terminal_setcolor(vga_color_entry(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_MAGENTA));
	terminal_print("VGA Colors :");
	terminal_setcolor(vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_WHITE));
	terminal_print(" Successfully!\n");



}
