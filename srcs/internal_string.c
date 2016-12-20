/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpinchon <gpinchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/13 10:52:53 by gpinchon          #+#    #+#             */
/*   Updated: 2016/12/20 13:29:48 by gpinchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_framework.h>

static size_t	int_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

static void		int_putnbr_recursif(int n, int fd)
{
	char	c;

	if (n > 9)
		int_putnbr_recursif(n / 10, fd);
	c = '0' + (n % 10);
	write(fd, &c, 1);
}

void			int_putstr_fd(char const *str, int fd)
{
	write(fd, str, int_strlen(str));
}

void			int_putnbr_fd(int n, int fd)
{
	char	c;
	if (n == __MAX(int))
	{
		int_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		c = '-';
		write(fd, &c, 1);
		n = -n;
	}
	int_putnbr_recursif(n, fd);
}

void			int_putendl_fd(char const *s, int fd)
{
	char	c;

	int_putstr_fd((char *)s, fd);
	c = '\n';
	write(fd, &c, 1);
}
