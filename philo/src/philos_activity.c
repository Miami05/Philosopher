/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_activity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:09 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/29 13:32:41 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <pthread.h>

void	philo_sleep(t_philo *philo)
{
	t_data			*data;

	if (check_death(philo))
		return ;
	data = philo->data;
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is sleeping\n", ft_time() - data->start_time,
		philo->id);
	pthread_mutex_unlock(&data->print_lock);
	ft_usleep(data->time_to_sleep);
}

void	philo_eat(t_philo *philo, pthread_mutex_t *first_fork,
			pthread_mutex_t *second_fork)
{
	t_data			*data;

	if (check_death(philo))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	data = philo->data;
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is eating\n", ft_time() - data->start_time,
		philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(&data->meal_lock);
	philo->last_meal = ft_time();
	pthread_mutex_unlock(&data->meal_lock);
	ft_usleep(data->time_to_eat);
	pthread_mutex_lock(&data->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_lock);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	philo_sleep(philo);
}

void	philo_take_forks(t_data *data, t_philo *philo,
			pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d has taken a fork\n", ft_time() - data->start_time,
		philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(second_fork);
	if (check_death(philo))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d has taken a fork\n", ft_time() - data->start_time,
		philo->id);
	pthread_mutex_unlock(&data->print_lock);
	philo_eat(philo, first_fork, second_fork);
}

void	philo_has_fork(t_philo *philo)
{
	t_data			*data;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (check_death(philo))
		return ;
	data = philo->data;
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
	pthread_mutex_lock(first_fork);
	if (check_death(philo))
	{
		pthread_mutex_unlock(first_fork);
		return ;
	}
	philo_take_forks(data, philo, first_fork, second_fork);
}

int	check_all_ate(t_data *data, int *all_ate)
{
	int			j;
	int			meals;

	j = 0;
	*all_ate = 1;
	while (j < data->nb_philos)
	{
		pthread_mutex_lock(&data->meal_lock);
		meals = data->philo[j].meals_eaten;
		pthread_mutex_unlock(&data->meal_lock);
		if (meals < data->must_eat)
		{
			*all_ate = 0;
			return (0);
		}
		j++;
	}
	return (1);
}
