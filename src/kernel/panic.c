#include "kernel/terminal.h"
#include "kernel/io.h"

static const char *art[] = {
	"    .--.        _ ",
	"   |o_o |      | |",
	"   |:_/ |      | |",
	"  //   \\ \\     |_|",
	" (|     | )         ",
	"/'\\_   _/`\\     _",
	"\\___)=(___/    (_)",
	" ",
	" ",
	" ",
	"                                 KERNEL PANIC!                                  ",
	"                          Please reboot your computer.                          ",
	"                        Attempted to kill init! exitcode=0x42                   ",
	"                                                                                ",
	"                                                                                ",
	"                                                                                ",
	NULL,
};

void	terminal_panic(void)
{
	terminal_disable_cursor();
	terminal_setcolor(vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	terminal_clear();
	for (int i = 0; art[i] != NULL; i++)
	{
		ft_printf("%s\n", art[i]);
	}

	__asm__ volatile ("cli");
	for (;;)
	{
		__asm__ volatile ("hlt");
	}
}
