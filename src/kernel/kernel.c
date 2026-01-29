# include "kernel/terminal.h"

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
	
	// Print many lines to demonstrate scrolling
	for (int i = 1; i <= 30; i++)
	{
		terminal_print("Line ");
		// Simple number printing (hackish but works for demo)
		if (i >= 10)
			terminal_putc('0' + (i / 10));
		terminal_putc('0' + (i % 10));
		terminal_print("\n");
	}
	
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_print("\n42\n\n\n");

	terminal_setcolor(vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
	terminal_print("Sucessfuly changed terminal color!\n");

}
