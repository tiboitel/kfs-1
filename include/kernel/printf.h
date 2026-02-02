/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-tay <sben-tay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 21:30:15 by sben-tay          #+#    #+#             */
/*   Updated: 2026/02/02 15:46:45 by sben-tay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdarg.h>

# define UINT_MAX 4294967295
# define INT_MAX 2147483647
# define INT_MIN -2147483648

int		ft_printf(const char *s, ...);

char	*ft_strdup(const char *s1);

void	ft_putchar(char c, int *compteur);

void	ft_putstr(char *str, int *compteur);

void	ft_putnbr(int nb, int *compteur);

void	ft_putnbr_unsigned(unsigned int nb, int *compteur);

void	ft_putnbr_base(\
		unsigned long long nbr, char *base, int *compteur, char c);

unsigned int	ft_strlen(const char *str);

char	*ft_strdup(const char *s1);

unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size);
int		check_arg(char c, va_list arg, int *compteur);

#endif