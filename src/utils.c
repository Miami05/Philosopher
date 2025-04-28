/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 22:24:23 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/27 17:56:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_atoi(char *str)
{
	int					res;

	res = 0;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res);
}

void	ft_usleep(int time_in_ms)
{
	struct timeval		start;
	struct timeval		current;
	long				elapsed_time;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed_time = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed_time >= time_in_ms)
			break ;
		usleep(500);
	}
}

long	ft_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	clear_mutex(t_data *data)
{
	int					i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(data->philo);
	free(data->forks);
}

long	get_time_in_ms(t_philo *current)
{
	long				current_time;
	long				last_meal_time;

	current_time = ft_time();
	pthread_mutex_lock(&current->data->meal_lock);
	last_meal_time = current->last_meal;
	pthread_mutex_unlock(&current->data->meal_lock);
	return (current_time - last_meal_time);
}
