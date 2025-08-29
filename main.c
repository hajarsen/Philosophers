/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 07:48:26 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/28 23:35:19 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_input(ac, av, &data))
		return (EXIT_FAILURE);
	if (setup_data(&data))
		return (EXIT_FAILURE);
	if (launch_threads(data.philos) || launch_monitor_thread(data.philos))
	{
		destroy_mutexes(&data);
		return (EXIT_FAILURE);
	}
	if (join_threads(data.philos) || join_monitor_thread(data.philos))
	{
		destroy_mutexes(&data);
		return (EXIT_FAILURE);
	}
	if (destroy_mutexes(&data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
