/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:47:13 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/29 11:33:25 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_is_alive(t_philo *philosopher)
{
	pthread_mutex_lock(philosopher->death_mutex);
	if (*philosopher->is_dead == 1)
	{
		pthread_mutex_unlock(philosopher->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(philosopher->death_mutex);
	return (1);
}

static int	check_starvation(t_philo *philosopher)
{
	size_t	current_time;
	size_t	last_meal_time;
	int		eating_flag;

	current_time = ft_get_time();
	pthread_mutex_lock(&philosopher->eating_mutex);
	eating_flag = philosopher->is_eating;
	pthread_mutex_unlock(&philosopher->eating_mutex);
	pthread_mutex_lock(philosopher->meal_mutex);
	last_meal_time = philosopher->last_meal;
	pthread_mutex_unlock(philosopher->meal_mutex);
	if ((current_time - last_meal_time > philosopher->context->time_to_die)
		&& !eating_flag)
		return (1);
	return (0);
}

static int	scan_deaths(t_philo *philosophers)
{
	int	i;

	i = 0;
	while (i < philosophers->context->nbr_of_philos)
	{
		if (check_starvation(&philosophers[i]))
		{
			pthread_mutex_lock(&philosophers->context->death_mutex);
			*philosophers->is_dead = 1;
			pthread_mutex_unlock(&philosophers->context->death_mutex);
			print_status(&philosophers[i], DIED);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_meal_completion(t_philo *philos)
{
	int	i;

	if (philos->context->meals_required == -1)
		return (0);
	i = 0;
	while (i < philos->context->nbr_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_mutex);
		if (philos[i].nbr_of_meals < philos->context->meals_required)
		{
			pthread_mutex_unlock(philos[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_lock(&philos->context->death_mutex);
	*philos->is_dead = 1;
	pthread_mutex_unlock(&philos->context->death_mutex);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)arg;
	while (1)
	{
		if (scan_deaths(philosophers))
			break ;
		if (check_meal_completion(philosophers))
			break ;
		usleep(1000);
	}
	return (arg);
}
