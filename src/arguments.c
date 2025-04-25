/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 arguments.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: ldurmish < ldurmish@student.42wolfsburg.d	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/17 22:29:23 by ldurmish		   #+#	  #+#			  */
/*	 Updated: 2025/04/23 01:00:57 by ldurmish		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../include/philo.h"
#include <pthread.h>

int	init_data(t_data *data, int argc, char **argv)
{
	int		i;

	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->someone_died = 0;
	data->all_ate = 0;
	data->start_time = ft_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	i = -1;
	while (++i < data->nb_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	data->philo = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philo || !data->forks)
		return (1);
	return (0);
}

int	one_philosopher(t_data *data, t_philo *philo)
{
	if (data->nb_philos == 1)
	{
		pthread_mutex_lock(&data->print_lock);
		printf("%ld Philo %d is thinking\n",
			ft_time() - data->start_time, data->philo->id);
		pthread_mutex_unlock(&data->print_lock);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&data->print_lock);
		printf("%ld Philo %d has taken a fork\n",
			ft_time() - data->start_time, data->philo->id);
		pthread_mutex_unlock(&data->print_lock);
		ft_usleep(data->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	return (0);
}

void	init_philosophers(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < data->nb_philos)
	{
		philo = &data->philo[i];
		philo->id = i + 1;
		philo->meals_eaten = 0;
		philo->last_meal = ft_time();
		philo->data = data;
		philo->left_fork = &data->forks[i];
		philo->right_fork = &data->forks[(i + 1) % data->nb_philos];
		i++;
	}
}
