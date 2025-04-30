/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:38:20 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/29 18:01:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_philosopher_death(t_data *data)
{
	long	elapsed_time;
	int		i;

	i = 0;
	while (i < data->nb_philos)
	{
		elapsed_time = get_time_in_ms(&data->philo[i]);
		if (elapsed_time > data->time_to_die)
		{
			pthread_mutex_lock(&data->death_lock);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_lock);
			pthread_mutex_lock(&data->print_lock);
			printf("%ld %d died\n",
				ft_time() - data->start_time, data->philo[i].id);
			pthread_mutex_unlock(&data->print_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		all_ate;

	data = (t_data *)arg;
	while (1)
	{
		if (check_philosopher_death(data))
			return (NULL);
		if (data->must_eat != -1)
		{
			all_ate = 1;
			if (check_all_ate(data, &all_ate))
			{
				pthread_mutex_lock(&data->death_lock);
				data->all_ate = 1;
				pthread_mutex_unlock(&data->death_lock);
				pthread_mutex_lock(&data->print_lock);
				printf("All philo have eaten enough\n");
				pthread_mutex_unlock(&data->print_lock);
				return (NULL);
			}
		}
		usleep(500);
	}
	return (NULL);
}

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died || philo->data->all_ate)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

void	philo_think(t_philo *philo)
{
	t_data		*data;
	long		since_last_meal;

	if (check_death(philo))
		return ;
	data = philo->data;
	since_last_meal = get_time_in_ms(philo);
	if (since_last_meal < data->time_to_die - data->time_to_sleep)
		ft_usleep(10);
	if (check_death(philo))
		return ;
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is thinking\n", ft_time() - data->start_time,
		philo->id);
	pthread_mutex_unlock(&data->print_lock);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (one_philosopher(philo->data, philo))
		return (NULL);
	while (!check_death(philo))
	{
		philo_has_fork(philo);
		philo_think(philo);
	}
	return (NULL);
}
