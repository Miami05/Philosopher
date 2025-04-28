/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 philo.h											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: ldurmish < ldurmish@student.42wolfsburg.d	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/17 20:44:34 by ldurmish		   #+#	  #+#			  */
/*	 Updated: 2025/04/24 22:34:53 by ldurmish		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# define _DEFAULT_SOURCE
# define _BSD_SOURCE
# define _XOPEN_SOURCE 500
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				someone_died;
	long			start_time;
	int				all_ate;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philo;
};

int		ft_atoi(char *str);
int		init_data(t_data *data, int argc, char **argv);
void	init_philosophers(t_data *data);
void	*philo_routine(void *arg);
void	ft_usleep(int time_in_ms);
long	ft_time(void);
void	*monitor_routine(void *arg);
long	get_time_in_ms(t_philo *current);
void	clear_mutex(t_data *data);
int		one_philosopher(t_data *data, t_philo *philo);
void	philo_activity(t_data *data, t_philo *philo,
			pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
int		check_death(t_philo *philo);
int		check_all_ate(t_data *data, int *all_ate);

#endif
