/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 23:35:45 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 20:17:57 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	ft_get_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL) != 0)
	{
		ft_putstr_fd(ERR_TIME_FAIL, 2);
		return (0);
	}
	return ((current_time.tv_sec * (size_t)1000) + (current_time.tv_usec
			/ 1000));
}

int	ft_usleep(size_t duration_ms)
{
	size_t	start_time;
	size_t	passed_time;

	start_time = ft_get_time();
	while (1)
	{
		passed_time = ft_get_time() - start_time;
		if (passed_time >= duration_ms)
			break ;
		if (duration_ms - passed_time > 50)
			usleep(500);
		else
			usleep(100);
	}
	return (0);
}
