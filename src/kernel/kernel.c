# include "kernel/terminal.h"
# include "kernel/keyboard.h"
# include "kernel/io.h"
# include "kernel/gdt.h"


char *g_kernel_version = "KFS Kernel v0.2";

#if defined(_linux_)
#error "This code must be compiled with a cross-compiler"
#endif

// kernel_main: main function.
void	kernel_main()
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
	
	terminal_setcolor(vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
	ft_printf("Welcome to %s\n\n", g_kernel_version);
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	ft_printf("42\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
	ft_printf("- Use Arrow Keys to move cursor. Backspace and Delete supported.\n");
	ft_printf("- Use Alt+Left/Right arrows to switch between screens.\n");
	ft_printf("- Use keyboard to write in the terminal.\n");
	ft_printf("- Use command help to see available commands.\n\n");
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_display_prompt(PROMPT);
	
	// Infinite loop - poll keyboard
	while (1)
	{
		keyboard_poll();
	}
}
