/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 19:42:38 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 23:34:17 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	setup_forks(pthread_mutex_t *forks, int nbr_of_philos)
{
	int	i;

	i = 0;
	while (i < nbr_of_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			ft_putstr_fd(ERR_MUTEX_INIT, 2);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	setup_philos(t_data *data)
{
	int		i;
	size_t	start_time;

	start_time = ft_get_time();
	i = 0;
	while (i < data->nbr_of_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].context = data;
		data->philos[i].thread_handle = 0;
		data->philos[i].start_time = start_time;
		data->philos[i].last_meal = start_time;
		data->philos[i].nbr_of_meals = 0;
		data->philos[i].is_eating = 0;
		data->philos[i].is_dead = &data->is_dead;
		data->philos[i].left_fork = &data->fork_mutexes[i];
		data->philos[i].right_fork = &data->fork_mutexes[(i + 1)
			% data->nbr_of_philos];
		data->philos[i].print_mutex = &data->print_mutex;
		data->philos[i].death_mutex = &data->death_mutex;
		data->philos[i].meal_mutex = &data->meal_mutex;
		if (pthread_mutex_init(&data->philos[i].eating_mutex, NULL) != 0)
			ft_putstr_fd(ERR_MUTEX_INIT, 2);
		i++;
	}
}

int	setup_data(t_data *data)
{
	if (setup_forks(data->fork_mutexes, data->nbr_of_philos))
		return (1);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		ft_putstr_fd(ERR_MUTEX_INIT, 2);
		return (1);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		ft_putstr_fd(ERR_MUTEX_INIT, 2);
		return (1);
	}
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
	{
		ft_putstr_fd(ERR_MUTEX_INIT, 2);
		return (1);
	}
	data->is_dead = 0;
	setup_philos(data);
	return (0);
}
