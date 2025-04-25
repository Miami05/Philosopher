/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:38:20 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/25 04:18:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	elapsed_time;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->nb_philos)
		{
			elapsed_time = get_time_in_ms(&data->philo[i]);
			if (elapsed_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->death_lock);
				data->someone_died = 1;
				pthread_mutex_unlock(&data->death_lock);
				pthread_mutex_lock(&data->print_lock);
				printf("%ld Philo %d died\n",
					ft_time() - data->start_time, data->philo[i].id);
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

void	philo_activity(t_philo *philo, t_data *data)
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
	pthread_mutex_lock(&data->print_lock);
	printf("Philosopher %d is thinking\n", philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&data->print_lock);
	printf("Philosopher %d has taken a fork\n", philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&data->print_lock);
	printf("Philosopher %d has taken a fork\n", philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(&data->print_lock);
	printf("Philosopher %d is eating\n", philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(&data->meal_lock);
	philo->last_meal = ft_time();
	pthread_mutex_unlock(&data->meal_lock);
	ft_usleep(data->time_to_eat);
	philo->meals_eaten++;
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_lock(&data->print_lock);
	printf("Philosopher %d is sleeping\n", philo->id);
	pthread_mutex_unlock(&data->print_lock);
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
	while (1)
	{
		if (check_death(philo))
			break ;
		if (data->must_eat != -1)
		{
			pthread_mutex_lock(&data->meal_lock);
			if (philo->meals_eaten >= data->must_eat)
			{
				pthread_mutex_unlock(&data->meal_lock);
				break ;
			}
			pthread_mutex_unlock(&data->meal_lock);
		}
		philo_activity(philo, data);
	}
	return (NULL);
}
