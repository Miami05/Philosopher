/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:19:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/16 20:12:03 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"
#include <semaphore.h>

int	validate_input(int argc, char **argv)
{
	int			i;
	int			j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
		error("Usage: ./philo number_of_philo time_to_die time_to_die"
			"time_to_sleep [numbers_of_each_time_philo_must_eat");
	if (validate_input(argc, argv))
		error("Each number needs to be positive and not greater than INT_MAX");
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nb_of_times_each_philo_eat = -1;
	if (argc == 6)
		data->nb_of_times_each_philo_eat = ft_atoi(argv[5]);
	return (0);
}
