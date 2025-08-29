/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:13:26 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/29 12:00:58 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	launch_threads(t_philo *philosophers)
{
	int		index;
	t_data	*ctx;

	ctx = philosophers->context;
	index = 0;
	while (index < ctx->nbr_of_philos)
	{
		if (pthread_create(&philosophers[index].thread_handle,
				NULL, &philo_routine, &philosophers[index]))
		{
			ft_putstr_fd(ERR_THREAD_CREATE, 2);
			return (1);
		}
		index++;
	}
	return (0);
}

int	join_threads(t_philo *philosophers)
{
	int		index;
	t_data	*ctx;

	ctx = philosophers->context;
	index = 0;
	while (index < ctx->nbr_of_philos)
	{
		if (pthread_join(philosophers[index].thread_handle, NULL))
		{
			ft_putstr_fd(ERR_THREAD_CREATE, 2);
			return (1);
		}
		index++;
	}
	return (0);
}

int	launch_monitor_thread(t_philo *philos)
{
	t_data	*ctx;

	ctx = philos->context;
	if (pthread_create(&ctx->monitor_thread, NULL, &monitor_routine, philos))
	{
		ft_putstr_fd(ERR_THREAD_CREATE, 2);
		return (1);
	}
	return (0);
}

int	join_monitor_thread(t_philo *philosophers)
{
	t_data	*ctx;

	ctx = philosophers->context;
	if (pthread_join(ctx->monitor_thread, NULL))
	{
		ft_putstr_fd(ERR_THREAD_CREATE, 2);
		return (1);
	}
	return (0);
}
