
#include "kernel/io.h"
#include "kernel/terminal.h"

static int	check_arg(char c, va_list arg, int *compteur);
static void	ft_putchar(char c, int *compteur);
static void	ft_putstr(char *str, int *compteur);
static void	ft_putnbr(int nb, int *compteur);
static void	ft_putnbr_unsigned(unsigned int nb, int *compteur);
static void	ft_putnbr_base(unsigned long long nbr, char *base, int *compteur, char c);

int	ft_printf(const char *s, ...)
{
	unsigned int		i;
	va_list				arg;
	char				*str;
	int					compteur;

	if (!s)
		return (-1);
	i = 0;
	compteur = 0;
	str = (char *)s;
	va_start(arg, s);
	while (str[i])
	{
		if (str[i] == '%')
		{
			check_arg(str[i + 1], arg, &compteur);
			i++;
		}
		else
			ft_putchar(str[i], &compteur);
		i++;
	}
	va_end(arg);
	return (compteur);
}


static int	check_arg(char c, va_list arg, int *compteur)
{
	if (c == 'c')
		ft_putchar((char)va_arg(arg, int), compteur);
	else if (c == '%')
		ft_putchar('%', compteur);
	else if (c == 's')
		ft_putstr(va_arg(arg, char *), compteur);
	else if (c == 'd' || c == 'i')
		ft_putnbr((int)va_arg(arg, int), compteur);
	else if (c == 'u')
		ft_putnbr_unsigned(va_arg(arg, unsigned int), compteur);
	else if (c == 'x')
		ft_putnbr_base((unsigned int)va_arg(arg, unsigned int), \
		"0123456789abcdef", compteur, c);
	else if (c == 'X')
		ft_putnbr_base((unsigned int)va_arg(arg, unsigned int), \
		"0123456789ABCDEF", compteur, c);
	else if (c == 'p')
		ft_putnbr_base((unsigned long long)va_arg(arg, unsigned long long), \
		"0123456789abcdef", compteur, c);
	else
		return (-1);
	return (1);
}


static void	ft_putchar(char c, int *compteur)
{
	terminal_putc(c);	
	(*compteur)++;
}

static void	ft_putstr(char *str, int *compteur)
{
	int	i;

	i = 0;
	if (!str)
		return (ft_putstr("(null)", compteur));
	while (str[i])
	{
		ft_putchar(str[i], compteur);
		i++;
	}
}

static void	ft_putnbr(int nb, int *compteur)
{
	if (nb == INT_MIN)
	{
		terminal_print("-2147483648");
		*compteur += 11;
		return ;
	}
	else if (nb < 0)
	{
		ft_putchar('-', compteur);
		ft_putnbr(nb *(-1), compteur);
	}
	else if (nb >= 0 && nb <= 9)
		ft_putchar(nb + 48, compteur);
	else
	{
		ft_putnbr(nb / 10, compteur);
		ft_putnbr(nb % 10, compteur);
	}
}

static void	ft_putnbr_unsigned(unsigned int nb, int *compteur)
{
	if (nb >= 10)
	{
		ft_putnbr_unsigned(nb / 10, compteur);
		nb = nb % 10;
	}
	if (nb < 10)
	{
		ft_putchar(nb + 48, compteur);
	}
}

static void	ft_putnbr_base(\
		unsigned long long nbr, char *base, int *compteur, char c)
{
	if (c == 'p')
	{
		if (nbr != 0)
			ft_putstr("0x", compteur);
		if (nbr == 0)
		{
			ft_putstr("(nil)", compteur);
			return ;
		}
		c++;
	}
	if (c != 'p')
	{
		if (nbr >= 16)
			ft_putnbr_base(nbr / 16, base, compteur, c);
		ft_putchar(base[nbr % 16], compteur);
	}
}

