/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:26:19 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/30 18:17:49 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	error(const char *mssg)
{
	printf("%s\n", mssg);
	exit(1);
}

int	ft_atoi(char *str)
{
	int		result;
	int		digit;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (result > (INT_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		str++;
	}
	return (result);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_philo *philo, char *mssg)
{
	sem_wait(philo->data->print);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
		mssg);
	sem_post(philo->data->print);
}

void	smart_sleep(long time_in_ms)
{
	long		start;

	start = get_time();
	while (get_time() - start < time_in_ms)
		usleep(100);
}
