/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:23:07 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/29 11:46:42 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	acquire_forks(t_philo *philosopher, pthread_mutex_t **f_fork,
		pthread_mutex_t **s_fork)
{
	if (philosopher->philo_id % 2 == 0)
	{
		*f_fork = philosopher->left_fork;
		*s_fork = philosopher->right_fork;
	}
	else
	{
		*f_fork = philosopher->right_fork;
		*s_fork = philosopher->left_fork;
	}
	pthread_mutex_lock(*f_fork);
	print_status(philosopher, FORK);
	if (philosopher->context->nbr_of_philos == 1)
	{
		ft_usleep(philosopher->context->time_to_die);
		pthread_mutex_unlock(*f_fork);
		return ;
	}
	pthread_mutex_lock(*s_fork);
	print_status(philosopher, FORK);
}

static void	do_eat(t_philo *philosopher)
{
	print_status(philosopher, EAT);
	pthread_mutex_lock(&philosopher->eating_mutex);
	philosopher->is_eating = 1;
	pthread_mutex_unlock(&philosopher->eating_mutex);
	pthread_mutex_lock(philosopher->meal_mutex);
	philosopher->last_meal = ft_get_time();
	philosopher->nbr_of_meals++;
	pthread_mutex_unlock(philosopher->meal_mutex);
	ft_usleep(philosopher->context->time_to_eat);
	pthread_mutex_lock(&philosopher->eating_mutex);
	philosopher->is_eating = 0;
	pthread_mutex_unlock(&philosopher->eating_mutex);
}

static void	do_sleep_and_think(t_philo *philosopher)
{
	print_status(philosopher, SLEEP);
	ft_usleep(philosopher->context->time_to_sleep);
	print_status(philosopher, THINK);
}

static void	execute_cycle(t_philo *philosopher)
{
	pthread_mutex_t	*f_fork;
	pthread_mutex_t	*s_fork;

	acquire_forks(philosopher, &f_fork, &s_fork);
	if (philosopher->context->nbr_of_philos > 1)
	{
		do_eat(philosopher);
		pthread_mutex_unlock(s_fork);
		pthread_mutex_unlock(f_fork);
		do_sleep_and_think(philosopher);
	}
}

void	*philo_routine(void *philo_arg)
{
	t_philo	*philosophers;

	if (!philo_arg)
		return (NULL);
	philosophers = (t_philo *)philo_arg;
	if (philosophers->philo_id % 2 != 0)
		ft_usleep(philosophers->context->time_to_eat);
	while (philo_is_alive(philosophers))
	{
		execute_cycle(philosophers);
		if (philosophers->context->nbr_of_philos == 1)
			break ;
		usleep(500);
	}
	return (philo_arg);
}
