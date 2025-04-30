/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:57:12 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/30 19:12:07 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	cleanup_semaphore(t_data *data)
{
	if (data->forks != SEM_FAILED)
		sem_close(data->forks);
	if (data->print != SEM_FAILED)
		sem_close(data->print);
	if (data->all_ate_lock != SEM_FAILED)
		sem_close(data->all_ate_lock);
	if (data->meal_lock != SEM_FAILED)
		sem_close(data->meal_lock);
	if (data->finished != SEM_FAILED)
		sem_close(data->finished);
}

void	unlink_semaphore(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal_lock");
	sem_unlink("/finished");
	sem_unlink("/all_ate_lock");
}

int	init_semaphore(t_data *data)
{
	data->forks = SEM_FAILED;
	data->print = SEM_FAILED;
	data->meal_lock = SEM_FAILED;
	data->finished = SEM_FAILED;
	data->all_ate_lock = SEM_FAILED;
	unlink_semaphore();
	data->forks = sem_open("/forks", O_CREAT, 0644, data->nb_philo);
	data->print = sem_open("/print", O_CREAT, 0644, 1);
	data->meal_lock = sem_open("/meal_lock", O_CREAT, 0644, 1);
	data->finished = sem_open("/finished", O_CREAT, 0644, 0);
	data->all_ate_lock = sem_open("/all_ate_lock", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->print == SEM_FAILED
		|| data->meal_lock == SEM_FAILED || data->finished == SEM_FAILED
		|| data->all_ate_lock == SEM_FAILED)
		return (1);
	return (0);
}

void	init_philo(t_data *data, t_philo **philo)
{
	int			i;

	data->start_time = get_time();
	*philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
		error("Memory allcation failed");
	i = 0;
	while (i < data->nb_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].last_meal_time = data->start_time;
		(*philo)[i].meals_eaten = 0;
		(*philo)[i].data = data;
		i++;
	}
}

void	start_simulation(t_data *data, t_philo *philo)
{
	int		i;

	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
			error("Fork failed");
		else if (philo[i].pid == 0)
		{
			philosopher_routine(&philo[i]);
			cleanup_semaphore(data);
			free(philo);
			exit(0);
		}
		usleep(100);
		i++;
	}
}
