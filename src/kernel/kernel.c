# include "kernel/terminal.h"

#if defined(_linux_)
#error "This code must be compiled with a cross-compiler"
#endif

// kernel_main: main function.
void				kernel_main()
{
	// Initiate the terminal.
	terminal_init();

	// Display a welcome message.
	terminal_print("42");
}
