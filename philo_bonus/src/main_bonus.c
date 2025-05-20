/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:42:35 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/16 21:47:18 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"
#include <unistd.h>

void	print_error(t_data *data, const char *mssg)
{
	cleanup_semaphore(data);
	unlink_semaphore();
	error(mssg);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philo;

	philo = NULL;
	if (parse_arguments(argc, argv, &data))
		return (1);
	unlink_semaphore();
	if (init_semaphore(&data))
		print_error(&data, "Error creating the semaphores");
	if (init_philo(&data, &philo))
		print_error(&data, "Error creating the philosophers");
	start_simulation(&data, philo);
	wait_philo(&data, philo);
	cleanup_semaphore(&data);
	unlink_semaphore();
	return (0);
}
