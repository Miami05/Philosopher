/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:38:20 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/27 21:31:30 by ldurmish         ###   ########.fr       */
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
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

void	philos_routine(t_philo *philo, t_data *data)
{
	pthread_mutex_t		*first_fork;
	pthread_mutex_t		*second_fork;

	if (one_philosopher(data, philo))
		return ;
	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	philo_activity(data, philo, first_fork, second_fork);
	ft_usleep(data->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_data		*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!check_death(philo))
	{
		philos_routine(philo, data);
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
