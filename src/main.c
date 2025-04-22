/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:44:14 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/18 02:03:32 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	handle_philos(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

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
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &data))
	{
		printf("Error creating monitor thread\n");
		return ;
	}
	i = -1;
	while (++i < data->nb_philos)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat"
			"time_to_sleep number_of_times_each_philosopher_must_eat\n");
		return (1);
	}
	if (init_data(&data, argc, argv))
	{
		printf("Init failed\n");
		return (1);
	}
	init_philosophers(&data);
	handle_philos(&data);
	i = 0;
	while (i < data.nb_philos)
	{
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data.print_lock);
	free(data.philo);
	free(data.forks);
}
