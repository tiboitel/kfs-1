#include "kernel/terminal.h"
#include "kernel/io.h"
#include "kernel/libc.h"
#include "kernel/keyboard.h"
#include "kernel/gdt.h"

void				cmd_terminal_setcolor(char *color);

void	terminal_handle_command(char *str)
{
	char *cmd = ft_strtrim(str, " \t\n\r\v\f");
	if (ft_strncmp(cmd, "help", ft_strlen(cmd)) == 0)
	{
		ft_printf(" - help: Show this help message\n");
		ft_printf(" - clear: Clear the terminal screen\n");
		ft_printf(" - print stack: Print the current kernel stack trace\n");
		ft_printf(" - reboot: Reboot the system\n");
		ft_printf(" - setcolor [color]: Change terminal color (red, green, blue, white)\n");
		// Add more commands as needed
	}
	else if (ft_strncmp(cmd, "clear", ft_strlen(cmd)) == 0)
{
		terminal_clear();
	}
	else if (ft_strncmp(cmd, "print stack", ft_strlen(cmd)) == 0)
	{
		print_stack();
	}
	else if (ft_strncmp(cmd, "reboot", ft_strlen(cmd)) == 0)
	{
		terminal_reboot();
	}
	else if (ft_strncmp(cmd, "setcolor ", 9) == 0)
	{
		char *color = cmd + 9;
		handle_setcolor(color);
	}
	else if (ft_strlen(cmd) == 0)
	{
		return ;
	}
	else
	{
		ft_printf("Unknown command: %s\n", cmd);
	}
	terminal_remove_char_from_buffer();
}

