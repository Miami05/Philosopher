/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:19:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/30 19:08:48 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int	validate_input(int argc, char **argv)
{
	int			i;
	int			j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
		error("Usage: ./philo number_of_philo time_to_die time_to_die"
			"time_to_sleep [numbers_of_each_time_philo_must_eat");
	if (validate_input(argc, argv))
		error("Each number needs to be positive and not greater than INT_MAX");
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nb_of_times_each_philo_eat = -1;
	if (argc == 6)
		data->nb_of_times_each_philo_eat = ft_atoi(argv[5]);
	return (0);
}

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
	printf("All philosophers eaten enough\n");
	sem_wait(data->print);
	data->all_ate = 1;
	sem_post(data->print);
	return (NULL);
}

void	wait_philo_utils(t_data *data, t_philo *philo)
{
	int				i;

	i = 0;
	while (i < data->nb_philo)
	{
		kill(philo[i].pid, SIGTERM);
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
