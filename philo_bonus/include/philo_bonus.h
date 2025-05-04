/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:53:33 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/04 16:03:47 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H

# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <semaphore.h>
# include <sys/types.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <limits.h>

typedef struct s_data
{
	int		nb_philo;
	int		time_to_eat;
	int		time_to_die;
	int		time_to_sleep;
	int		nb_of_times_each_philo_eat;
	long	start_time;
	int		all_ate;
	sem_t	*forks;
	sem_t	*print;
	sem_t	*meal_lock;
	sem_t	*finished;
	sem_t	*all_ate_lock;
}	t_data;

typedef struct s_philo
{
	int		id;
	long	last_meal_time;
	int		meals_eaten;
	pid_t	pid;
	t_data	*data;
}	t_philo;

int		parse_arguments(int argc, char **argv, t_data *data);
void	error(const char *mssg);
int		ft_atoi(char *str);
int		init_semaphore(t_data *data);
void	unlink_semaphore(void);
int		init_philo(t_data *data, t_philo **philo);
long	get_time(void);
void	start_simulation(t_data *data, t_philo *philo);
void	philosopher_routine(t_philo *philo);
void	*monitor_routine(void *arg);
void	print_action(t_philo *philo, char *mssg);
void	smart_sleep(long time_in_ms);
void	wait_philo(t_data *data, t_philo *philo);
void	cleanup_semaphore(t_data *data);

#endif
