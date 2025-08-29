/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_runtime.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:08:52 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 23:35:37 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_destroy(&data->fork_mutexes[i]) != 0)
		{
			ft_putstr_fd(ERR_MUTEX_DESTROY, 2);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_destroy(&data->print_mutex) != 0)
		return (ft_putstr_fd(ERR_MUTEX_DESTROY, 2), 1);
	if (pthread_mutex_destroy(&data->death_mutex) != 0)
		return (ft_putstr_fd(ERR_MUTEX_DESTROY, 2), 1);
	if (pthread_mutex_destroy(&data->meal_mutex) != 0)
		return (ft_putstr_fd(ERR_MUTEX_DESTROY, 2), 1);
	return (0);
}

void	print_status(t_philo *philosopher, int status)
{
	size_t		now;
	static char	*status_msgs[] = {"is eating", "is sleeping", "is thinking",
		"has taken a fork", "died"};
	static char	*status_colors[] = {"\033[0;32m", "\033[0;33m", "\033[0;34m",
		"\033[0;35m", "\033[0;31m"};

	pthread_mutex_lock(philosopher->print_mutex);
	now = ft_get_time() - philosopher->start_time;
	if (status == DIED || (status != DIED && *philosopher->is_dead == 0))
	{
		printf("%zu %d %s%s\033[0m\n", now, philosopher->philo_id,
			status_colors[status], status_msgs[status]);
	}
	pthread_mutex_unlock(philosopher->print_mutex);
}
