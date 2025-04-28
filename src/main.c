/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:44:14 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/27 21:00:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	handle_philos(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	init_philosophers(data);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philo[i].thread,
				NULL, philo_routine, &data->philo[i]))
		{
			printf("Error created thread\n");
			return ;
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data))
	{
		printf("Error creating monitor thread\n");
		return ;
	}
	i = -1;
	while (++i < data->nb_philos)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
}

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

int	main(int argc, char **argv)
{
	t_data		data;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat"
			"time_to_sleep number_of_times_each_philosopher_must_eat\n");
		return (1);
	}
	if (validate_input(argc, argv))
	{
		printf("The number of threads needs to be a positive nunber\n");
		return (1);
	}
	if (init_data(&data, argc, argv))
	{
		printf("Init failed\n");
		return (1);
	}
	handle_philos(&data);
	clear_mutex(&data);
	return (0);
}
