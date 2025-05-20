/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:11:44 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/16 20:11:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	*check_meals_routine(void *arg)
{
	int			i;
	t_data		*data;

	data = (t_data *)arg;
	i = 0;
	while (i < data->nb_philo)
	{
		sem_wait(data->finished);
		i++;
	}
	sem_wait(data->print);
	printf("All philo have eaten enough\n");
	sem_post(data->print);
	sem_wait(data->all_ate_lock);
	data->all_ate = 1;
	sem_post(data->all_ate_lock);
	return (NULL);
}

void	wait_philo_utils(t_data *data, t_philo *philo)
{
	int				i;

	i = 0;
	while (i < data->nb_philo)
	{
		kill(philo[i].pid, SIGKILL);
		i++;
	}
	i = 0;
	while (i < data->nb_philo)
	{
		waitpid(philo[i].pid, NULL, 0);
		i++;
	}
	cleanup_semaphore(data);
	unlink_semaphore();
	if (philo != NULL)
		free(philo);
}

void	wait_philo_monitor(t_data *data, int *all_ate)
{
	int			status;

	while (1)
	{
		sem_wait(data->all_ate_lock);
		*all_ate = data->all_ate;
		sem_post(data->all_ate_lock);
		if (*all_ate)
			break ;
		if (waitpid(-1, &status, WNOHANG) > 0)
			break ;
		usleep(1000);
	}
}

void	wait_philo(t_data *data, t_philo *philo)
{
	pthread_t	monitor_thread;
	int			all_ate;

	data->all_ate = 0;
	if (data->nb_of_times_each_philo_eat != -1)
	{
		if (pthread_create(&monitor_thread, NULL,
				check_meals_routine, data) != 0)
			error("Failed to create meals monitor thread");
		pthread_detach(monitor_thread);
	}
	wait_philo_monitor(data, &all_ate);
	wait_philo_utils(data, philo);
}
