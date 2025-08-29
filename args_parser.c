/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:56:09 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 23:34:52 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_values(t_data *data)
{
	if (data->nbr_of_philos < 1 || data->nbr_of_philos > NBR_MAX_PHILOSOPHERS)
		return (ft_putstr_fd(ERR_PHILOS, 2), 1);
	if (data->time_to_die < 60 || data->time_to_die > INT_MAX)
		return (ft_putstr_fd(ERR_DIE_VALUE, 2), 1);
	if (data->time_to_eat < 60 || data->time_to_eat > INT_MAX)
		return (ft_putstr_fd(ERR_EAT_VALUE, 2), 1);
	if (data->time_to_sleep < 60 || data->time_to_sleep > INT_MAX)
		return (ft_putstr_fd(ERR_SLEEP_VALUE, 2), 1);
	if (data->meals_required < -1 || data->meals_required > INT_MAX)
		return (ft_putstr_fd(ERR_MEALS_VALUE, 2), 1);
	return (0);
}

static int	check_numeric_args(int ac, char **av)
{
	int	index;

	index = 1;
	while (index < ac)
	{
		if (!ft_isnum(av[index]))
		{
			ft_putstr_fd(ERR_NOT_NUMERIC, 2);
			return (1);
		}
		index++;
	}
	return (0);
}

int	parse_input(int ac, char **av, t_data *data)
{
	if (!(ac == 5 || ac == 6))
	{
		ft_putstr_fd(ERR_INVALID_ARGS, 2);
		ft_putstr_fd(USAGE, 2);
		return (1);
	}
	if (check_numeric_args(ac, av))
		return (1);
	data->nbr_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->meals_required = ft_atoi(av[5]);
	else
		data->meals_required = -1;
	data->is_dead = 0;
	if (check_values(data))
		return (1);
	return (0);
}
