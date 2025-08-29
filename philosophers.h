/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 07:34:58 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/29 12:21:05 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define NBR_MAX_PHILOSOPHERS 200

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DIED 4

# define USAGE "Usage: ./philo <4 or 5 arguments> (last one = meals_required)\n"
# define ERR_INVALID_ARGS "Error: invalid number of arguments\n"
# define ERR_NOT_NUMERIC "Error: arguments must contain only digits\n"
# define ERR_PHILOS "Error: invalid philosophers count\n"
# define ERR_DIE_VALUE "Error: invalid time_to_die value\n"
# define ERR_EAT_VALUE "Error: invalid time_to_eat value\n"
# define ERR_SLEEP_VALUE "Error: invalid time_to_sleep value\n"
# define ERR_MEALS_VALUE "Error: invalid meals_required value (optional)\n"
# define ERR_INT_OVERFLOW "Error: integer overflow\n"
# define ERR_TIME_FAIL "Error: failed to retrieve current time\n"
# define ERR_ALLOC_FAIL "Error: memory allocation failed\n"
# define ERR_MUTEX_INIT "Error: failed to initialize mutex\n"
# define ERR_MUTEX_DESTROY "Error: failed to destroy mutex\n"
# define ERR_THREAD_CREATE "Error: failed to create thread\n"
# define ERR_THREAD_DETACH "Error: failed to detach thread\n"

typedef struct s_philo
{
	int				philo_id;
	struct s_data	*context;
	pthread_t		thread_handle;

	size_t			start_time;
	size_t			last_meal;
	int				nbr_of_meals;
	int				is_eating;
	int				*is_dead;

	pthread_mutex_t	eating_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*death_mutex;
	pthread_mutex_t	*meal_mutex;
}					t_philo;

typedef struct s_data
{
	int				nbr_of_philos;
	int				meals_required;
	int				is_dead;

	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;

	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	fork_mutexes[NBR_MAX_PHILOSOPHERS];

	t_philo			philos[NBR_MAX_PHILOSOPHERS];
}					t_data;

int					ft_isnum(char *str);
void				ft_putstr_fd(char *s, int fd);
long int			ft_atoi(const char *str);
int					parse_input(int ac, char **av, t_data *data);
size_t				ft_get_time(void);
int					ft_usleep(size_t duration_ms);
int					destroy_mutexes(t_data *data);
void				print_status(t_philo *philo, int status);
int					philo_is_alive(t_philo *philosopher);
void				*monitor_routine(void *arg);
void				*philo_routine(void *philo_arg);
int					setup_data(t_data *data);
int					launch_threads(t_philo *philosophers);
int					join_threads(t_philo *philosophers);
int					launch_monitor_thread(t_philo *philosophers);
int					join_monitor_thread(t_philo *philosophers);

#endif
