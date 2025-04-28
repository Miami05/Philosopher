/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_activity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:09 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/27 21:30:18 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_eat_sleep(t_data *data, t_philo *philo,
	pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is eating\n",
		ft_time() - data->start_time, philo->id);
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
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is sleeping\n",
		ft_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print_lock);
}

void	philo_activity(t_data *data, t_philo *philo,
	pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	long		time_since_meal;

	time_since_meal = get_time_in_ms(philo);
	if (time_since_meal < data->time_to_die - data->time_to_sleep)
		ft_usleep(5);
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d is thinking\n",
		ft_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d has taken a fork\n",
		ft_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&data->print_lock);
	printf("%ld %d has taken a fork\n",
		ft_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print_lock);
	philo_eat_sleep(data, philo, first_fork, second_fork);
}

int	check_all_ate(t_data *data, int *all_ate)
{
	int			j;
	int			meals;

	j = 0;
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
