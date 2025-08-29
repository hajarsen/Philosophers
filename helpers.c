/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:52:36 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 23:36:29 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	check;

	if (!s)
		return ;
	while (*s)
	{
		check = write(fd, s, 1);
		if (check == -1)
			return ;
		s++;
	}
}

static int	ft_sign(char *s)
{
	int	sign;

	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	return (sign);
}

long int	ft_atoi(const char *s)
{
	int			sign;
	long int	num;

	sign = 1;
	num = 0;
	while ((*s >= 9 && *s <= 13) || *s == ' ')
		s++;
	if (*s == '+' || *s == '-')
	{
		sign = ft_sign((char *)s);
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		if (num > (LONG_MAX - (*s - '0')) / 10)
		{
			if (sign == 1)
				return (ft_putstr_fd(ERR_INT_OVERFLOW, 2), LONG_MAX);
			else
				return (ft_putstr_fd(ERR_INT_OVERFLOW, 2), LONG_MIN);
		}
		num = num * 10 + (*s - '0');
		s++;
	}
	return (sign * num);
}
