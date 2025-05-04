/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:54:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/30 20:09:04 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"
#include <time.h>

void	take_forks(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_action(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	print_action(philo, "is eating");
	sem_wait(philo->data->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	sem_post(philo->data->meal_lock);
	smart_sleep(philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	*monitor_routine(void *arg)
{
	t_philo		*philo;
	long		current_time;
	long		time_since_last_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		usleep(500);
		current_time = get_time();
		sem_wait(philo->data->meal_lock);
		time_since_last_meal = current_time - philo->last_meal_time;
		if (time_since_last_meal > philo->data->time_to_die)
		{
			sem_wait(philo->data->print);
			printf("%ld %d %s\n", get_time() - philo->data->start_time,
				philo->id, "died");
			exit(1);
		}
		sem_post(philo->data->meal_lock);
	}
	return (NULL);
}

void	sleep_think(t_philo *philo)
{
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep);
	print_action(philo, "is thinking");
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t		monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor_routine, philo) != 0)
		error("Failed to create thread");
	pthread_detach(monitor_thread);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		if (philo->data->nb_of_times_each_philo_eat != -1
			&& philo->meals_eaten >= philo->data->nb_of_times_each_philo_eat)
		{
			sem_post(philo->data->finished);
			cleanup_semaphore(philo->data);
			exit(0);
		}
		sleep_think(philo);
	}
}
