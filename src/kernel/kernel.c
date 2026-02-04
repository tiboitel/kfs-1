# include "kernel/terminal.h"
# include "kernel/keyboard.h"
# include "kernel/io.h"
# include "kernel/gdt.h"

char *g_str = "KFS Kernel v0.1";

#if defined(_linux_)
#error "This code must be compiled with a cross-compiler"
#endif

// kernel_main: main function.
void				kernel_main()
{
	// Initialize GDT
	gdt_init();
	
	// Initiate the terminal.
	terminal_init();
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_clear();
	
	// Initialize keyboard (polling mode)
	keyboard_init();
	
	// Initialize virtual screens
	screen_init();
	
	ft_printf("42\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
	ft_printf("Welcome to %s\n", g_str);
	ft_printf("Bonus Implemented...\n\n\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_setcolor(vga_color_entry(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
	ft_printf("- Use Arrow Keys to move cursor. Backspace and Delete supported.\n");
	ft_printf("- Use Alt+Left/Right arrows to switch between screens.\n");
	ft_printf("- Each screen has its own buffer and cursor position.\n\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	
	ft_printf("KF 2:\n\n");
	//stack_dump();

	terminal_display_prompt(PROMPT);
	
	// Infinite loop - poll keyboard
	while (1)
	{
		keyboard_poll();
	}
}
